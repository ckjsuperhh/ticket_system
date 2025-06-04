//
// Created by ckjsuperhh6602 on 25-5-30.
//
#include"train.h"

#include <cassert>

#include "cmd.h"
#include "DataBase.h"
#include "ticket.h"

void sta_back(char p[100][31], const std::string &a) {
    int cnt = 0;
    int l = 0;
    while (a[l] == ' ') {
        l++;
    }
    while (l < a.size()) {
        while (a[l] == ' ' && l < a.size()) {
            l++;
        }
        if (l == a.size() - 1) {
            break;
        }
        int r = l;
        while (a[r] != ' ' && a[r] != '|' && r != a.size()) {
            r++;
        }
        strcpy(p[cnt++], char_more<char[31]>(a.substr(l, r - l)).get_char().data());
        l = r + 1;
    }
}

void array_back(int p[100], const std::string &a) {
    int cnt = 0;
    int l = 0;
    while (a[l] == ' ') {
        l++;
    }
    while (l < a.size()) {
        while (a[l] == ' ' && l < a.size()) {
            l++;
        }
        int r = l;
        while (a[r] != ' ' && a[r] != '|' && r != a.size()) {
            r++;
        }
        p[cnt++] = std::stoi(a.substr(l, r - l));
        l = r + 1;
    }
}

int train::add_train(char id[21], int stationnum, int seatnum, const std::string &stations, const std::string &prices, char starttime[6], const std::string &traveltime, const std::string &stopovertimes, const std::string &saleDate, char type) {
    if (cmd::check_id(id)) {
        return -1;
    }
    if (cmd::check_stationnum(stationnum)) {
        return -1;
    }
    if (cmd::check_seatnum(seatnum)) {
        return -1;
    }
    if (!train_file.empty()) {
        auto a = train_file.search(id);
        if (!a.empty()) {
            //id已經存在
            return -1;
        }
    }
    train_info tmp{};
    tmp.num = stationnum;
    tmp.seatnum = seatnum;
    tmp.type = type;
    sta_back(tmp.station, stations);
    array_back(tmp.traveltimes, traveltime);
    array_back(tmp.stopovertimes, stopovertimes);
    array_back(tmp.price, prices);
    strcpy(tmp.starttimes, starttime);
    strcpy(tmp.saledate[0], char_more<char[6]>(saleDate.substr(0, 5)).get_char().data());
    strcpy(tmp.saledate[1], char_more<char[6]>(saleDate.substr(6, 6)).get_char().data());
    for (auto &i: tmp.remain_seat) {
        for (int j = 0; j < stationnum - 1; j++) {
            i[j] = seatnum;
        }
    }
    train_file.insert(id, train_storage.size());
    train_storage.write(tmp);
    return 0;
}

int train::delete_train(char id[21]) {
    if (cmd::check_id(id)) {
        return -1;
    }
    auto a = train_file.search(id);
    if (a.empty()) {
        return -1; //刪除的車子不存在
    }
    auto p = train_storage.read(a[0]);
    if (p.release == 1) {
        return -1; //成功release了
    }
    train_file.Val_delete(id);
    return 0;
}

int train::release_train(char id[21]) {
    if (cmd::check_id(id)) {
        return -1;
    }
    auto a = train_file.search(id);
    if (a.empty()) {
        return -1; //释放的車子不存在
    }
    auto p = train_storage.read(a[0]);
    if (p.release == 1) {
        return -1; //成功release了
    }
    p.release = true;
    train_storage.update(p, a[0]);
    int prices[p.num][p.num];
    for (int I = 0; I < p.num; I++) {
        prices[I][I] = 0;
    }
    int setup_hour[p.num] = {};
    int setup_minute[p.num] = {};
    int arrival_hour[p.num] = {};
    int arrival_minute[p.num] = {};
    int arrival_nextday[p.num] = {};
    int setup_nextday[p.num] = {};
    int start_arrival_month[p.num] = {};
    int start_arrival_date[p.num] = {};
    int end_arrival_month[p.num] = {};
    int end_arrival_date[p.num] = {};
    int start_setup_month[p.num] = {};
    int start_setup_date[p.num] = {};
    int end_setup_month[p.num] = {};
    int end_setup_date[p.num] = {};
    setup_nextday[0] = 0;
    setup_hour[0] = 10 * (p.starttimes[0] - '0') + p.starttimes[1] - '0';
    setup_minute[0] = 10 * (p.starttimes[3] - '0') + p.starttimes[4] - '0';
    start_setup_month[0] = 10 * (p.saledate[0][0] - '0') + p.saledate[0][1] - '0';
    start_setup_date[0] = 10 * (p.saledate[0][3] - '0') + p.saledate[0][4] - '0';
    end_setup_month[0] = 10 * (p.saledate[1][0] - '0') + p.saledate[1][1] - '0';
    end_setup_date[0] = 10 * (p.saledate[1][3] - '0') + p.saledate[1][4] - '0';
    for (int I = 0; I < p.num - 1; I++) {
        if (I > 0) {
            setup_nextday[I] = arrival_nextday[I - 1];
            start_setup_month[I] = start_arrival_month[I - 1];
            start_setup_date[I] = start_arrival_date[I - 1];
            end_setup_month[I] = end_arrival_month[I - 1];
            end_setup_date[I] = end_arrival_date[I - 1];
            setup_hour[I] = arrival_hour[I - 1];
            setup_minute[I] = arrival_minute[I - 1];
            calculate_time(start_setup_month[I], start_setup_date[I], setup_hour[I], setup_minute[I], p.stopovertimes[I - 1]);
            if (start_setup_date[I] != start_arrival_date[I - 1]) {
                train::next_day(end_setup_month[I], end_setup_date[I]);
                setup_nextday[I]++;
            }
            arrival_nextday[I] = setup_nextday[I];
            start_arrival_month[I] = start_setup_month[I];
            start_arrival_date[I] = start_setup_date[I];
            arrival_hour[I] = setup_hour[I];
            arrival_minute[I] = setup_minute[I];
            end_arrival_month[I] = end_setup_month[I];
            end_arrival_date[I] = end_setup_date[I];
            calculate_time(start_arrival_month[I], start_arrival_date[I], arrival_hour[I], arrival_minute[I], p.traveltimes[I]);
            if (start_arrival_date[I] != start_setup_date[I]) {
                train::next_day(end_arrival_month[I], end_arrival_date[I]);
                arrival_nextday[I]++;
            }
        } else if (I == 0) {
            arrival_nextday[I] = setup_nextday[I];
            start_arrival_month[I] = start_setup_month[I];
            start_arrival_date[I] = start_setup_date[I];
            arrival_hour[I] = setup_hour[I];
            arrival_minute[I] = setup_minute[I];
            end_arrival_month[I] = end_setup_month[I];
            end_arrival_date[I] = end_setup_date[I];
            calculate_time(start_arrival_month[I], start_arrival_date[I], arrival_hour[I], arrival_minute[I], p.traveltimes[I]);
            if (start_arrival_date[I] != start_setup_date[I]) {
                train::next_day(end_arrival_month[I], end_arrival_date[I]);
                arrival_nextday[I]++;
            }
        }
        for (int J = I + 1; J < p.num; J++) {
            from_to_file.insert(p.station[I], char_31(p.station[J]));
            prices[I][J] = prices[I][J - 1] + p.price[J - 1];
        }
    }
    setup_hour[p.num - 1] = arrival_hour[p.num - 2];
    setup_minute[p.num - 1] = arrival_minute[p.num - 2];
    start_setup_month[p.num - 1] = start_arrival_month[p.num - 2];
    start_setup_date[p.num - 1] = start_arrival_date[p.num - 2];
    end_setup_month[p.num - 1] = end_setup_month[p.num - 2];
    end_setup_date[p.num - 1] = end_arrival_date[p.num - 2];

    for (int I = 0; I < p.num - 1; I++) {
        for (int J = I + 1; J < p.num; J++) {
            std::string str = p.station[I];
            str += p.station[J];
            transfer_file.insert(char_more<char[63]>(str).get_char().data(), transfer_storage.size());
            transfer_storage.write({
                p.station[I], p.station[J],
                I, J, id, a[0], prices[I][J], char_more<char[6]>(to_date(start_setup_month[I], start_setup_date[I])).get_char().data(),
                char_more<char[6]>(to_date(end_setup_month[I], end_setup_date[I])).get_char().data(), setup_hour[I],
                arrival_hour[J - 1], setup_minute[I], arrival_minute[J - 1],
                arrival_nextday[J - 1], setup_nextday[I]
            });
            auto dcgwi = transfer_file.search(char_more<char[63]>(str).get_char().data());
        }
    }
    return 0;
}

std::string train::to_date(const int &month, const int &date) {
    std::string timer;
    if (month <= 9) {
        timer.push_back('0');
        timer.push_back('0' + month);
    } else {
        timer += std::to_string(month);
    }
    timer.push_back('-');
    if (date <= 9) {
        timer.push_back('0');
        timer.push_back('0' + date);
    } else {
        timer += std::to_string(date);
    }
    return timer;
}

std::string train::query_train(char id[21], char data[6]) {
    if (cmd::check_id(id)) {
        return "-1";
    }
    if (cmd::check_saledate(data)) {
        return "-1";
    }
    auto a = train_file.search(id);
    if (a.empty()) {
        return "-1";
    }
    if (const auto p = train_storage.read(a[0]); strcmp(p.saledate[0], data) <= 0 && strcmp(p.saledate[1], data) >= 0) {
        std::string str = id;
        str.push_back(' ');
        str.push_back(p.type);
        str.push_back('\n');
        int sum = 0;
        int month, date, hour, minute;
        sjtu::vector<std::string> ve;
        ve.resize(2);
        int date_num = 0;
        for (int I = 0; I < p.num; I++) {
            if (I != 0) {
                sum += p.price[I - 1];
            }
            str += p.station[I];
            str.push_back(' ');
            if (I == 0) {
                month = data[1] - '0';
                date = 10 * (data[3] - '0') + data[4] - '0';
                date_num = train::date(month, date, p.saledate[0]);
                hour = (p.starttimes[0] - '0') * 10 + p.starttimes[1] - '0';
                minute = (p.starttimes[3] - '0') * 10 + p.starttimes[4] - '0';
                str += "xx-xx xx:xx -> ";
                str += data;
                str.push_back(' ');
                str += p.starttimes;
                str += " 0 ";
                str += std::to_string(p.remain_seat[date_num][I]);
                str.push_back('\n');
            } else if (I == p.num - 1) {
                calculate_time(ve, month, date, hour, minute, p.traveltimes[I - 1], p.stopovertimes[I - 1]);
                str += ve[0];
                str += " -> xx-xx xx:xx ";
                str += std::to_string(sum);
                str.push_back(' ');
                str += "x";
            } else {
                calculate_time(ve, month, date, hour, minute, p.traveltimes[I - 1], p.stopovertimes[I - 1]);
                str += ve[0];
                str += " -> ";
                str += ve[1];
                str.push_back(' ');
                str += std::to_string(sum);
                str.push_back(' ');
                str += std::to_string(p.remain_seat[date_num][I]);
                str.push_back('\n');
            }
        }
        return str;
    }
    return "-1"; //日期不在可以發車的期間
}

int train::date(const int month, const int date, const char d[6]) {
    const int init_month = d[1] - '0';
    const int init_date = 10 * (d[3] - '0') + d[4] - '0';
    if (init_month == month) {
        return date - init_date;
    }
    if (init_month == 6 && month == 7) {
        return 30 - init_date + date;
    }
    if (init_month == 6 && month == 8) {
        return 61 - init_date + date;
    }
    if (init_month == 7 && month == 8) {
        return 31 - init_date + date;
    }
    return 0; //無效語句
}

int train::date(char d1[6], char d2[6]) {
    return date(d1[1] - '0', 10 * (d1[3] - '0') + d1[4] - '0', d2);
}

void train::calculate_time(sjtu::vector<std::string> &a, int &month, int &date, int &hour, int &minute, int traveltimes, int stopovertimes) {
    if (const int next_day_time = 60 * (23 - hour) + (60 - minute); next_day_time > traveltimes) {
        cross_time(hour, minute, traveltimes);
    } else {
        train::next_day(month, date);
        minute = hour = 0;
        cross_time(hour, minute, traveltimes - next_day_time);
    }
    std::string timer;
    timer.push_back('0');
    timer.push_back('0' + month);
    timer.push_back('-');
    if (date <= 9) {
        timer.push_back('0');
        timer.push_back('0' + date);
    } else {
        timer += std::to_string(date);
    }
    timer.push_back(' ');
    if (hour <= 9) {
        timer.push_back('0');
        timer.push_back('0' + hour);
    } else {
        timer += std::to_string(hour);
    }
    timer.push_back(':');
    if (minute <= 9) {
        timer.push_back('0');
        timer.push_back('0' + minute);
    } else {
        timer += std::to_string(minute);
    }
    a[0] = timer;
    if (const int next_day_time = 60 * (23 - hour) + (60 - minute); next_day_time > stopovertimes) {
        cross_time(hour, minute, stopovertimes);
    } else {
        train::next_day(month, date);
        minute = hour = 0;
        cross_time(hour, minute, stopovertimes - next_day_time);
    }
    timer.clear();
    timer.push_back('0');
    timer.push_back('0' + month);
    timer.push_back('-');
    if (date <= 9) {
        timer.push_back('0');
        timer.push_back('0' + date);
    } else {
        timer += std::to_string(date);
    }
    timer.push_back(' ');
    if (hour <= 9) {
        timer.push_back('0');
        timer.push_back('0' + hour);
    } else {
        timer += std::to_string(hour);
    }
    timer.push_back(':');
    if (minute <= 9) {
        timer.push_back('0');
        timer.push_back('0' + minute);
    } else {
        timer += std::to_string(minute);
    }
    a[1] = timer;
}

void train::calculate_time(int &month, int &date, int &hour, int &minute, const int cross) {
    if (const int next_day_time = 60 * (23 - hour) + (60 - minute); next_day_time > cross) {
        cross_time(hour, minute, cross);
    } else {
        train::next_day(month, date);
        minute = hour = 0;
        cross_time(hour, minute, cross - next_day_time);
    }
}

void train::next_day(int &month, int &date) {
    if (month == 6) {
        if (date == 30) {
            month = 7, date = 1;
        } else {
            date++;
        }
    } else if (month == 7) {
        if (date == 31) {
            month = 8, date = 1;
        } else {
            date++;
        }
    } else if (month == 8) {
        if (date == 31) {
            month = 9, date = 1;
        } else {
            date++;
        }
    } else if (month == 9) {
        if (date == 30) {
            month = 10, date = 1;
        } else {
            date++;
        }
    }
}

void train::cross_time(int &hour, int &minute, int cross) {
    if (cross < 60 - minute) {
        minute += cross;
    } else {
        cross -= 60 - minute;
        minute = cross % 60;
        hour += cross / 60 + 1;
    }
}

int train::delta_time(int hour1, int hour2, int minute1, int minute2, int next_day) {
    return next_day * 1440 + (hour2 - hour1) * 60 + minute2 - minute1;
}

struct line {
    int time{};
    int cost{};
    char id[21]{};
    char from[31]{}, to[31]{};
    char time1[13]{}, time2[13]{};
    int seat{};

    line() = default;

    line(int time, int cost, char id[21], char from[31], char to[31],
         char time1[13], char time2[13], int seat): time(time), cost(cost), seat(seat) {
        strcpy(this->id, id), strcpy(this->from, from), strcpy(this->to, to), strcpy(this->time1, time1);
        strcpy(this->time2, time2);
    }
};

struct Less_time {
    bool operator()(const line &a, const line &b) const {
        if (a.time < b.time) {
            return true;
        }
        if (a.time == b.time) {
            return strcmp(a.id, b.id) < 0;
        }
        return false;
    }
};

struct Less_cost {
    bool operator()(const line &a, const line &b) const {
        if (a.cost < b.cost) {
            return true;
        }
        if (a.cost == b.cost) {
            return strcmp(a.id, b.id) < 0;
        }
        return false;
    }
};

std::string train::query_ticket(char threshold[31], char destination[31], char time[6], const string &p) {
    line l[10000];
    l[0] = line{};
    char a[63];
    strcpy(a, threshold);
    strcat(a, destination);
    auto b = transfer_file.search(a);
    if (b.empty()) {
        return "0";
    }
    int  cnt(0);
    for (auto x: b) {
        auto c = transfer_storage.read(x);
        if (strcmp(c.saledate[0], time) <= 0 && strcmp(c.saledate[1], time) >= 0) {
            int day = date(time, c.saledate[0]);
            auto d = train_storage.read(c.address);
            int min_seat = d.seatnum;
            int min_time = 10001, min_cost = 100001;
            for (int i = c.I; i < c.J; i++) {
                min_seat = std::min(min_seat, d.remain_seat[day][i]);
            }
            if (min_seat == 0) {
                continue;
            }
            string s = time;
            s += " ";
            s += to_accurate_time(c.setup_hour, c.setup_minute);
            string ss;
            if (c.arrival_nextday - c.setup_nextday == 0) {
                ss = time;
            } else {
                ss = over_date(time, c.arrival_nextday - c.setup_nextday);
            }
            ss += " ";
            ss += to_accurate_time(c.arrival_hour, c.arrival_minute);
            l[cnt++] = line{
                delta_time(c.setup_hour, c.arrival_hour, c.setup_minute,
                           c.arrival_minute, c.arrival_nextday - c.setup_nextday),
                c.price, c.id, threshold, destination, char_more<char[13]>(s).get_char().data(),
                char_more<char[13]>(ss).get_char().data(), min_seat
            };
        }
    }
    if (cnt == 0) {
        return "0";
    }
    if (cnt != 1) {
        if (p == "time") {
            sjtu::quick_sort(l, l + cnt , Less_time());
        } else {
            sjtu::quick_sort(l, l + cnt , Less_cost());
        }
    }
    std::string str = std::to_string(cnt);
    str.push_back('\n');
    for (int i = 0; i < cnt; i++) {
        str += l[i].id;
        str.push_back(' ');
        str += threshold;
        str.push_back(' ');
        str += l[i].time1;
        str += " -> ";
        str += destination;
        str.push_back(' ');
        str += l[i].time2;
        str.push_back(' ');
        str += std::to_string(l[i].cost);
        str.push_back(' ');
        str += std::to_string(l[i].seat);
        if (i != cnt - 1) {
            str += "\n";
        }
    }
    return str;
}

std::string train::over_date(const char ac_date[6], int next_day) {
    int month = ac_date[1] - '0';
    int date = 10 * (ac_date[3] - '0') + ac_date[4] - '0';
    while (next_day--) {
        train::next_day(month, date);
    }
    return to_date(month, date);
}

std::string train::to_accurate_time(const int &hour, const int &minute) {
    std::string timer;
    if (hour <= 9) {
        timer.push_back('0');
        timer.push_back('0' + hour);
    } else {
        timer += std::to_string(hour);
    }
    timer.push_back(':');
    if (minute <= 9) {
        timer.push_back('0');
        timer.push_back('0' + minute);
    } else {
        timer += std::to_string(minute);
    }
    return timer;
}

std::string train::query_transfer(char threshold[31], char destination[31], char time[6], const string &p) {
    auto to = from_to_file.search(threshold);
    std::string str;
    int cnt(0);
    for (auto x: to) {
        if (x == destination) {
            continue;
        }
        char a1[63], a2[63];
        strcpy(a1, threshold);
        strcat(a1, x.a);
        strcpy(a2, x.a);
        strcat(a2, destination);
        auto b1 = transfer_file.search(a1);
        if (b1.empty()) {
            continue;
        }
        auto b2 = transfer_file.search(a2);
        if (b2.empty()) {
            continue;
        }
        int pos1(0), pos2(0), minseat1(0), minseat2(0), timer(20000), pricer(200000);
        char train_id1[21], train_id2[21];
        for (auto y1: b1) {
            if (auto c1 = transfer_storage.read(y1); strcmp(c1.saledate[0], time) <= 0 && strcmp(c1.saledate[1], time) >= 0) {
                int day1 = date(time, c1.saledate[0]);
                auto d1 = train_storage.read(c1.address);
                int min_seat1 = d1.seatnum;
                int min_time = 2000, min_cost = 200000;
                for (int i = c1.I; i <= c1.J; i++) {
                    min_seat1 = std::min(min_seat1, d1.remain_seat[day1][i]);
                }
                if (min_seat1 == 0) {
                    continue;
                }
                int delta1 = delta_time(c1.setup_hour, c1.arrival_hour, c1.setup_minute
                                        , c1.arrival_minute, c1.arrival_nextday - c1.setup_nextday);
                for (auto y2: b2) {
                    auto c2 = transfer_storage.read(y2);
                    if (strcmp(c2.saledate[0], time) <= 0 && strcmp(c2.saledate[1], time) >= 0) {
                        int day2 = date(time, c2.saledate[0]);
                        auto d2 = train_storage.read(c2.address);
                        int min_seat2 = d2.seatnum;
                        for (int i = c2.I; i <= c2.J; i++) {
                            min_seat2 = std::min(min_seat2, d2.remain_seat[day2][i]);
                        }
                        if (min_seat2 == 0) {
                            continue;
                        }
                        cnt++;
                        if (p == "time") {
                            int delta2 = delta_time(c2.setup_hour, c2.arrival_hour, c2.setup_minute
                                                    , c2.arrival_minute, c2.arrival_nextday - c2.setup_nextday);
                            if (delta1 + delta2 < timer) {
                                pos1 = y1;
                                pos2 = y2;
                                minseat1 = min_seat1;
                                minseat2 = min_seat2;
                                strcpy(train_id1, c1.id);
                                strcpy(train_id2, c2.id);
                                pricer = c1.price + c2.price;
                                timer = delta1 + delta2;
                            } else if (delta1 + delta2 == timer) {
                                if (c1.price + c2.price < pricer) {
                                    pos1 = y1;
                                    pos2 = y2;
                                    minseat1 = min_seat1;
                                    minseat2 = min_seat2;
                                    strcpy(train_id1, c1.id);
                                    strcpy(train_id2, c2.id);
                                    pricer = c1.price + c2.price;
                                    timer = delta1 + delta2;
                                } else if (c1.price + c2.price < pricer) {
                                    if (strcmp(c1.id, train_id1) < 0) {
                                        pos1 = y1;
                                        pos2 = y2;
                                        minseat1 = min_seat1;
                                        minseat2 = min_seat2;
                                        strcpy(train_id1, c1.id);
                                        strcpy(train_id2, c2.id);
                                        pricer = c1.price + c2.price;
                                        timer = delta1 + delta2;
                                    } else if (strcmp(c1.id, train_id1) == 0) {
                                        if (strcmp(c2.id, train_id2) < 0) {
                                            pos1 = y1;
                                            pos2 = y2;
                                            minseat1 = min_seat1;
                                            minseat2 = min_seat2;
                                            strcpy(train_id1, c1.id);
                                            strcpy(train_id2, c2.id);
                                            pricer = c1.price + c2.price;
                                            timer = delta1 + delta2;
                                        }
                                    }
                                }
                            }
                        } else if (p == "cost") {
                            if (c1.price + c2.price < pricer) {
                                int delta2 = delta_time(c2.setup_hour,c2.arrival_hour , c2.setup_minute
                                                        , c2.arrival_minute, c2.arrival_nextday - c2.setup_nextday);

                                pos1 = y1;
                                pos2 = y2;
                                minseat1 = min_seat1;
                                minseat2 = min_seat2;
                                strcpy(train_id1, c1.id);
                                strcpy(train_id2, c2.id);
                                pricer = c1.price + c2.price;
                                timer = delta1 + delta2;
                            } else if (c1.price + c2.price == pricer) {
                                int delta2 = delta_time(c2.setup_hour,c2.arrival_hour , c2.setup_minute
                                                        , c2.arrival_minute, c2.arrival_nextday - c2.setup_nextday);
                                if (delta1 + delta2 < timer) {
                                    pos1 = y1;
                                    pos2 = y2;
                                    minseat1 = min_seat1;
                                    minseat2 = min_seat2;
                                    strcpy(train_id1, c1.id);
                                    strcpy(train_id2, c2.id);
                                    pricer = c1.price + c2.price;
                                    timer = delta1 + delta2;
                                } else if (delta1 + delta2 == timer) {
                                    if (strcmp(c1.id, train_id1) < 0) {
                                        pos1 = y1;
                                        pos2 = y2;
                                        minseat1 = min_seat1;
                                        minseat2 = min_seat2;
                                        strcpy(train_id1, c1.id);
                                        strcpy(train_id2, c2.id);
                                        pricer = c1.price + c2.price;
                                        timer = delta1 + delta2;
                                    } else if (strcmp(c1.id, train_id1) == 0) {
                                        if (strcmp(c2.id, train_id2) < 0) {
                                            pos1 = y1;
                                            pos2 = y2;
                                            minseat1 = min_seat1;
                                            minseat2 = min_seat2;
                                            strcpy(train_id1, c1.id);
                                            strcpy(train_id2, c2.id);
                                            pricer = c1.price + c2.price;
                                            timer = delta1 + delta2;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        if (cnt == 0) {
            return "0";
        }
        auto c1 = transfer_storage.read(pos1);
        auto c2 = transfer_storage.read(pos2);

        str = std::to_string(cnt);
        str.push_back('\n');
        str += train_id1;
        str.push_back(' ');
        str += threshold;
        str.push_back(' ');
        str += time;
        str.push_back(' ');
        str += to_accurate_time(c1.setup_hour, c1.setup_minute);
        str += " -> ";
        str += c1.to;
        str.push_back(' ');
        if (c1.arrival_nextday - c1.setup_nextday == 0) {
            str += time;
        } else {
            str += over_date(time, c1.arrival_nextday - c2.setup_nextday);
        }
        str.push_back(' ');
        str += to_accurate_time(c1.arrival_hour, c1.arrival_minute);
        str.push_back(' ');
        str += std::to_string(c1.price);
        str.push_back(' ');
        str += std::to_string(minseat1);
        str.push_back('\n');
        str += train_id2;
        str.push_back(' ');
        str += c2.from;
        str.push_back(' ');
        str += time;
        str.push_back(' ');
        str += to_accurate_time(c2.setup_hour, c2.setup_minute);
        str += " -> ";
        str += destination;
        str.push_back(' ');
        if (c2.arrival_nextday - c2.setup_nextday == 0) {
            str += time;
        } else {
            str += over_date(time, c2.arrival_nextday - c2.setup_nextday);
        }
        str.push_back(' ');
        str += to_accurate_time(c2.arrival_hour, c2.arrival_minute);
        str.push_back(' ');
        str += std::to_string(c2.price);
        str.push_back(' ');
        str += std::to_string(minseat2);
    }
    if (cnt == 0) {
        return "0";
    }
    return str;
}
