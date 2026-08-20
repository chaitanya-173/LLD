#pragma once
#include <iostream>
#include <string>
#include <mutex>
using namespace std;

class DBConnection {
private:
    static DBConnection* connection;
    static mutex mtx;

    DBConnection(string name);

public:
    static DBConnection* getDBConnection(string name);
};