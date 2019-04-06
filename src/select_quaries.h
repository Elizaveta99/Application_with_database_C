#include <stdio.h>
#include <sqlite3.h>
#include "user.h"

void select_data(sqlite3* db, struct User user);
void get_sum_for_period(sqlite3* db, sqlite3_stmt **res);
void get_max_demand_composition_info(sqlite3* db, sqlite3_stmt **res);
void get_express_orders_count(sqlite3* db, sqlite3_stmt **res);
void get_used_flowers_count(sqlite3* db, sqlite3_stmt **res);
void get_sailed_compositions_info(sqlite3* db, sqlite3_stmt **res);
void get_orders_for_date(sqlite3* db, sqlite3_stmt **res);
