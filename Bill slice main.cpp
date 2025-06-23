#include <iostream>
#include <string>
#include <fstream>
using namespace std;



bool yes_no(string Prompt) {
    return (Prompt[0] == 'y') ? true : false;
}

string** ReadGroup(string TargetGroup) {
    ifstream GroupFile(TargetGroup + ".txt");
    string line;
    GroupFile >> line; // Add the (integer) group size to line

    int size = stoi(line);
    string** entries = new string * [size + 1];

    for (int i = 0; i < size; i++) {
        GroupFile >> line;
        entries[i] = new string[2];
        entries[i][0] = line;
        GroupFile >> line;
        entries[i][1] = line;
    }

    entries[size] = nullptr; // Null termination of array

    return entries;
}

string ReadTransactions(string GroupName) {
    string FinalOutput = "";
    string Line;

    ifstream SaveFile;
    SaveFile.open((GroupName + "T.txt"));

    while (getline(SaveFile, Line)) {
        FinalOutput += Line;
        FinalOutput += "\n";
        continue;
    }

    return FinalOutput;
}

void SaveTransaction(string GroupName, float TransactionTotal, string TransactionPayer, string* TransactionPayees, float* PayeeContributions, int len) {
    string FinalOutput = "--- \n";

    FinalOutput += "PAID BY: "; FinalOutput += TransactionPayer;
    FinalOutput += " AMOUNT: "; FinalOutput += to_string(TransactionTotal);
    FinalOutput += "\n";
    for (int i = 0; i < len; i++) {
        FinalOutput += "PAYEE #"; FinalOutput += to_string(i); FinalOutput += ": "; FinalOutput += TransactionPayees[i];
        FinalOutput += "(OWES: "; FinalOutput += to_string(PayeeContributions[i]);  FinalOutput += ")\n";;
    }
    FinalOutput += "--- \n";

    std::ofstream SaveFile;
    SaveFile.open((GroupName + "T.txt"), std::ios_base::app);
    SaveFile << FinalOutput;
}

void SaveGroup(string GroupName, string* Names, float* Expenses, int len) {
    std::ofstream SaveFile;
    SaveFile.open((GroupName + ".txt"), std::ios_base::out);
    if (SaveFile.is_open()) {
        string FinalString;
        FinalString += std::to_string(len) + "\n";
        for (int i = 0; i < len; i++) {
            FinalString += Names[i];
            FinalString += "\t";
            FinalString += std::to_string(Expenses[i]);
            FinalString += "\n";
        }

        SaveFile << FinalString;
        SaveFile.close();
    }
}


void createaccount() {
    string phone_number, name;
    cout << "Enter your name: ";
    getline(cin, name);
    cout << endl;
    while (true) {
        cout << "\nInput phone number: ";
        getline(cin, phone_number);
        cout << endl;
        if (phone_number.length() == 11) {
            break;
        }
        else {
            cout << "Invalid length of number. Input again.\n";
        }
    }
    bool number_exists = false;
    ofstream o_userID;
    string number_check;
    ifstream u_userID("phone_number.txt");
    while (u_userID >> number_check) {
        if (number_check == phone_number) {
            cout << "Account already exists. Input a new number.";
            number_exists = true;
            createaccount();
        }

    }
    if (!number_exists) {
        ofstream o_userID("phone_number.txt", ios_base::app);
        o_userID << phone_number << endl;
        o_userID.close();
        cout << "Account created successfully with phone number " << phone_number << endl;
    }
}
bool signintoaccount() {
    string phone_number, name;
    cout << "Enter your name: ";
    //getline(cin, name);
    cin >> name;
    cout << endl;
    while (true) {
        cout << "\nInput phone number: ";
        //getline(cin, phone_number);
        cin >> phone_number;
        cout << endl;
        if (phone_number.length() == 11) {
            break;
        }
        else {
            cout << "Invalid length of number. Input again.\n";
        }
    }
    bool number_exists = false;
    ofstream o_userID;
    string number_check;
    ifstream u_userID("phone_number.txt");
    while (u_userID >> number_check) {
        if (number_check == phone_number) {
            cout << "Sign-in successful. Welcome!\n";
            number_exists = true;
            return number_exists;
            break;
        }
        else {
            cout << "Sign-in unsuccessful. This account does not exist. ";
            return false;
            break;
        }

    }
    u_userID.close();
}

float GetCredit(string** GroupData, string Name, int len) {
    for (int i = 0; i < len; i++) {
        if (GroupData[i][0] == Name) {
            return stof(GroupData[i][1]);
        }
    }

    throw "No matches";
}

int main() {
    string buffer;
    int op, op2;
    ofstream o_userID;
    string phone_number;
    string GroupName;
    int GroupMemberCount;
    string* MemberNames;
    float* MemberExpenses;
    float TotalExpenseAmount;
    string TotalExpensePayer;
    int TotalExpensePayeeCount;
    string TotalExpenseEqualityChoice;
    string* TotalExpensePayeeNames;
    float* TotalExpensePayeeContributions;
    int Size_TEP;
    bool The_Choice;
    float total = 0;
    string CurrentUser = "NA";
    string MemberInsertionName;
    string TransactionHistory;
    float MemberInsertionCredit;
    string** CurrentState;


    while (true) {
        cout << "Welcome to Bill Slice!\n\nChoose an option\n\n1. Create Account\n2. Sign-In\n"; cin >> op;
        switch (op) {
        case (1): {
            cin.ignore();
            createaccount();
            break;
        }
        case (2): {
            bool result = signintoaccount();
            if (result == true) {
                while (true) {
                    cout << "Select an option: " << endl;
                    cout << "3. Create A Group\n4. Add expense\n5. Add new member to group\n6. Get group summary\n7. Exit\n"; cin >> op2;
                    switch (op2) {
                    case (3): {
                        cout << "Please enter the group's name: ";
                        cin >> GroupName;


                        cout << "Please enter the number of group members: ";
                        cin >> GroupMemberCount;

                        MemberNames = new string[GroupMemberCount];
                        MemberExpenses = new float[GroupMemberCount];

                        for (int i = 0; i < GroupMemberCount; i++) {
                            cout << "Please enter the name of member number " << i + 1 << ": ";
                            cin >> MemberNames[i];
                            MemberExpenses[i] = 0;
                        }

                        SaveGroup(GroupName, MemberNames, MemberExpenses, GroupMemberCount);
                        break;
                    }
                    case (4): {
                        cout << "Please enter the group's name: ";
                        cin >> GroupName;
                        CurrentState = ReadGroup(GroupName);

                        cout << "Please enter the total amount for the expense: ";
                        cin >> TotalExpenseAmount;

                        cout << "Please enter the name of the person who paid for the expense: ";
                        cin >> TotalExpensePayer;

                        cout << "Please enter the number of group members involved in the expense: ";
                        cin >> TotalExpensePayeeCount;

                        TotalExpensePayeeNames = new string[TotalExpensePayeeCount];
                        TotalExpensePayeeContributions = new float[TotalExpensePayeeCount];

                        for (int i = 0; i < TotalExpensePayeeCount; i++)
                        {
                            while (true) {
                                cout << "Please enter the name of payee number " << i + 1 << ": ";
                                cin >> TotalExpensePayeeNames[i];
                                if (TotalExpensePayeeNames[i] == TotalExpensePayer) {
                                    cout << "This person has paid the expense. Invalid entry!" << endl;
                                }
                                else {
                                    break;
                                }
                            }
                        }
                        cout << "Please enter whether you want to split equally(yes/no): ";
                        cin >> TotalExpenseEqualityChoice;
                        The_Choice = yes_no(TotalExpenseEqualityChoice);

                        for (int i = 0; i < TotalExpensePayeeCount; i++) {
                            if (The_Choice) {
                                TotalExpensePayeeContributions[i] = TotalExpenseAmount / TotalExpensePayeeCount;
                            }
                            else {
                                cout << "Please enter the contribution made by " << TotalExpensePayeeNames[i] << ": ";
                                cin >> TotalExpensePayeeContributions[i];
                                total += TotalExpensePayeeContributions[i];
                            }

                        }

                        while (The_Choice == 0) {
                            if (total == TotalExpenseAmount) {
                                break;
                            }
                            total = 0.0;
                            for (int i = 0; i < TotalExpensePayeeCount; i++) {
                                cout << "Please enter the contribution made by " << TotalExpensePayeeNames[i] << ": ";
                                cin >> TotalExpensePayeeContributions[i];
                                total += TotalExpensePayeeContributions[i];

                            }
                        }


                        Size_TEP = 0;

                        while (CurrentState[Size_TEP]) { // Checks for the null-termination
                            Size_TEP++;
                        }

                        MemberNames = new string[Size_TEP];
                        MemberExpenses = new float[Size_TEP];

                        for (int i = 0; i < Size_TEP; i++) {
                            MemberNames[i] = CurrentState[i][0];
                            MemberExpenses[i] = stof(CurrentState[i][1]);


                            for (int j = 0; j < TotalExpensePayeeCount; j++) {
                                if (MemberNames[i] == TotalExpensePayeeNames[j]) {
                                    MemberExpenses[i] -= TotalExpensePayeeContributions[j];
                                }
                            }

                            if (MemberNames[i] == TotalExpensePayer) {
                                MemberExpenses[i] += TotalExpenseAmount;
                            }
                        }
                        cout << "total : " << total<<endl;
                        total = 0.0;

                        SaveTransaction(GroupName, TotalExpenseAmount, TotalExpensePayer, TotalExpensePayeeNames, TotalExpensePayeeContributions, TotalExpensePayeeCount);
                        SaveGroup(GroupName, MemberNames, MemberExpenses, Size_TEP);

                        break;
                    }
                    case (5): {
                        cout << "Please enter the name of the group: ";
                        cin >> GroupName;

                        CurrentState = ReadGroup(GroupName);

                        cout << "Please enter the name of the new member: ";
                        cin >> MemberInsertionName;

                        //cout << "Please enter the total credit of " << MemberInsertionName << ": ";
                        //cin >> MemberInsertionCredit;
                        MemberInsertionCredit = 0;


                        Size_TEP = 0;

                        while (CurrentState[Size_TEP]) { // Checks for the null-termination
                            Size_TEP++;
                        }

                        MemberNames = new string[Size_TEP + 1];
                        MemberExpenses = new float[Size_TEP + 1];

                        for (int i = 0; i < Size_TEP; i++) {
                            MemberNames[i] = CurrentState[i][0];
                            MemberExpenses[i] = stof(CurrentState[i][1]);
                        }

                        MemberNames[Size_TEP] = MemberInsertionName;
                        MemberExpenses[Size_TEP] = MemberInsertionCredit;

                        SaveGroup(GroupName, MemberNames, MemberExpenses, Size_TEP + 1);

                        break;
                    }
                    case (6): {
                        cout << "Please enter which group's record you would like to review: ";
                        cin >> GroupName;

                        CurrentState = ReadGroup(GroupName);
                        TransactionHistory = ReadTransactions(GroupName);

                        Size_TEP = 0;

                        cout << "* INFORMATION FOR GROUP " << GroupName << "*\n";
                        cout << endl;
                        while (CurrentState[Size_TEP]) {
                            string OwesIsOwedMessage = (stof(CurrentState[Size_TEP][1]) >= 0) ? " is owed: " : " owes: ";
                            cout << CurrentState[Size_TEP][0] << OwesIsOwedMessage << abs(stof(CurrentState[Size_TEP][1])) << "\n";
                            Size_TEP++;
                        }
                        cout << endl;
                        cout << "Members list: " << endl;
                        for (int i = 0; i < Size_TEP; i++) {
                            cout << CurrentState[i][0] << endl;
                        }
                        cout << endl;
                        cout << "TRANSACTION HISTORY: \n";
                        cout << TransactionHistory;
                        break;
                    }
                    case (7): {
                        cout << "Thank you for using Billslice!";
                        return 0;
                    }
                    }
                }
            }
            else {
                cout << "Sign-in unsuccessful!";

                break;
            }

            cout << endl;

        }
        }

    }
}