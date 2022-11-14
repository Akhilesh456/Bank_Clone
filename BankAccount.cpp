#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

// class is a user defined complex datatype (like string class)
class Account{
    public:
    string holder_name;
    double money;
    short pin;
    Account(string name, short pin, double start_money){
        holder_name = name;
        this->pin = pin;
        money = start_money;
    }

    void deposit(double start_money){
        money += start_money;
    }

    bool withdraw(short entered_pin, double money){
        if(entered_pin != pin){
            cout << "WRONG PIN!!" << endl;
            return 0;
        }
        this->money -= money;
        return 1;
    }

    void check_balance(short entered_pin){
        if(entered_pin != pin){
            cout << "WRONG PIN!!" << endl;
            return;
        }
        cout << "Your balance is: " << money << endl;
    }
};

class Bank{
    public:
    vector<Account> accounts;
    int no_of_accounts;
        Bank(){
            no_of_accounts = 0;
        }

        void open_account(string name, short pin, double start_money){
            Account ac(name, pin, start_money);
            cout << "Your account number is: " << no_of_accounts++ << endl;
            accounts.push_back(ac);
        }

        bool deposit(int account_no, double money){
            if(account_no >= no_of_accounts){
                cout << "Invalid account number!!" << endl;
                return 0;
            }
            accounts[account_no].deposit(money);
            return 1;
        }

        bool withdraw(int account_no, double money, short pin){
            if(account_no >= no_of_accounts){
                cout << "Invalid account number!!" << endl;
                return 0;
            }
            return accounts[account_no].withdraw(pin, money);
        }

        void show_account_details(int account_no, short pin){
            if(account_no >= no_of_accounts){
                cout << "Invalid account number!!" << endl;
                return;
            }
            cout << "Your account number is: " << account_no << endl;
            accounts[account_no].check_balance(pin);
        }

        bool transfer(int sender_ac, int receiver_ac, short pin, double money){
            if(sender_ac >= no_of_accounts){
                cout << "Invalid sender account number!!" << endl;
                return 0;
            }
            if(receiver_ac >= no_of_accounts){
                cout << "Invalid receiver account number!!" << endl;
                return 0;
            }
            bool check = accounts[sender_ac].withdraw(pin, money);
            if(check)
                accounts[receiver_ac].deposit(money);
            return check;
        }
};

vector<string> split(string str){
    string temp="";
    vector<string> splited_string;
    for(auto i:str){
        if(i == ' '){
            splited_string.push_back(temp);
            temp = "";
        }else   
            temp += i;
    }
    splited_string.push_back(temp);
    return splited_string;
}

int main(){
    Bank bank;
    fstream file;
    cout << "Welcome to bank interface!!\n";
    cout << "Press:\n";
    cout << "1: Open an account!\n";
    cout << "2: Deposit money into account\n";
    cout << "3: Withdraw money\n";
    cout << "4: Transfer money account to account\n";
    cout << "5: Show account details\n";
    cout << "6: Show the history\n";
    cout << "Press anything else to quit\n";
    
    file.open("Accounts.txt", ios::in);
    string line;
    vector<string> splited;
    while(getline(file, line)){
        splited = split(line);
        bank.open_account(splited[0], stoi(splited[1]), stod(splited[2]));
    }
    file.close();

    while(true){
        short action;
        cout << "Action: ";
        cin >> action;

        // open an account 
        if(action == 1){
            string name;
            short pin;
            double money;
            file.open("Accounts.txt", ios::app);
            cout << "Enter your name: ";
            cin >> name;
            cout << "Enter your chosen pin: ";
            cin >> pin;
            cout << "Enter starting money: ";
            cin >> money;
            bank.open_account(name, pin, money);
            if(file.is_open())
                file << name << " " << pin << " " << money << endl;
            file.close();
            cout << "Your account has been opened!" << endl;
            cout << endl;
        // deposite money
        }else if(action == 2){
            int account_no;
            double money;
            file.open("History.txt", ios::app);
            cout << "Enter the account number: ";
            cin >> account_no;
            cout << "Enter the money: ";
            cin >> money;
            bool check = bank.deposit(account_no, money);
            if(file.is_open() && check)
                file << account_no << " deposited " << money << endl;
            file.close();
            cout << "Money deposited!" << endl;
            cout << endl;
        // withdraw money
        }else if(action == 3){
            int account_no;
            double money;
            short pin;
            file.open("History.txt", ios::app);
            cout << "Enter the account number: ";
            cin >> account_no;
            cout << "Enter the money: ";
            cin >> money;
            cout << "Enter your pin: ";
            cin >> pin;
            bool check = bank.withdraw(account_no, money, pin);
            if(file.is_open() && check)
                file << account_no << " withdrew " << money << endl;
            file.close();
            cout << endl;
        // transfer money
        }else if(action == 4){
            int sender_ac, receiver_ac;
            short pin;
            double money;
            file.open("History.txt", ios::app);
            cout << "Enter your account number: ";
            cin >> sender_ac;
            cout << "Enter the receiver account number: ";
            cin >> receiver_ac;
            cout << "Enter the money: ";
            cin >> money;
            cout << "Enter your pin: ";
            cin >> pin;
            bool check = bank.transfer(sender_ac, receiver_ac, pin, money);
            if(file.is_open() && check)
                file << sender_ac << " sent " << money << " to " << receiver_ac << endl;
            file.close();
            cout << "Transfered money" << endl;
            cout << endl;
        // account details
        }else if(action == 5){
            int account_no;
            short pin;
            cout << "Enter your account number: ";
            cin >> account_no;
            cout << "Enter your pin: ";
            cin >> pin;
            bank.show_account_details(account_no, pin);
            cout << endl;
        // history
        }else if(action == 6){
            file.open("History.txt", ios::in);
            if(file.is_open()){
                string line;
                while (getline(file, line))
                    cout << line << endl;
            }
            file.close();
            cout << endl;
        // quitting and updating account details for storage
        }else{
            file.open("Accounts.txt", std::ofstream::out | std::ofstream::trunc);
            file.close();
            file.open("Accounts.txt", ios::app);
            for(auto i:bank.accounts)
                file << i.holder_name << " " << i.pin << " " << i.money << endl;
            file.close();
            break;
        }
    }
}