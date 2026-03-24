/**
 * @file universal_bill_acceptor.h
 * @brief Wrapper class to handle both RS232 and Pulse communication simultaneously.
 */

#ifndef UNIVERSAL_BILL_ACCEPTOR_H
#define UNIVERSAL_BILL_ACCEPTOR_H

#include "bill_acceptor_interface.h"
#include "bill_acceptor.h"
#include "pulse_bill_acceptor.h"

/**
 * @class UniversalBillAcceptor
 * @brief Implements IBillAcceptor by polling both RS232 and Pulse drivers.
 */
class UniversalBillAcceptor : public IBillAcceptor {
public:
    UniversalBillAcceptor(RS232BillAcceptor& rs232, PulseBillAcceptor& pulse)
        : _rs232(rs232), _pulse(pulse) {}

    void begin() override {
        _rs232.begin();
        _pulse.begin();
    }

    void enable() override {
        _rs232.enable();
        _pulse.enable();
    }

    void disable() override {
        _rs232.disable();
        _pulse.disable();
    }

    uint8_t read() override {
        // Poll RS232 first
        uint8_t code = _rs232.read();
        if (code != STATUS_IDLING && code != CODE_UNKNOWN) {
            return code;
        }

        // Then poll Pulse
        code = _pulse.read();
        return code;
    }

private:
    RS232BillAcceptor& _rs232;
    PulseBillAcceptor& _pulse;
};

#endif // UNIVERSAL_BILL_ACCEPTOR_H
