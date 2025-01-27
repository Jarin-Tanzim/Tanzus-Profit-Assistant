#include <iostream>
#include <fstream>
#include <string>
using namespace std;


void registerUser() {
    string username, password;
    cout << "Enter a new username: ";
    cin >> username;
    cout << "Enter a new password: ";
    cin >> password;


    ofstream usersFile("users.txt", ios::app);
    if (usersFile.is_open()) {
        usersFile << username << " " << password << endl;
        usersFile.close();
        cout << "User registered successfully!\n";
    } else {
        cout << "Error: Could not open the users file.\n";
    }
}


bool loginUser(string& loggedInUser) {
    string username, password, storedUsername, storedPassword;
    cout << "Enter your username: ";
    cin >> username;
    cout << "Enter your password: ";
    cin >> password;


    ifstream usersFile("users.txt");
    if (usersFile.is_open()) {
        while (usersFile >> storedUsername >> storedPassword) {
            if (storedUsername == username && storedPassword == password) {
                loggedInUser = username; // Store the logged-in user's name
                cout << "Login successful! Welcome, " << username << "!\n";
                usersFile.close();
                return true;
            }
        }
        usersFile.close();
    } else {
        cout << "Error: Could not open the users file.\n";
    }

    cout << "Invalid username or password. Please try again.\n";
    return false;
}


void profitCalculator(const string& loggedInUser) {
    char productName[100];
    int costPrice, sellingPrice;
    float profitMargin;
    char currentDate[11];

    cout << "Welcome to the Profit Calculator!\n";
    cout << "Enter today's date (YYYY-MM-DD): ";
    cin >> currentDate;

    cout << "Enter the product name: ";
    cin.ignore(); // Clear input buffer
    cin.getline(productName, 100);

    cout << "Enter the cost price (BDT): ";
    cin >> costPrice;

    cout << "Enter the selling price (BDT): ";
    cin >> sellingPrice;


    string filename = "ProfitRecords_" + loggedInUser + ".txt";
    ofstream file(filename, ios::app);
    if (!file) {
        cout << "Error: Could not save the record!\n";
        return;
    }

    if (sellingPrice > costPrice) {
        int profit = sellingPrice - costPrice;
        profitMargin = (float)profit / costPrice * 100;
        cout << "You made a profit of " << profit << " BDT.\n";
        cout << "Profit Margin: " << profitMargin << "%\n";


        file << "Date: " << currentDate << " | Product: " << productName
             << " | Profit: " << profit << " BDT | Profit Margin: " << profitMargin << "%\n";
    } else if (costPrice > sellingPrice) {
        int loss = costPrice - sellingPrice;
        profitMargin = (float)loss / costPrice * 100;
        cout << "You incurred a loss of " << loss << " BDT.\n";
        cout << "Loss Margin: " << profitMargin << "%\n";


        file << "Date: " << currentDate << " | Product: " << productName
             << " | Loss: " << loss << " BDT | Loss Margin: " << profitMargin << "%\n";
    } else {
        cout << "No profit or loss for this product.\n";
        file << "Date: " << currentDate << " | Product: " << productName << " | No profit or loss.\n";
    }

    file.close();
}


void generateSummaryReport(const string& loggedInUser) {
    string filename = "ProfitRecords_" + loggedInUser + ".txt";
    ifstream file(filename);

    if (!file) {
        cout << "No records found for this user.\n";
        return;
    }

    string dateOrMonth, line;
    float totalProfit = 0, totalLoss = 0;
    int transactionCount = 0;

    cout << "Enter a specific date (YYYY-MM-DD) or month (YYYY-MM): ";
    cin >> dateOrMonth;


    while (getline(file, line)) {
        if (line.find("Date: " + dateOrMonth) != string::npos) {
            transactionCount++; // Increment transaction count


            size_t posProfit = line.find("Profit: ");
            size_t posLoss = line.find("Loss: ");
            if (posProfit != string::npos) {
                float profit = stof(line.substr(posProfit + 8, line.find(" BDT", posProfit) - (posProfit + 8)));
                totalProfit += profit;
            } else if (posLoss != string::npos) {
                float loss = stof(line.substr(posLoss + 6, line.find(" BDT", posLoss) - (posLoss + 6)));
                totalLoss += loss;
            }
        }
    }

    file.close();


    cout << "\nSummary Report for " << dateOrMonth << ":\n";
    cout << "----------------------------------------\n";
    cout << "Total Transactions: " << transactionCount << "\n";
    cout << "Total Profit: " << totalProfit << " BDT\n";
    cout << "Total Loss: " << totalLoss << " BDT\n";
    cout << "----------------------------------------\n";

    if (transactionCount == 0) {
        cout << "No records found for the specified date or month.\n";
    }
}


int main() {
    string loggedInUser;
    int choice;


    do {
        cout << "\nLogin Menu:\n";
        cout << "1. Login\n";
        cout << "2. Register\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                if (loginUser(loggedInUser)) break;
                continue;
            case 2:
                registerUser();
                continue;
            case 3:
                cout << "Exiting program. Goodbye!\n";
                return 0;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (loggedInUser.empty());


    do {
        cout << "\nMain Menu:\n";
        cout << "1. Calculate Profit/Loss\n";
        cout << "2. Generate Summary Report\n";
        cout << "3. Logout\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                profitCalculator(loggedInUser);
                break;
            case 2:
                generateSummaryReport(loggedInUser);
                break;
            case 3:
                cout << "Logging out...\n";
                loggedInUser.clear();
                return 0;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (true);

    return 0;
}
