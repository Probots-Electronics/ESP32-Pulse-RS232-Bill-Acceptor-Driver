# Deployment Guide

This guide explains how to compile and upload the firmware to your ESP32 using PlatformIO.

## Prerequisites
1.  **Visual Studio Code (VS Code)** installed.
2.  **PlatformIO IDE Extension** installed inside VS Code.
3.  **ESP32 DevKit V1** connected to your computer via a data-capable USB cable.

## Step-by-Step Instructions

### 1. Open the Project
- Launch VS Code.
- Go to `File` > `Open Folder...`.
- Navigate to and select the `firmware/` directory of this project.

### 2. Select the Environment
PlatformIO supports multiple build environments for this project:
- **`esp32dev`**: Standard "Universal Mode" (Recommended).
- **`rs232_only`**: Disables Pulse detection.
- **`pulse_only`**: Disables RS232 detection.

You can switch environments using the **Project Environment Switcher** at the bottom status bar of VS Code (it usually says `Default`).

### 3. Compile (Build)
- Click the **Checkmark icon** (Build) in the PlatformIO status bar (bottom left).
- Wait for the terminal to show `[SUCCESS]`. This verifies the code is correct.

### 4. Upload
- Click the **Arrow icon** (Upload) next to the Checkmark.
- PlatformIO will automatically detect your COM port and flash the firmware.
- You should see a progress bar and finally `[SUCCESS]`.

### 5. Monitor
- Click the **Plug icon** (Serial Monitor).
- Ensure the baud rate is set to **9600** (this is already configured in `platformio.ini`).
- You should see "Vending Controller Initializing..." in the console.
- **Verification**: Insert a bill. You should see logs like `[RS232] Accepted...` or `[Pulse] Accepted...` on the screen.

## Command Line (CLI) Instructions

If you prefer working in the terminal, use these commands from inside the `firmware/` directory:

| Action | Command |
| :--- | :--- |
| **Complete Build** | `pio run` |
| **Upload (Default)** | `pio run -t upload` |
| **Build Specific Env** | `pio run -e rs232_only` |
| **Upload Specific Env**| `pio run -e pulse_only -t upload` |
| **Serial Monitor** | `pio device monitor` |

## Troubleshooting
- **Connection Error**: Ensure you have the CP210x or CH340 drivers installed for your ESP32.
- **Permission Denied (Linux)**: Run `sudo usermod -a -G dialout $USER` and restart your computer if you cannot access the serial port.
