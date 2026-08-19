#include <iostream>
#include <string>

#include "PaymentStrategy.h"
#include "CreditCardPayment.h"
#include "GooglePayPayment.h"
#include "PayPalPayment.h"
#include "PaymentProcessor.h"

using namespace std;

int main() {
    double amount;

    cout << "Enter payment amount: $";
    cin >> amount;
    cin.ignore();

    cout << "Select payment method:" << endl;
    cout << "1. PayPal" << endl;
    cout << "2. GPay" << endl;
    cout << "3. Credit Card" << endl;

    cout << "Your choice: ";
    int choice;
    cin >> choice;
    cin.ignore();

    PaymentStrategy* strategy = nullptr;

    // Factory Design Pattern can be used here
    switch (choice) {
        case 1: {
            string email;

            cout << "Enter your PayPal email: ";
            getline(cin, email);

            strategy = new PayPalPayment(email);
            break;
        }

        case 2: {
            string pin;

            cout << "Enter your pin: ";
            getline(cin, pin);

            strategy = new GooglePayPayment(pin);
            break;
        }

        case 3: {
            string name, card, cvv, expiry;

            cout << "Enter your name: ";
            getline(cin, name);
            cout << "Enter your card number: ";
            getline(cin, card);
            cout << "Enter your CVV: ";
            getline(cin, cvv);
            cout << "Enter your expiry date (MM/YY): ";
            getline(cin, expiry);

            strategy = new CreditCardPayment(name, card, cvv, expiry);
            break;
        }

        default:    
            cout << "Invalid choice. Exiting." << endl;
            return 0;
    }

    // Context
    PaymentProcessor processor(strategy);
    processor.processPayment(amount);

    delete strategy;

    return 0;
}