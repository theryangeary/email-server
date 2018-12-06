// Ryan Geary and Anna Schall

#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <sqlite3.h>
#include <stdio.h>

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
#define CHECK_USERNAME_UNIQUE "select name from users where name='%1$s';"
#define INSERT_USER "insert into users values (NULL, '%1$s', '%2$s'); select id, name from users where name='%1$s' and password='%2$s';"
#define INSERT_MESSAGE "insert into messages values (NULL, '%1$d', '%2$d', '%3$s', '%4$s');"
#define CHECK_USER "select id, name from users where name = ? and password = ?;"
#define GET_ALL_USERS "select id, name from users;"
#define GET_MAIL_USER_ID "select id, subject from messages where receiver='%1$s';"
#define GET_MAIL "select subject, message from messages where id='%1$s' and receiver='%2$s';"

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

typedef struct sessionUser {
  int id;
  string name;
} sessionUser;
