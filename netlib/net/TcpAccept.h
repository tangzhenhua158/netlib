#ifndef AGILNET_NET_TCPACCEPT
#define AGILNET_NET_TCPACCEPT


#include <IOEventLoop.h>
#include <Socket.h>


namespace net
{

class TcpAccept
{
public:
    TcpAccept(IOEventLoop* loop,SocketAddr& addr);
    ~TcpAccept();
    void listen();
    bool isListen();
    void setNewConnectCallback(const boost::function<void (int sockfd,const SocketAddr&)> & callback);
private:
    IOEventLoop* eventLoop;
    boost::shared_ptr<Socket> socket;
    boost::shared_ptr<IOEvent> event;
    bool listening;

    void acceptHandle();
    boost::function<void (int sockfd,const SocketAddr&)> newConnectCallback;

};

}

#endif // AGILNET_NET_TCPACCEPT
