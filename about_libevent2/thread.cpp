#include "thread.h"

Thread::Thread()
{
    m_base = event_base_new();
    if(!m_base)
    {
        printf("could not create an event_base");
        exit(1);
    }
    //创建管道
    int pipefd[2];

    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    m_pieReadFd = pipefd[0];
    m_pieWriteFd = pipefd[1];
    //把管道事件添加到m_base的事件集合中 ??
    event_set(&m_pipeEvent, m_pieReadFd, EV_READ|EV_PERSIST, pipeCb,this);
    event_base_set(m_base,&m_pipeEvent);
    event_add(&m_pipeEvent,0);
    
}

Thread::~Thread()
{

}

void Thread::start()
{
     pthread_create(&m_threadId, nullptr,worker, this);
     pthread_detach(m_threadId);
}

void *Thread::worker(void* arg)
{
    
    Thread *t = (Thread *)arg;
    t->run();
    return nullptr;

}


void Thread::run()
{
    printf("线程:%d开始运行\n",(int)m_threadId);
    //死循环,用来处理事件集合
    //当事件集合m_base为空时，event_base_dispatch立马返回
    //在初始化的时候，就令m_base这个时间集合不为空
    event_base_dispatch(m_base);  
    event_base_free(m_base);
    printf("线程:%d结束运行\n",(int)m_threadId);
}

void Thread::pipeCb(int,short,void*)
{

}

event_base *Thread::get_base()
{
    return m_base;
}

pthread_t Thread::get_threadId()
{
    return m_threadId;
}