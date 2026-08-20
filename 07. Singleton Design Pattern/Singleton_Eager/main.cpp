#include "DBConnection.h"

int main() {
    DBConnection& db1 = DBConnection::getDBConnection("Chaitanya");
    DBConnection& db2 = DBConnection::getDBConnection("Rahul");
}