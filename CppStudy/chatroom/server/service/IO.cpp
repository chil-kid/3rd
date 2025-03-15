#include "IO.h"

int FileOp::getFlieSize(const std::string& path) {
    std::ifstream is(path, std::ios::in | std::ios::binary);
    if(!is.is_open()) return 0;
    is.seekg(0, std::ios::end);
    int res = is.tellg();
    is.close();
    return res;
}

IO::IO() {
    
}

std::string IO::readText(int fd, int len) {
    int sz = 0;
    std::string res;
    while(sz < len) {
        int n = read(fd, buf, BUFSIZ);
        if(n <= 0) break;
        sz += n;
        res += std::string(buf, sz);
    }
    return res;
} 

std::string IO::readImg(int fd, int len) {
    /* use time stamp as name */
    std::string imagepath = ImgPath + TimeCount::getStamp() + ".png";
    std::ofstream os(imagepath, std::ios::out | std::ios::binary);
    int sz = 0;
    while(sz < len) {
        int n = recv(fd, buf, BUFSIZ, 0);
        if(n <= 0) break;
        sz += n;
        os.write(buf, n); 
    }
    os.close();
    return imagepath;
}

void IO::writeText(int fd, unsigned int account, const std::string& text) {
    /* send head data */
    Encoder encoder;
    std::string headstr = encoder.encode(SEND, account, TEXT, text.length());
    send(fd, headstr.data(), headstr.length(), MSG_NOSIGNAL);
    
    /* send true data */
    const char* data = text.data();
    int len = text.length();
    int sz = 0;

    while(sz < len) {
        int n = send(fd, data, len, MSG_NOSIGNAL);
        if(n <= 0) break;
        sz += n;
        data = data + n;
    }
}

void IO::writeImg(int fd, unsigned int account, const std::string& imgpath) {
    /* prepare for head data */
    std::ifstream is(imgpath, std::ios::in | std::ios::binary);
    if(!is.is_open()) {
        return;
    }
    int imageSize = getFlieSize(imgpath);

    /* send head data */
    Encoder encoder;
    std::string headstr = encoder.encode(SEND, account, IMG, imageSize);
    send(fd, headstr.data(), headstr.length(), MSG_NOSIGNAL);

    /* send true data */
    is.seekg(0);
    
    while(!is.eof()) {
        is.read(buf, BUFSIZ);
        send(fd, buf, is.gcount(), MSG_NOSIGNAL);
    }

    is.close();
}

void IO::writeMsg(int fd, unsigned int account, std::string& text) {
    Encoder encoder;
    std::string headstr = encoder.encode(SEND, account, TEXT, text.length());
    text = headstr + text;
    send(fd, text.data(), text.length(), MSG_NOSIGNAL);
}