//
// Created by ckjsuperhh6602 on 25-5-24.
//

#ifndef USER_H
#define USER_H
#include <cstring>
#include "cmd.h"
#include "DataBase.h"
#include "train.h"

class user {
public:
    static int add_user(char cur[21],char username[21], char password[31],char name[11],char mail[31], int privilege) ;
    static int log_in(char username[21], char password[31]);
    static int log_out(char username[21]);
    static std::string query_profile(char cur[21],char username[21]);
};
#endif //USER_H
