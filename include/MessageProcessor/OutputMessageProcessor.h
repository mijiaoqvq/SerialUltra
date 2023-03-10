//
// Created by mijiao on 23-2-21.
//

#ifndef SERIALULTRA_OUTPUTMESSAGEPROCESSOR_H
#define SERIALULTRA_OUTPUTMESSAGEPROCESSOR_H

#include <cstring>
#include <list>
#include <vector>
#include <functional>

#include "serialib.h"

namespace mp {
    template<typename Head, typename Tail>
    class OutputMessageProcessor {
    private:
        std::list<std::function<void(Head&, size_t)>> headPreprocessorList;
        std::list<std::function<void(Tail&, const uint8_t*/*数据指针*/, size_t/*头和数据长度*/)>> tailPreprocessorList;
        serialib& serial;

        template<typename Data>
        std::string serialize(Head head, Data data, Tail tail) {
            std::string message;
            message.resize(sizeof(Head) + sizeof(Data) + sizeof(Tail));
            for (auto headPreprocessor: headPreprocessorList) {
                headPreprocessor(head, sizeof(Data));
            }
            memcpy((void*) message.data(), &head, sizeof(Head));
            for (auto tailPreprocessor: tailPreprocessorList) {
                tailPreprocessor(tail, (uint8_t*) message.data(), sizeof(Head) + sizeof(Data));
            }
            memcpy((void*)(message.data() + sizeof(Head)), &data, sizeof(Data));
            memcpy((void*)(message.data() + sizeof(Head) + sizeof(Data)), &tail, sizeof(Tail));
            return std::move(message);

        }

    public:
        OutputMessageProcessor() = default;

        explicit OutputMessageProcessor(serialib& _serial) : serial(_serial) {}

        OutputMessageProcessor(OutputMessageProcessor& other) = delete;

        OutputMessageProcessor(OutputMessageProcessor&& other) noexcept = default;

        ~OutputMessageProcessor() = default;

        OutputMessageProcessor& operator=(OutputMessageProcessor&) = delete;

        void registerHeadPreprocessor(std::function<void(Head&, size_t)>& headPreprocessor) {
            headPreprocessorList.push_back(headPreprocessor);
        }

        void registerTailPreprocessor(std::function<void(Tail&, const uint8_t*, size_t)>& tailPreprocessor) {
            tailPreprocessorList.push_back(tailPreprocessor);
        }

        template<typename Data>
        int write(Head head, Data data, Tail tail){
            std::string message = serialize(head, data, tail);
            return serial.writeBytes(message.data(),message.size());
        }

    };
}
#endif //SERIALULTRA_OUTPUTMESSAGEPROCESSOR_H
