#include <iostream>

#include "SerialUltra.h"


MessageData Data2{int num = 64;};
MessageData Data{uint8_t msg[59];} data;
MessageData Head{uint8_t head1=0xF4;uint8_t head2=0xF1;uint8_t id=0xFF;uint8_t length = sizeof(Data);} head;
MessageData Tail{uint8_t sum;} tail;

void callbackFunction(const Data2& d){
    std::cout<<d.num<<std::endl;
}

int main() {
    su::SerialUltra<Head,Tail> s("/dev/ttyACM0",115200);
    s.registerChecker([](const Head& head){return !(head.head1 == 0xF4 && head.head2 == 0xF1);});
    s.registerChecker([](const Tail& tail, const uint8_t* p, int len){
        uint8_t sum = 0;
        for(int i=0;i<len;i++){
            sum+= p[i];
        }
        return sum != tail.sum;
        });
    s.setGetLength([](const Head& head){return head.length;});
    s.setGetId([](const Head& head){return head.id;});

    std::function<void(const Data& data)> cb = [](const Data& data){
        for(unsigned char i : data.msg){
            std::cout<<(int)i<<" ";
        }
        std::cout<<std::endl;
    };

    s.registerCallBack(0xFF, cb);
    s.registerCallBack(0x02, callbackFunction);
//    s.spinOnce();
    s.spin();
    while(1){
        char ch;
        std::cin>>ch;
        if(ch=='n') break;
    };
    s.close();
    return 0;
}
