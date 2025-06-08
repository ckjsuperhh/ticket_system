//
// Created by ckjsuperhh6602 on 25-6-1.
//
#include "ticket.h"
#include <string>
#include "ticket.h"
#include "cmd.h"
#include "DataBase.h"
#include "train.h"

std::string ticket::buy_ticket(char username[21], char id[21], char time[6], char start_station[31], char end_station[31], const int &n, const bool t = false) {
    if (login_state.find(username) == login_state.end()) {
        return "-1";
    }
    if (cmd::check_username(username)) {
        return "-1";
    }
    if (cmd::check_id(id)) {
        return "-1";
    }
    if (cmd::check_saledate(time)) {
        return "-1";
    }
    auto a = train_file.search(id);
    if (a.empty()) {
        return "-1";
    }
    auto b = train_storage.read(a[0]);
    if (n > b.seatnum) {
        return "-1";
    }
    if (b.release == 0) {
        return "-1";
    }
    int sum_time(0), I(0);
    for (I = 0; I < b.num - 1 && strcmp(b.station[I], start_station) != 0; I++) {
        sum_time += b.traveltimes[I] + b.stopovertimes[I];
    }
    if (strcmp(b.station[I], start_station) != 0) {
        return "-1";
    }
    int month1 = b.saledate[0][1] - '0', date1 = (b.saledate[0][3] - '0') * 10 + b.saledate[0][4] - '0';
    int hour1 = (b.starttimes[0] - '0') * 10 + b.starttimes[1] - '0', minute1 = (b.starttimes[3] - '0') * 10 + b.starttimes[4] - '0'; //一个不断浮动以计算的时间的变量
    int month = month1, date = date1; //用于指示变动了几天，便于保证末状态时间维护得正确
    train::calculate_time(month1, date1, hour1, minute1, sum_time);
    int month2 = b.saledate[1][1] - '0', date2 = (b.saledate[1][3] - '0') * 10 + b.saledate[1][4] - '0'; //末状态
    int next_(0);
    if (date != date1) {
        next_++;
        train::next_day(month, date);
        train::next_day(month2, date2);
    }
    if (date != date1) {
        next_++;
        train::next_day(month, date);
        train::next_day(month2, date2);
    }
    std::string time1 = time;
    time1.push_back(' ');
    time1 += train::to_accurate_time(hour1, minute1);
    if (train::to_date(month1, date1) <= time && train::to_date(month2, date2) >= time) {
        //如果能够成功购买票
        int day = train::date(time, b.saledate[0]) - next_, minseat(b.seatnum), price(0), sumtime(0);
        int i = I;
        for (; i < b.num - 1 && strcmp(b.station[i], end_station) != 0; i++) {
            sumtime += b.traveltimes[i] + b.stopovertimes[i];
            minseat = std::min(minseat, b.remain_seat[day][i]);
            price += b.price[i];
        }
        if (strcmp(b.station[i], end_station) != 0) {
            return "-1";
        }
        sumtime -= b.stopovertimes[i - 1];
        train::calculate_time(month1, date1, hour1, minute1, sumtime);
        int month3 = time[1] - '0', date3 = 10 * (time[3] - '0') + time[4] - '0'; //末状态的天数，会跟着原本的计算来向后推移
        if (date != date1) {
            train::next_day(month, date);
            train::next_day(month3, date3);
        }
        if (date != date1) {
            train::next_day(month, date);
            train::next_day(month3, date3);
        }
        std::string time2 = train::to_date(month3, date3);
        time2 += " ";
        time2 += train::to_accurate_time(hour1, minute1);
        if (minseat >= n) {
            for (int i1 = I; i1 < b.num - 1 && strcmp(b.station[i1], end_station) != 0; i1++) {
                b.remain_seat[day][i1] -= n;
            }
            train_storage.update(b, a[0]);
            if (const auto queues = queue_file.search(username); queues.empty()) {
                queue_file.insert(username, {
                                      -1, 1, a[0], day, char_more<char[21]>(id).get_char().data(), start_station, end_station, char_more<char[13]>(time1).get_char().data(),
                                      char_more<char[13]>(time2).get_char().data(), char_more<char[21]>(username).get_char().data(), n, price, 0
                                  });
            } else {
                queue_file.insert(username, {
                                      -1, queues.size() + 1, a[0], day, char_more<char[21]>(id).get_char().data(), start_station, end_station, char_more<char[13]>(time1).get_char().data(),
                                      char_more<char[13]>(time2).get_char().data(), char_more<char[21]>(username).get_char().data(), n, price, 0
                                  });
            }
            return std::to_string(n * price);
        }
        if (t == 0) {
            return "-1";
        }
        int order = 0;
        char p[31];
        strcpy(p, id);
        strcat(p, char_more<char[3]>(std::to_string(day)).get_char().data());
        auto Int = train_queue_file.search(p);
        if (const auto queues = queue_file.search(username); queues.empty()) {
            order = 1;
            queue_file.insert(username, {
                                  Int.empty() ? 1 : Int[0] + 1, 1, a[0], day, char_more<char[21]>(id).get_char().data(), start_station, end_station, char_more<char[13]>(time1).get_char().data(),
                                  char_more<char[13]>(time2).get_char().data(), char_more<char[21]>(username).get_char().data(), n, price, 1
                              });
        } else {
            order = queues.size() + 1;
            queue_file.insert(username, {
                                  Int.empty() ? 1 : Int[0] + 1, queues.size() + 1, a[0], day, char_more<char[21]>(id).get_char().data(), start_station, end_station, char_more<char[13]>(time1).get_char().data(),
                                  char_more<char[13]>(time2).get_char().data(), char_more<char[21]>(username).get_char().data(), n, price, 1
                              });
        }
        if (Int.empty()) {
            train_queue_file.insert(p, 1);
            queue_storage.insert(p, {1, username, order});
        } else {
            queue_storage.insert(p, {Int[0] + 1, username, order});
            train_queue_file.shift({p, Int[0]}, {p, Int[0] + 1});
        }
        return "queue";
    }
    return "-1";
}

std::string ticket::query_order(char username[21]) {
    if (login_state.find(username) == login_state.end()) {
        return "-1";
    }
    if (cmd::check_username(username)) {
        return "-1";
    }
    auto a = queue_file.search(username);
    if (a.empty()) {
        return "0";
    }
    std::string str;
    str += std::to_string(a.size());
    str.push_back('\n');
    for (int i = a.size() - 1; i >= 0; i--) {
        str += "[";
        if (a[i].state == 0) {
            str += "success] ";
        } else if (a[i].state == 1) {
            str += "pending] ";
        } else {
            str += "refunded] ";
        }
        str += a[i].id;
        str += " ";
        str += a[i].from;
        str += " ";
        str += a[i].time1;
        str += " -> ";
        str += a[i].to;
        str += " ";
        str += a[i].time2;
        str += " ";
        str += std::to_string(a[i].price);
        str += " ";
        str += std::to_string(a[i].buy_num);
        if (i != 0) {
            str += "\n";
        }
    }
    return str;
}

int ticket::refund_ticket(char username[21], const int &n) {
    if (login_state.find(username) == login_state.end()) {
        return -1;
    }
    if (cmd::check_username(username)) {
        return -1;
    }
    auto a = queue_file.search(username);
    if (a.size() < n) {
        return -1;
    }
    char ss[21];
    strcpy(ss, username);
    auto rr = a[a.size() - n];
    if (a[a.size() - n].state == 0) {
        a[a.size() - n].state = 2;
        auto p = train_storage.read(a[a.size() - n].address);
        int I = 0;
        for (I = 0; I < p.num - 1 && strcmp(p.station[I], a[a.size() - n].from) != 0; I++) {
        }
        for (int i = I; i < p.num - 1 && strcmp(p.station[i], a[a.size() - n].to) != 0; i++) {
            p.remain_seat[a[a.size() - n].day][i] += a[a.size() - n].buy_num;
        }
        train_storage.update(p, a[a.size() - n].address);
        char r[31];
        strcpy(r, a[a.size() - n].id);
        strcat(r, char_more<char[3]>(std::to_string(a[a.size() - n].day)).get_char().data());
        queue_file.shift({ss, rr}, {ss, a[a.size() - n]});
        process_queue(r);
        return 0;
    }
    if (a[a.size() - n].state == 1) {
        a[a.size() - n].state = 2;
        char r[31];
        strcpy(r, a[a.size() - n].id);
        strcat(r, char_more<char[3]>(std::to_string(a[a.size() - n].day)).get_char().data());
        queue_storage.Val_delete(r, {a[a.size() - n].queue_order, username, a.size() - n + 1});
        queue_file.shift({ss, rr}, {ss, a[a.size() - n]});
        return 0;
    }
    return -1;
}

void ticket::process_queue(char r[31]) {
    for (auto p = queue_storage.search(r); auto &j: p) {
        auto u = queue_file.search(j.username);
        auto uu = u[j.pos - 1];
        if (u[j.pos - 1].state == 1) {
            int minseat(2000000);
            auto b = train_storage.read(u[j.pos - 1].address);
            int I(0);
            for (I = 0; I < b.num - 1 && strcmp(b.station[I], u[j.pos - 1].from) != 0; I++) {
            }
            for (int i = I; i < b.num - 1 && strcmp(b.station[i], u[j.pos - 1].to) != 0; i++) {
                minseat = std::min(minseat, b.remain_seat[u[j.pos - 1].day][i]);
            }
            if (minseat == 0 || minseat < u[j.pos - 1].buy_num) {
                continue;
            }
            u[j.pos - 1].state = 0;
            for (int i = I; i < b.num - 1 && strcmp(b.station[i], u[j.pos - 1].to) != 0; i++) {
                b.remain_seat[u[j.pos - 1].day][i] -= u[j.pos - 1].buy_num;
            }
            train_storage.update(b, u[j.pos - 1].address);
            queue_storage.Val_delete(r, j);
            queue_file.shift({u[j.pos - 1].username, uu}, {u[j.pos - 1].username, u[j.pos - 1]});
        }
    }
}
