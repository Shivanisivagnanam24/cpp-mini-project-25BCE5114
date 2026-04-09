/*
Project Title: Mini Banking System – Personal Savings Account
Student Name: S Shivani
Register No: 25BCE5114
*/

#include <iostream>
#include <vector>
#include <fstream>
#include <ctime>
using namespace std;

// ---------------- HEADER FUNCTION ----------------
// Displays the header of the cooperative society
void showHeader() {
    cout << "\n====================================\n";
    cout << "   ABC Cooperative Society\n";
    cout << "        Chennai Branch\n";
    cout << "====================================\n\n";
}

// ---------------- ACCOUNT CLASS ----------------
// Stores account details: account number, name, balance
// Methods: deposit, withdraw, get balance
class Account {
private:
    int accNo;
    string name;
    double balance;

public:
    Account(int a, string n, double b) {
        accNo = a;
        name = n;
        balance = b;
    }

    int getAccNo() { return accNo; }
    string getName() { return name; }
    double getBalance() { return balance; }

    void deposit(double amt) { balance += amt; }

    bool withdraw(double amt) {
        if (amt > balance) return false;
        balance -= amt;
        return true;
    }
};

// ---------------- TRANSACTION CLASS ----------------
// Stores transaction details: transaction number, account number, type, amount, date, time
class Transaction {
public:
    int tNo;
    int accNo;
    string type;
    double amount;
    string date;
    string time;

    Transaction(int t, int a, string ty, double amt, string d, string ti) {
        tNo = t;
        accNo = a;
        type = ty;
        amount = amt;
        date = d;
        time = ti;
    }
};

// ---------------- BANK SYSTEM CLASS ----------------
// Manages accounts and transactions
// Handles operations: create account, deposit, withdraw, balance enquiry,
// last 5 transactions, reports, file operations
class BankSystem {
private:
    vector<Account> accounts;
    vector<Transaction> transactions;
    int transactionCounter = 1;

    // ---------------- GET CURRENT DATE & TIME ----------------
    void getDateTime(string &date, string &timeStr) {
        time_t now = time(0);
        tm *ltm = localtime(&now);

        date = to_string(ltm->tm_mday) + "-" +
               to_string(1 + ltm->tm_mon) + "-" +
               to_string(1900 + ltm->tm_year);

        timeStr = to_string(ltm->tm_hour) + ":" +
                  to_string(ltm->tm_min) + ":" +
                  to_string(ltm->tm_sec);
    }

public:

    // ---------------- LOAD ACCOUNTS FROM FILE ----------------
    void loadAccounts() {
        ifstream file("accounts.txt");
        if (!file) {
            // File doesn't exist → create empty file automatically
            ofstream createFile("accounts.txt");
            createFile.close();
            return;
        }

        int a;
        string n;
        double b;

        while (file >> a >> n >> b) {
            accounts.push_back(Account(a, n, b));
        }
        file.close();
    }

    // ---------------- LOAD TRANSACTIONS FROM FILE ----------------
    void loadTransactions() {
        ifstream file("transactions.txt");
        if (!file) {
            // File doesn't exist → create empty file automatically
            ofstream createFile("transactions.txt");
            createFile.close();
            return;
        }

        int t, a;
        string ty, d, ti;
        double amt;

        while (file >> t >> a >> ty >> amt >> d >> ti) {
            transactions.push_back(Transaction(t, a, ty, amt, d, ti));
            transactionCounter = t + 1; // Ensure next transaction number is correct
        }
        file.close();
    }

    // ---------------- SAVE ACCOUNTS TO FILE ----------------
    void saveAccounts() {
        ofstream file("accounts.txt");
        for (int i = 0; i < accounts.size(); i++) {
            file << accounts[i].getAccNo() << " "
                 << accounts[i].getName() << " "
                 << accounts[i].getBalance() << endl;
        }
        file.close();
    }

    // ---------------- SAVE TRANSACTIONS TO FILE ----------------
    void saveTransactions() {
        ofstream file("transactions.txt");
        for (int i = 0; i < transactions.size(); i++) {
            file << transactions[i].tNo << " "
                 << transactions[i].accNo << " "
                 << transactions[i].type << " "
                 << transactions[i].amount << " "
                 << transactions[i].date << " "
                 << transactions[i].time << endl;
        }
        file.close();
    }

    // ---------------- CREATE ACCOUNT ----------------
    void createAccount() {
        int acc;
        string name;

        cout << "Enter Account Number: ";
        cin >> acc;
        cout << acc << endl; // Echo input

        // Check for unique account number
        for (int i = 0; i < accounts.size(); i++) {
            if (accounts[i].getAccNo() == acc) {
                cout << "Account already exists!\n";
                return;
            }
        }

        cout << "Enter Name: ";
        cin >> name;
        cout << name << endl; // Echo input

        accounts.push_back(Account(acc, name, 0));
        saveAccounts();

        cout << "Account Created Successfully!\n";
    }

    // ---------------- FIND ACCOUNT ----------------
    int findAccount(int acc) {
        for (int i = 0; i < accounts.size(); i++) {
            if (accounts[i].getAccNo() == acc)
                return i;
        }
        return -1; // Account not found
    }

    // ---------------- DEPOSIT ----------------
    void deposit() {
        int acc;
        double amt;

        cout << "Enter Account Number: ";
        cin >> acc;
        cout << acc << endl; // Echo input

        int i = findAccount(acc);
        if (i == -1) {
            cout << "Account not found!\n";
            return;
        }

        cout << "Enter Amount (Rs.): ";
        cin >> amt;
        cout << amt << endl; // Echo input

        if (amt <= 0) {
            cout << "Invalid amount!\n";
            return;
        }

        accounts[i].deposit(amt);

        string d, t;
        getDateTime(d, t);

        transactions.push_back(Transaction(transactionCounter++, acc, "Deposit", amt, d, t));

        saveAccounts();
        saveTransactions();

        cout << "Deposit Successful! Amount: Rs. " << amt << endl;
    }

    // ---------------- WITHDRAW ----------------
    void withdraw() {
        int acc;
        double amt;

        cout << "Enter Account Number: ";
        cin >> acc;
        cout << acc << endl; // Echo input

        int i = findAccount(acc);
        if (i == -1) {
            cout << "Account not found!\n";
            return;
        }

        cout << "Enter Amount (Rs.): ";
        cin >> amt;
        cout << amt << endl; // Echo input

        if (amt <= 0) {
            cout << "Invalid amount!\n";
            return;
        }

        if (!accounts[i].withdraw(amt)) {
            cout << "Insufficient Balance!\n";
            return;
        }

        string d, t;
        getDateTime(d, t);

        transactions.push_back(Transaction(transactionCounter++, acc, "Withdraw", amt, d, t));

        saveAccounts();
        saveTransactions();

        cout << "Withdrawal Successful! Amount: Rs. " << amt << endl;
    }

    // ---------------- CHECK BALANCE ----------------
    void checkBalance() {
        int acc;

        cout << "Enter Account Number: ";
        cin >> acc;
        cout << acc << endl; // Echo input

        int i = findAccount(acc);
        if (i == -1) {
            cout << "Account not found!\n";
            return;
        }

        cout << "Balance: Rs. " << accounts[i].getBalance() << endl;
    }

    // ---------------- LAST 5 TRANSACTIONS ----------------
    void lastTransactions() {
        int acc;

        cout << "Enter Account Number: ";
        cin >> acc;
        cout << acc << endl; // Echo input

        int count = 0;

        for (int i = transactions.size() - 1; i >= 0 && count < 5; i--) {
            if (transactions[i].accNo == acc) {
                cout << "\nTransaction No: " << transactions[i].tNo << endl;
                cout << "Type: " << transactions[i].type << endl;
                cout << "Amount: Rs. " << transactions[i].amount << endl;
                cout << "Date: " << transactions[i].date << endl;
                cout << "Time: " << transactions[i].time << endl;
                cout << "-----------------------\n";
                count++;
            }
        }

        if (count == 0)
            cout << "No transactions found!\n";
    }

    // ---------------- REPORTS ----------------
    void reports() {
        double total = 0;

        for (int i = 0; i < accounts.size(); i++) {
            total += accounts[i].getBalance();
        }

        cout << "Total Money in Society: Rs. " << total << endl;

        cout << "\nAccounts with Balance < 500:\n";
        for (int i = 0; i < accounts.size(); i++) {
            if (accounts[i].getBalance() < 500) {
                cout << "AccNo: " << accounts[i].getAccNo()
                     << " Balance: Rs. " << accounts[i].getBalance() << endl;
            }
        }
    }
};

// ---------------- MAIN ----------------
int main() {
    showHeader();

    BankSystem bank;
    bank.loadAccounts();
    bank.loadTransactions();

    int choice;

    do {
        cout << "\n1. Create Account\n2. Deposit\n3. Withdraw\n4. Balance\n5. Last 5 Transactions\n6. Reports\n7. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;
        cout << choice << endl; // Echo input

        switch (choice) {
            case 1: bank.createAccount(); break;
            case 2: bank.deposit(); break;
            case 3: bank.withdraw(); break;
            case 4: bank.checkBalance(); break;
            case 5: bank.lastTransactions(); break;
            case 6: bank.reports(); break;
            case 7: cout << "Exiting...\n"; break;
            default: cout << "Invalid choice!\n";
        }

    } while (choice != 7);

    return 0;
}

