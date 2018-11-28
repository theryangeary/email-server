// Ryan Geary and Anna Schaal
#include "email.h"

using namespace std;

int main(){
  while(1) {
    start();
  }
  return 0;
}

void start(){
  string username = EMPTY_STRING;

  while (EMPTY_STRING == username) {
    int userInt = showMenu(START_MENU, START_MENU_OPT_NUM);
    switch(userInt) {
      case 0:
        username = reg();
        break;
      case 1:
        username = login();
        break;
    }
  }

  menu(username);
}

void menu(string sessionUser){
  cout << "Welcome " << sessionUser << endl;

  while(1) {
    int userInt = showMenu(MAIN_MENU, MAIN_MENU_OPT_NUM);
    switch (userInt) {
      case 0:
        read(sessionUser);
        break;
      case 1:
        send(sessionUser);
        break;
      case 2:
        return;
        break;
    }
  }
}

void read(string sessionUser) {
}
void send(string sessionUser) {
}

string login() {
  string username;
  string password;

  cout << USERNAME_PROMPT;
  cin >> username;

  cout << PASSWORD_PROMPT;
  cin >> password;

  // TODO Check that user exists and password is correct
  // If not, return an empty string
  return username;
}

string reg() {
  string username;
  string password;
  string passwordConfirm;

  cout << USERNAME_PROMPT << endl;
  cin >> username;

  cout << PASSWORD_PROMPT << endl;
  cin >> password;

  cout << CONFIRM_PASSWORD_PROMPT << endl;
  cin >> passwordConfirm;

  if (password == passwordConfirm) {
    // TODO store in db
    return username;
  }
  else {
    cout << PASSWORD_MISMATCH << endl;
    return EMPTY_STRING;
  }
}

int showMenu(string options, int max) {
  int userInt = -1;
  while(-1 == userInt) {
    cout << options << endl;
    string userReply;
    cin >> userReply;
    int userInt = atoi(userReply.c_str());
    if (userReply != to_string(userInt) || userInt > max || userInt < 0) {
      cout << INVALID_INPUT << endl;
      userInt = -1;
    }
    else {
      return userInt;
    }
  }
}
