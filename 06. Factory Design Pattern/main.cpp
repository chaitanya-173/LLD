#include <iostream>
#include <string>

#include "TroopFactory.h"

using namespace std;

int main() {
    cout << "Enter troop type (Barbarian, Archer, Wizard, HogRider): ";

    string type;
    cin >> type;

    Troop* troop = TroopFactory::createTroop(type);

    troop->move();
    troop->attack();

    delete troop;

    return 0;
}