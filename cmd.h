//
// Created by ckjsuperhh6602 on 25-5-26.
//

#ifndef CMD_H
#define CMD_H
#include "DataBase.h"
#include "user.h"
#include "ticket.h"
#include "train.h"
class user;
class cmd {
public:
    static void initialise();
    static bool check_username(const char x[61]);
    static bool check_password(const char x[31]) ;
    static bool check_mail(const char x[31]) ;
    static bool check_privilege(int x);
    static void analyse(const string & a);

};

#endif //CMD_H
