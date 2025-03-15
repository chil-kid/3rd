#pragma once
#define BASE_BUF_SIZE 8

#define PROTOCOL_ID_SIZE 1
#define ACCOUNT_SIZE 2
#define DATA_TYPE_SIZE 1
#define DATA_LEN_SIZE 4

#define LOGIN 1
#define REGIST 2
#define LOGOUT 3
#define SEND 4

#define TEXT 1
#define IMG 2

#define LOGIN_SUCCESS 0
#define LOGIN_FAIL 1
#define LOGIN_EXIST 2

#define REGIST_SUCCESS 0
#define REGIST_FAIL 1