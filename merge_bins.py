#!/usr/bin/env python3
"""
Create BambuHelper release firmware files.

Usage:
    python merge_bins.py              # auto-reads version, builds S3
    python merge_bins.py --board cyd  # build CYD firmware
    python merge_bins.py v2.5         # override version
    python merge_bins.py --ota        # OTA binary only
    python merge_bins.py --full       # WebFlasher binary only

Output:
    firmware/v2.4-Beta1/BambuHelper-WebFlasher-v2.4-Beta1.bin
    firmware/v2.4-Beta1/BambuHelper-OTA-v2.4-Beta1.bin
    firmware/v2.4-Beta1/BambuHelper-CYD-WebFlasher-v2.4-Beta1.bin
    firmware/v2.4-Beta1/BambuHelper-CYD-OTA-v2.4-Beta1.bin
"""

import argparse
import os
import re
import sys

# Board configurations
BOARDS = {
    's3': {
        'build_dir': '.pio/build/esp32s3',
        'bootloader_offset': 0x0,       # ESP32-S3 starts at 0x0
        'partitions_offset': 0x8000,
        'firmware_offset': 0x10000,
        'name_prefix': 'BambuHelper',
    },
    'cyd': {
        'build_dir': '.pio/build/cyd',
        'bootloader_offset': 0x1000,     # Standard ESP32 starts at 0x1000
        'partitions_offset': 0x8000,
        'firmware_offset': 0x10000,
        'name_prefix': 'BambuHelper-CYD',
    },
}

CONFIG_H = os.path.join('include', 'config.h')


def read_version_from_config():
    """Extract FW_VERSION from config.h."""
    if not os.path.exists(CONFIG_H):
        return None
    with open(CONFIG_H, 'r') as f:
        for line in f:
            m = re.match(r'#define\s+FW_VERSION\s+"([^"]+)"', line)
            if m:
                return m.group(1)
    return None


def get_paths(version, board):
    """Return output directory and file paths for a given version and board."""
    cfg = BOARDS[board]
    out_dir = os.path.join('firmware', version)
    prefix = cfg['name_prefix']
    merged = os.path.join(out_dir, f'{prefix}-WebFlasher-{version}.bin')
    ota = os.path.join(out_dir, f'{prefix}-OTA-{version}.bin')
    return out_dir, merged, ota


def create_ota_binary(out_dir, out_path, board):
    """Copy firmware.bin as OTA update file."""
    cfg = BOARDS[board]
    firmware = os.path.join(cfg['build_dir'], 'firmware.bin')
    if not os.path.exists(firmware):
        print(f"Error: {firmware} not found. Run 'pio run -e {board}' first.")
        return False

    os.makedirs(out_dir, exist_ok=True)

    with open(firmware, 'rb') as src, open(out_path, 'wb') as dst:
        dst.write(src.read())

    size = os.path.getsize(out_path)
    print(f"  OTA binary: {out_path} ({size / 1024:.1f} KB)")
    return True


def merge_binaries(out_dir, out_path, board):
    """Merge bootloader + partitions + firmware into a single flashable binary."""
    cfg = BOARDS[board]
    build_dir = cfg['build_dir']
    bootloader = os.path.join(build_dir, 'bootloader.bin')
    partitions = os.path.join(build_dir, 'partitions.bin')
    firmware = os.path.join(build_dir, 'firmware.bin')

    for path in [bootloader, partitions, firmware]:
        if not os.path.exists(path):
            print(f"Error: {path} not found. Run 'pio run -e {board}' first.")
            return False

    os.makedirs(out_dir, exist_ok=True)

    bl_offset = cfg['bootloader_offset']
    pt_offset = cfg['partitions_offset']
    fw_offset = cfg['firmware_offset']

    with open(out_path, 'wb') as out:
        # Pad to bootloader offset
        if bl_offset > 0:
            out.write(b'\xFF' * bl_offset)

        with open(bootloader, 'rb') as f:
            bl = f.read()
            out.write(bl)

        # Pad to partitions offset
        current = bl_offset + len(bl)
        out.write(b'\xFF' * (pt_offset - current))

        with open(partitions, 'rb') as f:
            pt = f.read()
            out.write(pt)

        # Pad to firmware offset
        current = pt_offset + len(pt)
        out.write(b'\xFF' * (fw_offset - current))

        with open(firmware, 'rb') as f:
            fw = f.read()
            out.write(fw)

    total = os.path.getsize(out_path)
    print(f"  WebFlasher: {out_path} ({total / 1024:.1f} KB)")
    return True


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Create BambuHelper release firmware')
    parser.add_argument('version', nargs='?', default=None, help='Version (default: read from config.h)')
    parser.add_argument('--board', choices=list(BOARDS.keys()), default='s3', help='Board target (default: s3)')
    parser.add_argument('--ota', action='store_true', help='OTA binary only')
    parser.add_argument('--full', action='store_true', help='WebFlasher binary only')
    args = parser.parse_args()

    version = args.version or read_version_from_config()
    if not version:
        print("Error: could not read FW_VERSION from config.h. Pass version as argument.")
        sys.exit(1)

    board = args.board
    out_dir, merged_path, ota_path = get_paths(version, board)
    print(f"Version: {version}  Board: {board}\n")

    if args.ota:
        success = create_ota_binary(out_dir, ota_path, board)
    elif args.full:
        success = merge_binaries(out_dir, merged_path, board)
    else:
        s1 = merge_binaries(out_dir, merged_path, board)
        s2 = create_ota_binary(out_dir, ota_path, board)
        success = s1 and s2

        if success:
            prefix = BOARDS[board]['name_prefix']
            print(f"\n{'='*60}")
            print(f"Release {version} ({board}) ready in {out_dir}/")
            print(f"{'='*60}")
            print(f"\nGitHub Release - attach both files:")
            print(f"  ...{prefix}-WebFlasher-{version}.bin  - new users (ESP Web Flasher)")
            print(f"  ...{prefix}-OTA-{version}.bin         - existing users (Web UI update)")

    sys.exit(0 if success else 1)
