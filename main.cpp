#include "ticket.h"
#include "train.h"
#include "user.h"
#include "DataBase.h"
#include "cmd.h"
#include <iostream>
#include <string>
using namespace  std;
[[noreturn]] int main() {
    sjtu::unordered_set<std::string> U;
    U.insert("I_am_the_admin");
    char u[21]="I_am_the_admin";
    if (U.find(u)==U.end()) {
        cout<<1<<endl;
    }else {
        cout<<0<<endl;
    }

    string str;
    cmd::initialise();
    while (true) {
        try {
            bool ok=true;
            while (getline(cin,str) and not cin.eof()) {
                ok=false;
                cmd::analyse(str);
            }
            if (ok or cin.eof()) {
                exit(0);
            }
        }catch (...) {
            cout<<"Invalid\n";
        }
    }
}
