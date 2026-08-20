#include "DBConnection.h"

DBConnection DBConnection::connection("System");

DBConnection::DBConnection(string name) {
    cout << "Connection to DB is established by " << name << endl;
}

DBConnection& DBConnection::getDBConnection(string name) {
    return connection;
}