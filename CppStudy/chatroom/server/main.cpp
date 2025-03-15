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
    /*
        prepare for accept
    */
    int lfd = socket(AF_INET, SOCK_STREAM, 0); //tcp protocol
    sockaddr_in servaddr, clitaddr;
    socklen_t clitaddr_len;

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    inet_pton(AF_INET, HOST, &servaddr.sin_addr);

    if(bind(lfd, (sockaddr*)& servaddr, sizeof(servaddr)) == -1) {
        perror("bind error");
        exit(1);
    }

    if(listen(lfd, MAX_CONNECTION) == -1) {
        perror("listen error");
        exit(1);
    }

    /*
        init epoll
    */
    int epfd = epoll_create(MAX_CONNECTION); //create root
    epoll_event epollTemp, epollList[MAX_CONNECTION]; //temp and list
    epollTemp.data.fd = lfd;
    epollTemp.events = EPOLLIN;
    if(epoll_ctl(epfd, EPOLL_CTL_ADD, lfd, &epollTemp) == -1) {
        std::cout << "epoll ctl error\n";
        exit(1);
    } //insert lfd

    std::cout << "accepting...\n";

    userService us; // user service
    online ol; //online service

    while(true) {
        int eventCount = epoll_wait(epfd, epollList, MAX_CONNECTION, -1);
        if(eventCount < 0) {
            std::cout << "epoll wait error\n";
            exit(1);
        } 
        std::cout << "get " << eventCount << " request\n";

        for(int i = 0; i < eventCount; ++i) {
            int fd = epollList[i].data.fd;
            if(fd == lfd) { // link client
                clitaddr_len = sizeof(clitaddr);
                int cfd = accept(lfd, (sockaddr*)&clitaddr, &clitaddr_len);
                epollTemp.data.fd = cfd;
                epollTemp.events = EPOLLIN;
                epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &epollTemp);
            } else { //read event

                /* parse the head first */
                HeadData HD(fd);
                unsigned int Protocol_ID = HD.get_Protocol_ID();
                unsigned int Account = HD.get_Account();
                unsigned int Data_Type = HD.get_Data_Type();
                unsigned int Data_Len = HD.get_Data_Len();

                /* a debug sentence */
                // std::cout << Protocol_ID << '\n';
                // std::cout << Account << '\n';
                // std::cout << Data_Type << '\n';
                // std::cout << Data_Len << '\n';

                IO io;
                if(Protocol_ID == LOGIN) {
                    /* deal login message */
                    std::string loginMsg = io.readText(fd, Data_Len);
                    Json::Reader jsonreader;
                    Json::Value msg;
                    jsonreader.parse(loginMsg, msg);

                    /* get info and check */
                    std::string account = msg["account"].asString();
                    std::string password = msg["password"].asString();
                    std::pair<unsigned int, std::string> user = us.checkLogin(account, password);

                    /* callback */
                    Json::Value loginResult;
                    if(user.first != -1) {
                        /* success */
                        if(ol.isLogin(atoi(account.data()))) {
                            //has already Login
                            loginResult["status"] = LOGIN_EXIST;
                        } else {
                            std::pair<unsigned int, std::string> info = std::make_pair(atoi(account.data()), us.getUsername(account));
                            ol.appendUser(info, fd);
                            loginResult["status"] = LOGIN_SUCCESS;
                        }
                    } else {
                        /* error */
                        loginResult["status"] = LOGIN_FAIL;
                    }
                    std::string res = loginResult.toStyledString();
                    io.writeMsg(fd, Account, res);

                } else if(Protocol_ID == REGIST) {
                    /*deal register message */
                    std::string registMsg = io.readText(fd, Data_Len);
                    Json::Reader jsonreader;
                    Json::Value Msg;
                    jsonreader.parse(registMsg, Msg);

                    /* get info */
                    std::string account = Msg["account"].asString();
                    std::string password = Msg["password"].asString();
                    std::string username = Msg["username"].asString();

                    /* callback */
                    Json::Value registResult;
                    if(us.isRegist(account) || !us.registUser(account, password, username)) {
                        registResult["status"] = REGIST_FAIL;
                    } else {
                        registResult["status"] = REGIST_SUCCESS;
                    }
                    std::string res = registResult.toStyledString();
                    io.writeMsg(fd, Account, res);

                } else if(Protocol_ID == SEND) {
                    /* identify base */
                    std::string baseMsg = ol.getOnlineUsername(Account) + "(" + std::to_string(Account) + ")" + "说：";
                    ol.boardcastText(baseMsg, Account);

                    if(Data_Type == TEXT) {
                        /* info is text */
                        std::string getData = io.readText(fd, Data_Len);
                        ol.boardcastText(getData, Account);

                    } else if(Data_Type == IMG) {
                        /* info is img */
                        std::string getData = io.readImg(fd, Data_Len);
                        ol.boardcastImg(getData, Account);
                    }

                } else if(Protocol_ID == LOGOUT) {
                    epollTemp.data.fd = fd;
                    epollTemp.events = EPOLLIN;
                    epoll_ctl(epfd, EPOLL_CTL_DEL, fd, &epollTemp);
                    close(fd);
                    ol.removeUser(Account);
                }
            }
        }
    }
    
    close(lfd);
}