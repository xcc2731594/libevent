#include "thread.h"

Thread::Thread()
{
    m_base = event_base_new();
    if(!m_base)
    {
        printf("could not create");
        exit(1);
    }
    int pipefd[2];
    if(pipe(pipefd)==-1)
    {
        exit(EXIT_FAILURE);
    }
    m_pipReadFd = pipefd[0];
    m_pipWriteFd = pipefd[1];;
    event_set(&m_pipeEvent, m_pipReadFd, EV_READ|EV_PERSIST, pipeCb, this);
    event_base_set(m_base,&m_pipeEvent);
    event_add(&m_pipeEvent,0);
}

Thread::~Thread()
{}

void Thread::start()
{
    pthread_create(&m_threadId,NULL,worker,this);
    pthread_detach(m_threadId);
}

void *Thread::worker(void *arg)
{
    Thread *t = (Thread *)arg;
    t->run();
    return NULL;
}

void Thread::run()
{
    printf("thread:%drunning\n",(int)m_threadId);
    event_base_dispatch(m_base);
    event_base_free(m_base);
    printf("thread:%dend\n",(int)m_threadId);
}

void Thread::pipeCb(int, short, void *)
{

}

pthread_t Thread::get_threadId()
{
    return m_threadId;
}

event_base * Thread::get_base()
{
    return m_base;
}