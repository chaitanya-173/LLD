#pragma once
#include "Troop.h"

class Barbarian : public Troop {
public:
    Barbarian() : Troop("Barbarian", 150, 50) {}

    void attack() override {
        cout << name << " swings sword causing " << damage << " damage!" << endl;
    }

    void move() override {
        cout << name << " charges quickly towards the enemy!" << endl;
    }
};