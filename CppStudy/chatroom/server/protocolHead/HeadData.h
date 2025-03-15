#pragma once
#include "protocolMsg.h"

/*
self protocol
*/

class HeadData {
public:
    HeadData();

    HeadData(int fd);

    unsigned int get_Protocol_ID();
    unsigned int get_Account();
    unsigned int get_Data_Type();
    unsigned int get_Data_Len();

    void parse(const char* buf);

private:
    void parse_template();

    unsigned int parseToUint(int len);

    char buf[BASE_BUF_SIZE];
    const char* ptr = nullptr;
    unsigned int Protocol_ID;
    unsigned int Account;
    unsigned int Data_Type;
    unsigned int Data_Len;
};