#include "userservice.h"

bool userService::checkAccount(const std::string& account) {
    for(auto c:account) {
        /* account only consist of number */
        if(c > '9' || c < '0') return false;
    }

    /* account is considered not to exceed 65535 and not to less than one */
    if(stoi(account) > 65535 || stoi(account) < 1) return false;

    return true;
}

bool userService::checkPassword(const std::string& password) {
    /* password's length is considered between 6 and 16 */
    return password.length() >= 6 && password.length() <= 16;
}

userService::userService() {
    connector = MysqlConnector::getMysqlConnector();
}

std::pair<unsigned int,std::string> userService::checkLogin(const std::string& account, const std::string& password) {
    std::pair<int, std::string> user{-1, ""};
    if(checkAccount(account) && checkPassword(password)) {
        user = connector->userMatch(account, password);
    }
    return user;
}

bool userService::isRegist(const std::string& account) {
    if(!checkAccount(account)) return true;
    return connector->isUser(account);
}

bool userService::registUser(const std::string& account, const std::string& password, const std::string& username) {
    if(checkAccount(account) && checkPassword(password)) {
        return connector->insertUser(account, password, username);
    }
    return false;
}

std::string userService::getUsername(const std::string& account) {
    return connector->accountGetUsername(account);
}