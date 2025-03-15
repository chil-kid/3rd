#include <iostream>
#include <algorithm>
#include <pthread.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <jsoncpp/json/json.h>
#include <cstdlib>
#include "protocolHead/protocolMsg.h"
#include "config/serverConfig.h"
#include "protocolHead/HeadData.h"
#include "gui/gui.hpp"
#include "service/IO.h"

/* start IOservice */
IO io;
std::string thisAccount; //save for current account

void* input(void* arg) {
    uintptr_t fd = reinterpret_cast<uintptr_t>(arg);
    while(true) {
        std::string content; std::getline(std::cin, content);
        if(content == "") continue;
        if(content == "/kill") {
            /* logout */
            io.writeText(fd, atoi(thisAccount.data()), "", (unsigned int)LOGOUT);
            close(fd);
            exit(0);
        } else if(content.substr(0, 4) == "IMG:") {
            content.erase(0, 4);
            io.writeImg(fd, atoi(thisAccount.data()), content);
        } else {
            io.writeText(fd, atoi(thisAccount.data()), content);
        }
    }
    return NULL;
}

void* output(void* arg) {
    uintptr_t fd = reinterpret_cast<uintptr_t>(arg);
    while(true) {
        HeadData HD(fd);
        unsigned int Data_Len = HD.get_Data_Len();
        unsigned int Data_Type = HD.get_Data_Type();
        std::string content;
        if(Data_Type == TEXT) {
            content = io.readText(fd, Data_Len);
        } else if(Data_Type == IMG) {
            content = "IMG:" + io.readImg(fd, Data_Len);
        }
        std::cout << content << '\n';
    }
    return NULL;
}

int main() {
    /* prepare for connect */
    sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET, SERV_IP, &servaddr.sin_addr);

    int cfd = socket(AF_INET, SOCK_STREAM, 0);

    if(connect(cfd, (sockaddr*)&servaddr, sizeof(servaddr)) == -1) {
        perror("connect error");
        exit(1);
    }
    /* connect finish */

    /* option is login or register */
    while(true) {
        system("clear");
        int protocol_ID = 0;
        UserIn(protocol_ID);
        if(protocol_ID == LOGIN) {
            system("clear");
            /* input */
            std::cout << "input your account(number only and between 1 and 65535):";
            std::string account; std::cin >> account;
            std::cout << "input your password(len between 6 and 16):";
            std::string password; std::cin >> password;

            /* package */
            Json::Value data;
            data["account"] = account;
            data["password"] = password;
            std::string res = data.toStyledString();

            /* send */
            io.writeText(cfd, 0, res, protocol_ID);

            /* get callback */
            HeadData HD(cfd);
            int Data_Len = HD.get_Data_Len();
            std::string loginMsg = io.readText(cfd, Data_Len);
            Json::Reader jsonreader;
            jsonreader.parse(loginMsg, data);
            std::string status = data["status"].asString();
            bool nextpage = false;
            switch (atoi(status.data())) {
                case LOGIN_EXIST:
                    std::cout << "already exist !!!\n";
                    break;
                case LOGIN_FAIL:
                    std::cout << "login failed !!!\n";
                    break;
                case LOGIN_SUCCESS:
                    std::cout << "login success !!!\n";
                    thisAccount = account;
                    nextpage = true;
                    break;
                default :
                    break;
            }
            if(nextpage) break;
                
        } else if(protocol_ID == REGIST) {
            system("clear");
            /* input */
            std::cout << "input your account(number only and between 1 and 65535):";
            std::string account; std::cin >> account;
            std::cout << "input your password(len between 6 and 16):";
            std::string password; std::cin >> password;
            std::cout << "input your username:";
            std::string username; std::cin >> username;

            /* package */
            Json::Value data;
            data["account"] = account;
            data["password"] = password;
            data["username"] = username;
            std::string res = data.toStyledString();

            /* send */
            io.writeText(cfd, 0, res, protocol_ID);

            /* get callback */
            HeadData HD(cfd);
            int Data_Len = HD.get_Data_Len();
            std::string registMsg = io.readText(cfd, Data_Len);
            Json::Reader jsonreader;
            jsonreader.parse(registMsg, data);
            std::string status = data["status"].asString();
            if(atoi(status.data()) == REGIST_SUCCESS) {
                std::cout << "successful !!!\n";
            } else {
                std::cout << "failed !!!\n";
            }

        } else {
            std::cout << "unexcepted option...\n";
        }
        sleep(1);
    }

    std::cout << "haha, you login !!!\n";
    std::cout << "if exit is considered, input \"/kill\" \n";
    std::cout << "send picture please add \"IMG:\" at front\n";

    /* read and write */
    pthread_t pid;
    pthread_create(&pid, NULL, input, (void*)cfd);
    pthread_detach(pid);
    pthread_create(&pid, NULL, output, (void*)cfd);
    pthread_detach(pid);

    pthread_exit(NULL);
}
