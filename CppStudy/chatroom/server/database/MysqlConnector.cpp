#include "MysqlConnector.h"
#include "../config/mysqlConfig.h"

void MysqlConnector::init() {
    conn = mysql_init(NULL);
    conn = mysql_real_connect(conn, HOST, USERNAME, PASSWORD, DATABASE, PORT, NULL, 0);
    if(!conn) {
        std::cout << "mysql_real_connect error\n";
        exit(1);
    }
    mysql_query(conn, "set names utf8");
}

MysqlConnector* MysqlConnector::getMysqlConnector() {
    if(connector == nullptr) {
        connector = new MysqlConnector;
        connector->init();
    }
    return connector;
}

/* username wheather match password */
std::pair<int, std::string> MysqlConnector::userMatch(const std::string& account, const std::string& password) {
    std::string sql = "select account, username from user where account = " + account + " and password = " + password;
    std::pair<int, std::string> user;
    int res = mysql_query(conn, sql.data());
    if(!res) {
        MYSQL_RES *result = mysql_store_result(conn);
        if(result && mysql_num_rows(result)) {
            MYSQL_ROW row = mysql_fetch_row(result);
            user.first = atoi(row[0]);
            user.second = row[1];
        }
        mysql_free_result(result);
    }
    return user;
}

/* user wheather exist */
bool MysqlConnector::isUser(const std::string& account) {
    std::string sql = "select account from user where account = " + account;
    int res = mysql_query(conn, sql.data());
    bool flag = false;
    if(!res) {
        MYSQL_RES *result = mysql_store_result(conn);
        flag = (result && mysql_num_rows(result));
        mysql_free_result(result);
    }
    return flag;
}

/* insert new user */
bool MysqlConnector::insertUser(const std::string& account, const std::string& password, const std::string& username) {
    std::string sql = "insert into `user` values(" + account + ","
        +"\"" + password + "\"" + "," + "\"" + username + "\"" + ")";
    int res = mysql_query(conn, sql.data());
    return !res;
}

MysqlConnector::~MysqlConnector() {
    mysql_close(conn);
    delete connector;
}

std::string MysqlConnector::accountGetUsername(const std::string& account) {
    std::string sql = "select username from user where account = " + account;
    if(mysql_query(conn, sql.data())) {
        std::cout << mysql_error(conn) << '\n';
        return "";
    }
    MYSQL_RES *result = mysql_store_result(conn);
    if(result == nullptr) {
        std::cout << "null\n";
        return "";
    }
    MYSQL_ROW row = mysql_fetch_row(result);
    std::string res = row[0];
    mysql_free_result(result);
    return res;
}

MysqlConnector *MysqlConnector::connector = nullptr;