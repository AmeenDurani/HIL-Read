#include <iostream>
#include <sqlite3.h>
#include <string>
using namespace std;

int main() {
    sqlite3* db;
    const char* sql;
    int rc;
    char* errMsg = nullptr;

    // Open database
    rc = sqlite3_open("myDB.db", &db);
    if (rc != SQLITE_OK) {
        cerr << "Can't open database: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return -1;
    } else {
        cout << "DB opened successfully" << endl;
    }

    // Create table if not exists
    sql = "CREATE TABLE IF NOT EXISTS logtable ("
          "No INTEGER PRIMARY KEY AUTOINCREMENT, "
          "Timestamp TEXT NOT NULL, "
          "Log TEXT);";
    rc = sqlite3_exec(db, sql, nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        cerr << "SQL Error: " << errMsg << endl;
        sqlite3_free(errMsg);
        sqlite3_close(db);
        return -1;
    } else {
        cout << "Table creation successful" << endl;
    }

    // Sample data
    string timestp = "12:34:20 pm";
    string logtxt = "Temperature: 0.000000";

    // Prepare INSERT statement
    const char* sql_insert = "INSERT INTO logtable (Timestamp, Log) VALUES (?, ?);";

    // Prepare statement
    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, sql_insert, -1, &stmt, nullptr);
    if (rc == SQLITE_OK) {
        // Bind parameters
        sqlite3_bind_text(stmt, 1, timestp.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, logtxt.c_str(), -1, SQLITE_TRANSIENT);

        // Execute statement
        rc = sqlite3_step(stmt);
        if (rc != SQLITE_DONE) {
            cerr << "Error inserting data: " << sqlite3_errmsg(db) << endl;
        } else {
            cout << "Insertion successful" << endl;
        }

        // Finalize statement
        sqlite3_finalize(stmt);
    } else {
        cerr << "Failed to prepare stmt: " << sqlite3_errmsg(db) << endl;
    }

    // Close database connection
    sqlite3_close(db);

    return 0;
}
