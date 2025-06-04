//
// Created by ckjsuperhh6602 on 25-5-26.
//
#include<string>
#include "DataBase.h"
#include "user.h"
#include "ticket.h"
#include "train.h"
void cmd::initialise() {
        user_file.initialise("user_file(1)","user_file(2)");//存儲用戶名到存儲地址的映射
        user_storage.initialise("user_storage");
    train_file.initialise("train_file(1)","train_file(2)");
    train_storage.initialise("train_storage");
    from_to_file.initialise("s_from_to_file(1)","s_from_to_file(2)");
    transfer_file.initialise("transfer_file(1)","transfer_file(2)");
    transfer_storage.initialise("transfer_storage");
    queue_file.initialise("queue_file(1)","queue_file(2)");
    queue_storage.initialise("queue_storage(1)","queue_storage(2)");
    }

bool cmd::check_username(const char x[61]) {
        if (!((x[0]>='a'&&x[0]<='z')||(x[0]>='A'&&x[0]<='Z'))) {
            return true;//檢驗成功，不對勁
        }
        for (int a=1;x[a]!='\0';a++) {
            if (!(x[a]=='_'||(x[a]>='0'&&x[a]<='9')||(x[a]>='a'&&x[a]<='z')||(x[a]>='A'&&x[a]<='Z'))){
                return true;
            }
        }
        return false;
    }
bool cmd::check_password(const char x[31]) {
        for (int a=0;x[a]!='\0';a++) {
            if (!(x[a]>=32&&x[a]<=126)){
                return true;
            }
        }
        return false;
    }
bool cmd::check_mail(const char x[31]) {
        for (int a=0;x[a]!='\0';a++) {
            if (!(x[a]=='@'||x[a]=='.'||(x[a]>='0'&&x[a]<='9')||(x[a]>='a'&&x[a]<='z')||(x[a]>='A'&&x[a]<='Z'))){
                return true;
            }
        }
        return false;
    }
bool cmd::check_privilege(const int x) {
    if (x<0||x>10) {
        return true;
    }
    return false;
}
bool cmd::check_id(const char x[21]) {
    if (!((x[0]>='a'&&x[0]<='z')||(x[0]>='A'&&x[0]<='Z'))) {
        return true;//檢驗成功，不對勁
    }
    for (int a=1;x[a]!='\0';a++) {
        if (!(x[a]=='_'||(x[a]>='0'&&x[a]<='9')||(x[a]>='a'&&x[a]<='z')||(x[a]>='A'&&x[a]<='Z'))){
            return true;
        }
    }
    return false;
}
bool cmd::check_stationnum(const int x) {
    return !(x>=2&&x<=100);
}
bool cmd::check_seatnum(const int x) {
    return x>100000;
}
bool cmd::check_price(const int x) {
    return x>100000;
}
bool cmd::check_traveltimes(const int x) {
    return x>10000;
}
bool cmd::check_stopovertimes(const int x) {
    return x>10000;
}
bool cmd::check_saledate(const char x[6]) {
    if (x[0]=='0'&&(x[1]>='6'&&x[1]<='8')&&x[2]=='-') {
        std::string tmp;
        tmp[0]=x[3];
        tmp[1]=x[4];
        if (x[1]=='6') {
            if (std::stoi(tmp)<31) {
                return false;
            }
            return true;
        }
        if (std::stoi(tmp)<=31) {
            return false;
        }
        return true;
    }
    return true;
}

void cmd::analyse(const string & a) {
        // std::cout<<a<<std::endl;
        std::string str;
        std::string c, u, p, n, m, g,i,s,x,t,o,d,y,f,q;
        int cnt = 0;
        sjtu::vector<string> tokens;
        int l = 0;
        while (a[l] == ' ') {
            l++;
        }
        int r = l;
        while (a[r] != ' ' && r != a.size()) {
            r++;
        }
        const string time = a.substr(l, r - l);
        l = r;
        while (l != a.size()) {
            while (a[l] == ' ' && l < a.size()) {
                l++;
            }
            if (l == a.size()) {
                break;
            }
            r = l;
            while (a[r] != ' ' && r != a.size()) {
                r++;
            }
            cnt++;
            tokens.push_back(a.substr(l, r - l));
            l = r;
        }
    if (time=="[133324]") {
puts(" ");
    }
        if (tokens[0]=="add_user") {
            for (int i=1;i<12;i+=2) {
                if (tokens[i]=="-g") {
                    g=tokens[i+1];
                }else if (tokens[i]=="-p") {
                    p=tokens[i+1];
                }else if (tokens[i]=="-u") {
                    u=tokens[i+1];
                }else if (tokens[i]=="-m") {
                    m=tokens[i+1];
                }else if (tokens[i]=="-n") {
                    n=tokens[i+1];
                }else if (tokens[i]=="-c") {
                    c=tokens[i+1];
                }
            }
            std::cout<<time<<" "<<user::add_user(char_more<char[21]>(c).get_char().data(),char_more<char[21]>(u).get_char().data(),char_more<char[31]>(p).get_char().data(),char_more<char[16]>(n).get_char().data(),char_more<char[31]>(m).get_char().data(),std::stoi(g))<<std::endl;
        }else if (tokens[0]=="login") {
            for (int i=1;i<4;i+=2) {
                if (tokens[i]=="-p") {
                    p=tokens[i+1];
                }else if (tokens[i]=="-u") {
                    u=tokens[i+1];
                }
                }
            std::cout<<time<<" "<<user::log_in(char_more<char[21]>(u).get_char().data(),char_more<char[31]>(p).get_char().data())<<std::endl;
            }else if (tokens[0]=="logout") {
                std::cout<<time<<" "<<user::log_out(char_more<char[21]>(tokens[2]).get_char().data())<<std::endl;
            }else if (tokens[0]=="query_profile") {
                for (int i=1;i<4;i+=2) {
                    if (tokens[i]=="-c") {
                        c=tokens[i+1];
                    }else if (tokens[i]=="-u") {
                        u=tokens[i+1];
                    }
                }
                std::cout<<time<<" "<<user::query_profile(char_more<char[21]>(c).get_char().data(),char_more<char[31]>(u).get_char().data())<<std::endl;
            }else if (tokens[0]=="modify_profile") {
                int ct=0;
                bool flag[4]{};
                for (int i=1;i<tokens.size()-1;i+=2) {
                    if (tokens[i]=="-g") {
                        g=tokens[i+1];
                        flag[2]=true;
                    }else if (tokens[i]=="-p") {
                        p=tokens[i+1];
                        flag[3]=true;
                    }else if (tokens[i]=="-u") {
                        u=tokens[i+1];
                        ct++;
                    }else if (tokens[i]=="-m") {
                        m=tokens[i+1];
                        flag[1]=true;
                    }else if (tokens[i]=="-n") {
                        n=tokens[i+1];
                        flag[0]=true;
                    }else if (tokens[i]=="-c") {
                        c=tokens[i+1];
                        ct++;
                    }
                }
                bool ok=true;
                if (flag[1]&&check_mail(char_more<char[31]>(m).get_char().data())) {
                    ok=false;
                }
                if (flag[2]&&(std::stoi(g)<0||std::stoi(g)>10)) {
                    ok=false;
                }
                if (flag[3]&&check_password(char_more<char[31]>(p).get_char().data())) {
                    ok=false;
                }
                if (ok==0||ct!=2) {
                    std::cout<<time<<" "<<-1<<std::endl;
                }else {
                    char cur[21];
                    strcpy(cur,char_more<char[21]>(c).get_char().data());
                    const auto r=user_file.search(cur);
                    if (r.empty()) {
                        std::cout<<time<<" "<<-1<<std::endl;//cur不存在
                    }else if (login_state.find(cur)==login_state.end()) {
                        std::cout<<time<<" "<<-1<<std::endl;//并沒有登錄
                    }else if (c==u) {
                        if (const auto up=user_storage.read(r[0]); flag[2]==true&&std::stoi(g)>=up.privilege) {
                            std::cout<<time<<" "<<-1<<std::endl;//如果權限錯誤，放棄修改
                        }else {
                            std::string out;
                            out=u;
                            out+=" ";
                            if (flag[0]) {
                                out+=n;
                            }else {
                                out+=up.name;
                            }
                            out+=" ";
                            if (flag[1]) {
                                out+=m;
                            }else {
                                out+=up.mail;
                            }
                            out+=" ";
                            if (flag[2]) {
                                out+=p;
                            }else{
                                out+=std::to_string(up.privilege);
                            }
                            char pa[31],na[16],ma[31];
                            strcpy(pa,flag[3]==1?char_more<char[31]>(p).get_char().data():up.password);
                            strcpy(na,flag[0]==1?char_more<char[16]>(n).get_char().data():up.name);
                            strcpy(ma,flag[1]==1?char_more<char[31]>(m).get_char().data():up.mail);
                            int pr=flag[2]==1?std::stoi(g):up.privilege;
                            user_storage.update({pa,na,ma,pr},r[0]);
                            std::cout<<time<<" "<<out<<std::endl;
                        }
                    }else{
                        char username[21];
                        strcpy(username,char_more<char[21]>(u).get_char().data());
                        const auto b=user_file.search(username);
                        if (b.empty()) {
                            std::cout<<time<<" "<<-1<<std::endl;//u不存在
                        }else {
                            const auto p1=user_storage.read(r[0]),p2=user_storage.read(b[0]);
                            if (p1.privilege<=p2.privilege) {
                                std::cout<<time<<" "<<-1<<std::endl;//cur權限過低
                            }else if (flag[2]&&std::stoi(g)>p2.privilege) {
                                std::cout<<time<<" "<<-1<<std::endl;//需要修改權限并且把權限變高
                            }else {
                                std::string out;
                                out=u;
                                out+=" ";
                                if (flag[0]) {
                                    out+=n;
                                }else {
                                    out+=p2.name;
                                }
                                out+=" ";
                                if (flag[1]) {
                                    out+=m;
                                }else {
                                    out+=p2.mail;
                                }
                                out+=" ";
                                if (flag[2]) {
                                    out+=g;
                                }else{
                                    out+=std::to_string(p2.privilege);
                                }
                                char pa[31],na[16],ma[31];
                                strcpy(pa,flag[3]==1?char_more<char[31]>(p).get_char().data():p2.password);
                                strcpy(na,flag[0]==1?char_more<char[16]>(n).get_char().data():p2.name);
                                strcpy(ma,flag[1]==1?char_more<char[31]>(m).get_char().data():p2.mail);
                                int pr=flag[2]==1?std::stoi(g):p2.privilege;
                                user_storage.update({pa,na,ma,pr},b[0]);
                                std::cout<<time<<" "<<out<<std::endl;
                            }
                        }
                    }

                }
            }else if (tokens[0]=="exit") {
                login_state.clear();
                std::cout<<time<<" bye"<<std::endl;
                exit(0);
            }else if (tokens[0]=="add_train") {
                for (int II=1;II<tokens.size()-1;II+=2) {
                    if (tokens[II]=="-i") {
                        i=tokens[II+1];
                    }else if (tokens[II]=="-n") {
                        n=tokens[II+1];
                    }else if (tokens[II]=="-m") {
                        m=tokens[II+1];
                    }else if (tokens[II]=="-s") {
                        s=tokens[II+1];
                    }else if (tokens[II]=="-p") {
                        p=tokens[II+1];
                    }else if (tokens[II]=="-x") {
                        x=tokens[II+1];
                    }else if (tokens[II]=="-t") {
                        t=tokens[II+1];
                    }else if (tokens[II]=="-o") {
                        o=tokens[II+1];
                    }else if (tokens[II]=="-d") {
                        d=tokens[II+1];
                    }else if (tokens[II]=="-y") {
                        y=tokens[II+1];
                    }
                }
                std::cout<<time<<" "<<train::add_train(char_more<char[21]>(i).get_char().data(),std::stoi(n),std::stoi(m),s,p,char_more<char[6]>(x).get_char().data(),t,o,d,y[0])<<std::endl;
            }else if (tokens[0]=="delete_train") {
                i=tokens[2];
                std::cout<<time<<" "<<train::delete_train(char_more<char[21]>(i).get_char().data())<<std::endl;
            }else if (tokens[0]=="release_train") {
                i=tokens[2];
                std::cout<<time<<" "<<train::release_train(char_more<char[21]>(i).get_char().data())<<std::endl;
            }else if (tokens[0]=="query_train") {
                for (int II=1;II<tokens.size()-1;II+=2) {
                    if (tokens[II]=="-i") {
                        i=tokens[II+1];
                    }else if (tokens[II]=="-d") {
                        d=tokens[II+1];
                    }
                }
                std::cout<<time<<" "<<train::query_train(char_more<char[21]>(i).get_char().data(),char_more<char[6]>(d).get_char().data())<<std::endl;
            }else if (tokens[0]=="query_ticket") {
                for (int II=1;II<tokens.size()-1;II+=2) {
                    if (tokens[II]=="-s") {
                        s=tokens[II+1];
                    }else if (tokens[II]=="-t") {
                        t=tokens[II+1];
                    }else if (tokens[II]=="-d") {
                        d=tokens[II+1];
                    }else if (tokens[II]=="-p"){
                        p=tokens[II+1];
                    }
                }
                std::cout<<time<<" "<<train::query_ticket(char_more<char[31]>(s).get_char().data(),char_more<char[31]>(t).get_char().data(),char_more<char[6]>(d).get_char().data(),p)<<std::endl;
            }else if (tokens[0]=="query_transfer") {
                for (int II=1;II<tokens.size()-1;II+=2) {
                    if (tokens[II]=="-s") {
                        s=tokens[II+1];
                    }else if (tokens[II]=="-t") {
                        t=tokens[II+1];
                    }else if (tokens[II]=="-d") {
                        d=tokens[II+1];
                    }else if (tokens[II]=="-p"){
                        p=tokens[II+1];
                    }
                }
                std::cout<<time<<" "<<train::query_transfer(char_more<char[31]>(s).get_char().data(),char_more<char[31]>(t).get_char().data(),char_more<char[6]>(d).get_char().data(),p)<<std::endl;
            }else if (tokens[0]=="buy_ticket") {
                bool ok=false;
                for (int II=1;II<tokens.size()-1;II+=2) {
                    if (tokens[II]=="-u") {
                        u=tokens[II+1];
                    }else if (tokens[II]=="-i") {
                        i=tokens[II+1];
                    }else if (tokens[II]=="-d") {
                        d=tokens[II+1];
                    }else if (tokens[II]=="-n"){
                        n=tokens[II+1];
                    }else if (tokens[II]=="-f") {
                        f=tokens[II+1];
                    }else if (tokens[II]=="-t") {
                        t=tokens[II+1];

                    }else if (tokens[II]=="-q") {
                        q=tokens[II+1];
                        ok=true;
                    }
                }
                if (ok==0) {
                    std::cout<<time<<" "<<ticket::buy_ticket(char_more<char[21]>(u).get_char().data(),
                    char_more<char[21]>(i).get_char().data(),
                    char_more<char[6]>(d).get_char().data(),
                    char_more<char[31]>(f).get_char().data(),
                    char_more<char[31]>(t).get_char().data(),std::stoi(n),false)<<std::endl;
                }else {
                    std::cout<<time<<" "<<ticket::buy_ticket(char_more<char[21]>(u).get_char().data(),
                    char_more<char[21]>(i).get_char().data(),
                    char_more<char[6]>(d).get_char().data(),
                    char_more<char[31]>(f).get_char().data(),
                    char_more<char[31]>(t).get_char().data(),std::stoi(n)
                    ,q != "false")<<std::endl;
                }

            }else if (tokens[0]=="query_order") {
                u=tokens[2];
                std::cout<<time<<" "<<ticket::query_order(char_more<char[21]>(u).get_char().data())<<std::endl;
            }else if (tokens[0]=="refund_ticket") {
                n="0";
                for (int II=1;II<tokens.size()-1;II+=2) {
                    if (tokens[II]=="-u") {
                        u=tokens[II+1];
                    }else if (tokens[II]=="-n") {
                        n=tokens[II+1];
                    }
                }
                if (n=="0") {
                    std::cout<<time<<" "<<ticket::refund_ticket(char_more<char[21]>(u).get_char().data(),1)<<std::endl;
                }else {
                    std::cout<<time<<" "<<ticket::refund_ticket(char_more<char[21]>(u).get_char().data(),std::stoi(n))<<std::endl;
                }
            }
        }


