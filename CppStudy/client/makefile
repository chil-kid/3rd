subdirs = $(wildcard */)
src = $(foreach dir, $(subdirs), $(wildcard $(dir)*.cpp))

target = $(patsubst %.cpp, %.o, $(src))

ALL:main test

main:$(target) main.cpp
	g++ $^ -o main -ljsoncpp

%.o:%.cpp
	g++ -c $< -o $@
