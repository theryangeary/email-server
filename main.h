// Ryan Geary and Anna Schall

#include <stdlib.h>
#include <string.h>
#include <iostream>

#define START_MENU "(0): Register\n(1): Login"
#define MAIN_MENU "(0): Read mail\n(1): Send mail\n(2): Logout"

using namespace std;

string reg();
string login();
void read();
void send();
void start();
void menu();
void showUsers();
int showMenu(string options, int max);

