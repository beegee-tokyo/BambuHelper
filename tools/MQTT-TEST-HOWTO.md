# MQTT Test Debug Guide

Use this guide when I ask you to collect a raw MQTT dump from your printer for debugging.

This is especially useful when a printer model exposes different field names than expected. For example, some H2D values do not match what I see on H2C, so I need a real dump from the affected printer to map the fields correctly.

## What this script does

`mqtt_test.py` connects directly to your Bambu printer over MQTT, requests a full `pushall` status payload, and saves the result to JSON files.

The script can create:

- `pushall_dump.json` - full printer status dump
- `ams_dump.json` - AMS-only extract, if AMS data is present

## Before you start

Please make sure:

- your printer is powered on
- your computer and printer are on the same local network
- LAN mode is enabled on the printer
- if you have an H2S, H2C, or H2D, Developer Mode is also enabled

You will need these values from the printer:

- printer IP address
- LAN access code
- serial number

## Step 1: Install Python

If Python is already installed, you can skip this step.

### Windows

1. Go to https://www.python.org/downloads/
2. Download the latest Python 3 installer.
3. Run the installer.
4. Make sure `Add Python to PATH` is checked.
5. Finish the installation.

To verify it worked, open Command Prompt and run:

```bash
python --version
```

If that does not work, try:

```bash
py --version
```

### macOS / Linux

Open Terminal and check:

```bash
python3 --version
```

If Python is missing, install Python 3 using your normal system package manager or from https://www.python.org/downloads/

## Step 2: Get the script

You need the file:

- `tools/mqtt_test.py`

If you already downloaded the BambuHelper repository, just open the `tools` folder and use the existing file.

If not, download `mqtt_test.py` from the repository and save it somewhere easy to find.

## Step 3: Open a terminal in the `tools` folder

Open Command Prompt, PowerShell, or Terminal in the same folder where `mqtt_test.py` is located.

If you downloaded the full repository, go into the `tools` folder first.

## Step 4: Install the required Python package

Run one of these commands:

### Windows

```bash
python -m pip install paho-mqtt
```

If `python` does not work, try:

```bash
py -m pip install paho-mqtt
```

### macOS / Linux

```bash
python3 -m pip install paho-mqtt
```

## Step 5: Edit the config in `mqtt_test.py`

Open `mqtt_test.py` in any text editor and update the config block near the top of the file.

For LAN testing, it should look like this:

```python
MODE         = "lan"

PRINTER_IP   = "YOUR_PRINTER_IP"
ACCESS_CODE  = "YOUR_ACCESS_CODE"

SERIAL       = "YOUR_SERIAL_NUMBER"
```

Replace the placeholders with your real printer values.

Important:

- `SERIAL` must be uppercase
- `ACCESS_CODE` is the 8-character LAN access code shown on the printer
- for H2S, H2C, and H2D local MQTT testing, Developer Mode must be enabled

Where to find the values:

- IP address: printer Settings > Network
- Access code: printer Settings > LAN Only Mode
- Serial number: printer Settings > Device > Serial Number

## Step 6: Run the script

Run one of these commands from the folder that contains `mqtt_test.py`.

### Windows

```bash
python mqtt_test.py
```

If needed:

```bash
py mqtt_test.py
```

### macOS / Linux

```bash
python3 mqtt_test.py
```

Let it run for about 30 seconds.

## Step 7: Check the generated files

If the connection works and the printer sends data, the script should create:

- `pushall_dump.json`
- `ams_dump.json`

These files are usually created in the same folder where you ran the script.

## Step 8: Send the results to me

Preferred option:

- send `pushall_dump.json` by email to `keralots@gmail.com`

Why this is preferred:

- it contains the full raw payload
- I can check chamber temperature fields, AMS fields, heater fields, and any printer-model-specific naming differences

Smaller alternative if you do not want to send the full dump:

- send `ams_dump.json`
- or send only the `ams` section from `pushall_dump.json`
- if possible, also include any fields related to `chamber`, `temp`, `heater`, or `heat`

You may redact private values like the serial number if you want, but please do not change the JSON field names or structure.

## What I need most

For unusual printer-specific bugs, the most helpful file is:

- `pushall_dump.json`

For AMS-only investigation, the next best option is:

- `ams_dump.json`

## Common problems

### `python` is not recognized

Try:

```bash
py mqtt_test.py
```

or:

```bash
python3 mqtt_test.py
```

### `ModuleNotFoundError: No module named 'paho'`

Install the package first:

```bash
python -m pip install paho-mqtt
```

or:

```bash
py -m pip install paho-mqtt
```

### The script connects but receives no data

Please double-check:

- printer IP address
- access code
- serial number is uppercase
- printer is on the same network
- LAN mode is enabled
- on H2S, H2C, and H2D: Developer Mode is enabled

### Authentication failed

The LAN access code is usually incorrect. Re-check it on the printer screen and run the script again.

## Quick summary

1. Install Python 3.
2. Install `paho-mqtt`.
3. Edit the config in `mqtt_test.py`.
4. Run the script for 30 seconds.
5. Send me `pushall_dump.json`, or at minimum the AMS-related extract.

Thanks for helping me debug printer-specific MQTT data. It makes it much easier to fix support for new models and edge cases.
