#include "select_quaries.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void print_head (sqlite3_stmt *res, int column)
{
    for (int i = 0; i< column - 1; i++)
        printf("%s\t",sqlite3_column_name(res,i));
    printf("\n");
}

void print_results (sqlite3_stmt *res, int column)
{
    for (int i = 0; i< column - 1; i++)
        printf("%s\t",sqlite3_column_text(res,i));
    printf("\n");
}

void select_data(sqlite3* db)
{
    sqlite3_stmt *res = NULL;
    int choice;
    
    fprintf(stdout, "select all (1)\n");
    fprintf(stdout, "select sum for period (2 [data_begin] [data_end])\n");
    fprintf(stdout, "select composition info with max demand (3)\n");
    fprintf(stdout, "select count of performed express orders (4)\n");
    fprintf(stdout, "select used flowers info for period (5)\n");
    fprintf(stdout, "select sailed compositions info (6)\n");
    fprintf(stdout, "select orders info for data (7 [data])\n");
    fprintf(stdout, "Your choice: ");
    scanf( "%d", &choice);
    
    switch(choice)
    {
        case 1:
        {
            char* sql = "SELECT * FROM Orders";
            int rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
            if (rc != SQLITE_OK)
                fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
            break;
        }
        case 2:
            get_sum_for_period(db, &res);
            break;
        case 3:
            get_max_demand_composition_info(db, &res);
            break;
        case 4:
            get_express_orders_count(db, &res);
            break;
        case 5:
            get_used_flowers_count(db, &res);
        case 6:
            get_sailed_compositions_info(db, &res);
            break;
        case 7:
            get_orders_for_date(db, &res);
            break;
        default:
            break;
    }
    int step = sqlite3_step(res);
    int column = sqlite3_column_count(res);
    if (step == SQLITE_ROW)
        print_head(res, column);
    else
        fprintf(stdout,"No such records\n");
    while (step == SQLITE_ROW) {
        print_results(res, column);
        step = sqlite3_step(res);
    }
     sqlite3_finalize(res);
}

void get_sum_for_period (sqlite3* db, sqlite3_stmt **res)
{
    //get datas
}

void get_max_demand_composition_info(sqlite3* db, sqlite3_stmt **res)
{
    char* sql;
    sql = "SELECT Composition_name, Flower_compositions.Amount, Flowers.Name AS Flower_name, Flowers.Kind, Flowers.Cost, count (Orders.Id) AS Orders_num \
        FROM Orders,    OrdersFlower_compositions, Flower_compositions , FlowersFlower_compositions, Flowers \
        WHERE Orders.Id= OrdersFlower_compositions.Order_id AND OrdersFlower_compositions.Flower_compositions_id=Flower_compositions.Id AND Flower_compositions.Id=FlowersFlower_compositions.Flower_compositions_id AND  FlowersFlower_compositions.Flowers_id=Flowers.Id \
    GROUP BY Orders.Id HAVING count(Orders.Id) =\
    (SELECT max(A.Orders_num) FROM (SELECT count(Orders.Id) AS Orders_num \
    FROM Orders , OrdersFlower_compositions, Flower_compositions , FlowersFlower_compositions, Flowers \
    WHERE Orders.Id= OrdersFlower_compositions.Order_id AND OrdersFlower_compositions.Flower_compositions_id=Flower_compositions.Id AND Flower_compositions.Id=FlowersFlower_compositions.Flower_compositions_id AND  FlowersFlower_compositions.Flowers_id=Flowers.Id   \
    GROUP BY Orders.Id) AS A)";
    int rc = sqlite3_prepare_v2(db, sql, -1, res, 0);
    if (rc != SQLITE_OK)
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
}

void get_express_orders_count(sqlite3* db, sqlite3_stmt **res)
{
    
}

void get_used_flowers_count(sqlite3* db, sqlite3_stmt **res)
{
    
}

void get_sailed_compositions_info(sqlite3* db, sqlite3_stmt **res)
{
    char *sql = malloc(256);
    char current_date[11];
    char date_end [11];
    time_t seconds = time(0);
    struct tm* time_info = localtime(&seconds);
    strftime(current_date, 11, "%Y%m%d", time_info);
   // char* temp_date =(char*)sqlite3_column_text(*res,2);
    //temp_date to tm
    // strftime(date_end, 11, "%Y%m%d", tm_date);
    //Date_end must be YYYYMMDD ???
    
    sprintf(sql, "SELECT Composition_name, Flower_compositions.Amount, Flowers.Name AS Flower_name, Flowers.Kind, Flowers.Cost\
    FROM Orders,    OrdersFlower_compositions, Flower_compositions , FlowersFlower_compositions, Flowers \
    WHERE Orders.Id= OrdersFlower_compositions.Order_id AND OrdersFlower_compositions.Flower_compositions_id=Flower_compositions.Id AND Flower_compositions.Id=FlowersFlower_compositions.Flower_compositions_id AND  FlowersFlower_compositions.Flowers_id=Flowers.Id \
        AND Date_end <='%s'  GROUP BY Orders.Id", current_date);
    int rc = sqlite3_prepare_v2(db, sql, -1, res, 0);
    if (rc != SQLITE_OK)
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
    free(sql);
}

void get_orders_for_date (sqlite3* db, sqlite3_stmt **res)
{
    char *sql = malloc(256);
    char temp_date[20];
    char date[11];
    
    scanf(" %s",temp_date);
    
    //temp_date to tm ?
    // strftime(date, 11, "%Y%m%d", tm_date);

    sprintf(sql, "SELECT * FROM Orders WHERE Date_begin ='%s' GROUP BY Orders.Id", date);
    int rc = sqlite3_prepare_v2(db, sql, -1, res, 0);
    if (rc != SQLITE_OK)
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
    free(sql);
}
