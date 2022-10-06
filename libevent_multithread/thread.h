#ifndef THREAD_H
#define THREAD_H

#include <event.h>
#include <event2/event.h>
#include <stdlib.h>
#include <stdio.h>
#include <thread>
#include <unistd.h>

class Thread
{
private:
    struct event_base *m_base;
    pthread_t m_threadId;
    int m_pipReadFd;
    int m_pipWriteFd;
    struct event m_pipeEvent;
    
public:
    Thread();
    ~Thread();
    void start();
    void run();
    pthread_t get_threadId();
    event_base * get_base();
protected:

    static void *worker(void *);
    static void pipeCb(int, short, void *);

    
};

#endif