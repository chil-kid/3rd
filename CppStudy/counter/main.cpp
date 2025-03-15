// ChiLkiD
// 定时器

#include <iostream>
#include <functional>
#include <chrono>
#include <queue>
#include <pthread.h>
#include <unistd.h>
#include <memory>

class TimeTask {
public:
    TimeTask() {

    }

    TimeTask(void(*func)(), std::chrono::steady_clock::time_point exetime) {
        task = func;
        ExeTime = exetime;
    }

    bool operator>(const TimeTask& others) const {
        return ExeTime > others.ExeTime;
    }

    void(*task)();
    std::chrono::steady_clock::time_point ExeTime;
};

class Timer {
public:
    Timer():shutdown(false) {
        pthread_mutex_init(&T_mutex, NULL);
        pthread_cond_init(&T_cond, NULL);
        T_thread_ptr = std::make_unique<pthread_t>(0);
        pthread_create(T_thread_ptr.get(), NULL, work, this);
    }

    ~Timer() {
        pthread_mutex_destroy(&T_mutex);
        pthread_cond_destroy(&T_cond);
        pthread_join(*T_thread_ptr, NULL);
    }

    void AddTask(const TimeTask& task) {
        pthread_mutex_lock(&T_mutex);
        T_que.push(task);
        pthread_mutex_unlock(&T_mutex);
        pthread_cond_signal(&T_cond);
    }

    void TimerExit() {
        shutdown = true;
        pthread_cond_signal(&T_cond);
    }

private:
    static void* work(void* arg) {
        Timer* timer = static_cast<Timer*>(arg);
        TimeTask task;
        while(!timer->shutdown) {
            bool isExe = false;
            pthread_mutex_lock(&timer->T_mutex);
            if(timer->T_que.empty()) {
                pthread_cond_wait(&timer->T_cond, &timer->T_mutex);
                if(timer->shutdown) return nullptr;
                if(timer->T_que.top().ExeTime <= std::chrono::steady_clock::now()) {
                    task = timer->T_que.top();
                    timer->T_que.pop();
                    isExe = true;
                }
            } 
            else if(timer->T_que.top().ExeTime <= std::chrono::steady_clock::now()) {
                task = timer->T_que.top();
                timer->T_que.pop();
                isExe = true;
            }
            pthread_mutex_unlock(&timer->T_mutex);

            if(isExe) task.task();
        }

        return nullptr;
    }

private:
    std::priority_queue<TimeTask, std::vector<TimeTask>, std::greater<>> T_que;
    pthread_mutex_t T_mutex;
    pthread_cond_t T_cond;
    bool shutdown;
    std::unique_ptr<pthread_t> T_thread_ptr;
};

void test() {
    auto TimePoint = std::chrono::steady_clock::now();
    std::cout << "current time:: " << TimePoint.time_since_epoch().count() << std::endl;
}

int main() {

    Timer timer;

    auto e1 = std::chrono::steady_clock::now() + std::chrono::milliseconds(1000);
    TimeTask t1(test, e1);
    timer.AddTask(t1);

    auto e2 = std::chrono::steady_clock::now() + std::chrono::milliseconds(500);
    TimeTask t2(test, e2);
    timer.AddTask(t2);

    auto e3 = std::chrono::steady_clock::now() + std::chrono::milliseconds(1500);
    TimeTask t3(test, e3);
    timer.AddTask(t3);

    sleep(5);
    timer.TimerExit();
}