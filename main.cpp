//
//  main.cpp
//  VirtualFileSys
//
//  Created by darknight on 2018/4/10.
//  Copyright © 2018年 darknight. All rights reserved.
//

#include "OS.h"
#include "FuncState.h"

int main(int argc, const char * argv[]) {
    string command, tmp, userName, userPass;
    intialize_memory();
    while (1) {
        if (userName == "") {
            cout<<"login:";
            cin>>userName;
            cout<<"pass:";
            cin>>userPass;
            if( !login(userName, userPass) ) {
                userName = "";
                continue;
            }
        }
        load_cur_block();
        cout<<"[ "<<cstate->current_User<<" @ "<<cstate->current_Path<<" ]$ ";
        cin>>command;
        if (command == "cd") {
            cin>> tmp;
            cd(tmp);
        }
        if (command == "create") {
            cin>> tmp;
            create(tmp);
        }
        if (command == "del") {
            cin>> tmp;
            del(tmp);
        }
        if (command == "open") {
            cin>> tmp;
            open(tmp);
        }
        if (command == "close") {
            cin>> tmp;
            close(tmp);
        }
        if (command == "read") {
            cin>> tmp;
            read(tmp);
        }
        if (command == "write") {
            cin>> tmp;
            write(tmp);
        }
        if (command == "dir") {
            dir();
        }
        if (command == "rd") {
            cin>> tmp;
            rd(tmp);
        }
        if (command == "md") {
            cin>> tmp;
            md(tmp);
        }
        if (command == "exit") {
            break;
        }
        if (command == "useradd") {
            cin >>tmp;
            useradd(tmp);
        }
        if (command == "su") {
            cin >>tmp;
            su(tmp);
        }
        if (command == "help") {
            help();
        }
        if (command == "ckdisk") {
            ckdisk();
        }
        if (command == "format") {
            char c;
            cout<<"Format will erase all the data in the Disk"<<endl;
            cout<<"Are you sure?y/n"<<endl;
            fflush(stdin);
            cin>>c;
            if (c == 'y' || c == 'Y') {
                format();
            } else {
                cout<<"Format Disk canceled"<<endl;
            }
        }
    }
    return 0;
}
