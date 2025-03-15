#include "TimeCount.h"

std::string TimeCount::getStamp() {
    timeval stamp;
    gettimeofday(&stamp, NULL);
    return std::to_string(stamp.tv_sec) + std::to_string(stamp.tv_usec);
}