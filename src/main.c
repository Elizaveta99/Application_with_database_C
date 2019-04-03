#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include "select_quaries.h"
#include "change_quaries.h"
#include "process_images.h"

void perform_operation(sqlite3* db, char* login) {
    int choice = 0;
    int role = 1; //int role = check_role ()
    
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
                select_data(db);
                break;
            case 2:
            {
                if (role == 0)
                    insert_image(db);
                else
                    insert_data(db, login);
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
    char* login = NULL;
    //autorize func (login)
    perform_operation(db, login);
    sqlite3_close(db);
    return 0;
}
