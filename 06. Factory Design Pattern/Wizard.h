#pragma once
#include "Troop.h"

class Wizard : public Troop {
public:
    Wizard() : Troop("Wizard", 120, 70) {}

    void attack() override {
        cout << name << " casts a fireball causing " << damage << " magical damage!" << endl;
    }

    void move() override {
        cout << name << " teleports short distances!" << endl;
    }
};