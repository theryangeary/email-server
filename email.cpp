#include "main.h"

using namespace std;

int main(){
  start();
  return 0;
}


void start(){
  int userInt = showMenu("(0): Register\n(1): Login", 1);
  if(userInt == 0){
    reg();
  }
  else if(userInt == 1){
    login();
  }
}

void login() {
}

void reg() {
}

int showMenu(string options, int max) {
  int userInt = -1;
  while(-1 == userInt) {
    cout << options << endl;
    string userReply;
    cin >> userReply;
    int userInt = atoi(userReply.c_str());
    if (userInt > max) {
      cout << "Please enter a valid number" << endl;
      userInt = -1;
    }
    else {
      return userInt;
    }
  }
}
