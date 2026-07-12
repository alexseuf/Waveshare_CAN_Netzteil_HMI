#include "display.h"
#include "app_config.h"
#include <Arduino.h>
#include <lvgl.h>
#include <atomic>
#include "esp_heap_caps.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_rgb.h"
#include "debug_log.h"

namespace {
esp_lcd_panel_handle_t panel = nullptr;
lv_display_t *display = nullptr;
uint8_t *drawBuffer = nullptr;
std::atomic<uint32_t> flushCount{0};
uint32_t fpsWindowMs = 0;
uint16_t currentFps = 0;

constexpr int PIN_LCD_HSYNC = 46;
constexpr int PIN_LCD_VSYNC = 3;
constexpr int PIN_LCD_DE = 5;
constexpr int PIN_LCD_PCLK = 7;
constexpr int LCD_DATA_PINS[16] = {
  14, 38, 18, 17, 10,
  39, 0, 45, 48, 47, 21,
  1, 2, 42, 41, 40
};

uint32_t tickMs() { return millis(); }

void flush(lv_display_t *disp, const lv_area_t *area, uint8_t *pixels) {
  const esp_err_t result = esp_lcd_panel_draw_bitmap(
      panel, area->x1, area->y1, area->x2 + 1, area->y2 + 1, pixels);
  if (result != ESP_OK) {
    DebugLog::printf("[DISPLAY] draw_bitmap Fehler: %s\n", esp_err_to_name(result));
  }
  flushCount.fetch_add(1, std::memory_order_relaxed);
  lv_display_flush_ready(disp);
}

bool initPanel() {
  DebugLog::println("[DISPLAY] RGB-Panel stabil konfigurieren");
  esp_lcd_rgb_panel_config_t cfg = {};
  cfg.clk_src = LCD_CLK_SRC_DEFAULT;

  // 12 MHz reduziert die Speicherbandbreite deutlich und verhindert
  // RGB-Unterläufe bei gleichzeitigem WiFi-/PSRAM-Zugriff.
  cfg.timings.pclk_hz = 12000000;
  cfg.timings.h_res = AppConfig::LCD_WIDTH;
  cfg.timings.v_res = AppConfig::LCD_HEIGHT;
  cfg.timings.hsync_pulse_width = 4;
  cfg.timings.hsync_back_porch = 8;
  cfg.timings.hsync_front_porch = 8;
  cfg.timings.vsync_pulse_width = 4;
  cfg.timings.vsync_back_porch = 8;
  cfg.timings.vsync_front_porch = 8;
  cfg.timings.flags.pclk_active_neg = true;

  cfg.data_width = 16;
  cfg.num_fbs = 1;

  // Der Framebuffer bleibt im PSRAM. Ein ausreichend großer interner
  // Bounce-Buffer entkoppelt den RGB-DMA von kurzzeitigen PSRAM-Latenzen.
  cfg.bounce_buffer_size_px = AppConfig::LCD_WIDTH * 16;
  cfg.hsync_gpio_num = PIN_LCD_HSYNC;
  cfg.vsync_gpio_num = PIN_LCD_VSYNC;
  cfg.de_gpio_num = PIN_LCD_DE;
  cfg.pclk_gpio_num = PIN_LCD_PCLK;
  cfg.disp_gpio_num = -1;
  for (int i = 0; i < 16; ++i) cfg.data_gpio_nums[i] = LCD_DATA_PINS[i];
  cfg.flags.fb_in_psram = true;

  esp_err_t result = esp_lcd_new_rgb_panel(&cfg, &panel);
  DebugLog::printf("[DISPLAY] esp_lcd_new_rgb_panel: %s\n", esp_err_to_name(result));
  if (result != ESP_OK) return false;

  result = esp_lcd_panel_reset(panel);
  DebugLog::printf("[DISPLAY] esp_lcd_panel_reset: %s\n", esp_err_to_name(result));
  if (result != ESP_OK) return false;

  result = esp_lcd_panel_init(panel);
  DebugLog::printf("[DISPLAY] esp_lcd_panel_init: %s\n", esp_err_to_name(result));
  return result == ESP_OK;
}
}  // namespace

bool Display::begin() {
  if (!initPanel()) return false;

  DebugLog::println("[DISPLAY] LVGL initialisieren");
  lv_init();
  lv_tick_set_cb(tickMs);

  // Kritischer LVGL-Zeichenpuffer bewusst im internen DMA-RAM.
  // Nur ein kleiner 8-Zeilen-Puffer wird verwendet, damit genügend
  // interner Heap für WiFi, CAN und LVGL verfügbar bleibt.
  constexpr size_t lines = 8;
  constexpr size_t bufferBytes =
      AppConfig::LCD_WIDTH * lines * sizeof(lv_color_t);
  drawBuffer = static_cast<uint8_t *>(
      heap_caps_malloc(bufferBytes, MALLOC_CAP_INTERNAL | MALLOC_CAP_DMA | MALLOC_CAP_8BIT));

  DebugLog::printf("[DISPLAY] Interner LVGL-Puffer=%p, %u Byte\n",
                   drawBuffer, static_cast<unsigned>(bufferBytes));
  if (!drawBuffer) {
    DebugLog::println("[DISPLAY] Interner LVGL-Puffer konnte nicht reserviert werden");
    return false;
  }

  display = lv_display_create(AppConfig::LCD_WIDTH, AppConfig::LCD_HEIGHT);
  if (!display) return false;

  lv_display_set_flush_cb(display, flush);
  lv_display_set_buffers(display, drawBuffer, nullptr, bufferBytes,
                         LV_DISPLAY_RENDER_MODE_PARTIAL);

  fpsWindowMs = millis();
  DebugLog::println("[DISPLAY] READY - stabiler Einzelpufferbetrieb");
  return true;
}

void Display::task() {
  lv_timer_handler();
  const uint32_t now = millis();
  if (now - fpsWindowMs >= 1000) {
    const uint32_t frames = flushCount.exchange(0, std::memory_order_relaxed);
    const uint32_t elapsed = now - fpsWindowMs;
    currentFps = elapsed
                     ? static_cast<uint16_t>((frames * 1000UL) / elapsed)
                     : 0;
    fpsWindowMs = now;
  }
}

uint16_t Display::fps() { return currentFps; }
