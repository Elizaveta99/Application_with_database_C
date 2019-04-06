#include <stdio.h>
#include <sqlite3.h>
#include "user.h"

void insert_data(sqlite3* db, struct User user);
void update_data(sqlite3* db);
void delete_data(sqlite3* db);
