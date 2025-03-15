#include "pth_pool.hpp"

void func(void* arg) {
    int* id = static_cast<int*>(arg);
    std::cout << "thread " << pthread_self() << " function: " << (*id) << '\n';
    sleep(1);
}

int main() {
    Pthread_pool pool(10, 200);
    for(int i = 0; i < 1; ++i) {
        Task task(func, new int(i));
        
        pool.addTask(task);
    }

    sleep(10);
}