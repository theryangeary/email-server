//Ryan Geary and Anna Schaal
#include "email.h"

using namespace std;

sqlite3* db;
int callbackFlag = 0;
string listMenu  = "";
int listMenuLength = 0;
int maxMailID = 0;
sessionUser user;

static int authorizationCallback(void *data, int argc, char** argv,
    char** azColName) {
  callbackFlag = 1;
  if (argc != 2) {
    return 1;
  }
  else {
    printf(WELCOME_MESSAGE, argv[1] ? argv[1] : NULL_STRING);
    user.id = atoi(argv[0]);
    user.name = argv[1];
    return 0;
  }
}

static int usernameUniqueCallback(void *data, int argc, char** argv,
    char** azColName) {
  return 1;
}

static int makeMenuCallback(void *data, int argc, char** argv,
    char** azColName) {
  listMenuLength++;
  maxMailID = max(maxMailID, atoi(argv[0]));
  listMenu = listMenu + "\n" + MENU_WRAP_BEGIN + argv[0] +
    MENU_WRAP_END + SEPARATOR + argv[1];
  return 0;
}

static int printMailCallback(void *data, int argc, char** argv,
    char** azColName) {
  if(argc != 2){
    return 1;
  }
  else{
    printf(MAIL_FORMAT, argv[0], argv[1]);
    return 0;
  }
}

int main(){
  int result;
  char* zErrMsg = 0;

  result = sqlite3_open(DB_NAME, &db);

  if( result ){
    fprintf(stderr, DB_OPEN_ERROR, sqlite3_errmsg(db));
    sqlite3_close(db);
    return 1;
  };

  char* create_table_users = CREATE_TABLE_USERS;
  result = sqlite3_exec(db, create_table_users, authorizationCallback, 0,
      &zErrMsg);
  char* create_table_messages = CREATE_TABLE_MESSAGES;
  result = sqlite3_exec(db, create_table_messages, authorizationCallback, 0,
      &zErrMsg);

  int next = 1;
  while(next) {
    next = start();
  }

  return 0;
}

int start(){
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
      case 2:
        return 0;
    }
  }

  menu();
  return 1;
}

void menu(){
  while(1) {
    int userInt = showMenu(MAIN_MENU, MAIN_MENU_OPT_NUM);
    switch (userInt) {
      case 0:
        read();
        break;
      case 1:
        send();
        break;
      case 2:
        return;
        break;
    }
  }
}

void read() {
  int result;
  char* zErrMsg = 0;

  listMenu = CHOOSE_MAIL_PROMPT;
  listMenuLength = 1;

  int length = strlen(INSERT_USER) + to_string(user.id).length() + 1;
  char* getMailList = (char*) malloc(length);
  snprintf(getMailList, length, GET_MAIL_USER_ID, to_string(user.id).c_str());
  result = sqlite3_exec(db, getMailList, makeMenuCallback, 0, &zErrMsg);
  free(getMailList);

  int choice = showMenu(listMenu, maxMailID);

  if (!choice) {
    return;
  }

  length = strlen(GET_MAIL) + to_string(choice).length() + 1;
  char* getMail = (char*) malloc(length);
  snprintf(getMail, length, GET_MAIL, to_string(choice).c_str(),
      to_string(user.id).c_str());
  result = sqlite3_exec(db, getMail, printMailCallback, 0, &zErrMsg);
  free(getMail);
}

void send() {
  int result;
  char* zErrMsg = 0;

  listMenu = CHOOSE_USER_PROMPT;
  listMenuLength = 0;

  result = sqlite3_exec(db, GET_ALL_USERS, makeMenuCallback, 0, &zErrMsg);

  int choice = showMenu(listMenu, listMenuLength);

  cout << SUBJECT_PROMPT << endl;
  cin.ignore();
  string subject = "";
  getline(cin, subject);
  cout << MESSAGE_PROMPT << endl;
  string message = "";
  getline(cin, message);


  int length = strlen(INSERT_MESSAGE) + to_string(user.id).length() +
    to_string(choice).length() + message.length() + 1;
  char* messageInsert = (char*) malloc(length);
  snprintf(messageInsert, length, INSERT_MESSAGE, user.id, choice,
      subject.c_str(), message.c_str());
  zErrMsg = 0;
  result = sqlite3_exec(db, messageInsert, NULL, 0, &zErrMsg);
  free(messageInsert);

  if (!result) {
    cout << MESSAGE_SENT;
  }
  else {
    cout << SOMETHING_WENT_WRONG << endl;
  }
}

string login() {
  int result = 1;
  char* zErrMsg = 0;

  string username;
  string password;

  cout << USERNAME_PROMPT;
  cin >> username;

  cout << PASSWORD_PROMPT;
  cin >> password;

  // TODO Check that user exists and password is correct
  // If not, return an empty string

  int length = strlen(CHECK_USER) + 2*strlen(username.c_str()) +
    2*strlen(password.c_str()) + 1;
  char* sql = (char*) malloc(length);
  snprintf(sql, length, CHECK_USER, username.c_str(), password.c_str());
  callbackFlag = 0;
  result = sqlite3_exec(db, sql, authorizationCallback, 0, &zErrMsg);
  if (callbackFlag) {
    return username;
  }
  else {
    return "";
  }
}

string reg() {
  int result;
  char* zErrMsg = 0;

  string username;
  string password;
  string passwordConfirm;

  cout << USERNAME_PROMPT << endl;
  cin >> username;

  result = 0;
  int length = strlen(CHECK_USERNAME_UNIQUE) + username.length() + 1;
  char* checkUsernameUnique = (char*) malloc(length*sizeof(char));
  snprintf(checkUsernameUnique, length, CHECK_USERNAME_UNIQUE,
      username.c_str());
  result = sqlite3_exec(db, checkUsernameUnique, usernameUniqueCallback,
      0, &zErrMsg);
  if (result) {
    cout << USERNAME_TAKEN << endl;
    return EMPTY_STRING;
  }
  zErrMsg = 0;

  cout << PASSWORD_PROMPT << endl;
  cin >> password;

  cout << CONFIRM_PASSWORD_PROMPT << endl;
  cin >> passwordConfirm;

  if (password == passwordConfirm) {
    int length = strlen(INSERT_USER) + 2*strlen(username.c_str()) +
      2*strlen(password.c_str()) + 1;
    char* sql = (char*) malloc(length);
    snprintf(sql, length, INSERT_USER, username.c_str(), password.c_str());
    result = sqlite3_exec(db, sql, authorizationCallback, 0, &zErrMsg);
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
