#pragma once
#include "Troop.h"

class Archer : public Troop {
private:
    int range;

public:
    Archer() : Troop("Archer", 100, 40), range(5) {}

    void attack() override {
        cout << name << " shoots an arrow from " << range << " units away causing " << damage << " damage!" << endl;
    }

    void move() override {
        cout << name << " moves stealthily into shooting position." << endl;
    }
};