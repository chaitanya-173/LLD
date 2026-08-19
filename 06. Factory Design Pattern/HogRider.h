#pragma once
#include "Troop.h"

class HogRider : public Troop {
public:
    HogRider() : Troop("Hog Rider", 200, 60) {}

    void attack() override {
        cout << name << " smashes defenses causing " << damage << " damage!" << endl;
    }

    void move() override {
        cout << name << " moves fast and jumps over walls" << endl;
    }
};