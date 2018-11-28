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
  string username = "";

  while ("" == username) {
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

  cout << "Username: ";
  cin >> username;

  cout << "Password: ";
  cin >> password;

  // TODO Check that user exists and password is correct
  // If not, return an empty string
  return username;
}

string reg() {
  string username;
  string password;
  string passwordConfirm;

  cout << "Username: " << endl;
  cin >> username;

  cout << "Password: " << endl;
  cin >> password;

  cout << "Confirm password: " << endl;
  cin >> passwordConfirm;

  if (password == passwordConfirm) {
    // TODO store in db
    return username;
  }
  else {
    cout << "Passwords don't match" << endl;
    return "";
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
      cout << "Please enter a valid number" << endl;
      userInt = -1;
    }
    else {
      return userInt;
    }
  }
}
