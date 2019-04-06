#include "select_quaries.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void print_head (sqlite3_stmt *res, int column)
{
    for (int i = 0; i< column ; i++)
        printf("%s\t",sqlite3_column_name(res,i));
    printf("\n");
}

void print_results (sqlite3_stmt *res, int column)
{
    for (int i = 0; i< column ; i++)
    {
        char* c_name = (char*)sqlite3_column_name(res,i);
        if(!strcmp(c_name,"Date_end") || !strcmp(c_name,"Date_begin")) {
            char* temp = (char*)sqlite3_column_text(res,i);
            char year[5];
            memcpy(year, &temp[0], 4);
            year[4]='\0';
            char month[3];
            memcpy(month, &temp[4], 2);
            year[2]='\0';
            char day[3];
            memcpy(day, &temp[6], 3);
            printf("%s.%s.%s\t",day,month,year);
        }
        else
            printf("%s\t",sqlite3_column_text(res,i));
    }
    printf("\n");
}

void select_data(sqlite3* db, struct User user)
{
    sqlite3_stmt *res = NULL;
    int choice;
    int role = user.role;
    char* login = user.login;
    
    if(role == 0)
        fprintf(stdout, "select all (1 [table])\n");
    else
         fprintf(stdout, "select all (1)\n");
    fprintf(stdout, "select sum for period (2 [date_begin] [date_end])\n");
    fprintf(stdout, "select composition info with max demand (3)\n");
    fprintf(stdout, "select count of performed express orders (4)\n");
    fprintf(stdout, "select used flowers info for period (5 [date_begin] [date_end])\n");
    fprintf(stdout, "select sailed compositions info (6)\n");
    fprintf(stdout, "select orders info for data (7 [date (YYYYMMDD)])\n");
    fprintf(stdout, "Your choice: ");
    scanf( "%d", &choice);
    
    switch(choice)
    {
        case 1:
        {
            char sql[256];
            int rc;
            
            if(role == 0){
                char table[80];
                scanf( " %s", table);
                sprintf (sql, "SELECT * FROM '%s'", table);
            }
            else {
                int id = 0;
                sprintf(sql,"SELECT User_id FROM Authorization WHERE Login='%s'",login);
                rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
                int step = sqlite3_step(res);
                if (step == SQLITE_ROW)
                    id = atoi((char*)sqlite3_column_text(res, 0));
                sprintf (sql, "SELECT * FROM Orders WHERE User_id = %d", id);
            }
            rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
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
    // разность между датами 0 или 1
}

void get_used_flowers_count(sqlite3* db, sqlite3_stmt **res)
{
 // для каждого цветка вывести
}

void get_sailed_compositions_info(sqlite3* db, sqlite3_stmt **res)
{
    char *sql = malloc(256);
    char current_date[9];
    time_t seconds = time(0);
    struct tm* time_info = localtime(&seconds);
    strftime(current_date, 9, "%Y%m%d", time_info);
    
    sprintf(sql, "SELECT Composition_name, Flower_compositions.Amount, Flowers.Name AS Flower_name, Flowers.Kind, Flowers.Cost\
    FROM Orders,    OrdersFlower_compositions, Flower_compositions , FlowersFlower_compositions, Flowers \
    WHERE Orders.Id= OrdersFlower_compositions.Order_id AND OrdersFlower_compositions.Flower_compositions_id=Flower_compositions.Id AND Flower_compositions.Id=FlowersFlower_compositions.Flower_compositions_id AND  FlowersFlower_compositions.Flowers_id=Flowers.Id \
        AND Date_end <=%s  GROUP BY Orders.Id", current_date);
    int rc = sqlite3_prepare_v2(db, sql, -1, res, 0);
    if (rc != SQLITE_OK)
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
    free(sql);
}

void get_orders_for_date (sqlite3* db, sqlite3_stmt **res)
{
    char *sql = malloc(256);
    char date[9];
    
    scanf(" %s",date);
    sprintf(sql, "SELECT * FROM Orders WHERE Date_begin =%s GROUP BY Orders.Id", date);
    int rc = sqlite3_prepare_v2(db, sql, -1, res, 0);
    if (rc != SQLITE_OK)
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
    free(sql);
}
