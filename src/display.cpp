#include "display.h"
#include "app_config.h"
#include <Arduino.h>
#include <lvgl.h>
#include "esp_heap_caps.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_rgb.h"
#include "debug_log.h"

namespace {
esp_lcd_panel_handle_t panel = nullptr;
lv_display_t *display = nullptr;
uint8_t *drawBuffer1 = nullptr;
uint8_t *drawBuffer2 = nullptr;
volatile uint32_t flushCount = 0;
uint32_t fpsWindowMs = 0;
uint16_t currentFps = 0;

constexpr int PIN_LCD_HSYNC = 46;
constexpr int PIN_LCD_VSYNC = 3;
constexpr int PIN_LCD_DE = 5;
constexpr int PIN_LCD_PCLK = 7;
// Correct data-lane order confirmed by the two-phase LCD color test.
// esp_lcd expects B0..B4, G0..G5, R0..R4 for RGB565 on this panel.
constexpr int LCD_DATA_PINS[16] = {
  14, 38, 18, 17, 10,
  39, 0, 45, 48, 47, 21,
  1, 2, 42, 41, 40
};

uint32_t tickMs() { return millis(); }

void flush(lv_display_t *disp, const lv_area_t *area, uint8_t *pixels) {
  const esp_err_t result = esp_lcd_panel_draw_bitmap(panel, area->x1, area->y1,
                                                     area->x2 + 1, area->y2 + 1, pixels);
  if (result != ESP_OK) DebugLog::printf("[DISPLAY] draw_bitmap Fehler: %s\n", esp_err_to_name(result));
  ++flushCount;
  lv_display_flush_ready(disp);
}

bool initPanel() {
  DebugLog::println("[DISPLAY] RGB-Panel konfigurieren");
  esp_lcd_rgb_panel_config_t cfg = {};
  cfg.clk_src = LCD_CLK_SRC_DEFAULT;
  cfg.timings.pclk_hz = 16000000;
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
  cfg.bounce_buffer_size_px = AppConfig::LCD_WIDTH * 10;
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
}

bool Display::begin() {
  if (!initPanel()) return false;
  DebugLog::println("[DISPLAY] LVGL initialisieren");
  lv_init();
  lv_tick_set_cb(tickMs);

  constexpr size_t lines = 24;
  constexpr size_t bufferBytes = AppConfig::LCD_WIDTH * lines * sizeof(lv_color_t);
  drawBuffer1 = static_cast<uint8_t *>(heap_caps_malloc(bufferBytes, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT));
  drawBuffer2 = static_cast<uint8_t *>(heap_caps_malloc(bufferBytes, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT));
  DebugLog::printf("[DISPLAY] Puffer 1=%p Puffer 2=%p, je %u Byte\n",
                drawBuffer1, drawBuffer2, static_cast<unsigned>(bufferBytes));
  if (!drawBuffer1 || !drawBuffer2) return false;

  display = lv_display_create(AppConfig::LCD_WIDTH, AppConfig::LCD_HEIGHT);
  if (!display) return false;
  lv_display_set_flush_cb(display, flush);
  lv_display_set_buffers(display, drawBuffer1, drawBuffer2, bufferBytes, LV_DISPLAY_RENDER_MODE_PARTIAL);
  fpsWindowMs = millis();
  DebugLog::println("[DISPLAY] READY");
  return true;
}

void Display::task() {
  lv_timer_handler();
  const uint32_t now = millis();
  if (now - fpsWindowMs >= 1000) {
    noInterrupts();
    const uint32_t frames = flushCount;
    flushCount = 0;
    interrupts();
    const uint32_t elapsed = now - fpsWindowMs;
    currentFps = elapsed ? static_cast<uint16_t>((frames * 1000UL) / elapsed) : 0;
    fpsWindowMs = now;
  }
}

uint16_t Display::fps() { return currentFps; }
