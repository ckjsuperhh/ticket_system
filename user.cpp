//
// Created by ckjsuperhh6602 on 25-5-26.
//
#include "user.h"

#include <cmath>

#include "cmd.h"
#include "DataBase.h"
int user::add_user(char cur[21],char username[21], char password[31],char name[11],char mail[31], const int privilege) {
    if (cmd::check_username(username)) {
        return -1;
    }
    if (cmd::check_mail(mail)) {
        return -1;
    }
    if (cmd::check_password(password)) {
        return -1;
    }
    if (cmd::check_privilege(privilege)) {
        return -1;
    }
    if (user_file.empty()) {
        user_file.insert(username,user_storage.size());
        user_storage.write({password,name,mail,10});
        return 0;
    }
    if (cmd::check_username(cur)) {
        return -1;
    }
    if (const auto a=user_file.search(username);!a.empty()) {
        return -1;
    }
    const auto a=user_file.search(cur);
    if (login_state.find(cur)==login_state.end()) {//未登錄
        return -1;
    }
    if (const auto p=user_storage.read(a[0]);p.privilege<privilege) {
        return -1;
    }
    user_file.insert(username,user_storage.size());
    user_storage.write({password,name,mail,privilege});
    return 0;
}
int user::log_in(char username[21], char password[31]) {
    if (cmd::check_username(username)) {
        return -1;
    }
    if (cmd::check_password(password)) {
        return -1;
    }
    const auto a=user_file.search(username);
    if (a.empty()) {
        return -1;//登錄的用戶不存在
    }
    if (login_state.find(username)!=login_state.end()) {
        return -1;//已經登錄
    }
    if (const auto p=user_storage.read(a[0]);strcmp(p.password,password) != 0) {
        return -1;//密碼不正確
    }
    login_state.insert(username);//修改登陸狀態
    if(login_state.find(username)==login_state.end()) {
        std::cout<<"------------------------\n";
    }
    return 0;
}
int user::log_out(char username[21]) {
    if (cmd::check_username(username)) {
        return -1;
    }
    const auto a=user_file.search(username);
    if (a.empty()) {
        return -1;//登出的用戶不存在
    }
    if (login_state.find(username)==login_state.end()) {//并沒有登錄
        return -1;
    }
    login_state.erase(username);//修改登陸狀態
    return 0;
}
std::string user::query_profile(char cur[21], char username[21]) {
    std::string user,name,mail,privilege;
    if (cmd::check_username(cur)) {
        return "-1";
    }
    if (cmd::check_username(username)) {
        return "-1";
    }
    const auto a=user_file.search(cur);
    if (a.empty()) {
        return "-1";//cur不存在
    }
    if (login_state.find(cur)==login_state.end()) {
        return "-1";//并沒有登錄
    }
    const auto p1=user_storage.read(a[0]);
    if (!strcmp(cur,username)) {
        user=username;
        name=p1.name;
        mail=p1.mail;
        if (p1.privilege==10) {
            privilege="10";
        }else {
            privilege=std::to_string('0'+p1.privilege);
        }
        return user+" "+name+" "+mail+" "+privilege;
    }
    const auto b=user_file.search(username);
    if (b.empty()) {
        return "-1";
    }
    if (const auto p2=user_storage.read(b[0]); p1.privilege>p2.privilege) {
        user=username;
        name=p2.name;
        mail=p2.mail;
        if (p2.privilege==10) {
            privilege="10";
        }else {
            privilege=std::to_string('0'+p2.privilege);
        }
        return user+" "+name+" "+mail+" "+privilege;
    }
    return "-1";
}

