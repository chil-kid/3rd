#include "HeadData.h"
#include <unistd.h>

unsigned int HeadData::get_Protocol_ID() {
    return this->Protocol_ID;
}

unsigned int HeadData::get_Account() {
    return this->Account;
}

unsigned int HeadData::get_Data_Type() {
    return this->Data_Type;
}

unsigned int HeadData::get_Data_Len() {
    return this->Data_Len;
}

HeadData::HeadData() {

}

HeadData::HeadData(int fd) {
    read(fd, buf, BASE_BUF_SIZE);
    ptr = buf;
    parse_template();
}

void HeadData::parse(const char* buf) {
    ptr = buf;
    parse_template();
}

void HeadData::parse_template() {
    this->Protocol_ID = parseToUint(PROTOCOL_ID_SIZE);
    this->Account = parseToUint(ACCOUNT_SIZE);
    this->Data_Type = parseToUint(DATA_TYPE_SIZE);
    this->Data_Len = parseToUint(DATA_LEN_SIZE);
}

unsigned int HeadData::parseToUint(int len) {
    unsigned res = 0;
    for(int i = 0; i < len; ++i, ++ptr) {
        res = (res << 8) + (unsigned int)((unsigned char)*ptr); //1 byte --> 8 bit
    }
    return res;
}