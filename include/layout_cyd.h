#ifndef LAYOUT_CYD_H
#define LAYOUT_CYD_H

// Layout profile: ILI9341 240x320 portrait (ESP32-2432S028 "CYD")
// MVP: same layout as 240x240 but bottom-anchored elements moved to 320px.
// The extra ~80px between gauge rows and ETA is intentionally unused for now.

// --- Screen dimensions ---
#define LY_W    240
#define LY_H    320

// --- LED progress bar (top, y=0) ---
#define LY_BAR_W   236
#define LY_BAR_H   5

// --- Header bar (same as default) ---
#define LY_HDR_Y        7
#define LY_HDR_H        20
#define LY_HDR_NAME_X   6
#define LY_HDR_CY       17
#define LY_HDR_BADGE_RX 8
#define LY_HDR_DOT_CY   10

// --- Printing: 2x3 gauge grid (same as default) ---
#define LY_GAUGE_R   32
#define LY_GAUGE_T   6
#define LY_COL1      42
#define LY_COL2      120
#define LY_COL3      198
#define LY_ROW1      60
#define LY_ROW2      148

// --- Printing: ETA / info zone (moved down for 320px) ---
#define LY_ETA_Y        250
#define LY_ETA_H        30
#define LY_ETA_TEXT_Y   267

// --- Printing: bottom status bar (anchored to bottom) ---
#define LY_BOT_Y    298
#define LY_BOT_H    18
#define LY_BOT_CY   308

// --- Printing: WiFi signal indicator ---
#define LY_WIFI_X    4
#define LY_WIFI_Y    308

// --- Idle screen (with printer) - same as default ---
#define LY_IDLE_NAME_Y      30
#define LY_IDLE_STATE_Y     50
#define LY_IDLE_STATE_H     20
#define LY_IDLE_STATE_TY    60
#define LY_IDLE_DOT_Y       85
#define LY_IDLE_GAUGE_R     30
#define LY_IDLE_GAUGE_Y     140
#define LY_IDLE_G_OFFSET    55

// --- Idle screen (no printer) - same as default ---
#define LY_IDLE_NP_TITLE_Y  40
#define LY_IDLE_NP_WIFI_Y   80
#define LY_IDLE_NP_DOT_Y    105
#define LY_IDLE_NP_MSG_Y    140
#define LY_IDLE_NP_OPEN_Y   165
#define LY_IDLE_NP_IP_Y     200

// --- Finished screen ---
#define LY_FIN_GAUGE_R   32
#define LY_FIN_GL        72
#define LY_FIN_GR        168
#define LY_FIN_GY        80
#define LY_FIN_TEXT_Y    148
#define LY_FIN_FILE_Y   178
#define LY_FIN_BOT_Y    290
#define LY_FIN_BOT_H    22
#define LY_FIN_WIFI_Y   308

// --- AP mode screen (same as default) ---
#define LY_AP_TITLE_Y     40
#define LY_AP_SSID_LBL_Y  80
#define LY_AP_SSID_Y      110
#define LY_AP_PASS_LBL_Y  140
#define LY_AP_PASS_Y       158
#define LY_AP_OPEN_Y      185
#define LY_AP_IP_Y        210

// --- Simple clock (centered in 320px height) ---
#define LY_CLK_CLEAR_Y   70
#define LY_CLK_CLEAR_H   200
#define LY_CLK_TIME_Y    140
#define LY_CLK_AMPM_Y    175
#define LY_CLK_DATE_Y    205

// --- Pong/Breakout clock ---
#define LY_ARK_BRICK_ROWS   5
#define LY_ARK_COLS          10
#define LY_ARK_BRICK_W      22
#define LY_ARK_BRICK_H      8
#define LY_ARK_BRICK_GAP    2
#define LY_ARK_START_X      3
#define LY_ARK_START_Y      28
#define LY_ARK_PADDLE_Y     304
#define LY_ARK_PADDLE_W     30
#define LY_ARK_TIME_Y       150
#define LY_ARK_DATE_Y       8
#define LY_ARK_DIGIT_W      32
#define LY_ARK_DIGIT_H      48
#define LY_ARK_COLON_W      12
#define LY_ARK_DATE_CLR_X   40
#define LY_ARK_DATE_CLR_W   160

#endif // LAYOUT_CYD_H
