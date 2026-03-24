/**
 * @file main.cpp
 * @brief Main application entry point for the ESP32 Vending Controller.
 * @details This firmware manages a bill acceptor and cash persistence using NVS.
 */

#include <Arduino.h>
#include "universal_bill_acceptor.h"
#include "cash_storage.h"
#include "protocol_map.h"

// --- Hardware Definitions ---
#define BILL_ACCEPTOR_RX_PIN 16
#define BILL_ACCEPTOR_TX_PIN 17
#define BILL_ACCEPTOR_PULSE_PIN 27  // Switched to D27 to avoid onboard LED/strapping noise
#define BILL_ACCEPTOR_INHIBIT_PIN 5

// --- NVS Storage Definitions ---
#define STORAGE_NAMESPACE "vending"
#define STORAGE_KEY_CASH "totalCash"

// --- Global Objects ---
RS232BillAcceptor rs232(Serial2, 9600, BILL_ACCEPTOR_RX_PIN, BILL_ACCEPTOR_TX_PIN);
PulseBillAcceptor pulse(BILL_ACCEPTOR_PULSE_PIN, BILL_ACCEPTOR_INHIBIT_PIN);

#if defined(MODE_RS232_ONLY)
    IBillAcceptor* acceptor = &rs232;
#elif defined(MODE_PULSE_ONLY)
    IBillAcceptor* acceptor = &pulse;
#else
    // Default to Universal Mode
    UniversalBillAcceptor universal(rs232, pulse);
    IBillAcceptor* acceptor = &universal;
#endif

CashStorage storage(STORAGE_NAMESPACE, STORAGE_KEY_CASH);

// --- Global State ---
float totalCash = 0.0;
uint8_t lastCode = 0;

void setup() {
    Serial.begin(9600);
    Serial.println("Vending Controller Initializing...");

    // Initialize persistence and load previous total
    storage.begin();
    totalCash = storage.loadTotal();
    Serial.printf("Loaded previous total: %.2f\n", totalCash);

    // Initialize the bill acceptor
    acceptor->begin();
    acceptor->enable();

    Serial.println("Initialization complete. Insert a bill now...");
}

void loop() {
    uint8_t code = acceptor->read();

    // Process code only if it's new
    if (code != lastCode && code != STATUS_IDLING) {
        lastCode = code;
        int billValue = 0;

        if (code == CODE_UNKNOWN) {
            Serial.println("[ERROR] Unknown bill code or pulse count mismatch.");
        } else {
            switch (code) {
                case CODE_BILL_5:   billValue = 5;   break;
                case CODE_BILL_10:  billValue = 10;  break;
                case CODE_BILL_20:  billValue = 20;  break;
                case CODE_BILL_50:  billValue = 50;  break;
                case CODE_BILL_100: billValue = 100; break;
                case CODE_BILL_200: billValue = 200; break;
                case CODE_BILL_500: billValue = 500; break;

                case STATUS_REJECTING:    Serial.println("Status: Bill Rejected");    break;
                case STATUS_JAMMED:       Serial.println("Status: ERROR - Jammed");   break;
                case STATUS_STACKER_FULL: Serial.println("Status: ERROR - Stacker Full"); break;
            }
        }

        if (billValue > 0) {
            totalCash += billValue;
            storage.saveTotal(totalCash); // Persist after every transaction
            Serial.printf(">>> Accepted %d. New total: %.2f\n", billValue, totalCash);
        }
        
        delay(1000); // 1 second gap between bills
    }
}
