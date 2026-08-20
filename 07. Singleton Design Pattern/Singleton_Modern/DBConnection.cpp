#include "DBConnection.h"

DBConnection::DBConnection(string name) {
    cout << "Connection to DB is established by " << name << endl;
}

DBConnection& DBConnection::getDBConnection(string name) {
    static DBConnection connection(name);
    return connection;
}