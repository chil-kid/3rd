#pragma once
#include <cstdio>
#include <iostream>
#include <unistd.h>
#include <fstream>
#include <sys/socket.h>
#include "TimeCount.h"
#include "Encoder.h"

#define ImgPath "./img"

class FileOp {
public:
    int getFlieSize(const std::string& path);
};

class IO: public FileOp {
public:
    IO();

    std::string readText(int fd, int len);

    std::string readImg(int fd, int len);

    void writeText(int fd, unsigned int account, const std::string& text);

    void writeImg(int fd, unsigned int account, const std::string& imgpath);

    void writeMsg(int fd, unsigned int account, std::string& text);

private:
    char buf[BUFSIZ];
};