#include "online.h"

std::string online::getOnlineList() {
    Json::Value onlineList;
    for(auto x: userSet) {
        Json::Value userJson;
        userJson["account"] = x.account;
        userJson["username"] = x.username;
        onlineList.append(userJson);
    }
    return onlineList.toStyledString();
}

void online::appendUser(unsigned int account, const std::string& username, int fd) {
    user u = (user){account, username, fd};
    userSet.insert(u);
}

void online::appendUser(std::pair<unsigned int, std::string> &user, int fd) {
    appendUser(user.first, user.second, fd);
}

void online::removeUser(unsigned int account) {
    userSet.erase({account, ""});
}

bool online::isLogin(unsigned int account) {
    auto it = userSet.lower_bound({account, ""});
    if(it != userSet.end() && it->account == account) return true;
    return false;
}

/* get username by account */
std::string online::getOnlineUsername(unsigned int account) {
    auto it = userSet.lower_bound({account, ""});
    if(it == userSet.end()) return "";
    return it->username;
}

/* send text to all online user */
void online::boardcastText(const std::string& text, unsigned int account) {
    IO io;
    for(auto [x,y,fd]: userSet) {
        if(x == account) continue;
        io.writeText(fd, account, text);
    }
}

/* send picture to all online user */
void online::boardcastImg(const std::string& imgpath, unsigned int account) {
    IO io;
    for(auto [x,y,fd]: userSet) {
        if(x == account) continue;
        io.writeImg(fd, account, imgpath);
    }
}