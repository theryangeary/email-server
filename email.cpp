//Ryan Geary and Anna Schaal
#include "email.h"

using namespace std;

sqlite3* db;
int callbackFlag = 0;
string listMenu  = "";
int listMenuLength = 0;
int maxMailID = 0;
sessionUser user;

int emptyCallback(vector<string> parameters){
  return 0;
}

static int authorizationCallback(string id, string username) {
  callbackFlag = 1;
  printf(WELCOME_MESSAGE, username.c_str());
  user.id = atoi(id.c_str());
  user.name = username;
  return 0;
}

static int makeMenuCallback(void *data, int argc, char** argv,
    char** azColName) {
  listMenuLength++;
  maxMailID = max(maxMailID, atoi(argv[0]));
  listMenu = listMenu + "\n" + MENU_WRAP_BEGIN + argv[0] +
    MENU_WRAP_END + SEPARATOR + argv[1];
  return 0;
}

static int printMailCallback(vector<string> parameters) {
  if(parameters.size() != 2){
    return 1;
  }
  else{
    cin.ignore();
    cout << KEY_PROMPT << endl;
    string key = "";
    getline(cin, key);

    parameters[1] = decrypt(parameters[1], key);
    printf(MAIL_FORMAT, parameters[0].c_str(), parameters[1].c_str());
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
  result = sqlite3_exec(db, create_table_users, NULL, 0,
      &zErrMsg);
  char* create_table_messages = CREATE_TABLE_MESSAGES;
  result = sqlite3_exec(db, create_table_messages, NULL, 0,
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

  int length = strlen(GET_MAIL_USER_ID) + to_string(user.id).length() + 1;
  char* getMailList = (char*) malloc(length);
  snprintf(getMailList, length, GET_MAIL_USER_ID, to_string(user.id).c_str());
  result = sqlite3_exec(db, getMailList, makeMenuCallback, 0, &zErrMsg);
  free(getMailList);

  int choice = showMenu(listMenu, maxMailID);

  if (!choice) {
    return;
  }

  vector<string> parameters;
  parameters.push_back(to_string(choice).c_str());
  parameters.push_back(to_string(user.id).c_str());
  result = secureSqlQuery(GET_MAIL, parameters, parameters.size(),
      printMailCallback);
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
string key = "";
while(key.length() <= 0) {
  cout << KEY_PROMPT << endl;
  getline(cin, key);
}

  message = encrypt(message, key);

  vector<string> parameters;
  parameters.push_back(to_string(user.id).c_str());
  parameters.push_back(to_string(choice).c_str());
  parameters.push_back(subject);
  parameters.push_back(message);
  result = secureSqlQuery(INSERT_MESSAGE, parameters,
      parameters.size(), emptyCallback);

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

  string id;
  string username;
  string password;

  cout << USERNAME_PROMPT;
  cin.ignore();
  getline(cin, username);

  cout << PASSWORD_PROMPT;
  getline(cin, password);
  password = string(hashPassword(password));

  // TODO Check that user exists and password is correct
  // If not, return an empty string

  callbackFlag = 0;
  sqlite3_stmt *stmt;
  const char* pzTest;
  result = sqlite3_prepare(db, CHECK_USER, strlen(CHECK_USER), &stmt, &pzTest);
  if(!result) {
    sqlite3_bind_text(stmt, 1, username.c_str(), username.length(), 0);
    sqlite3_bind_text(stmt, 2, password.c_str(), password.length(), 0);

    bool done = false;
    while (!done) {
      switch(sqlite3_step(stmt)){
        case SQLITE_ROW:
          id = string(reinterpret_cast<const char*>(
                sqlite3_column_text(stmt, 0)));
          username = string(reinterpret_cast<const char*>(
                sqlite3_column_text(stmt, 1)));
          result = authorizationCallback(id, username);
          break;
        case SQLITE_DONE:
          done = true;
          break;
        default:
          cout << SOMETHING_WENT_WRONG << endl;
      }
    }
    sqlite3_finalize(stmt);
  }
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

  string id;
  string username;
  string password;
  string passwordConfirm;

  cout << USERNAME_PROMPT << endl;
  cin.ignore();
  getline(cin, username);

  result = 0;
  callbackFlag = 0;
  sqlite3_stmt *stmt;
  const char* pzTest;
  result = sqlite3_prepare(db, CHECK_USERNAME_UNIQUE,
      strlen(CHECK_USERNAME_UNIQUE), &stmt, &pzTest);
  if(!result) {
    sqlite3_bind_text(stmt, 1, username.c_str(), username.length(), 0);

    bool done = false;
    while (!done) {
      switch(sqlite3_step(stmt)){
        case SQLITE_ROW:
          result = 1;
          break;
        case SQLITE_DONE:
          done = true;
          break;
        default:
          cout << SOMETHING_WENT_WRONG << endl;
      }
    }
  }
  sqlite3_finalize(stmt);
  if (result) {
    cout << USERNAME_TAKEN << endl;
    return EMPTY_STRING;
  }
  zErrMsg = 0;

  cout << PASSWORD_PROMPT << endl;
  getline(cin, password);

  cout << CONFIRM_PASSWORD_PROMPT << endl;
  getline(cin, passwordConfirm);

  if (password == passwordConfirm) {
    password = string(hashPassword(password));
    callbackFlag = 0;
    sqlite3_stmt *stmt;
    const char* pzTest;
    result = sqlite3_prepare(db, INSERT_USER, strlen(INSERT_USER), &stmt,
        &pzTest);
    if(!result) {
      sqlite3_bind_text(stmt, 1, username.c_str(), username.length(), 0);
      sqlite3_bind_text(stmt, 2, password.c_str(), password.length(), 0);
      sqlite3_bind_text(stmt, 3, username.c_str(), username.length(), 0);
      sqlite3_bind_text(stmt, 4, password.c_str(), password.length(), 0);

      bool done = false;
      while (!done) {
        switch(sqlite3_step(stmt)){
          case SQLITE_ROW:
            id = string(reinterpret_cast<const char*>(
                  sqlite3_column_text(stmt, 0)));
            username = string(reinterpret_cast<const char*>(
                  sqlite3_column_text(stmt, 1)));
            result = authorizationCallback(id, username);
            break;
          case SQLITE_DONE:
            done = true;
            break;
          default:
            cout << SOMETHING_WENT_WRONG << endl;
        }
      }
      sqlite3_finalize(stmt);
    }
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

int secureSqlQuery(char* query, vector<string> parameters,
    int numResultColumns, int (callback(vector<string>))) {
  callbackFlag = 0;
  sqlite3_stmt *stmt;
  const char* pzTest;
  int result = sqlite3_prepare(db, query, strlen(query), &stmt, &pzTest);
  if(!result) {
    int i;
    for (i = 0; i < parameters.size(); i++) {
      sqlite3_bind_text(stmt, i+1, parameters[i].c_str(),
          parameters[i].length(), 0);
    }
  }

  bool done = false;
  while (!done) {
    switch(sqlite3_step(stmt)){
      case SQLITE_ROW:
        if (callback) {
          vector<string> resultColumns;
          int i;
          for (i = 0; i < numResultColumns; i++) {
            resultColumns.push_back(string(reinterpret_cast<const char*>(
                    sqlite3_column_text(stmt, i))));
          }
          result = callback(resultColumns);
        }
        else {
          result = 0;
        }
        break;
      case SQLITE_DONE:
        done = true;
        break;
      default:
        cout << SOMETHING_WENT_WRONG << endl;
    }
  }
  sqlite3_finalize(stmt);
  return result;
}

char* hashPassword(string password) {
  int msg_len = password.length();
  int hash_len = gcry_md_get_algo_dlen( GCRY_MD_SHA1 );
  unsigned char hash[ hash_len ];
  char *out = (char *) malloc( sizeof(char) * ((hash_len*2)+1) );
  gcry_md_hash_buffer( GCRY_MD_SHA1, hash, password.c_str(), msg_len );
  char* p = out;
  for (int i = 0; i < hash_len; i++, p+=2) {
    snprintf(p, 3, "%02x", hash[i]);
  }
  return out;
}
