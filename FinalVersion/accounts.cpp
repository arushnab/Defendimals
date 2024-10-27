#include "accounts.h"

#include <string>
using namespace std;

accounts::accounts() {
  platform = " ";
  password = " ";
  twoFactorAuth = false;
  sofwareUpdateDetected = false;
}

accounts::accounts(string pform, string ps, bool tfa) {
  platform = pform;
  password = ps;
  twoFactorAuth = tfa;
  sofwareUpdateDetected = false;
}

string accounts::getPlatform() { return platform; }

string accounts::getPassword() { return password; }

bool accounts::getTwoFact() { return twoFactorAuth; }

void accounts::setPassword(string ps) { password = ps; }

void accounts::setSofwareUp(bool up) { sofwareUpdateDetected = up; }

bool accounts::getSoftwareup() { return sofwareUpdateDetected; }
