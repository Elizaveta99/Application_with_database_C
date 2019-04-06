#include "orders_work.h"

void forbid_increase_price ()
{
    
}

void count_order_price (sqlite3* db)
{
    //called from insert
    sqlite3_stmt *res = NULL;
    char *sql = malloc(256);
    //char date[9];
    sprintf(sql, "SELECT COUNT(*) FROM Orders");
    int rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    if (rc != SQLITE_OK)
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
    
    
//    sprintf(sql, "SELECT * FROM Orders WHERE Date_begin ='%s' GROUP BY Orders.Id", date);
//    int rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
//    if (rc != SQLITE_OK)
//        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
//
//    int step = sqlite3_step(res);
//    int column = sqlite3_column_count(res);
//    if (step == SQLITE_ROW)
//        print_head(res, column);
//    else
//        fprintf(stdout,"No such records\n");
//    while (step == SQLITE_ROW) {
//        print_results(res, column);
//        step = sqlite3_step(res);
//    }
//    sqlite3_finalize(res);
//
//    free(sql);
}
