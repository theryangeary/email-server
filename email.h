// Ryan Geary and Anna Schall

#include <stdlib.h>
#include <string.h>
#include <iostream>

#define START_MENU "(0): Register\n(1): Login"
#define START_MENU_OPT_NUM 1
#define MAIN_MENU "(0): Read mail\n(1): Send mail\n(2): Logout"
#define MAIN_MENU_OPT_NUM 2

using namespace std;

string reg();
string login();
void read(string sessionUser);
void send(string sessionUser);
void start();
void menu(string sessionUser);
void showUsers();
int showMenu(string options, int max);

