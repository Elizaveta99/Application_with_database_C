<<<<<<< HEAD
=======
#include <stdlib.h>
>>>>>>> dev_database
#include "orders_work.h"
#include "select_quaries.h"

void forbid_increase_price ()
{
    
}

void count_order_price (sqlite3* db, int order_id, char date_begin[], char date_end[], int cust_id, int comp_id, int amount)
{
<<<<<<< HEAD
    //called from insert
    sqlite3_stmt *res = NULL;
    char *sql = malloc(256);
    int rc, step;
    
    sprintf (sql, "SELECT * FROM OrdersFlower_compositions");
=======
    sqlite3_stmt *res = NULL;
    char *sql = malloc(256);
    int rc, step;
    char* err_msg = NULL;

    sprintf(sql, "SELECT sum(Cost) FROM Flowers WHERE Flowers.Id IN \
            (SELECT Flowers_id FROM FlowersFlower_compositions WHERE Flower_compositions_id IN \
            (SELECT Flower_compositions_id FROM OrdersFlower_compositions WHERE Order_id=%d))", order_id);
>>>>>>> dev_database
    
    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    if (rc != SQLITE_OK)
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
<<<<<<< HEAD
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
=======

    step = sqlite3_step(res);
    int cost = -1;
    if (step == SQLITE_ROW)
        cost = sqlite3_column_int(res, 0);
    else
        fprintf(stdout,"No such records\n");
    
    //fprintf(stdout, "%d\n", cost);
    
    
    sprintf(sql, "SELECT Amount FROM Flower_compositions WHERE Id IN \
            (SELECT Flower_compositions_id FROM OrdersFlower_compositions WHERE Order_id=%d)", order_id);
>>>>>>> dev_database
    
    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    if (rc != SQLITE_OK)
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
    
    step = sqlite3_step(res);
<<<<<<< HEAD
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
=======
    int amount_flowers = -1;
    if (step == SQLITE_ROW)
        amount_flowers = sqlite3_column_int(res, 0);
    else
        fprintf(stdout,"No such records\n");
    
    //fprintf(stdout, "%d\n", amount_flowers);
    
    int sum = amount * amount_flowers * cost;
    
    sprintf(sql, "SELECT max(Order_id) FROM OrderCost");
    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    step = sqlite3_step(res);
    int order_cost_id = 0;
    if (step == SQLITE_ROW)
        order_cost_id = sqlite3_column_int(res, 0);
        //atoi((char*)sqlite3_column_text(res, 0));
    
    sprintf(sql, "INSERT INTO OrderCost VALUES (%d, %d)", order_cost_id + 1, sum);
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK ) {
        fprintf(stderr, "Failed to insert data\n");
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
    }
    else {
        fprintf(stdout, "Order with the cost = %d was inserted successfully\n", sum);
    }
    
    sqlite3_finalize(res);
>>>>>>> dev_database
}
