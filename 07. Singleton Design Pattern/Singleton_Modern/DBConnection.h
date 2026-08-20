#pragma once
#include <iostream>
#include <string>
using namespace std;

class DBConnection {
private:
    DBConnection(string name);

public:
    static DBConnection& getDBConnection(string name);
};