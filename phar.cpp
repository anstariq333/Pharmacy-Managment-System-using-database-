#include "sqlite3.h"
#include <iostream>
#include <string.h>
#include <string>
using namespace std;

struct Med
{
    int MedID;
    string MedName;
    string Exp_date;
    float Price;
    string MedCompny;
    int Storage;
};
int AddMedicine()
{
    Med newMedicine;
    cin.ignore();
    cout << "Enter Medicine Name: ";
    getline(cin, newMedicine.MedName);

    cout << "Enter Expiry Date (DD-MM-YYYY): ";
    getline(cin, newMedicine.Exp_date);

    cout << "Enter Price: ";
    cin >> newMedicine.Price;
    cin.ignore();

    cout << "Enter Medicine Company: ";
    getline(cin, newMedicine.MedCompny);

    cout << "Enter Storage: ";
    cin >> newMedicine.Storage;

    sqlite3 *db;
    int rc = sqlite3_open("storage.db", &db);
    char *errMsg = nullptr;

    if (rc != SQLITE_OK)
    {
        std::cerr << "Error opening SQLite3 database: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return 1;
    }

    rc = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS Medicine (MedID INTEGER PRIMARY KEY AUTOINCREMENT, MedName  TEXT  NOT NULL, Exp_date INTEGER NOT NULL, Price REAL NOT NULL, MedCompny TEXT, Storage INTEGER NOT NULL);", nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK)
    {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        sqlite3_close(db);
        return 1;
    }

    rc = sqlite3_exec(db, ("INSERT INTO Medicine (MedName, Exp_date, Price, MedCompny, Storage) VALUES ('" + newMedicine.MedName + "', '" + newMedicine.Exp_date + "', " + to_string(newMedicine.Price) + ", '" + newMedicine.MedCompny + "', " + to_string(newMedicine.Storage) + ")").c_str(), nullptr, nullptr, &errMsg);

    if (rc != SQLITE_OK)
    {
        cerr << "SQL error: " << errMsg << endl;
        sqlite3_free(errMsg);
        sqlite3_close(db);
        return 1;
    }

    if (sqlite3_changes(db) > 0)
    {
        cout << "Medicine Inserted successfully" << endl;
    }
    else
    {
        cout << "Medicine not inserted" << endl;
    }

    sqlite3_close(db);
    return 0;
}

int DeleteMedicine()
{
    cin.ignore();
    string medicineName;
    cout << "Enter the name of the medicine to delete: ";
    getline(cin, medicineName);

    sqlite3 *db;
    int rc = sqlite3_open("Storage.db", &db);
    char *errMsg = nullptr;

    if (rc)
    {
        std::cerr << "Error opening SQLite3 database: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return 1;
    }
    else
    {
        std::cout << "Opened SQLite3 database successfully" << std::endl;
    }
    rc = sqlite3_exec(db, ("DELETE from Medicine where MedName = '" + medicineName + "'").c_str(), nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK)
    {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        sqlite3_close(db);
        return 1;
    }
    if (sqlite3_changes(db) > 0)
    {
        cout << "Medicin  Deleted  successfully" << endl;
    }
    else
    {
        cout << "Medicine not found " << endl;
    }

    sqlite3_close(db);
    return 0;
}
bool CustomerLogin()
{

    string CustomerName;
    int password;

    cout << "Please confirm Customer Name and Password to login" << endl;
    cout << "Enter   Customer Name:" << endl;
    cin.ignore();
    getline(cin, CustomerName);
    cout << "Enter your Password:" << endl;
    cin >> password;

    sqlite3 *db;
    int rc = sqlite3_open("storage.db", &db);
    char *errMsg = nullptr;
    bool found = false;
    if (rc != SQLITE_OK)
    {
        cerr << "Error opening SQLite3 database: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return false;
    }

    string sql = "SELECT * FROM Rigisterd_Customer WHERE  Customer_Name = '" + CustomerName + "' AND Password = " + to_string(password) + ";";
    rc = sqlite3_exec(
        db, sql.c_str(), [](void *data, int argc, char **argv, char **azColName) -> int
        {
            // This callback is called for each row returned by the SELECT statement
            *static_cast<bool *>(data) = true; // Set the flag to true
            return 0; },
        &found, &errMsg);

    if (rc != SQLITE_OK)
    {
        cerr << "SQL error: " << errMsg << endl;
        sqlite3_free(errMsg);
        sqlite3_close(db);
        return false;
    }

    sqlite3_close(db);

    if (found)
    {
        cout << "***** You are able to perform Customer Activities *****" << endl;
        return true;
    }
    else
    {
        cout << "You entered the wrong name or password" << endl;
        cout << "You can't perform Customer activities" << endl;
        return false;
    }
}

int ViewMedicine()
{
    sqlite3 *db;
    int rc = sqlite3_open("storage.db", &db);
    char *errMsg = nullptr;

    if (rc != SQLITE_OK)
    {
        cerr << "Error opening SQLite3 database: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return 1;
    }

    rc = sqlite3_exec(
        db, "SELECT * FROM Medicine;", [](void *data, int argc, char **argv, char **azColName) -> int
        {
        for (int i = 0; i < argc; i++)
        {
            cout << azColName[i] << " = " << (argv[i] ? argv[i] : "NULL") << endl;
        }
        cout << endl;
        return 0; },
        nullptr, &errMsg);

    if (rc != SQLITE_OK)
    {
        cerr << "SQL error: " << errMsg << endl;
        sqlite3_free(errMsg);
        sqlite3_close(db);
        return 1;
    }

    sqlite3_close(db);
    return 0;
}
int ShowCustomer()
{
    sqlite3 *db;
    int rc = sqlite3_open("storage.db", &db);
    char *errMsg = nullptr;

    if (rc != SQLITE_OK)
    {
        cerr << "Error opening SQLite3 database: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return 1;
    }

    rc = sqlite3_exec(
        db, "SELECT * FROM Rigisterd_Customer", [](void *data, int argc, char **argv, char **azColName) -> int
        {
        for (int i = 0; i < argc; i++)
        {
            cout << azColName[i] << " = " << (argv[i] ? argv[i] : "NULL") << endl;
        }
        cout << endl;
        return 0; },
        nullptr, &errMsg);

    if (rc != SQLITE_OK)
    {
        cerr << "SQL error: " << errMsg << endl;
        sqlite3_free(errMsg);
        sqlite3_close(db);
        return 1;
    }

    sqlite3_close(db);
    return 0;
}
int UpdateMedicine()
{
    string SearchedName;
    cout << "Enter the name of the medicine to update: ";
    cin.ignore();
    getline(cin, SearchedName);

    Med UpdateMedicine;
    cout << "Enter Update Medicine Name: ";
    getline(cin, UpdateMedicine.MedName);

    cout << "Enter Update Expiry Date (YYYY-MM-DD): ";
    getline(cin, UpdateMedicine.Exp_date);

    cout << "Enter Update Price: ";
    cin >> UpdateMedicine.Price;
    cin.ignore();

    cout << "Enter Update Medicine Company: ";
    getline(cin, UpdateMedicine.MedCompny);

    cout << "Enter Update Storage: ";
    cin >> UpdateMedicine.Storage;

    sqlite3 *db;
    int rc = sqlite3_open("storage.db", &db);
    char *errMsg = nullptr;

    if (rc)
    {
        std::cerr << "Error opening SQLite3 database: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return 1;
    }
    else
    {
        std::cout << "Opened SQLite3 database successfully" << std::endl;
    }

    string sql = "UPDATE Medicine SET MedName = '" + UpdateMedicine.MedName + "', Exp_date = '" + UpdateMedicine.Exp_date + "', Price = " + to_string(UpdateMedicine.Price) + ", MedCompny = '" + UpdateMedicine.MedCompny + "', Storage = " + to_string(UpdateMedicine.Storage) + " WHERE MedName = '" + SearchedName + "'";
    rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);

    if (rc != SQLITE_OK)
    {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        sqlite3_close(db);
        return 1;
    }

    if (sqlite3_changes(db) > 0)
    {
        cout << "Medicine Updated successfully" << endl;
    }
    else
    {
        cout << "Medicine not found or not updated" << endl;
    }

    sqlite3_close(db);
    return 0;
}
int SearchMedicine()
{
    cin.ignore();
    string SearchedName;
    cout << "Enter the name of the medicine to search: ";
    getline(cin, SearchedName);

    sqlite3 *db;
    int rc = sqlite3_open("storage.db", &db);
    char *errMsg = nullptr;
    bool found = false;

    if (rc != SQLITE_OK)
    {
        cerr << "Error opening SQLite3 database: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return 1;
    }

    string sql = "SELECT * FROM Medicine WHERE MedName = '" + SearchedName + "';";
    rc = sqlite3_exec(
        db, sql.c_str(), [](void *data, int argc, char **argv, char **azColName) -> int
        {
            for (int i = 0; i < argc; i++)
            {
                cout << azColName[i] << " = " << (argv[i] ? argv[i] : "NULL") << endl;
            }
            cout << endl;
            *static_cast<bool *>(data) = true; // Set the flag to true
            return 0; },
        &found, &errMsg);

    if (rc != SQLITE_OK)
    {
        cerr << "SQL error: " << errMsg << endl;
        sqlite3_free(errMsg);
        sqlite3_close(db);
        return 1;
    }

    if (!found)
    {
        cout << "Medicine not found." << endl;
    }
    else
    {
        cout << "Medicine searched successfully." << endl;
    }

    sqlite3_close(db);
    return 0;
} // g++ phar.cpp -o phar.exe sqlite3.dll

bool AdminLogin()
{
    string name = "Admin";
    int password = 12345;

    cout << "Please confirm your Name and Password to login" << endl;
    cout << "Enter your Name:" << endl;
    cin >> name;
    cout << "Enter your Password:" << endl;
    cin >> password;
    if (name == "Admin" && password == 12345)
    {
        cout << "***** You are able to perform Admin activities *****" << endl;
        return true;
    }
    cout << "You entered the wrong name or password" << endl;
    cout << "You can't perform Admin activities" << endl;

    return false;
}
int PurchaseRecord()
{

    int customerID;
    cout << "If you want to see the records of a specific customer, enter the Customer ID: ";
    cin >> customerID;

    sqlite3 *db;
    int rc = sqlite3_open("storage.db", &db);
    char *errMsg = nullptr;
    bool found = false;

    if (rc != SQLITE_OK)
    {
        cerr << "Error opening SQLite3 database: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return 1;
    }

    string sql = "SELECT * FROM Purchase_Medicine WHERE  Customer_ID = " + to_string(customerID);

    rc = sqlite3_exec(
        db, sql.c_str(), [](void *data, int argc, char **argv, char **azColName) -> int
        {
            for (int i = 0; i < argc; i++)
            {
                cout << azColName[i] << " = " << (argv[i] ? argv[i] : "NULL") << endl;
            }
            cout << endl;
            *static_cast<bool *>(data) = true; // Set the flag to true
            return 0; },
        &found, &errMsg);

    if (rc != SQLITE_OK)
    {
        cerr << "SQL error: " << errMsg << endl;
        sqlite3_free(errMsg);
        sqlite3_close(db);
        return 1;
    }

    if (!found)
    {
        cout << "No purchasing records found for this date or customer ID." << endl;
    }
    else
    {
        cout << "Purchasing records found successfully." << endl;
    }

    sqlite3_close(db);
    return 0;
}
int PurchaseMedicine()
{
    int Customer_Id;
    cout << "Enter Customer_Id: ";
    cin >> Customer_Id;

    int Medicine_Id;
    cout << "Enter Medicine_Id: ";
    cin >> Medicine_Id;

    cin.ignore();
    string CurrentDate;
    cout << "Enter date: ";
    getline(cin, CurrentDate);

    int Quantity;
    cout << "Enter Quantity: ";
    cin >> Quantity;

    float Price;
    cout << "Enter Price of Medicine: ";
    cin >> Price;

    sqlite3 *db;
    char *errMsg = nullptr;

    int rc = sqlite3_open("storage.db", &db);
    if (rc != SQLITE_OK)
    {
        cerr << "Error opening SQLite3 database: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return 1;
    }

    string createTableQuery = "CREATE TABLE IF NOT EXISTS Purchase_Medicine ("
                              "Purchase_ID INTEGER PRIMARY KEY AUTOINCREMENT,"
                              "Customer_Id INTEGER,"
                              "Medicine_Id INTEGER,"
                              "Quantity INTEGER,"
                              "Price REAL,"
                              "CurrentDate TEXT,"
                              "FOREIGN KEY (Customer_Id) REFERENCES Customers(Customer_Id),"
                              "FOREIGN KEY (Medicine_Id) REFERENCES Medicines(Medicine_Id));";

    rc = sqlite3_exec(db, createTableQuery.c_str(), nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK)
    {
        cerr << "SQL error: " << errMsg << endl;
        sqlite3_free(errMsg);
        sqlite3_close(db);
        return 1;
    }

    string insertQuery = "INSERT INTO Purchase_Medicine (Customer_Id, Medicine_Id, Quantity, Price, CurrentDate) VALUES (" +
                         to_string(Customer_Id) + ", " + to_string(Medicine_Id) + ", " + to_string(Quantity) + ", " + to_string(Price) + ", '" + CurrentDate + "');";

    rc = sqlite3_exec(db, insertQuery.c_str(), nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK)
    {
        cerr << "SQL error: " << errMsg << endl;
        sqlite3_free(errMsg);
        sqlite3_close(db);
        return 1;
    }

    if (sqlite3_changes(db) > 0)
    {
        cout << "Medicine purchased successfully" << endl;
        float TotalPrice = Price * Quantity;
        cout << "You have to pay " << TotalPrice << endl;
    }
    else
    {
        cout << "Medicine not inserted" << endl;
    }

    sqlite3_close(db);
    return 0;
}

int CustomerRegistration()
{

    string CustomerName;
    int Password;

    cout << "Enter   Customer  Name:" << endl;
    cin.ignore();
    getline(cin, CustomerName);

    cout << "Enter your Password:" << endl;
    cin >> Password;

    sqlite3 *db;
    int rc = sqlite3_open("storage.db", &db);
    char *errMsg = nullptr;

    if (rc != SQLITE_OK)
    {
        std::cerr << "Error opening SQLite3 database: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return 1;
    }

    rc = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS Rigisterd_Customer (Cudtomer_ID INTEGER PRIMARY KEY AUTOINCREMENT, Customer_Name  TEXT  NOT NULL, Password INTEGER NOT NULL);", nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK)
    {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        sqlite3_close(db);
        return 1;
    }

    rc = sqlite3_exec(db, ("INSERT INTO Rigisterd_Customer (Customer_Name ,Password) VALUES ( '" + CustomerName + "', " + to_string(Password) + ")").c_str(), nullptr, nullptr, &errMsg);

    if (rc != SQLITE_OK)
    {
        cerr << "SQL error: " << errMsg << endl;
        sqlite3_free(errMsg);
        sqlite3_close(db);
        return 1;
    }

    if (sqlite3_changes(db) > 0)
    {
        cout << "YOU have Registerd successfully" << endl;
    }
    else
    {
        cout << "Customer not Rigisterd" << endl;
    }

    sqlite3_close(db);
    return 0;
}

int CheckFeedback()
{

    cin.ignore();
    string date;
    cout << "Enter the date for which you want to see the records: ";
    getline(cin, date);

    sqlite3 *db;
    int rc = sqlite3_open("storage.db", &db);
    char *errMsg = nullptr;
    bool found = false;

    if (rc != SQLITE_OK)
    {
        cerr << "Error opening SQLite3 database: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return 1;
    }

    string sql = "SELECT * FROM Feedback WHERE CurrentDate = '" + date + "' ";
    rc = sqlite3_exec(
        db, sql.c_str(), [](void *data, int argc, char **argv, char **azColName) -> int
        {
            for (int i = 0; i < argc; i++)
            {
                cout << azColName[i] << " = " << (argv[i] ? argv[i] : "NULL") << endl;
            }
            cout << endl;
            *static_cast<bool *>(data) = true; // Set the flag to true
            return 0; },
        &found, &errMsg);

    if (rc != SQLITE_OK)
    {
        cerr << "SQL error: " << errMsg << endl;
        sqlite3_free(errMsg);
        sqlite3_close(db);
        return 1;
    }

    if (!found)
    {
        cout << "No Feedback records found for this date or customer ID." << endl;
    }
    else
    {
        cout << "Feedback records found successfully." << endl;
    }

    sqlite3_close(db);
    return 0;
}

int Feedback()
{
    int Customer_ID;
    cout << " Enter Customer ID";
    cin >> Customer_ID;

    cin.ignore();
    string Feedback;
    cout << "Enter Feed back about Pharmacy or Medicine: ";
    getline(cin, Feedback);

    cin.ignore();
    string CurrentDate;
    cout << "Enter  the Today 's  date(DD-MM-YYYY) : ";
    getline(cin, CurrentDate);

    sqlite3 *db;
    int rc = sqlite3_open("storage.db", &db);
    char *errMsg = nullptr;

    if (rc != SQLITE_OK)
    {
        std::cerr << "Error opening SQLite3 database: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return 1;
    }

    rc = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS Feedback (Feedback_ID INTEGER PRIMARY KEY AUTOINCREMENT, Customer_ID INTEGER NOT NULL, FeedbackText TEXT,CurrentDate INTEGER,FOREIGN KEY(Customer_ID) REFERENCES Customer(Customer_ID));", nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK)
    {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        sqlite3_close(db);
        return 1;
    }

    rc = sqlite3_exec(db, ("INSERT INTO Feedback (Customer_ID, FeedbackText,CurrentDate) VALUES ( " + to_string(Customer_ID) + ", '" + Feedback + "','" + CurrentDate + "')").c_str(), nullptr, nullptr, &errMsg);

    if (rc != SQLITE_OK)
    {
        cerr << "SQL error: " << errMsg << endl;
        sqlite3_free(errMsg);
        sqlite3_close(db);
        return 1;
    }

    if (sqlite3_changes(db) > 0)
    {
        cout << "Feedback have been Submitted successfully" << endl;
    }
    else
    {
        cout << "Feedback not  Submitted" << endl;
    }

    sqlite3_close(db);
    return 0;
}

int main()

{
    int choice;

    do
    {
        cout << "******************Pharmacy Management System***************" << endl;
        cout << "************************ Welcome***************************" << endl;
        cout << "1. Admin" << endl;
        cout << "2. Customer" << endl;
        cout << "3. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            // Handle admin functions
            if (AdminLogin())
            {
                int choice;

                do
                {

                    cout << "1. Medicin Mangment" << endl;
                    cout << "2. Customer Mangment" << endl;
                    cout << "3. Pharchase Records" << endl;
                    cout << "4. Check feed back" << endl;
                    cout << "5. exit" << endl;
                    cout << "Enter your choice: ";
                    cin >> choice;

                    switch (choice)
                    {
                    case 1:
                        cout << "Handle Medicine:" << endl;
                        cout << "****************" << endl;
                        int choice;

                        do
                        {
                            cout << " 1.Add Medicine" << endl;
                            cout << "2. View Medicine" << endl;
                            cout << "3. Search Medicine " << endl;
                            cout << "4. delete Medicine " << endl;
                            cout << "5. update Medicine " << endl;
                            cout << "6. Exit" << endl;
                            cout << "Enter your choice: ";
                            cin >> choice;

                            switch (choice)
                            {
                            case 1:
                                // add medicine
                                AddMedicine();
                                break;
                            case 2:
                                // view medicine
                                ViewMedicine();
                                break;
                            case 3:
                                // search Medicine
                                SearchMedicine();

                                break;
                            case 4:
                                //   delete medicine
                                DeleteMedicine();

                                break;
                            case 5:
                                // update medicine
                                UpdateMedicine();
                                break;
                            case 6:
                                cout << "you have done  Medicine Mengement" << endl;
                                break;
                            default:
                                cout << "Invalid choice. Please try again." << endl;
                            }
                        } while (choice != 6);

                        break;
                    case 2:
                        ShowCustomer();
                        break;
                    case 3:

                        PurchaseRecord();
                        break;
                    case 4:
                        CheckFeedback();
                        break;
                    case 5:
                        cout << "you have done Admin Activities!" << endl;
                        break;
                    default:
                        cout << "Invalid choice. Please try again." << endl;
                    }
                } while (choice != 5);
            }
            break;
        case 2:
            int n;
            do
            {
                // Handle customer functions
                int choice;
                cout << " firstly rigister yourself by pressing 1,If you are  A new Customer ";
                cout << " otherwise press 2  for login" << endl;
                cout << "press 3 for exit" << endl;

                cin >> n;

                switch (n)
                {
                case 1:
                    CustomerRegistration();
                    break;
                case 2:
                    if (CustomerLogin())
                    {
                        int choice;
                        do
                        {

                            cout << "1. purchase Opreation" << endl;
                            cout << "2. Order History" << endl;
                            cout << "3. Feed back" << endl;
                            cout << "4. Exit" << endl;
                            cout << "Enter your choice: ";
                            cin >> choice;

                            switch (choice)
                            {
                            case 1:
                                // purchase opreation
                                int choice;
                                do
                                {

                                    cout << "1. Search Medicine" << endl;
                                    cout << "2.  Veiw Avaliable Medicine " << endl;
                                    cout << "3.  Purchase Medicine" << endl;
                                    cout << "4. Exit" << endl;
                                    cout << "Enter your choice: ";
                                    cin >> choice;

                                    switch (choice)
                                    {
                                    case 1:
                                        //  Search Medicine
                                        SearchMedicine();
                                        break;
                                    case 2:
                                        //   Veiw Avaliable Medicine
                                        ViewMedicine();
                                        break;
                                    case 3:
                                        // Purchase Medicine
                                        PurchaseMedicine();

                                        break;
                                    case 4:
                                        cout << "you have exit from  the purchase opreations" << endl;
                                        break;
                                    default:
                                        cout << "Invalid choice. Please try again." << endl;
                                    }
                                } while (choice != 4);
                                break;
                            case 2:
                                //  order history
                                PurchaseRecord();
                                break;
                            case 3:
                                // feed back
                                Feedback();
                                break;
                            case 4:
                                cout << "Cutomer have exit the program";
                                break;
                            default:
                                cout << "Invalid choice. Please try again." << endl;
                            }
                        } while (choice != 4);
                    }
                    break;
                case 3:
                    cout << "you  have exit  from the program. " << endl;
                    break;
                default:
                    cout << "Invalid choice. Please try again." << endl;
                }
            } while (n != 3);
            break;
        case 3:
            cout << "Exiting the program. Goodbye!" << endl;
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 3);

    return 0;
}
