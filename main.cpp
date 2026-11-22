
#include <iostream>
#include <string>
#include <sqlite3.h>
#include <limits>
#include <iomanip>

using namespace std;

sqlite3* db;

// Create expenses table
void createTable()
{
    const char* sql =
        "CREATE TABLE IF NOT EXISTS expenses ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "amount REAL NOT NULL,"
        "category TEXT NOT NULL,"
        "description TEXT NOT NULL);";

    char* errorMessage = nullptr;

    if (sqlite3_exec(db, sql, nullptr, nullptr, &errorMessage) != SQLITE_OK)
    {
        cout << "Table creation error: " << errorMessage << endl;
        sqlite3_free(errorMessage);
    }
}

// Add expense
void addExpense()
{
    double amount;
    string category, description;

    cout << "\nEnter amount (Rs.): ";
    cin >> amount;

    if (cin.fail() || amount <= 0)
    {
        cout << "Invalid amount!\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter category: ";
    getline(cin, category);

    cout << "Enter description: ";
    getline(cin, description);

    const char* sql =
        "INSERT INTO expenses (amount, category, description) "
        "VALUES (?, ?, ?);";

    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        cout << "Error preparing query!\n";
        return;
    }

    sqlite3_bind_double(stmt, 1, amount);
    sqlite3_bind_text(stmt, 2, category.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, description.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmt) == SQLITE_DONE)
        cout << "\nExpense added successfully!\n";
    else
        cout << "Error adding expense!\n";

    sqlite3_finalize(stmt);
}

// View all expenses
void viewExpenses()
{
    const char* sql =
        "SELECT id, amount, category, description FROM expenses;";

    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        cout << "Error retrieving expenses!\n";
        return;
    }

    cout << "\n========== ALL EXPENSES ==========\n";

    bool found = false;

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        found = true;

        cout << "ID: " << sqlite3_column_int(stmt, 0)
             << " | Amount: Rs. " << fixed << setprecision(2)
             << sqlite3_column_double(stmt, 1)
             << " | Category: " << sqlite3_column_text(stmt, 2)
             << " | Description: " << sqlite3_column_text(stmt, 3)
             << endl;
    }

    if (!found)
        cout << "No expenses found.\n";

    sqlite3_finalize(stmt);
}

// Calculate total spending
void showTotal()
{
    const char* sql = "SELECT COALESCE(SUM(amount), 0) FROM expenses;";

    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        cout << "Error calculating total!\n";
        return;
    }

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        cout << "\nTotal Spending: Rs. "
             << fixed << setprecision(2)
             << sqlite3_column_double(stmt, 0) << endl;
    }

    sqlite3_finalize(stmt);
}

// Delete expense using ID
void deleteExpense()
{
    int id;

    cout << "\nEnter Expense ID to delete: ";
    cin >> id;

    if (cin.fail() || id <= 0)
    {
        cout << "Invalid ID!\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    const char* sql = "DELETE FROM expenses WHERE id = ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        cout << "Error preparing delete query!\n";
        return;
    }

    sqlite3_bind_int(stmt, 1, id);

    if (sqlite3_step(stmt) == SQLITE_DONE)
    {
        if (sqlite3_changes(db) > 0)
            cout << "Expense deleted successfully!\n";
        else
            cout << "Expense ID not found!\n";
    }
    else
    {
        cout << "Error deleting expense!\n";
    }

    sqlite3_finalize(stmt);
}

// Search expenses by category
void searchByCategory()
{
    string category;

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "\nEnter category to search: ";
    getline(cin, category);

    const char* sql =
        "SELECT id, amount, category, description "
        "FROM expenses WHERE category = ?;";

    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        cout << "Error preparing search query!\n";
        return;
    }

    sqlite3_bind_text(stmt, 1, category.c_str(), -1, SQLITE_TRANSIENT);

    cout << "\n====== SEARCH RESULTS ======\n";

    bool found = false;

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        found = true;

        cout << "ID: " << sqlite3_column_int(stmt, 0)
             << " | Amount: Rs. " << fixed << setprecision(2)
             << sqlite3_column_double(stmt, 1)
             << " | Category: " << sqlite3_column_text(stmt, 2)
             << " | Description: " << sqlite3_column_text(stmt, 3)
             << endl;
    }

    if (!found)
        cout << "No expenses found in this category.\n";

    sqlite3_finalize(stmt);
}

// Main function
int main()
{
    if (sqlite3_open("expenses.db", &db) != SQLITE_OK)
    {
        cout << "Error opening database!\n";
        return 1;
    }

    cout << "Database connected successfully!\n";

    createTable();

    int choice;

    do
    {
        cout << "\n===== PERSONAL EXPENSE TRACKER =====\n";
        cout << "1. Add Expense\n";
        cout << "2. View Expenses\n";
        cout << "3. View Total Spending\n";
        cout << "4. Delete Expense\n";
        cout << "5. Search by Category\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";

        cin >> choice;

        if (cin.fail())
        {
            cout << "Please enter a valid number!\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            choice = 0;
            continue;
        }

        switch (choice)
        {
            case 1:
                addExpense();
                break;

            case 2:
                viewExpenses();
                break;

            case 3:
                showTotal();
                break;

            case 4:
                deleteExpense();
                break;

            case 5:
                searchByCategory();
                break;

            case 6:
                cout << "Exiting program...\n";
                break;

            default:
                cout << "Invalid choice!\n";
        }

    } while (choice != 6);

    sqlite3_close(db);

    return 0;
}
