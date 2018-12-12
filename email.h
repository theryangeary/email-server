// Ryan Geary and Anna Schall

#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <sqlite3.h>
#include <stdio.h>
#include <vector>
#include <cstdlib>
#include <gcrypt.h>
#include "encrypt.h"

#define START_MENU "(0): Register\n(1): Login\n(2): Quit"
#define START_MENU_OPT_NUM 2
#define MAIN_MENU "(0): Read mail\n(1): Send mail\n(2): Logout"
#define MAIN_MENU_OPT_NUM 2

#define SOMETHING_WENT_WRONG "Something went wrong"
#define WELCOME_MESSAGE "Welcome %s!\n"
#define MAIL_FORMAT "Subject: %s\n%s\n"
#define NULL_STRING "NULL"
#define SUBJECT_PROMPT "Enter subject: "
#define MESSAGE_PROMPT "Enter message: "
#define KEY_PROMPT "Enter secret key: "
#define MESSAGE_SENT "Message sent\n"
#define MESSAGE_RECEIVED "Message from %s:\n\n%s\n"
#define CHOOSE_USER_PROMPT "Enter user ID to send mail to:"
#define CHOOSE_MAIL_PROMPT "Enter Mail ID:"

#define INVALID_INPUT "Please enter a valid number"
#define USERNAME_TAKEN "Username already taken"

#define PASSWORD_PROMPT "Password: "
#define USERNAME_PROMPT "Username: "
#define CONFIRM_PASSWORD_PROMPT "Confirm password: "
#define PASSWORD_MISMATCH "Passwords don't match"
#define SEPARATOR ": "

#define DB_NAME "email.db"
#define DB_OPEN_ERROR "Can't open database: %s\n"
#define CREATE_TABLE_USERS ((char*) "create table if not exists USERS(id integer primary key autoincrement, NAME text, PASSWORD varchar(128));")
#define CREATE_TABLE_MESSAGES ((char*) "create table if not exists MESSAGES(id integer primary key autoincrement, SENDER integer, RECEIVER integer, SUBJECT text, MESSAGE text);")
#define CHECK_USERNAME_UNIQUE "select name from users where name = ?;"
#define INSERT_USER "insert into users (name, password) values (?, ?); select id, name from users where name = ? and password = ?;"
#define INSERT_MESSAGE ((char*) "insert into messages (sender, receiver, subject, message) values (?, ?, ?, ?);")
#define CHECK_USER "select id, name from users where name = ? and password = ?;"
#define GET_ALL_USERS "select id, name from users;"
#define GET_MAIL_USER_ID "select id, subject from messages where receiver='%1$s';"
#define GET_MAIL ((char*) "select subject, message from messages where id=? and receiver=?;")

#define MENU_WRAP_BEGIN "("
#define MENU_WRAP_END ")"

#define EMPTY_STRING ""
using namespace std;

string reg();
string login();
void read();
void send();
int start();
void menu();
void showUsers();
int showMenu(string options, int max);
int secureSqlQuery(
    char* query,
    vector<string> parameters,
    int numResultColumns,
    int (callback(vector<string>))
    );
char* hashPassword(string password);

typedef struct sessionUser {
  int id;
  string name;
} sessionUser;
