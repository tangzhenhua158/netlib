#ifndef AGILNET_SOCKET
#define AGILNET_SOCKET

#include <SocketOperation.h>
#include <SocketAddr.h>

namespace net
{

class Socket
{
public:
    Socket(int fd);
    ~Socket();
    void bind(SocketAddr& addr);
    int accept(SocketAddr& addr);
    void listen();
    int getFd();
    void setTcpNoDelay(bool enable);
    int shutdownWrite();
private:
    int socketFd;
};
}

#endif
