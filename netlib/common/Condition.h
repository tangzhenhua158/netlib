#ifndef AGILNET_SUPPORT_CONDITION_H
#define AGILNET_SUPPORT_CONDITION_H

#include "Mutex.h"

#include <boost/noncopyable.hpp>
#include <Log.h>


namespace net
{

class Condition : boost::noncopyable
{
public:
    explicit Condition(Mutex& mutex)
      : mutex(mutex)
    {
        if(pthread_cond_init(&pcond, NULL)<0)
        {
            Log::getSingle()->write(Log::error,"error when init condition.");
        }
    }

    ~Condition()
    {
        if(pthread_cond_destroy(&pcond)<0)
        {
            Log::getSingle()->write(Log::error,"error when destroy condition.");
        }
    }

    void wait()
    {
        if(pthread_cond_wait(&pcond, mutex.getPthreadMutex())<0)
        {
            Log::getSingle()->write(Log::error,"error when wait condition.");
        }
    }
    void notify()
    {
        if(pthread_cond_signal(&pcond))
        {
            Log::getSingle()->write(Log::error,"error when notify condition.");
        }
    }

    void notifyAll()
    {
        if(pthread_cond_broadcast(&pcond))
        {
            Log::getSingle()->write(Log::error,"error when notifyAll condition.");
        }
    }

private:
    Mutex& mutex;
    pthread_cond_t pcond;
};

}
#endif
