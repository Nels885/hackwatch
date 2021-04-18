#include "tile-scanner.h"

#define TAG "[tile::wifi::scanner]"

static wifiscan_t g_wifiscan;
static tile_t scanner_tile;
static widget_button_t scanner_btn;

volatile bool scanner_enabled = false;

void scanner_event_handler(wifiscan_event_t event)
{
  wifi_ap_t *p_ap;

  if (event == WS_EVENT_SELECTED)
  {
    /* Process selected AP. */
    p_ap = wifiscan_get_selected(&g_wifiscan);
    ESP_LOGI(TAG, "AP selected: %s", p_ap->essid);

    /* Switch to the next screen (bottom). */
    tile_apinfo_set_ap(p_ap);
    ui_go_down();
  }
}

void scanner_toggle(struct widget_t *p_widget)
{
  if (!scanner_enabled)
  {
    /* Enable scanner ! */
    wifi_set_mode(WIFI_SCANNER);

    /* Change button text. */
    widget_button_set_text(&scanner_btn, "Scan Off");

    /* Scanner is on. */
    scanner_enabled = true;
  }
  else
  {
    /* Stop scanner. */
    wifi_set_mode(WIFI_OFF);

    /* Set button text. */
    widget_button_set_text(&scanner_btn, "Scan On");

    /* Scanner is off. */
    scanner_enabled = false;
  }
}

tile_t *tile_scanner_init(void)
{
  /* Initialize our tile. */
  tile_init(&scanner_tile, NULL);

  /* Add a wifiscan widget. */
  wifiscan_init(&g_wifiscan, &scanner_tile, 5, 5, 230, 195);
  wifiscan_set_event_handler(&g_wifiscan, scanner_event_handler);

  /* Add scanner button. */
  widget_button_init(&scanner_btn, &scanner_tile, (240-120)/2, 205, 120, 30, "Scan On");
  widget_button_set_handler(&scanner_btn, scanner_toggle);

  /* Return our tile. */
  return &scanner_tile;
}