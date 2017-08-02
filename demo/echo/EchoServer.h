#ifndef AGILNET_DOME_ECHOSERVER
#define AGILNET_DOME_ECHOSERVER

#include <net/TcpServer.h>



class EchoServer : public net::TcpServer
{
public:
    EchoServer(net::IOEventLoop* loop,net::SocketAddr& addr);
    //考虑到tcpConnect可能关闭后内存被自动释放，传shared_ptr比较安全。
    virtual void connectCallback(boost::shared_ptr<net::TcpConnect> tcpConnect);
    virtual void messageCallback(boost::shared_ptr<net::TcpConnect>, net::Buffer&);
    virtual void writeCompletCallback(boost::shared_ptr<net::TcpConnect> tcpConnect) ;
    virtual void connectCloseCallback( boost::shared_ptr<net::TcpConnect>);
};

#endif // AGILNET_DOME_ECHOSERVER
