#include <iostream>
#include "taskqueue.hpp"
#include <cstring>
#include <unistd.h>

class Pthread_pool {
    public:
        Pthread_pool(int minnum, int maxnum) {
            p_taskQ = new TaskQueue;

            //init pthread pool
            p_minNUM = minnum;
            p_maxNUM = maxnum;
            p_busynum = 0;
            p_alivenum = minnum;
            
            //分配内存
            p_threadIDs = new pthread_t[maxnum];
            if(p_threadIDs == nullptr) {
                std::cout << "new failed \n";
                return;
            }

            //init pthread
            memset(p_threadIDs, 0, sizeof(pthread_t) * maxnum);

            //init mutex && cond
            if(pthread_mutex_init(&p_mutex, NULL) != 0 || pthread_cond_init(&p_notEmpty, NULL)) {
                std::cout << "mutex or cond failed";
                return;
            } 

            //create pthread
            for(int i = 0; i < minnum; ++i) {
                pthread_create(&p_threadIDs[i], NULL, worker, this);
                std::cout << "create thread, ID: " << p_threadIDs[i] << '\n';
            }

            //create manager
            pthread_create(&p_managerID, NULL, manager, this);
        }

        ~Pthread_pool() {
            p_shutdown = true;
            //destory manager
            pthread_join(p_managerID, NULL);

            //awake pthread
            for(int i = 0; i < p_alivenum; ++i) {
                pthread_cond_signal(&p_notEmpty);
            }

            if(p_taskQ) delete p_taskQ;
            if(p_threadIDs) delete[]p_threadIDs;

            pthread_mutex_destroy(&p_mutex);
            pthread_cond_destroy(&p_notEmpty);
        }

        void addTask(const Task& task) {
            if(p_shutdown) return;
            p_taskQ->addTask(task);
            pthread_cond_signal(&p_notEmpty);
        }

        int getAlive() {
            int res = 0;
            pthread_mutex_lock(&p_mutex);
            res = p_alivenum;
            pthread_mutex_unlock(&p_mutex);
            return res;
        }

        int getBusy() {
            int res = 0;
            pthread_mutex_lock(&p_mutex);
            res = p_busynum;
            pthread_mutex_unlock(&p_mutex);
            return res;
        }

    private:
        static void* worker(void* arg) {
            //transfer
            Pthread_pool* pool = static_cast<Pthread_pool*>(arg);
            
            //always work
            while(true) {
                pthread_mutex_lock(&pool->p_mutex);
                while(!pool->p_taskQ->Qsize() && !pool->p_shutdown) {
                    
                    //wait for notEmpty
                    std::cout << "pthread waiting, id:" << pthread_self() << '\n';
                    pthread_cond_wait(&pool->p_notEmpty, &pool->p_mutex);

                    //wheather destroy
                    if(pool->p_exitnum > 0) {
                        pool->p_exitnum--;
                        if(pool->p_alivenum > pool->p_minNUM) {
                            pool->p_alivenum--;
                            pthread_mutex_unlock(&pool->p_mutex);
                            pool->threadExit();
                        }
                    }
                }

                //wheather pool is closed
                if(pool->p_shutdown) {
                    pthread_mutex_unlock(&pool->p_mutex);
                    pool->threadExit();
                }

                //take a task from queue
                Task task = pool->p_taskQ->TakeTask();

                //working thread +1
                pool->p_busynum++;

                //unlock
                pthread_mutex_unlock(&pool->p_mutex);

                //just do it !!!
                std::cout << "thread " << pthread_self() << " start working" << std::endl;
                task.func(task.arg);
                // delete task.arg;
                // task.arg = nullptr;

                //end
                std::cout << "thread " << pthread_self() << "ending working" << std::endl;
                pthread_mutex_lock(&pool->p_mutex);
                pool->p_busynum--;
                pthread_mutex_unlock(&pool->p_mutex);
            }

            return NULL;
        }

        static void* manager(void* arg) {
            //transfer
            Pthread_pool* pool = static_cast<Pthread_pool*>(arg);
            
            //always scan if not closed 
            while(!pool->p_shutdown) {
                //scan interval is 5s
                sleep(5);

                // take out info
                pthread_mutex_lock(&pool->p_mutex);
                int queuesize = pool->p_taskQ->Qsize();
                int livnum = pool->p_alivenum;
                int busynum = pool->p_busynum;
                pthread_mutex_unlock(&pool->p_mutex);

                const int NUMBER = 2; // steplen

                //dynamic create
                //queuesize > livenum && livenum < maxnum
                if(queuesize > livnum && livnum < pool->p_maxNUM) {
                    pthread_mutex_lock(&pool->p_mutex);
                    int cnt = 0;
                    for(int i = 0; i < pool->p_maxNUM && cnt < NUMBER && pool->p_alivenum < pool->p_maxNUM; ++i) {
                        if(pool->p_threadIDs[i] == 0) {
                            pthread_create(&pool->p_threadIDs[i], NULL, worker, pool);
                            cnt++;
                            pool->p_alivenum++;
                        }
                    }
                    pthread_mutex_unlock(&pool->p_mutex);
                }

                //dyamic destroy
                if(busynum * 2 < livnum && livnum > pool->p_minNUM) {
                    pthread_mutex_lock(&pool->p_mutex);
                    pool->p_exitnum = NUMBER;
                    pthread_mutex_unlock(&pool->p_mutex);
                    for(int i = 0; i < NUMBER; ++i) {
                        pthread_cond_signal(&pool->p_notEmpty);
                    }
                }
            }

            return NULL;
        }

        void threadExit() {
            pthread_t tid = pthread_self();
            for(int i = 0; i < p_maxNUM; ++i) {
                if(p_threadIDs[i] == tid) {
                    std::cout << "threadExit thread " << tid << "exiting" << std::endl;
                    p_threadIDs[i] = 0;
                    break;
                }
            } 
            pthread_exit(NULL);
        }

    private:
        pthread_mutex_t p_mutex;
        pthread_cond_t p_notEmpty;
        pthread_t* p_threadIDs;
        pthread_t p_managerID;
        TaskQueue* p_taskQ;
        int p_minNUM;
        int p_maxNUM;
        int p_busynum;
        int p_alivenum;
        int p_exitnum;
        bool p_shutdown = false;
};