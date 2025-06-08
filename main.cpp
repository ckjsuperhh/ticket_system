#include <cassert>

#include "ticket.h"
#include "train.h"
#include "user.h"
#include "DataBase.h"
#include "cmd.h"
#include <iostream>
#include <string>
using namespace std;

int main() {
    string str;
    cmd::initialise();

    while (true) {
        // try {
        bool ok = true;
        while (getline(cin, str)) {
            ok = false;
            cmd::analyse(str);
        }
        if (ok or cin.eof()) {
            exit(0);
        }
        // } catch (...) {
        //     cout << "Invalid\n";
        // }
    }
}
