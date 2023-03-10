#include <iostream>

#include "SerialUltra.h"


MessageData Data2{int num = 64;};
MessageData Data{char msg[200];} data;
MessageData Head{int head=0xAA;int id=0x01;int length = sizeof(Data);} head;
MessageData Tail{} tail;

void callbackFunction(const Data2& d){
    std::cout<<d.num<<std::endl;
}

int main() {
    su::SerialUltra<Head,Tail> s("/dev/pts/2",115200);
    s.registerChecker([](const Head& head){return head.head != 0xAA;});
    s.registerChecker([](const Tail&, const uint8_t*, int){return 0;});
    s.setGetLength([](const Head& head){return head.length;});
    s.setGetId([](const Head& head){return head.id;});

    std::function<void(const Data& data)> cb = [](const Data& data){std::cout<<"Received:"<<data.msg<<std::endl;};

    s.registerCallBack(0x01, cb);
    s.registerCallBack(0x02, callbackFunction);
    s.spin(true);//std::cout << "Hello, World!" << std::endl;
    while(1){
        char ch;
        std::cin>>ch;
        if(ch=='n') break;
    };
    s.close();
    return 0;
}
