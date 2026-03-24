# BambuHelper v2.4 Release Notes

## OTA firmware update (NEW)

- **Web-based OTA updates** - upload new firmware directly from the web UI, no USB cable needed
- Upload a .bin file in the "WiFi & System" section, progress bar shows upload status
- All settings preserved after update (WiFi, printers, cloud tokens, display config)
- Client-side validation (file type, size) + server-side ESP32 magic byte check
- MQTT disconnects during upload to free memory, device restarts automatically after success
- Current firmware version shown in web UI

## Date format fix

- **Locale-aware date format** - clock screens (standard and Pong) now use DD.MM.YYYY in 24h mode and MM/DD/YYYY in 12h mode (previously always DD.MM.YYYY regardless of setting)
- **ETA date format** - print ETA on the dashboard now uses MM/DD format in 12h mode (was DD.MM)

## Screen wakeup fix

- **Wake from screen off on print end** - when display auto-off was active and printer left FINISH state, the screen now properly wakes up to IDLE instead of staying off until button press
- SCREEN_CLOCK remains sticky (only button or new print exits it)

## Night mode (NEW)

- **Scheduled display dimming** - automatically dim the screen during set hours (e.g. 22:00-07:00), with a separate brightness slider for the night period
- Supports wrap-around schedules (across midnight)
- Night brightness respects all wake-up paths (button press, print start, screen off exit)
- Requires NTP time sync to activate
- Configurable in the Display section of the web UI

## Live brightness preview

- **Instant brightness feedback** - moving the brightness slider (normal or night) now updates the display in real-time without pressing Save
- Lightweight `/brightness` endpoint - no NVS writes until Apply is clicked

## Bug fixes

- **Stale cloud data showing wrong screen** - when cloud printer (H2C) stopped sending MQTT data for 5+ minutes, the display fell back to 2-gauge idle screen with "RUNNING" text instead of the 6-gauge printing dashboard. Stale timeout now properly resets both the printing flag and gcode state.
- **Idle/blank screen never triggered with offline printers** - when both printers were off or unreachable at startup, the display stayed on the "Connecting to Printer" screen forever and never transitioned to clock or blank screen. Idle timeout now also applies to the connecting screen.
- **No path from idle to screen off** - when "Show clock after finish" was disabled, the idle screen had no timeout path to screen off (only idle-to-clock existed). Both clock and off transitions now work from idle and connecting screens.

## Build stats

- Flash: 90.2% (1182KB / 1310KB)
- RAM: 15.7% (51KB / 328KB)
- Board: lolin_s3_mini (ESP32-S3)
