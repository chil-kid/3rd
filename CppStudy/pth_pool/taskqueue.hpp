#include <pthread.h>
#include <queue>

class Task {
    public:
        Task() {
            func = nullptr;
            arg = nullptr;
        }

        Task(void(*f)(void*), void* arg) {
            func = f;
            this->arg = arg;
        }

        Task(const Task& task) {
            func = task.func;
            arg = new int(*((int*)task.arg));
        }

        ~Task() {
            if(arg == nullptr) return;
            delete arg;
        }

        void operator=(const Task& task) {
            func = task.func;
            arg = new int(*((int*)task.arg));
        }

        void(*func)(void*);
        void* arg;
};

class TaskQueue {
    public:
        TaskQueue() {
            pthread_mutex_init(&t_mutex, NULL);
        }

        ~TaskQueue(){
            pthread_mutex_destroy(&t_mutex);
        }

        void addTask(const Task& task) {
            pthread_mutex_lock(&t_mutex);
            t_queue.push(task);
            pthread_mutex_unlock(&t_mutex);
        }

        void addTask(void(*f)(void*), void* arg) {
            pthread_mutex_lock(&t_mutex);
            Task temp(f, arg);
            t_queue.push(temp);
            pthread_mutex_unlock(&t_mutex);
        }

        Task TakeTask() {
            Task res;
            pthread_mutex_lock(&t_mutex);
            if(!t_queue.empty()) {
                res = t_queue.front();
                t_queue.pop();
            }
            pthread_mutex_unlock(&t_mutex);
            return res;
        }

        int Qsize() {
            return t_queue.size();
        }


    private:
        pthread_mutex_t t_mutex; //mutex lock
        std::queue<Task> t_queue; // task queue
};  