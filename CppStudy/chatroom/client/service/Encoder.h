#pragma once
#include "../protocolHead/protocolMsg.h"
#include <iostream>

class Encoder {
public:
    Encoder();

    std::string encode(unsigned int protocol_ID, unsigned int Account, unsigned int Data_Type, unsigned int Data_Len);

private:
    unsigned char head[BASE_BUF_SIZE];
    unsigned char* ptr;

    void encodeunit(unsigned int data, int len);
};