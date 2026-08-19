#include <iostream>
using namespace std;

// Abstraction / Interface
class DBPersistence {
public:
    virtual void save(const string& data) = 0;
    virtual ~DBPersistence() = default;
};

// Low-level implementations
class SQLPersistence : public DBPersistence {
public:
    void save(const string& data) override {
        cout << "Saving data to SQL: " << data << endl;
    }
};

class MongoPersistence : public DBPersistence {
public:
    void save(const string& data) override {
        cout << "Saving data to MongoDB: " << data << endl;
    }
};

// High-level class
class StudentService {
private:
    DBPersistence* database;

public:
    StudentService(DBPersistence* database) : database(database) {}

    void saveStudent(const string& data) {
        // Business logic can go here
        database->save(data);
    }
};

int main() {

    DBPersistence* database = new MongoPersistence();

    StudentService studentService(database);

    studentService.saveStudent("Name: Chaitanya, Age: 21");

    delete database;
}