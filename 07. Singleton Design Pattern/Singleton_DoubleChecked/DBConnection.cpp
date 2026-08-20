#include "DBConnection.h"

DBConnection* DBConnection::connection = nullptr;
mutex DBConnection::mtx;

DBConnection::DBConnection(string name) {
    cout << "Connection to DB is established by " << name << endl;
}

DBConnection* DBConnection::getDBConnection(string name) {
    if (connection == nullptr) {
        lock_guard<mutex> lock(mtx);
        if (connection == nullptr) {
            connection = new DBConnection(name);
        }
    }
    return connection;
}