#include <iostream>
using namespace std;

class TwoDshape {
public:
    virtual double area() = 0;
};

class ThreeDshape {
public:
    virtual double area() = 0;
    virtual double volume() = 0;
};

class Square : public TwoDshape {
private:
    double side;

public:
    Square(double s) : side(s) {}

    double area() override {
        return side * side;
    }
};

class Rectangle : public TwoDshape {
private:
    double length, width;

public:
    Rectangle(double l, double w) : length(l), width(w) {};

    double area() override {
        return length * width;
    }
};

class Cube : public ThreeDshape {
private:
    double side;

public:
    Cube(double s) : side(s) {};

    double area() override {
        return 6 * side * side;
    }

    double volume() override {
        return side * side * side;
    }
};

int main() {
    TwoDshape* square = new Square(5);
    TwoDshape* rectangle = new Rectangle(4, 6);
    ThreeDshape* cube = new Cube(3);

    cout << "Square area: " << square->area() << endl;
    cout << "Rectangle area: " << rectangle->area() << endl;
    cout << "Cube area: " << cube->area() << endl;
    cout << "Cube volume: " << cube->volume() << endl;

    return 0;
}