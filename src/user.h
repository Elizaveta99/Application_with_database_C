#ifndef USER_H
#define USER_H

#define login_size 30
#define password_size 30
#define role_size 10

struct User
{
    char login[login_size];
    int role;
};

#endif 
