#include <stdio.h>
#include <sqlite3.h>
//#include "user.h"

void forbid_increase_price();
void count_order_price(sqlite3* db, int order_id, char date_begin[], char date_end[], int cust_id, int comp_id, int amount);
