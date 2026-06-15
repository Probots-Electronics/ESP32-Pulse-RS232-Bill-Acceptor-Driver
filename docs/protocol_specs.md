# Bill Acceptor Protocol Specifications

```markdown
This document defines the communication protocols supported by the ESP32 Vending Controller for interfacing with bill acceptors, specifically ICT models (such as the P70 and P77 series).
```

## 1. RS232 Communication (Hex-Based)

The RS232 protocol uses hex codes for commands and status updates. It is the preferred method for robust communication.

### Serial Settings
- **Baud Rate:** 9600 bps
- **Data Bits:** 8
- **Parity:** Even (8E1)
- **Stop Bits:** 1

### Command Codes (Main to Acceptor)
| Command | Hex Code | Description |
| --- | --- | --- |
| `ENABLE_ALL` | `0x3E` | Enables all programmed bill types. |
| `DISABLE_ALL` | `0x5E` | Disables all bill types. |

### Status Codes (Acceptor to Main)
| Status | Hex Code | Description |
| --- | --- | --- |
| `IDLING` | `0x80` | Acceptor is ready and waiting for a bill. |
| `ACCEPTING` | `0x81` | A bill is being drawn in. |
| `ESCROWED` | `0x82` | Bill is held in escrow for approval. |
| `STACKING` | `0x83` | Bill is being moved to the stacker. |
| `VEND_VALID` | `0x84` | Bill successfully validated and stacked. |
| `STACKED` | `0x85` | Bill has reached the stacker. |
| `REJECTING` | `0x86` | Bill is being returned to the user. |
| `JAMMED` | `0x8A` | A bill jam has occurred. |
| `STACKER_FULL` | `0x8B` | The bill cassette is full. |
| `CASSETTE_REMOVED` | `0x8C` | The bill cassette has been removed. |

### Bill Value Codes
Upon successful validation, the acceptor sends one of the following codes:
- `0x40`: 10 Currency Units
- `0x41`: 20 Currency Units
- `0x42`: 50 Currency Units
- `0x43`: 100 Currency Units
- `0x44`: 200 Currency Units
- `0x45`: 500 Currency Units

---

## 2. Pulse Communication

Pulse communication uses a series of digital pulses to represent the value of the bill.

### Hardware Requirements
- **Pulse Pin:** Connected to the acceptor's Pulse output (Open Collector, requires pull-up).
- **Inhibit Pin:** Connected to the acceptor's Inhibit input (**Active High** by default).

### Pulse Specifications
- **Level:** Active Low (Pulses drop to 0V).
- **Pulse Width:** Typ. 50ms (Low) / 50ms (High).
- **Inactivity Timeout:** 500ms (used to signal end of pulse train).

### Software Processing Constraints (Pulse mode)
- **Interrupt Type:** The ESP32 utilizes a **FALLING** edge hardware interrupt to detect each pulse instantly without blocking the main event loop.
- **Debouncing:** The firmware implements a basic **40ms software lockout filter**. Any subsequent signal state changes (electrical bouncing/noise) occurring within 40ms of the initial falling edge are ignored.

### Pulse-to-Value Mapping
The firmware currently implements the following default mapping (configurable):
| Pulse Count | Bill Value | Code Returned |
| --- | --- | --- |
| 1 | 5 | `CODE_BILL_5` |
| 2 | 10 | `CODE_BILL_10` |
| 4 | 20 | `CODE_BILL_20` |
| 10 | 50 | `CODE_BILL_50` |
| 20 | 100 | `CODE_BILL_100` |
| 40 | 200 | `CODE_BILL_200` |
| 100 | 500 | `CODE_BILL_500` |

---

---

## 3. Global Currency Support (Multi-Currency)

The firmare defaults to **Indian Rupees (INR)**, but can be adapted for any currency.

### 3.1 Pulse Mapping Adaptation
To change pulse values (e.g., USD: 1 pulse = $1), modify the `switch` statement in `firmware/lib/BillAcceptor/src/pulse_bill_acceptor.cpp`:

```cpp
switch (count) {
    case 1:  return CODE_BILL_1;  // Update definitions in protocol_map.h if needed
    case 5:  return CODE_BILL_5;
    ...
}
```

### 3.2 RS232 Mapping Adaptation
RS232 codes are fixed by the acceptor's firmware. To map them to new values, update the hex definitions in `firmware/lib/BillAcceptor/include/protocol_map.h`:

```cpp
#define CODE_BILL_5  0x40 // Update based on your currency's protocol manual
```

---

## 4. Universal Mode

The firmware is designed to operate in **Universal Mode** by default. This means the ESP32 listens to both RS232 and Pulse signals simultaneously.

- **Priority:** If a valid signal is received on both interfaces at the same millisecond, the RS232 signal takes precedence.
- **Benefit:** No code changes or DIP switch re-uploads are required when switching between hardware communication modes.

## 4. Hardware Wiring (Universal Mode)

To use both communication modes simultaneously, wire your ESP32 to the Bill Acceptor as shown below. This setup allows the firmware to automatically detect whichever signal (RS232 or Pulse) the acceptor sends.


### Wiring Table

| BA Pin (TP77) | Signal Name | ESP32 Pin | Logic Level | Notes |
| :--- | :--- | :--- | :--- | :--- |
| **Pin 8** | RS232 TXD | **GPIO 16** | 3.3V TTL | Direct Connect |
| **Pin 7** | RS232 RXD | **GPIO 17** | 3.3V TTL | Direct Connect |
| **Pin ?** | Pulse Output | **GPIO 27** | 3.3V | External 10k Pull-up used |
| **Pin 1** | Inhibit Input  | **GPIO 5** | 3.3V | **High = Stop**, **Low = Enable** |
| **GND** | Ground | **GND** | 0V | Common Ground required |
| **+12V** | Power | **N/A** | 12V DC | External Power Supply |

---

## 5. Harness Pinout Guide

Depending on your bill acceptor, you may have the **WEL-R7U02** (9-pin primary) or the **CU-R961-1** harness. Both follow a standard color-coding scheme.

### Primary Power & Control (9-Pin AMP Connector)
*This is the large connector that provides main 12V power and inhibit control.*

| Pin | Color | Function | ESP32 Connection |
| :--- | :---: | :--- | :--- |
| **Pin 1** | Yellow | **Inhibit (+)** | **GPIO 5** (via Logic Level) |
| **Pin 2** | Green | **Inhibit (-)** | Connect to **GND** |
| **Pin 3** | Red | **+12V DC** | Main Power (+) |
| **Pin 5** | Red | **+12V DC** | Main Power (+) (Alt Position) |
| **Pin 7** | Blue | **Meter (+)** | (Optional) |
| **Pin 8** | Purple | **Meter (-)** | (Optional) |
| **Pin 9** | Orange | **Ground** | Common **GND** |

> [!NOTE]
> On some `CU-R961-1` harnesses, Pin 3 is Red, while on others it is Pin 5. Always look for the thick Red wire for 12V DC.

### Pulse Interface (6-Pin TMT Connector)
*Used for standard Pulse mode. Pin 5 is the critical trigger pin.*

| Pin | Color | Function | ESP32 Connection |
| :--- | :---: | :--- | :--- |
| **Pin 1** | Red | **+12V DC** | Power Input |
| **Pin 2** | Orange | **Ground** | Common **GND** |
| **Pin 3** | Yellow | **Inhibit (+)** | (ALT) Inhibit Control |
| **Pin 4** | Green | **Inhibit (-)** | (ALT) GND |
| **Pin 5** | Blue | **Pulse Output** | **GPIO 27** (10k Pull-Up Req) |

### RS232 Interface (8-Pin TMT Connector)
*Used for serial communication (RX2/TX2).*

| Pin | Color | Function | ESP32 Connection |
| :--- | :---: | :--- | :--- |
| **Pin 1** | Blue | **Ground** | Common **GND** |
| **Pin 6** | White | **VCC** | 5V TTL VCC (Leave Disconnected) |
| **Pin 7** | Black | **RXD** | **GPIO 17** (Direct Connect) |
| **Pin 8** | Purple | **TXD** | **GPIO 16** (Direct Connect) |

---

## 6. How to Test Protocols Separately

While the firmware is "Universal," you may want to test each protocol individually to ensure your wiring and level shifters are working correctly.

### Method A: Hardware Isolation (Recommended)
This method tests the "Universal" logic with specific hardware configurations.

1.  **To Test RS232 Only**:
    *   Set the Bill Acceptor DIP switches to **RS232 Mode**.
    *   Disconnect the Pulse cable (Pin 5) from the ESP32.
    *   The ESP32 will still poll both, but since Pulse is inactive, you can verify if RS232 commands (0x3E) and status codes (0x80) are visible in the Serial Monitor.

2.  **To Test Pulse Only**:
    *   Set the Bill Acceptor DIP switches to **Pulse Mode**.
    *   Disconnect the RS232 TX/RX lines (Pins 3 & 4) or remove the MAX3232.
    *   The ESP32 will only receive pulse trains on GPIO 4.

### Method B: Firmware Isolation
If you want to completely disable one protocol in the code for debugging:

Open `firmware/src/main.cpp` and change the `acceptor` pointer:

**For RS232 Only:**
```cpp
// Change line 31
IBillAcceptor* acceptor = &rs232; 
```

**For Pulse Only:**
```cpp
// Change line 31
IBillAcceptor* acceptor = &pulse; 
```

> [!TIP]
> Use the **Serial Monitor** (set to 9600 baud) to watch the "Accepted..." messages. This will confirm which driver successfully parsed the bill.

---

## Switching/Configuring Modes
While the firmware is universal, specific settings can be adjusted in `firmware/src/main.cpp`:
- **Pins:** Update `BILL_ACCEPTOR_RX_PIN`, `BILL_ACCEPTOR_TX_PIN`, and `BILL_ACCEPTOR_PULSE_PIN` as needed.
- **Inhibit Control:** Set `BILL_ACCEPTOR_INHIBIT_PIN` to `-1` if the ESP32 should not control the acceptor's inhibit line (Direct Pulse mode).
