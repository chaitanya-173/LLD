#include <iostream>
#include <vector>
using namespace std;

class Product {
public:
    string name;
    int price;

    Product(string name, int price) {
        this->name = name;
        this->price = price;
    }
};

class ShoppingCart {
private:
    vector<Product*> products;

public:
    void addProduct(Product* p) {
        products.push_back(p);
    }

    vector<Product*> getProducts() {
        return products;
    }

    double calculateTotal() {
        double total = 0;
        for(auto p : products) {
            total += p->price;
        }
        return total;
    }
};

class ShoppingCartPrinter {
private:
    ShoppingCart* cart;

public:
    ShoppingCartPrinter(ShoppingCart* cart) {
        this->cart = cart;
    }

    void printInvoice() {
        cout << "Shopping cart invoice:" << endl;
        for(auto p : cart->getProducts()) {
            cout << p->name << " Rs." << p->price << endl;
        }
        cout << "Total: Rs." << cart->calculateTotal() << endl;
    }
};

class Persistence {
private:
    ShoppingCart* cart;

public:
    virtual void save(ShoppingCart* cart) = 0;
};

class SQLPersistence : public Persistence {
public:
    void save(ShoppingCart* cart) override {
        cout << "Saving shopping cart to SQL DB..." << endl;
    }
};

class MongoDBPersistence : public Persistence {
public:
    void save(ShoppingCart* cart) override {
        cout << "Saving shopping cart to MongoDB..." << endl;
    }
};

class FilePersistence : public Persistence {
public:
    void save(ShoppingCart* cart) override {
        cout << "Saving shopping cart to File..." << endl;
    }
}; 

int main() {
    ShoppingCart* cart = new ShoppingCart();

    cart->addProduct(new Product("Laptop", 55000));
    cart->addProduct(new Product("Mouse", 800));

    ShoppingCartPrinter* printer = new ShoppingCartPrinter(cart);
    printer->printInvoice();

    Persistence* sql = new SQLPersistence();
    Persistence* mongo = new SQLPersistence();
    Persistence* file = new SQLPersistence();

    sql->save(cart);
    mongo->save(cart);
    file->save(cart);
}