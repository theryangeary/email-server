#include "main.h"

using namespace std;

int main(){
  start();
  return 0;
}

void start(){
  string username = "";

  while ("" == username) {
    int userInt = showMenu(START_MENU, 1);
    if(userInt == 0){
      username = reg();
    }
    else if(userInt == 1){
      username = login();
    }
  }
}

string login() {
  string username;
  string password;

  cout << "Username: " << endl;
  cin >> username;

  cout << "Password: " << endl;
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
    if (userInt > max || userInt < 0) {
      cout << "Please enter a valid number" << endl;
      userInt = -1;
    }
    else {
      return userInt;
    }
  }
}
