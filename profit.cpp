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
    if (usersFile) {
        usersFile << username << " " << password << endl;
        cout << "User registered successfully!\n";
    } else {
        cout << "Error: Could not open the users file.\n";
    }
}

string loginUser() {
    string username, password, storedUsername, storedPassword;
    cout << "Enter your username: ";
    cin >> username;
    cout << "Enter your password: ";
    cin >> password;

    ifstream usersFile("users.txt");
    if (usersFile) {
        while (usersFile >> storedUsername >> storedPassword) {
            if (storedUsername == username && storedPassword == password) {
                cout << "Login successful! Welcome, " << username << "!\n";
                return username;
            }
        }
    } else {
        cout << "Error: Could not open the users file.\n";
    }

    cout << "Invalid username or password.\n";
    return "";
}

void profitCalculator(string loggedInUser) {
    string productName, currentDate;
    int costPrice, sellingPrice;

    cout << "Enter today's date (YYYY-MM-DD): ";
    cin >> currentDate;
    cout << "Enter the product name: ";
    cin.ignore();
    getline(cin, productName);
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

    int profit = sellingPrice - costPrice;
    float profitMargin = (costPrice == 0) ? 0 : (float)abs(profit) / costPrice * 100;

    if (profit > 0) {
        cout << "You made a profit of " << profit << " BDT. Profit Margin: " << profitMargin << "%\n";
        file << "Date: " << currentDate << " | Product: " << productName << " | Profit: " << profit << " BDT | Profit Margin: " << profitMargin << "%\n";
    } else if (profit < 0) {
        cout << "You incurred a loss of " << -profit << " BDT. Loss Margin: " << profitMargin << "%\n";
        file << "Date: " << currentDate << " | Product: " << productName << " | Loss: " << -profit << " BDT | Loss Margin: " << profitMargin << "%\n";
    } else {
        cout << "No profit or loss.\n";
        file << "Date: " << currentDate << " | Product: " << productName << " | No profit or loss.\n";
    }
}

void generateSummaryReport(string loggedInUser) {
    string filename = "ProfitRecords_" + loggedInUser + ".txt";
    ifstream file(filename);

    if (!file) {
        cout << "No records found.\n";
        return;
    }

    string dateOrMonth, line;
    float totalProfit = 0, totalLoss = 0;
    int transactionCount = 0;

    cout << "Enter date (YYYY-MM-DD) or month (YYYY-MM): ";
    cin >> dateOrMonth;

    while (getline(file, line)) {
        if (line.find("Date: " + dateOrMonth) != string::npos) {
            transactionCount++;
            if (line.find("Profit: ") != string::npos) {
                totalProfit += stof(line.substr(line.find("Profit: ") + 8));
            } else if (line.find("Loss: ") != string::npos) {
                totalLoss += stof(line.substr(line.find("Loss: ") + 6));
            }
        }
    }

    cout << "\nSummary for " << dateOrMonth << ":\n";
    cout << "Transactions: " << transactionCount << "\n";
    cout << "Total Profit: " << totalProfit << " BDT\n";
    cout << "Total Loss: " << totalLoss << " BDT\n";
}

int main() {
    string loggedInUser;
    int choice;

    while (loggedInUser.empty()) {
        cout << "\nLogin Menu:\n1. Login\n2. Register\n3. Exit\nEnter choice: ";
        cin >> choice;

        if (choice == 1) {
            loggedInUser = loginUser();
        } else if (choice == 2) {
            registerUser();
        } else {
            return 0;
        }
    }

    while (true) {
        cout << "\nMain Menu:\n1. Calculate Profit/Loss\n2. Summary Report\n3. Logout\nEnter choice: ";
        cin >> choice;

        if (choice == 1) {
            profitCalculator(loggedInUser);
        } else if (choice == 2) {
            generateSummaryReport(loggedInUser);
        } else {
            break;
        }
    }

    return 0;
}
