#include <stdlib.h>
#include "orders_work.h"
#include "select_quaries.h"

void forbid_increase_price (sqlite3* db)
{
    int inc, flower_id;
    fprintf(stdout, "put increase value and id of the flower : ");
    scanf("%d %d", &inc, &flower_id);
    
    sqlite3_stmt *res = NULL;
    char *sql = malloc(256);
    int rc, step;
    char* err_msg = NULL;
    
    // cost of por entered flower
    sprintf(sql, "SELECT Cost FROM Flowers WHERE Flowers.Id=%d", flower_id);
    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    if (rc != SQLITE_OK)
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
    step = sqlite3_step(res);
    int cost_beg = -1;
    if (step == SQLITE_ROW)
        cost_beg = sqlite3_column_int(res, 0);
    else
        fprintf(stdout,"No such records\n");
    
    int new_cost = cost_beg + inc;

    int com_old_cost = 0, com_new_cost = 0;
    
    sprintf(sql, "SELECT FlowersFlower_compositions.Flower_compositions_id FROM FlowersFlower_compositions \
            WHERE FlowersFlower_compositions.Flowers_id=%d", flower_id);
    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    if (rc != SQLITE_OK)
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
    step = sqlite3_step(res);
    while (step == SQLITE_ROW)
    {
        sqlite3_stmt *res1 = NULL;
        char *sql1 = malloc(256);
        int rc1, step1;
        
        int comp_id = sqlite3_column_int(res, 0);
        
        // get amount of each flower in this composition (in while do this for each composition with our entered flower)
        sprintf(sql1, "SELECT Amount FROM Flower_compositions WHERE Flower_compositions.Id=%d", comp_id);
        rc1 = sqlite3_prepare_v2(db, sql1, -1, &res1, 0);
        if (rc1 != SQLITE_OK)
            fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
        step1 = sqlite3_step(res1);
        int amount_in_comp = -1;
        if (step1 == SQLITE_ROW)
            amount_in_comp = sqlite3_column_int(res1, 0);
        else
            fprintf(stdout,"No such records\n");
        
        // for this composition choose all flowers in it and calc sum of costs
        sprintf(sql1, "SELECT sum(Cost) FROM Flowers WHERE Flowers.Id IN \
                (SELECT Flowers_id FROM FlowersFlower_compositions WHERE Flower_compositions_id=%d)", comp_id);
        rc1 = sqlite3_prepare_v2(db, sql1, -1, &res1, 0);
        if (rc1 != SQLITE_OK)
            fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
        step1 = sqlite3_step(res1);
        int sum_costs_in_comp = -1;
        if (step1 == SQLITE_ROW)
            sum_costs_in_comp = sqlite3_column_int(res1, 0);
        else
            fprintf(stdout,"No such records\n");
        
        // calc cost of this composition
        int comp_cost = amount_in_comp * sum_costs_in_comp;
        com_old_cost += comp_cost;
        
        // for this composition choose flowers except our entered flower in it and calc sum of costs
        sprintf(sql1, "SELECT sum(Cost) FROM Flowers WHERE Flowers.Id IN \
                (SELECT Flowers_id FROM FlowersFlower_compositions WHERE Flower_compositions_id=%d) \
                AND Flowers.Id!=%d", comp_id, flower_id);
        rc1 = sqlite3_prepare_v2(db, sql1, -1, &res1, 0);
        if (rc1 != SQLITE_OK)
            fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
        step1 = sqlite3_step(res1);
        int sum_costs_in_comp_without_fl = -1;
        if (step1 == SQLITE_ROW)
            sum_costs_in_comp_without_fl = sqlite3_column_int(res1, 0);
        else
            fprintf(stdout,"No such records\n");
        
        // calc cost of this composition
        int comp_without_fl_cost = amount_in_comp * sum_costs_in_comp_without_fl;
        com_new_cost += comp_without_fl_cost;
        
        com_new_cost += amount_in_comp * new_cost;
        
        free(sql1);
        step = sqlite3_step(res);
    }
    

    if (com_new_cost - com_old_cost <= com_old_cost * 0.1)
    {
        sprintf(sql, "UPDATE Flowers SET Cost = %d", new_cost);
        rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
        if (rc != SQLITE_OK ) {
            fprintf(stderr, "Failed to update data\n");
            fprintf(stderr, "SQL error: %s\n", err_msg);
            sqlite3_free(err_msg);
        }
        else
            fprintf(stdout, "Data was updated successfully\n");
    }
    else
        fprintf(stdout, "Failed! Summary cost of all compositions with this flower increased more than 10 percent\n");
    
    free(sql);
}

void count_order_price (sqlite3* db, int order_id, char date_begin[], char date_end[], int cust_id, int comp_id, int amount)
{
    sqlite3_stmt *res = NULL;
    char *sql = malloc(256);
    int rc, step;
    char* err_msg = NULL;

    sprintf(sql, "SELECT sum(Cost) FROM Flowers WHERE Flowers.Id IN \
            (SELECT Flowers_id FROM FlowersFlower_compositions WHERE Flower_compositions_id IN \
            (SELECT Flower_compositions_id FROM OrdersFlower_compositions WHERE Order_id=%d))", order_id);
    
    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    if (rc != SQLITE_OK)
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));

    step = sqlite3_step(res);
    int cost = -1;
    if (step == SQLITE_ROW)
        cost = sqlite3_column_int(res, 0);
    else
        fprintf(stdout,"No such records\n");
    
    //fprintf(stdout, "%d\n", cost);
    
    
    sprintf(sql, "SELECT Amount FROM Flower_compositions WHERE Id IN \
            (SELECT Flower_compositions_id FROM OrdersFlower_compositions WHERE Order_id=%d)", order_id);
    
    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    if (rc != SQLITE_OK)
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
    
    step = sqlite3_step(res);
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
}
