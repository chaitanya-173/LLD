#pragma once
#include "PaymentStrategy.h"

class GooglePayPayment : public PaymentStrategy {
private:
    string pin;

public:
    GooglePayPayment(string pin) : pin(pin) {}

    void pay(double amount) override {
        cout << "Processing $" << amount << " payment through GooglePay." << endl;
    }
};