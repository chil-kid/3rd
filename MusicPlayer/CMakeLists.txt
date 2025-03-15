cmake_minimum_required(VERSION 3.28)
project(MusicPlayer)

set(CMAKE_CXX_STANDARD 20)

add_executable(MusicPlayer main.cpp
        FileManage.cpp
        MusicPlayer.cpp
        PathCoder.cpp)

include_directories(${CMAKE_SOURCE_DIR}/dirent-master/include)
target_link_libraries(${PROJECT_NAME} winmm.lib)