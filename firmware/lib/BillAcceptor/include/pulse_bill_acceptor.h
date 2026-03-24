/**
 * @file pulse_bill_acceptor.h
 * @brief Header for the PulseBillAcceptor class.
 */

#ifndef PULSE_BILL_ACCEPTOR_H
#define PULSE_BILL_ACCEPTOR_H

#include <Arduino.h>
#include "bill_acceptor_interface.h"
#include "protocol_map.h"

/**
 * @class PulseBillAcceptor
 * @brief Implements IBillAcceptor using pulse counting.
 */
class PulseBillAcceptor : public IBillAcceptor {
public:
    /**
     * @brief Construct a new Pulse Bill Acceptor object.
     * @param pulsePin GPIO pin connected to the pulse output of the acceptor.
     * @param inhibitPin GPIO pin connected to the inhibit input of the acceptor.
     */
    PulseBillAcceptor(int8_t pulsePin, int8_t inhibitPin);

    void begin() override;
    void enable() override;
    void disable() override;
    uint8_t read() override;

    /**
     * @brief Interrupt handler for pulse detection.
     */
    void IRAM_ATTR handlePulse();

private:
    int8_t _pulsePin;
    int8_t _inhibitPin;
    volatile uint32_t _pulseCount;
    volatile uint32_t _lastPulseTime;
    volatile uint32_t _lastInterruptTime; // For de-bouncing
    uint32_t _lastPrintedCount;           // For debugging
    uint32_t _lastReadTime;

    // Static wrapper for ISR
    static void handlePulseISR();
    static PulseBillAcceptor* _instance;
};

#endif // PULSE_BILL_ACCEPTOR_H
