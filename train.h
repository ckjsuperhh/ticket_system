//
// Created by ckjsuperhh6602 on 25-5-24.
//

#ifndef TRAIN_H
#define TRAIN_H
#include <string>

#include "DataBase.h"

class train {
    class ticket;

private:
    static void cross_time(int &hour, int &minute, int cross);

    static int delta_time(int hour1, int hour2, int minute1, int minute2, int next_day);

    static std::string over_date(const char ac_date[6], int next_day);

public:
    static std::string to_accurate_time(const int &hour, const int &minute);

    static int date(const int month, const int date, const char d[6]);

    static int date(char d1[6], char d2[6]);

    static void next_day(int &month, int &date);

    static void calculate_time(sjtu::vector<std::string> &a, int &month, int &date, int &hour, int &minute, int traveltimes, int stopovertimes);

    static void calculate_time(int &month, int &date, int &hour, int &minute, int cross);

    static int add_train(char id[21], int num, int seatnum, const std::string &stations, const std::string &prices, char starttime[6], const std::string &traveltime, const std::string &stopovertimes, const std::string &saleDate, char type);

    static int delete_train(char id[21]);

    static int release_train(char id[21]);

    static std::string to_date(const int &month, const int &date);

    static std::string query_train(char id[21], char data[6]);

    static std::string query_ticket(char threshold[31], char destination[31], char time[6], const string &p);

    static std::string query_transfer(char threshold[31], char destination[31], char time[6], const string &p);
};
#endif //TRAIN_H
