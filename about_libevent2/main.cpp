#include "thread.h"
#include "tcpserver.h"
int main()
{

    Tcpserver *t = new Tcpserver;
    t->listen(8000);
    t->start();
    return 0;


    // Thread *t1 = new Thread;
    // Thread *t2 = new Thread;
    // t1->start();
    // t2->start();
    // while(1)
    // {
    //     printf("主线程\n");
    //     sleep(3);
    // }
    // return 0;
}

// g++ *.cpp -o main -lpthread -levent -std=c++11