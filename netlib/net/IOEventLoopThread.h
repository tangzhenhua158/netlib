#ifndef AGILNET_IOEVENTLOOPTHREAD
#define AGILNET_IOEVENTLOOPTHREAD

#include <common/Thread.h>
#include <common/Condition.h>
#include <IOEventLoop.h>


namespace net
{

class IOEventLoopThread : public Thread
{
public :
    IOEventLoopThread();
    virtual void run();
    IOEventLoop* getLoopInThread();
private:
    IOEventLoop* loop;
    Mutex mutex;
    Condition condtion;
};


}

#endif // AGILNET_IOEVENTLOOPTHREAD
