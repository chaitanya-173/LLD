#pragma once

#include "Troop.h"
#include "Barbarian.h"
#include "Archer.h"
#include "Wizard.h"
#include "HogRider.h"

#include <stdexcept>
#include <string>

using namespace std;

class TroopFactory {
public:
    static Troop* createTroop(const string& type) {

        if (type == "Barbarian") {
            return new Barbarian();
        }
        else if (type == "Archer") {
            return new Archer();
        }
        else if (type == "Wizard") {
            return new Wizard();
        }
        else if (type == "HogRider") {
            return new HogRider();
        }
        else {
            throw invalid_argument("Unknown troop type: " + type);
        }
    }
};