#include "Encoder.h"

void Encoder::encodeunit(unsigned int data, int len) {
    for(unsigned char* end = ptr + len - 1; end >= ptr; --end) {
        *end = (unsigned char) (data & 0xff);
        data >>= 8;
    }
    ptr = ptr + len;
}

std::string Encoder::encode(unsigned int protocol_ID, unsigned int Account, unsigned int Data_Type, unsigned int Data_Len) {
    ptr = head;
    encodeunit(protocol_ID, PROTOCOL_ID_SIZE);
    encodeunit(Account, ACCOUNT_SIZE);
    encodeunit(Data_Type, DATA_TYPE_SIZE);
    encodeunit(Data_Len, DATA_LEN_SIZE);
    return std::string(reinterpret_cast<const char*>(head), sizeof(head));
}

Encoder::Encoder() {

}