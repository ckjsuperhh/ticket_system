//
// Created by ckjsuperhh6602 on 25-5-26.
//
#include "DataBase.h"
#include "user.h"
#include "ticket.h"
#include "train.h"
void cmd::initialise() {
        user_file.initialise("user_file(1)","user_file(2)");//存儲用戶名到存儲地址的映射
        user_storage.initialise("user_storage");
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
void cmd::analyse(const string & a) {
        std::cout<<a<<std::endl;
        std::string str;
        std::string c, u, p, n, m, g;
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
            std::cout<<time<<" "<<user::add_user(char_more<char[21]>(c).get_char().data(),char_more<char[21]>(u).get_char().data(),char_more<char[31]>(p).get_char().data(),char_more<char[11]>(n).get_char().data(),char_more<char[31]>(m).get_char().data(),std::stoi(g))<<std::endl;
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
                    const auto a=user_file.search(cur);
                    if (a.empty()) {
                        std::cout<<time<<" "<<-1<<std::endl;//cur不存在
                    }else if (login_state.find(cur)==login_state.end()) {
                        std::cout<<time<<" "<<-1<<std::endl;//并沒有登錄
                    }else if (c==u) {
                        if (const auto up=user_storage.read(a[0]); flag[2]==true&&std::stoi(g)>=up.privilege) {
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
                            char pa[31],na[11],ma[31];
                            strcpy(pa,flag[3]==1?char_more<char[31]>(p).get_char().data():up.password);
                            strcpy(na,flag[0]==1?char_more<char[11]>(n).get_char().data():up.name);
                            strcpy(ma,flag[1]==1?char_more<char[31]>(m).get_char().data():up.mail);
                            int pr=flag[2]==0?std::stoi(g):up.privilege;
                            user_storage.update({pa,na,ma,pr},a[0]);
                            std::cout<<time<<" "<<out<<std::endl;
                        }
                    }else{
                        char username[21];
                        strcpy(username,char_more<char[21]>(u).get_char().data());
                        const auto b=user_file.search(username);
                        if (b.empty()) {
                            std::cout<<time<<" "<<-1<<std::endl;//u不存在
                        }else {
                            const auto p1=user_storage.read(a[0]),p2=user_storage.read(b[0]);
                            if (p1.privilege<=p2.privilege) {
                                std::cout<<time<<" "<<-1<<std::endl;//cur權限過低
                            }else if (flag[2]&&std::stoi(g)>=p2.privilege) {
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
                                    out+=p;
                                }else{
                                    out+=std::to_string(p2.privilege);
                                }
                                char pa[31],na[11],ma[31];
                                strcpy(pa,flag[3]==1?char_more<char[31]>(p).get_char().data():p2.password);
                                strcpy(na,flag[0]==1?char_more<char[11]>(n).get_char().data():p2.name);
                                strcpy(ma,flag[1]==1?char_more<char[31]>(m).get_char().data():p2.mail);
                                int pr=flag[2]==0?std::stoi(g):p2.privilege;
                                user_storage.update({pa,na,ma,pr},a[0]);
                                std::cout<<time<<" "<<out<<std::endl;
                            }
                        }
                    }

                }
            }else if (tokens[0]=="exit") {
                login_state.clear();
                std::cout<<time<<" bye"<<std::endl;
                exit(0);
            }
        }


