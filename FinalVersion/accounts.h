#ifndef ACCOUNTS_H
#define ACCOUNTS_H

#include <string>
using namespace std;

class accounts{
    private:
        string platform;
        string password;
        bool twoFactorAuth;
        bool sofwareUpdateDetected; // worry about implementing this later
        // double timeAccountLastUsed; // worry about implementing this later 
    public:
        accounts(); // maybe make another constructor later, we'll see
        accounts(string pform, string ps, bool tfa);
        string getPlatform();
        string getPassword();
        bool getTwoFact();
        int getNumPlat();
        void setPassword(string ps);
        void setSofwareUp(bool up);
        bool getSoftwareup();

};

#endif
