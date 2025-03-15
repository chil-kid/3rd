#pragma once
#include <mysql/mysql.h>
#include <iostream>

class MysqlConnector {
private:
    MYSQL *conn;

    static MysqlConnector *connector;

    void init();

public:
    static MysqlConnector *getMysqlConnector();

    std::pair<int, std::string> userMatch(const std::string& account, const std::string& password);

    bool insertUser(const std::string& account, const std::string& password, const std::string& username);

    bool isUser(const std::string& account);

    std::string accountGetUsername(const std::string& account);

    ~MysqlConnector();
};