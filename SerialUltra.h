//
// Created by mijiao on 23-2-21.
//

#ifndef SERIALULTRA_SERIALULTRA_H
#define SERIALULTRA_SERIALULTRA_H

#include <iostream>
#include "serialib.h"
#include "MessageProcessor/OutputMessageProcessor.h"
#include "MessageProcessor/InputMessageProcessor.h"


namespace su {
    template<typename Head, typename Tail>
    class SerialUltra {
    private:
        serialib serial;
        mp::OutputMessageProcessor<Head, Tail> OMP;
        mp::InputMessageProcessor<Head, Tail> IMP;

    public:
        SerialUltra() : OMP(serial), IMP(serial) {}

        SerialUltra(const std::string& device, int baud) : OMP(serial), IMP(serial) {
            if(open(device,baud)==1){
                IMP.start();
            }else{
                std::cout<<"open Failed!"<<std::endl;
            }
        }

        bool status(){
            return serial.isDeviceOpen();
        }

        char open(const std::string& device, int baud){
            if(!serial.isDeviceOpen())
            {
                char ret = serial.openDevice(device.c_str(),baud);
                if(ret==1){
                    IMP.start();
                }
                return ret;
            }else{
                return 0;
            }
        }

        void close(){
            IMP.stop();
            serial.closeDevice();
        }

        void spinOnce() {
            IMP.spinOnce();
        }

        void spin(bool background = false) {
            IMP.spin(background);
        }

        bool setMaxSize(size_t _maxSize){
            IMP.setMaxSize(_maxSize);
        }

        void registerChecker(std::function<int(const Head&)> checker) {
            IMP.registerChecker(checker);
        }

        void registerChecker(std::function<int(const Tail&, const uint8_t*, int)> checker) {
            IMP.registerChecker(checker);
        }

        void setGetLength(std::function<size_t(const Head&)> _getLength) {
            IMP.setGetLength(_getLength);
        }

        void setGetId(std::function<int(const Head&)> _getID) {
            IMP.setGetId(_getID);
        }

        template<typename functionType>
        void registerCallBack(size_t id, functionType callback){
            IMP.registerCallBack(id,callback);
        }

        void registerHeadPreprocessor(std::function<void(Head&, size_t)>& headPreprocessor) {
            OMP.registerHeadPreprocessor(headPreprocessor);
        }

        void registerTailPreprocessor(std::function<void(Tail&, const uint8_t*, size_t)>& tailPreprocessor) {
            OMP.registerTailPreprocessor(tailPreprocessor);
        }

        template<typename Data>
        int write(Head head, Data data, Tail tail){
            return OMP.write(head,data,tail);
        }

    };
}

#endif //SERIALULTRA_SERIALULTRA_H
