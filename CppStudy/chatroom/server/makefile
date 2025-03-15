subdirs = $(wildcard */)
src = $(foreach dir, $(subdirs), $(wildcard $(dir)*.cpp))

target = $(patsubst %.cpp, %.o, $(src))

ALL:main test

test:$(target) test.cpp 
	g++ $^ -o $@ `mysql_config --cflags --libs` -ljsoncpp

main:$(target) main.cpp
	g++ $^ -o main `mysql_config --cflags --libs` -ljsoncpp

%.o:%.cpp
	g++ -c $< -o $@
