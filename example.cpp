//
// Created by llx on 2018/9/24.
//


#include "EventEngine.h"


//-----------------------------------------------

struct testStruct{
    std::string id;
    int t;
};

class IntStringSrtuctLsner:public  MutiListenerInterface<int,std::string,testStruct>{
public:
    void On(int * number) override {
        std::cout<<"int method called :"<<*number<<std::endl;
    }

    void On(std::string * str)override {
        std::cout<<"string method called :"<<(*str)<<std::endl;
    }

    void On(testStruct * s)override {
        std::cout<<"teststruct method called :"<<s->id<<"-"<<s->t<<std::endl;
    }

};




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
