#include <iostream>
using namespace std;

// Interface / Abstraction
class Payment {
public:
    virtual void pay(double amount) = 0;
    virtual ~Payment() = default;
};

// Low-level implementation
class UPI : public Payment {
public:
    void pay(double amount) override {
        cout << "Paid ₹" << amount << " using UPI" << endl;
    }
};

class Card : public Payment {
public:
    void pay(double amount) override {
        cout << "Paid ₹" << amount << " using Card" << endl;
    }
};

// High-level class
class Order {
private:
    double amount;
    Payment* payment;

public:
    Order(double amount, Payment* payment) : amount(amount), payment(payment) {}

    void checkout() {
        cout << "Processing order of ₹" << amount << endl;
        payment->pay(amount);
        cout << "Order placed successfully" << endl;
    }
};

int main() {

    // Main decides which concrete payment to use
    Payment* payment = new UPI();

    // Inject Payment dependency into Order
    Order order(2499, payment);

    order.checkout();

    delete payment;
}