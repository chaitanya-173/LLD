#pragma once
#include "PaymentStrategy.h"

class PaymentProcessor {
private:
    PaymentStrategy* strategy;

public:
    PaymentProcessor(PaymentStrategy* strategy) : strategy(strategy) {}

    void setStrategy(PaymentStrategy* strategy) {
        this->strategy = strategy;
    }

    void processPayment(double amount) {
        strategy->pay(amount);
    }
};