//
// Created by ckjsuperhh6602 on 25-5-24.
//

#ifndef TICKET_H
#define TICKET_H
#include <string>

class ticket {
public:
    static std::string buy_ticket(char username[21],char id[21],char time[6],char start_station[31],char end_station[31],const int & n,bool t);
static std::string query_order(char username[21]);
    static int refund_ticket(char username[21],const int & n);
    static void process_queue(char r[63],const int &I);
};
#endif //TICKET_H
