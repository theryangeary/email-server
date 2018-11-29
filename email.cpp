// Ryan Geary and Anna Schaal
#include "email.h"

using namespace std;

sqlite3* db;

static int authorizationCallback(void *data, int argc, char** argv, char** azColName) {
  if (argc != 1) {
    return 1;
  }
  else {
    printf(WELCOME_MESSAGE, argv[0] ? argv[0] : NULL_STRING);
    return 0;
  }
}

static int sendCallback(void *data, int argc, char** argv, char** azColName) {
  printf(MESSAGE_SENT);
}

static int receiveCallback(void *data, int argc, char** argv, char** azColName) {
  printf("%s, %s", "SENDER", "THIS IS THE MESSAGE");
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

  char* sql = CREATE_TABLE_USERS;
  result = sqlite3_exec(db, sql, authorizationCallback, 0, &zErrMsg);

  while(1) {
    start();
  }

  return 0;
}

void start(){
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
    }
  }

  menu(username);
}

void menu(string sessionUser){
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

  cout << USERNAME_PROMPT;
  cin >> username;

  cout << PASSWORD_PROMPT;
  cin >> password;

  // TODO Check that user exists and password is correct
  // If not, return an empty string
  return username;
}

string reg() {
  int result;
  char* zErrMsg = 0;

  string username;
  string password;
  string passwordConfirm;

  cout << USERNAME_PROMPT << endl;
  cin >> username;

  cout << PASSWORD_PROMPT << endl;
  cin >> password;

  cout << CONFIRM_PASSWORD_PROMPT << endl;
  cin >> passwordConfirm;

  if (password == passwordConfirm) {
    int length = strlen(INSERT_USER) + 2*strlen(username.c_str()) + 2*strlen(password.c_str()) + 1;
    char* sql = (char*) malloc(length);
    string usernameCopy = username;
    string passwordCopy = password;
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
