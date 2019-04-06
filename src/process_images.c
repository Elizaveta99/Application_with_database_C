#include <stdio.h>
#include <stdlib.h>
#include "process_images.h"

void insert_image (sqlite3* db)
{
    char file_name[256];
    int id;
    fprintf(stdout, "put info ([file_name] [id])\n");
    scanf("%s %d", file_name, &id);
    
    FILE *fp = fopen(file_name, "rb");
    
    if (fp == NULL) {
        fprintf(stderr, "Cannot open image file\n");
        return;
    }
    fseek(fp, 0, SEEK_END);
    if (ferror(fp)) {
        fprintf(stderr, "fseek() failed\n");
        int r = fclose(fp);
        if (r == EOF)
            fprintf(stderr, "Cannot close file handler\n");
        return;
    }
    int flen = (int)ftell(fp);
    
    if (flen == -1) {
        perror("error occurred");
        int r = fclose(fp);
        if (r == EOF)
            fprintf(stderr, "Cannot close file handler\n");
        return;
    }
    fseek(fp, 0, SEEK_SET);
    if (ferror(fp)) {
        fprintf(stderr, "fseek() failed\n");
        int r = fclose(fp);
        if (r == EOF)
            fprintf(stderr, "Cannot close file handler\n");
        return;
    }
    char data[flen+1];
    int size = (int)fread(data, 1, flen, fp);
    
    if (ferror(fp)) {
        fprintf(stderr, "fread() failed\n");
        int r = fclose(fp);
        if (r == EOF) {
            fprintf(stderr, "Cannot close file handler\n");
        }
        return;
    }
    int r = fclose(fp);
    
    if (r == EOF)
        fprintf(stderr, "Cannot close file handler\n");
    
    sqlite3_stmt *p_stmt;
    int rc;
    char* sql;
    
    sql = "UPDATE states SET Photo=@Photo WHERE id = @id";
    rc = sqlite3_prepare(db, sql, -1, &p_stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot prepare statement: %s\n", sqlite3_errmsg(db));
        return;
    }
    int idx = sqlite3_bind_parameter_index(p_stmt, "@id");
    
    sqlite3_bind_int(p_stmt, idx, id);
    idx = sqlite3_bind_parameter_index(p_stmt, "@Photo");
    sqlite3_bind_blob(p_stmt, idx, data, size, SQLITE_STATIC);
    rc = sqlite3_step(p_stmt);
    if (rc != SQLITE_DONE)
        printf("Execution failed: %s", sqlite3_errmsg(db));
    
    fprintf(stdout,"Image inserted into database\n");
    sqlite3_finalize(p_stmt);
}

void read_image(sqlite3 *db)
{
    int id;
    char file_name[256];
    
    printf("put info ([file_name] [object_id]): ");
    scanf(" %s %d", file_name, &id);
    FILE *fp = fopen(file_name, "wb");
    
    if (fp == NULL) {
        fprintf(stderr, "Cannot open image file\n");
        return;
    }
    
    sqlite3_stmt *p_stmt;
    int rc;
    char* sql = "SELECT Photo FROM Flowers WHERE Id = @id";
    
    rc = sqlite3_prepare_v2(db, sql, -1, &p_stmt, 0);
    if (rc != SQLITE_OK ) {
        fprintf(stderr, "Failed to prepare statement\n");
        return;
    }
    
    int idx = sqlite3_bind_parameter_index(p_stmt, "@id");
    sqlite3_bind_int(p_stmt, idx, id);
    rc = sqlite3_step(p_stmt);
    
    int bytes = 0;
    idx = sqlite3_bind_parameter_index(p_stmt, "@Photo");
    
    if (rc == SQLITE_ROW) {
        bytes = sqlite3_column_bytes(p_stmt, idx);
    }
    fwrite(sqlite3_column_blob(p_stmt, idx), bytes, 1, fp);
    if (ferror(fp)) {
        fprintf(stderr, "fwrite() failed\n");
        return;
    }
    fprintf(stdout,"File %s with object photo was created\n", file_name);
    int r = fclose(fp);
    
    if (r == EOF)
        fprintf(stderr, "Cannot close file handler\n");
    
    sqlite3_finalize(p_stmt);
}
