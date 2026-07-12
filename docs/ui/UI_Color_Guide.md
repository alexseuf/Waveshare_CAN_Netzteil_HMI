# UI-Farbpalette V9.1

Die Farben sind zentrale Konstanten in `src/ui_core.cpp`. Buttons entfernen zunächst alle LVGL-Theme-Stile, damit die Palette deterministisch angewendet wird.

| Verwendung | Hex |
|---|---|
| Hintergrund | `#020304` |
| Panels | `#080A0D` |
| Rahmen | `#4B5057` |
| Haupt-/Messüberschriften | `#0754C9` |
| Plus/Minus und Presets | `#0754C9` |
| Button gedrückt | automatisch ca. 30 % dunkler |
| Aktiver Reiter / CC / Warnung | `#F6A000` |
| Systemstatus / CV | `#22D12F` |
| START | `#149B20` |
| STOP | `#D00000` |
| Aktiver Fehlerpunkt | `#F02020` |
| Inaktiv | `#8D9298` |
| Primärtext | Weiß |

## Verbindliche Regel

Für interaktive Objekte dürfen keine LVGL-Theme-Farben verwendet werden. `makeButton()` ruft `lv_obj_remove_style_all()` auf und setzt Default-, Pressed- und Focus-State explizit.
