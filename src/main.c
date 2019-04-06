#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "select_quaries.h"
#include "change_quaries.h"
#include "process_images.h"

int autorize(sqlite3 *db, struct User* user)
{
    char login_s[login_size];
    char password_s[password_size];
    char role_s[role_size];
    
    char* password_ok_s = malloc(password_size);
    
    fprintf(stdout, "Enter login: ");
    scanf("%s", login_s);
    
    char *sql = malloc(256);
    sqlite3_stmt *res = NULL;
    int rc;
    
    (*user).role = -1;
    
    sprintf(sql,"SELECT Login, Password, Role FROM Authorization WHERE Login=@login");
    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);
    if (rc == SQLITE_OK)
    {
        int idx = sqlite3_bind_parameter_index(res, "@login");
        sqlite3_bind_text(res, idx, login_s, -1, NULL);
    }
    else
    {
        sprintf((*user).login, "failed_login");
        return -1;
    }
    int step = sqlite3_step(res);
    if (step == SQLITE_ROW)
    {
        sprintf((*user).login, "%s", login_s);
        sprintf(password_ok_s, "%s", sqlite3_column_text(res, 1));
        sprintf(role_s, "%s", sqlite3_column_text(res, 2));
        (*user).role = 0;
        if (strcmp(role_s, "customer") == 0)
            (*user).role = 1;
    }
    else
    {
        sprintf((*user).login, "failed_login");
        return -1;
    }
    
    
    fprintf(stdout, "\nEnter password: ");
    scanf("%s", password_s);
    
    if (strcmp(password_s, password_ok_s) == 0)
        fprintf(stdout, "Successful enter\n");
    else
    {
        sprintf((*user).login, "failed_password");
        return -1;
    }
    
    sqlite3_reset(res);
    sqlite3_finalize(res);
    return 1;
}


void perform_operation(sqlite3* db, struct User user) {
    int choice = 0;
    int role = user.role;
    
    do {
        fprintf(stdout, "1 - SELECT\n");
        if (role == 0)
            fprintf(stdout, "2 - INSERT PHOTO\n");
        else
            fprintf(stdout, "2 - INSERT\n");
        fprintf(stdout, "3 - UPDATE\n");
        fprintf(stdout, "4 - DELETE\n");
        if (role == 0)
            fprintf(stdout, "5 - PRINT PHOTO TO FILE\n");
        
        fprintf(stdout, "0 - EXIT\n");
        fprintf(stdout, "Choose one of operations: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                select_data(db, user);
                break;
            case 2:
            {
                if (role == 0)
                    insert_image(db);
                else
                    insert_data(db, user);
                break;
            }
            case 3:
                update_data(db);
                break;
            case 4:
                delete_data(db);
                break;
            case 5:
                read_image(db);
                break;
            case 0:
                break;
            default:
                break;
        }
    }while (choice != 0);
}


int main()
{
    sqlite3* db;
    int rc = sqlite3_open("Flowers.db", &db);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }
    
    // trying authorise
    struct User user;
    int res = autorize(db, &user);
    if (res == -1)
    {
        fprintf(stdout, "%s\n", user.login);
        if (strcmp(user.login, "failed_login") == 0)
            fprintf(stdout, "Invalid login\n");
        else
            fprintf(stdout, "Invalid password\n");
        sqlite3_close(db);
        return 0;
    }
    
    perform_operation(db, user);
    sqlite3_close(db);
    
    return 0;
}
