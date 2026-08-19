#pragma once
#include <iostream>
#include <string>
using namespace std;

class Troop {
protected:
    string name;
    int health, damage;

public:
    Troop(string name, int health, int damage) : name(name), health(health), damage(damage) {}

    virtual void attack() = 0;
    virtual void move() = 0;

    virtual ~Troop() = default;
};