
#include<iostream>
#include<string>
#include<algorithm>
#include<vector>
#include<queue>
#include<stack>
#include<set>
#include<map>

#include <unistd.h>
#include <stdint.h>

void* zxt1(void *arg) {
    int* ptr = new int;
    *ptr = 111;
    return (void*)ptr;
}

void* zxt2(void *arg) {
    int* ptr = new int;
    *ptr = 333;
    pthread_exit((void*)ptr);
}
 
void* zxt3(void *arg) {
    
    while(1) {
        std::cout << "thread 3th: i'm going " << '\n';
        sleep(1);

        //pthread_testcancel();
    }
    return (void*)666;
}
signed main() {
    pthread_t tid;
    int* ptr = NULL;

    pthread_create(&tid, NULL, zxt1, NULL);
    pthread_join(tid, (void**)&ptr);
    std::cout << "pthread 1th return state: " << (*ptr) << '\n' << '\n';
    delete ptr;


    pthread_create(&tid, NULL, zxt2, NULL);
    pthread_join(tid, (void**)&ptr);
    std::cout << "pthread 2th return state: " << (*ptr) << '\n' << '\n';
    delete ptr;
    
    void* ptr2 = NULL;
    pthread_create(&tid, NULL, zxt3, NULL);
    sleep(1);
    std::cout << "main: oversleep\n"; 
    pthread_cancel(tid);
    pthread_join(tid, (void**)&ptr2);
    int i = (int)reinterpret_cast<uintptr_t>(ptr2);
    std::cout << "pthread 3th return state: " << (int)i << '\n' << '\n';
    //printf("pthread 3th return with exit code: %d", i);

    while(1) {
        std::cout << "ciallo\n";
        sleep(1);
    }

    return 0;
}
