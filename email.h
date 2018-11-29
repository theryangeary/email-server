// Ryan Geary and Anna Schall

#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <sqlite3.h>
#include <stdio.h>

#define START_MENU "(0): Register\n(1): Login"
#define START_MENU_OPT_NUM 1
#define MAIN_MENU "(0): Read mail\n(1): Send mail\n(2): Logout"
#define MAIN_MENU_OPT_NUM 2

#define WELCOME_MESSAGE "Welcome %s!\n"
#define NULL_STRING "NULL"
#define MESSAGE_SENT "Message sent\n"
#define MESSAGE_RECEIVED "Message from %s:\n\n%s\n"

#define INVALID_INPUT "Please enter a valid number"

#define PASSWORD_PROMPT "Password: "
#define USERNAME_PROMPT "Username: "
#define CONFIRM_PASSWORD_PROMPT "Confirm password: "
#define PASSWORD_MISMATCH "Passwords don't match"

#define DB_NAME "email.db"
#define DB_OPEN_ERROR "Can't open database: %s\n"
#define CREATE_TABLE_USERS ((char*) "create table if not exists USERS(id integer primary key autoincrement, NAME text, PASSWORD varchar(128));")

#define EMPTY_STRING ""
using namespace std;

string reg();
string login();
void read(string sessionUser);
void send(string sessionUser);
void start();
void menu(string sessionUser);
void showUsers();
int showMenu(string options, int max);

