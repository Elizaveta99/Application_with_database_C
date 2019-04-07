#include "change_quaries.h"
#include "orders_work.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void insert_data (sqlite3* db, struct User user)
{
    char *sql = malloc(256);
    sqlite3_stmt *res = NULL;
    int rc;
    char* err_msg = NULL;
    char date_end[256];
    char composition[256];
    int amount;
    
    fprintf(stdout, "put info ([date_end (YYMMDD)] [composition] [amount]: \n");
    scanf("%s %s %d",date_end, composition, &amount);
    
    int comp_id = 0;
    int cust_id = 0;
    int order_id = 0;
    int step;
    
    sprintf(sql,"SELECT Id FROM Flower_compositions WHERE Composition_name='%s'",composition);
    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    step = sqlite3_step(res);
    if (step == SQLITE_ROW)
       comp_id = atoi((char*)sqlite3_column_text(res, 0));
    
    sprintf(sql,"SELECT User_id FROM Authorization WHERE Login='%s'",user.login);
    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    step = sqlite3_step(res);
    if (step == SQLITE_ROW)
        cust_id = atoi((char*)sqlite3_column_text(res, 0));
    
    sprintf(sql, "SELECT max(Id) FROM Orders");
    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    step = sqlite3_step(res);
    if (step == SQLITE_ROW)
        order_id = atoi((char*)sqlite3_column_text(res, 0));
    
    sqlite3_finalize(res);
    
    char date_begin[80];
    time_t seconds = time(0);
    struct tm* time_info = localtime(&seconds);
    strftime(date_begin, 80, "%d%d%d", time_info);
    
    sprintf(sql, "INSERT INTO Orders VALUES (%d, %s, %s, %d, %d)", order_id + 1 ,date_begin, date_end, amount, cust_id);
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK ) {
        fprintf(stderr, "Failed to insert data\n");
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
    }
    else {
        fprintf(stdout, "Data was inserted successfully\n");
        printf("The last id of the inserted row is %lld\n",sqlite3_last_insert_rowid(db));
    }
    free(sql);
    
    sql = malloc(256);
    sprintf(sql, "SELECT max(Id) FROM OrdersFlower_compositions");
    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    step = sqlite3_step(res);
    int comp_order_id = -1;
    if (step == SQLITE_ROW)
        comp_order_id = atoi((char*)sqlite3_column_text(res, 0));
    free(sql);
    
    
    sql = malloc(256);
    sprintf(sql, "INSERT INTO OrdersFlower_compositions VALUES (%d, %d, %d)", comp_order_id + 1, comp_id, comp_order_id + 1);
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK ) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
    }
    free(sql);
    
    count_order_price(db, order_id + 1, date_begin, date_end, cust_id, comp_id, amount);
}

void update_data (sqlite3* db)
{
    char *sql = malloc(256);
    int rc;
    char* err_msg;
    char field[256];
    char value[256];
    int id;
    
    fprintf(stdout, "put 1 to update cost of the flower or 2 to update order : ");
    int type;
    scanf("%d", &type);
    if (type == 1)
    {
        forbid_increase_price(db);
    }
    else
    {
        fprintf(stdout, "\nput data need to update ([id] [field] [value]): ");
        scanf("%d %s %s",&id, field, value);
        sprintf(sql, "UPDATE Orders SET %s = '%s' WHERE Id = %d", field, value, id);
        rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
        if (rc != SQLITE_OK ) {
            fprintf(stderr, "Failed to update data\n");
            fprintf(stderr, "SQL error: %s\n", err_msg);
            sqlite3_free(err_msg);
        }
        else
            fprintf(stdout, "Data was updated successfully\n");
        free(sql);
    }
}

void delete_data(sqlite3* db)
{
    char *sql = malloc(256);
    int rc;
    char* err_msg;
    int id;
    
    fprintf(stdout, "put id of order: ");
    scanf("%d",&id);
    sprintf(sql, "DELETE FROM Orders WHERE Id = %d", id);
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK ) {
        fprintf(stderr, "Failed to delete data\n");
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
    }
    else
        fprintf(stdout, "Data was deleted successfully\n");
    free(sql);
}
