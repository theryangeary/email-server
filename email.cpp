#include <stdlib.h>
#include <string.h>
#include <iostream>
using namespace std;

void reg();
void login();
void read();
void send();
void start();
void menu();
void showUsers();


int main(){
  start();
  return 0;
}


void start(){
  string userReply;
  cout << "(0): Register\n(1): Login" << endl;
  cin >> userReply;
  cout << userReply<< endl;
  int userInt = atoi(userReply.c_str());
  if(userInt == 1){
    login();
  }
  else if(userInt == 0){
    reg();
  }
  else{
    cout << "Please enter a valid number" << endl;
  }


}
