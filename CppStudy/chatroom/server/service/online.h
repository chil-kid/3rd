#pragma once

#include <iostream>
#include <jsoncpp/json/json.h>
#include <sys/socket.h>
#include <set>
#include "IO.h"

class online {
public:
    std::string getOnlineList();

    void appendUser(unsigned int account, const std::string& username, int fd);
    void appendUser(std::pair<unsigned int, std::string> &user, int fd);

    void removeUser(unsigned int account);

    bool isLogin(unsigned int account);

    std::string getOnlineUsername(unsigned int account);

    void boardcastText(const std::string& text, unsigned int account);

    void boardcastImg(const std::string& imgpath, unsigned int account);

private:
    struct user {
        unsigned int account;
        std::string username;
        int fd;
        bool operator<(const user& x) const {
            return account < x.account;
        }
    };

    std::set<user> userSet;
};