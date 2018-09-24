//
// Created by llx on 2018/9/24.
//


#include "EventEngine.h"
int main() {

    IntStringSrtuctLsner ls;
    MultiSender<std::string,int,testStruct> sender;

    ls.regist(&sender);

    for(int i=0;i!=100;++i){
        sender.pushData(new int(i));
        sender.pushData(new std::string(std::to_string(i)));
        sender.pushData(new testStruct{"struct",i});
    }
    sender.loop();

}