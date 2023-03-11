#include "SerialUltra.h"


MessageData Data2{int num = 64;} data2;
MessageData Data{char msg[200]="abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz";} data;
MessageData Head{int head=0xAA;int id=0x01;int length = sizeof(Data2);} head;
MessageData Tail{} tail;

int main() {
    su::SerialUltra<Head,Tail> s("/dev/pts/1",115200);
    s.write(head,data,tail);
    s.write(Head{.id=0x02},data2,tail);
    return 0;
}
