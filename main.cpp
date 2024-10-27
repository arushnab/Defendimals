// taking in input from user

#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "accounts.h"

using namespace std;

void initialize(string name, ifstream& inFile, int numOfpform,
                accounts**& arr2p);
void passwordAsk(int numOfpform, accounts**& arr2p);
void resetPassword(int numOfpform, accounts**& arr2p);
void Update(string name, int numOfpform, accounts**& arr2p);
void removeAccount(accounts& arr2p, int& numOfpform);
void addAccount(accounts& arr2p, int& numOfpform);
int passwordChecker(const string& password);
int movingThroughThePasswords(accounts** arr2p, int numOfpform,
                              string* usedPasswords, int& usedCount);
void softwareUpdate(accounts**& arr2p, int numOfpform, int& counter,
                    bool& updateInProgress);
bool tenPercent();
int ranNum(int num);
int checkrepeats(int usedCount, accounts**& arr2p);

string commands[3] = {"CheckAccounts",   // 0
                      "ResetPassword",   // 1
                      "AddAccount",      // 2
                      "RemoveAccount"};  // 3

int main() {
  string name;
  int numOfpform;

  ifstream inFile(
      "test.txt");  // I'm going to assume the file will always exist
  inFile >> numOfpform;
  accounts** arr2p = new accounts*[numOfpform];
  initialize(name, inFile, numOfpform, arr2p);

  string line;
  string command;
  cout << ">>> ";
  cout.flush();
  cin >> command;

  int counter = 0;
  bool updateInProgress = false;

  while (cin.eof() == false) {
    // not sure where I should be putting the random software update stuff!!!

    softwareUpdate(arr2p, numOfpform, counter, updateInProgress);

    int whichCommand = -1;

    for (int i = 0; i < 4; i++) {
      if (command == commands[i]) {
        whichCommand = i;
      }
    }

    switch (whichCommand) {
      case 0:
        movingThroughThePasswords(arr2p, numOfpform);
        break;
      case 1:
        resetPassword(numOfpform, arr2p);
        break;
      case 2:
        addAccount(arr2p, numOfpform);
        break;
      case 3:
        removeAccount(arr2p, numOfpform);
        break;
    }

    if (whichCommand < 0) {
      cin.clear();
      cin.ignore(1000, '\n');
      cout << "Error: invalid command" << endl;
      cout.flush();
    }

    cout << ">>> ";
    cout.flush();
  }
}

void softwareUpdate(accounts**& arr2p, int numOfpform, int& counter,
                    bool& updateInProgress) {
  bool softUp = tenPercent();
  if (softUp == true && updateInProgress == false) {
    arr2p[ranNum(numOfpform)]->setSofwareUp(softUp);
    updateInProgress = true;
  }
  counter++;

  if (counter >= 10) {
    string pf;
    for (int i = 0; i < numOfpform; i++) {
      if (arr2p[i]->getSoftwareup() == true) {
        cout << "Please update update your sofware for "
             << arr2p[i]->getPlatform() << endl;
      }
    }
  }
}

void Update(string pf, int numOfpform, accounts**& arr2p) {
  for (int i = 0; i < numOfpform; i++) {
    if (arr2p[i]->getPlatform() == pf) {
      arr2p[i]->setSofwareUp(false);
    }
  }
  return;
}

bool tenPercent() {
  return (rand() % 10) ==
         0;  // generate true 10% of the time!!! will change later if need be
}

int ranNum(int num) {
  return rand() % num;  // this is to return a random number to access one of
                        // the accounts.
}

void initialize(string name, ifstream& inFile, int numOfpform,
                accounts**& arr2p) {
  cout << "Hi, it is nice to meet you. I'm your personal defendimal" << endl;
  cout << "What do you want to name me?: ";
  cin >> name;
  cout << "Great! My name is " << name << endl;

  cout << "In order to keep you safe I have access to the plateforms and its "
          "respective passwords"
       << endl;
  string pform;
  string ps;
  bool tfa;

  // maybe consider a case where you run out of room, but maybe worry about it
  // later

  for (int i = 0; i < numOfpform; i++) {
    inFile >> pform;
    inFile >> ps;
    inFile >> tfa;
    arr2p[i] = new accounts(pform, ps, tfa);
  }
}

void addAccount(accounts**& arr2p, int& numOfpform) {
  string response;

  accounts** newArray = new accounts*[numOfpform + 1];
  for (int i = 0; i < numOfpform; i++) {
    newArray[i] = arr2p[i];
  }

  delete[] arr2p;

  arr2p = newArray;

  string password;
  string platform;
  string tfaResponse;
  bool twoFA;

  cout << "Enter the platform for the account" << endl;
  cin >> platform;

  cout << "Enter the password for the account" << endl;
  cin >> password;

  cout << "Do you have Two Factor Authentication Enabled? (Y/N)" << endl;
  cin >> tfaResponse;

  if (tfaResponse == "y" || tfaResponse == "Y") {
    twoFA = true;
  } else {
    twoFA = false;
  }

  arr2p[numOfpform] = new accounts(platform, password, twoFA);
  numOfpform++;
}

void removeAccount(accounts**& arr2p, int& numOfpform) {
  if (numOfpform == 0) {
    cout << "There are no accounts to remove" << endl;
    return;
  }
  string platform;

  cout << "What platform do you want to remove your account information for"
       << endl;

  cin >> platform;

  int indexOfRemoval = -1;

  for (int i = 0; i < numOfpform; i++) {
    if (arr2p[i]->getPlatform() == platform) {
      indexOfRemoval = i;
      delete arr2p[i];
      break;
    }
  }

  if (indexOfRemoval == -1) {
    cout << "An account on the platform: " << platform << "wasn't found"
         << endl;
    return;
  }

  accounts** newArray = new accounts*[numOfpform - 1];
  for (int i = 0; i < indexOfRemoval; i++) {
    newArray[i] = arr2p[i];
  }

  for (int i = indexOfRemoval + 1; i < numOfpform; i++) {
    newArray[i - 1] = arr2p[i];
  }

  delete[] arr2p;
  arr2p = newArray;

  numOfpform--;
  cout << "You've removed your account information!" << endl;

  return;
}

// Function to check password strength
int passwordChecker(const string& password) {
  int score = 0;

  if (password.length() < 8) {
    cout << "Password is less than 8 characters. Please enter a new password."
         << endl;
    return score;
  } else {
    score++;
  }

  bool hasLower = false, hasUpper = false, hasDigit = false, hasSpecial = false;

  for (char c : password) {
    if (islower(c))
      hasLower = true;
    else if (isupper(c))
      hasUpper = true;
    else if (isdigit(c))
      hasDigit = true;
    else
      hasSpecial = true;
  }

  // Increment score based on the presence of each character type
  if (hasLower) score++;
  if (hasUpper) score++;
  if (hasDigit) score++;
  if (hasSpecial) score++;

  string strength =
      (score >= 4) ? "Strong" : (score == 3 ? "Moderate" : "Weak");
  cout << "Password strength is " << strength << ". Score: " << score << endl;

  return score;
}

//
// Function to check passwords for each account in the array
int movingThroughThePasswords(accounts**& arr2p, int numOfpform,
                              string* usedPasswords) {
  int totalScore = 0;

  totalScore += checkrepeats(numOfpform, arr2p);

  // Iterate through each account and check the password
  for (int i = 0; i < numOfpform; ++i) {
    string password = arr2p[i]->getPassword();
    int score = passwordChecker(password);
    totalScore += score;
    cout << "Score for password of account " << arr2p[i]->getPlatform() << ": "
         << score << endl;
  }

  return totalScore;
}

int checkrepeats(int usedCount, accounts**& arr2p) {
  for (int i = 0; i < usedCount; i++) {
    for (int j = i + 1; j < usedCount; j++) {
      if (arr2p[i]->getPassword() == arr2p[j]->getPassword()) {
        cout << "Oh no! your password is resued. Please enter "
                "a new password. "
             << endl;
        return 0;
      }
    }
  }
  return 1;
}

void resetPassword(int numOfpform,
                   accounts**& arr2p) {  // this is what i changes laila
  string pf;
  string password;
  cout << "Which account do you want to update: ";
  cin >> pf;
  cout << "Type in your new password please: ";

  bool psIsStrong = false;

  cin >> password;
  while (psIsStrong == false) {
    if (passwordChecker(password) >= 5) {  // update the details of this later
      psIsStrong = true;
    } else {
      cout << "Password isn't strong enough, please re-enter your password: ";
      cin >> password;
    }
  }

  for (int i = 0; i < numOfpform; i++) {
    if (arr2p[i]->getPlatform() == pf) {
      arr2p[i]->setPassword(password);
    }
  }
}