#pragma once
#include "../database/MysqlConnector.h"

class userService {
private:
    MysqlConnector *connector;

    bool checkAccount(const std::string& account);

    bool checkPassword(const std::string& password);

public:
    userService();

    std::pair<unsigned int, std::string> checkLogin(const std::string& account, const std::string& password);

    bool isRegist(const std::string& account);

    bool registUser(const std::string& account, const std::string& password, const std::string& username);

    std::string getUsername(const std::string& account);
};