#pragma once
#include "PaymentStrategy.h"

class PayPalPayment : public PaymentStrategy {
private:
    string email;

public:
    PayPalPayment(string email) : email(email) {}

    void pay(double amount) override {
        cout << "Processing $" << amount << " payment through PayPal for " << email << "." << endl;
    }
};