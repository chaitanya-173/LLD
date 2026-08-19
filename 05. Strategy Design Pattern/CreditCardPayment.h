#pragma once
#include "PaymentStrategy.h"

class CreditCardPayment : public PaymentStrategy {
private:
    string name, cardNumber, cvv, expiryDate;

public:
    CreditCardPayment(string name, string cardNumber, string cvv, string expiryDate) : name(name), cardNumber(cardNumber), cvv(cvv), expiryDate(expiryDate) {}

    void pay(double amount) override {
        cout << "Processing $" << amount << " payment with Credit Card belonging to " << name << "." << endl;
    }
};