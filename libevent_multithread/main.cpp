#include "thread.h"
#include "myserver.h"
int main(int argc,char** argv)
{
    int port = atoi(argv[1]);
    Mserver* t = new Mserver;
    t->listen(port);
    t->start();
    
    return 0;
}

//g++ *.cpp -o main -levent -lpthread -std=c++11