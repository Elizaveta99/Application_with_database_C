#include "change_quaries.h"
#include "process_images.h"
#include "orders_work.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void insert_data (sqlite3* db, char* login)
{
    char *sql = malloc(256);
    sqlite3_stmt *res = NULL;
    int rc;
    char* err_msg;
    char date_end[256];
    char composition[256];
    int amount;
    
    fprintf(stdout, "put info ([date_end] [composition] [amount]: \n");
    scanf("%s %s %d",date_end, composition, &amount);
    
    sprintf(sql,"SELECT id FROM Compositions WHERE Composition_name='%s'",composition);
    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    int comp_id = atoi((char*)sqlite3_column_text(res, 0));
    sqlite3_finalize(res);
    
    sprintf(sql,"SELECT id FROM Flower_compositions WHERE Login='%s'",login);
    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    int cust_id = atoi((char*)sqlite3_column_text(res, 0));
    sqlite3_finalize(res);
    
    char date_begin[80];
    time_t seconds = time(0);
    struct tm* time_info = localtime(&seconds);
    strftime(date_begin, 80, "%d.%m.%y", time_info);
    
    sprintf(sql, "INSERT INTO Order VALUES (%lld, '%s', '%s', %d, %d, %d)",sqlite3_last_insert_rowid(db)+1,date_begin, date_end, cust_id, comp_id, amount);
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
    count_order_price(db);
}

void update_data (sqlite3* db)
{
    char *sql = malloc(256);
    int rc;
    char* err_msg;
    char field[256];
    char value[256];
    int id;
    
    fprintf(stdout, "put data need to update ([id] [field] [value]): ");
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
