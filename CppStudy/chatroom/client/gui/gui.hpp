#pragma once
#include <iostream>

void UserIn(int &x) {
    std::cout << "=========================================\n";
    std::cout << "choice 1: Login\n";
    std::cout << "choice 2: Register\n";
    std::cout << "=========================================\n";
    std::cin >> x;
}