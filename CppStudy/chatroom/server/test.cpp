#include <arpa/inet.h>
#include <iostream>
#include <algorithm>
#include <unistd.h>
#include <sys/epoll.h>
#include <jsoncpp/json/json.h>
#include "config/serverConfig.h"
#include "protocolHead/HeadData.h"
#include "service/userservice.h"
#include "service/online.h"
#include "service/IO.h"

int main() {
    std::cout << "hello?\n";

    /* send head data */
    unsigned int protocolID = LOGOUT;
    unsigned int account = 12345;
    std::string text = "";
    Encoder encoder;
    std::string headstr = encoder.encode(protocolID, account, TEXT, text.length());
    
    /* send true data */
    const char* data = text.data();
    int len = text.length();
    int sz = 0;

    std::cout << len;
}