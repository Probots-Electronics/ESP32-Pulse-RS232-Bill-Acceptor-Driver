/**
 * @file pulse_bill_acceptor.cpp
 * @brief Implementation of the PulseBillAcceptor class.
 */

#include "pulse_bill_acceptor.h"

// Static member initialization
PulseBillAcceptor* PulseBillAcceptor::_instance = nullptr;

/**
 * @brief Construct a new Pulse Bill Acceptor object.
 */
PulseBillAcceptor::PulseBillAcceptor(int8_t pulsePin, int8_t inhibitPin) 
    : _pulsePin(pulsePin), _inhibitPin(inhibitPin), _pulseCount(0), _lastPulseTime(0), _lastInterruptTime(0), _lastPrintedCount(0), _lastReadTime(0) {
    _instance = this;
}

/**
 * @brief Initializes the pulse communication.
 */
void PulseBillAcceptor::begin() {
    pinMode(_pulsePin, INPUT_PULLUP);
    if (_inhibitPin != -1) {
        pinMode(_inhibitPin, OUTPUT);
        digitalWrite(_inhibitPin, HIGH); // Start in Inhibit state (Active High = Stop)
    }

    attachInterrupt(digitalPinToInterrupt(_pulsePin), handlePulseISR, FALLING);
}

/**
 * @brief Enables the bill acceptor.
 * @details Sends a LOW signal to the Inhibit pin (for Active High configuration).
 */
void PulseBillAcceptor::enable() {
    if (_inhibitPin != -1) {
        digitalWrite(_inhibitPin, LOW); // Send Low to Enable (for Active High default)
    }
}

/**
 * @brief Disables the bill acceptor.
 * @details Sends a HIGH signal to the Inhibit pin (for Active High configuration).
 */
void PulseBillAcceptor::disable() {
    if (_inhibitPin != -1) {
        digitalWrite(_inhibitPin, HIGH); // Send High to Inhibit/Stop (for Active High default)
    }
}

void IRAM_ATTR PulseBillAcceptor::handlePulse() {
    uint32_t interruptTime = millis();
    // 40ms lockout to filter high-frequency noise/bouncing
    if (interruptTime - _lastInterruptTime > 40) {
        _pulseCount++;
        _lastPulseTime = interruptTime;
    }
    _lastInterruptTime = interruptTime;
}

void PulseBillAcceptor::handlePulseISR() {
    if (_instance) {
        _instance->handlePulse();
    }
}

/**
 * @brief Logic to monitor and finalize the pulse count.
 */
uint8_t PulseBillAcceptor::read() {
    unsigned long now = millis();
    
    // Debug: Only show if the count is new
    if (_pulseCount > 0 && _pulseCount != _lastPrintedCount) {
        _lastPrintedCount = _pulseCount;
        Serial.printf("[PULSE DEBUG] Current Count: %d, Silence: %lu ms\n", 
                      _pulseCount, (now - _lastPulseTime));
    }

    // Process if we haven't seen a pulse for 1.0 second (Balanced speed/stability)
    if (_pulseCount > 0 && (now - _lastPulseTime > 1000)) {
        int count = _pulseCount;
        _pulseCount = 0; // Reset for next bill
        _lastPrintedCount = 0;

        // DEBUG: Print raw count to help identify noise vs real bill
        if (count > 2) {
            Serial.print("[PULSE DEBUG] Raw count detected: ");
            Serial.println(count);
        }

        // Pulse-to-value mapping (1 Pulse = ₹1 base)
        // Ranges broadened to handle noise (e.g., 243 detected for ₹200)
        if (count >= 3 && count <= 8)       return CODE_BILL_5;
        if (count >= 9 && count <= 15)      return CODE_BILL_10;
        if (count >= 16 && count <= 35)     return CODE_BILL_20;
        if (count >= 36 && count <= 85)     return CODE_BILL_50;
        if (count >= 86 && count <= 165)    return CODE_BILL_100;
        if (count >= 166 && count <= 350)   return CODE_BILL_200;
        if (count >= 351 && count <= 1000)  return CODE_BILL_500;
        
        // If it's a tiny number, it's likely noise
        if (count < 3) return STATUS_IDLING;

        return CODE_UNKNOWN;
    }

    return STATUS_IDLING;
}
