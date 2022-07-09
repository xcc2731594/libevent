#ifndef THREAD_H
#define THREAD_H
#include <event.h>
#include <event2/event.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
class Thread
{
private:
    struct event_base *m_base;
    pthread_t m_threadId;
    int m_pieReadFd;//管道读端
    int m_pieWriteFd; //管道写端
    struct event m_pipeEvent;
public:
    Thread();
    ~Thread();

    void start();    //线程的运行函数
    void run(); //线程的逻辑函数
    struct event_base *get_base(); //获取事件集合
    pthread_t get_threadId(); //获取线程id
protected:
    static void *worker(void *);  //线程的工作函数
    static void pipeCb(int,short,void*);

};

#endif