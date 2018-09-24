#include <iostream>
#include <functional>
#include <vector>
#include <string>
template<typename DataType>
class EventBus{
public:
    typedef std::function<void(DataType *)> listener;
    void addListener(listener lsenr){
        lsenrs.push_back(lsenr);
    }
    void pushData(DataType * data){
        datas.push_back(data);
    }

    void loop(){
        for(auto data:datas){
            for(auto & ls:lsenrs)
            ls(data);
        }

    }

private:
    std::vector<listener> lsenrs;
    std::vector<DataType *>datas;
};

template <typename DataType>
class ListenerInterFace{
public:
    typedef std::function<void(ListenerInterFace<DataType> *,DataType *)> InternallistnerFunction;
    typedef std::function<void(DataType *)> listener;

    virtual void On(DataType *)=0;

    listener getListener(InternallistnerFunction func ){
        return std::bind(func,this,std::placeholders::_1);
    }

    void regist(EventBus<DataType>* evbus){
        evbus->addListener(getListener(ListenerInterFace<DataType>::On)) ;
    }

};



template <typename Head,typename ...Args>
class MultiSender:public MultiSender<Args...>,public EventBus<Head>{
public:
    using MultiSender<Args...>::pushData;
    using MultiSender<Args...>::addListener;
    using MultiSender<Args...>::loop;
    using EventBus<Head>::pushData;
    using EventBus<Head>::addListener;

    template<typename T>
    void loop(){
        EventBus<T>::loop();
    }

    void pushData(Head *data){
        static_cast<EventBus<Head> *>(this)->pushData(data);
    }
    void addListener( std::function<void(Head *)> lsenr){
        static_cast<EventBus<Head> *>(this)->addListener(lsenr);
    }

    void loop(){
        static_cast<EventBus<Head>*>(this)->loop();
        static_cast< MultiSender<Args...>*>(this)->loop();
    }

};


template <typename Tail>
class MultiSender<Tail>:public EventBus<Tail>{
public:
    using EventBus<Tail>::pushData;
    using EventBus<Tail>::addListener;
    void pushData(Tail *data){
        static_cast<EventBus<Tail> *>(this)->pushData(data);
    }
    void addListener(std::function<void(Tail *)>  lsenr){
        static_cast<EventBus<Tail> *>(this)->addListener(lsenr);
    }
    void loop(){
        static_cast<EventBus<Tail> *>(this)->loop();
    }

};

template <typename Head,typename ...Args>
class MutiListenerInterface;

template <typename Head,typename ...Args>
class MutiListenerInterface:public  MutiListenerInterface<Args...>,public ListenerInterFace<Head>
{
public:
    using MutiListenerInterface<Args...>::regist;
    void regist(EventBus<Head> * evbus){
        static_cast<ListenerInterFace<Head>*>(this)->regist(evbus);
    }



    template<typename T>
    void regist(T * sender){
        regist(static_cast<EventBus<Head>*>(sender));
        MutiListenerInterface<Args...>::regist(sender);
    }


};

template <typename Tail>
class MutiListenerInterface<Tail>:public ListenerInterFace<Tail>{


public:
    void regist(EventBus<Tail> * evbus){
        static_cast<ListenerInterFace<Tail>*>(this)->regist(evbus);
    }
    void regist(MultiSender<Tail> * sender){
        regist(static_cast<EventBus<Tail>*>(sender));
    }


};



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



