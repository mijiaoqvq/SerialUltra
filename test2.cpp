#include "SerialUltra.h"


MessageData Data{uint8_t msg[59];} data;
MessageData Head{uint8_t head1=0xF4;uint8_t head2=0xF1;uint8_t id=0xFF;uint8_t length = sizeof(Data);} head;
MessageData Tail{uint8_t sum;} tail;


int main() {
    su::SerialUltra<Head,Tail> s("/dev/ttyACM1",115200);
    s.registerTailPreprocessor([](Tail& tail, const uint8_t* p, size_t len){
        uint8_t sum = 0;
        for(int i=0;i<len;i++){
            sum+=p[i];
        }
        tail.sum = sum;
    });
    data.msg[0] = 2;
    data.msg[1] = 1;
    for(int i=2;i<59;i++){
        data.msg[i] = i+1;
    }
    while(true){
        for(int i=0;i<3;i++){
            std::cout<<"send: "<<i<<std::endl;
            head.id = i;
            s.write(head,data,tail);
            usleep(100000);
        }

    }

    return 0;
}
