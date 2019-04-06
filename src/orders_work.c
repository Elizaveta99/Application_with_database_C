#include "orders_work.h"
#include "select_quaries.h"

void forbid_increase_price ()
{
    
}

void count_order_price (sqlite3* db, int order_id, char date_begin[], char date_end[], int cust_id, int comp_id, int amount)
{
    //called from insert
    sqlite3_stmt *res = NULL;
    char *sql = malloc(256);
    int rc, step;
    
    sprintf (sql, "SELECT * FROM OrdersFlower_compositions");
    
    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    if (rc != SQLITE_OK)
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
    step = sqlite3_step(res);
    int column = sqlite3_column_count(res);
    if (step == SQLITE_ROW)
        print_head(res, column);
    else
        fprintf(stdout,"No such records\n");
    while (step == SQLITE_ROW) {
        print_results(res, column);
        step = sqlite3_step(res);
    }
    //(SELECT Amount FROM Flower_compositions WHERE Flower_compositions.Id IN \
    
    // int a = 4; // 3
    //sprintf(sql, "SELECT Cost FROM Flowers WHERE Flowers.Id=%d", a);
    
    //    sprintf(sql, "SELECT Cost FROM Flowers WHERE Flowers.Id IN \
    //            (SELECT Flowers_id FROM FlowersFlower_compositions WHERE Flower_compositions_id=%d)", a);
    //free(sql);
    //sql = malloc(256);
    sprintf(sql, "SELECT Cost FROM Flowers WHERE Flowers.Id IN \
            (SELECT Flowers_id FROM FlowersFlower_compositions WHERE Flower_compositions_id IN \
            (SELECT Flower_compositions_id FROM OrdersFlower_compositions WHERE Order_id=%d))", order_id);
    
    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    if (rc != SQLITE_OK)
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
    
    step = sqlite3_step(res);
    int cost = -1;
    
    //int column = sqlite3_column_count(res);
    if (step == SQLITE_ROW)
        cost = sqlite3_column_int(res, 0); // ??
    else
        fprintf(stdout,"No such records\n");
    
    fprintf(stdout, "%d\n", cost);
    
    //insert in orderscost
    
    sqlite3_finalize(res);
    //free(sql);
}
