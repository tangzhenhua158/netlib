

#ifndef AGILNET_DOME_ECHOSERVER
#define AGILNET_DOME_ECHOSERVER

#include <net/TcpServer.h>



class BaseServer : public net::TcpServer
{
public:
	BaseServer(net::IOEventLoop* loop,net::SocketAddr& addr);

	virtual void connectCallback(boost::shared_ptr<net::TcpConnect> tcpConnect);
	virtual void messageCallback(boost::shared_ptr<net::TcpConnect>, net::Buffer&);
	virtual void writeCompletCallback(boost::shared_ptr<net::TcpConnect> tcpConnect) ;
	virtual void connectCloseCallback( boost::shared_ptr<net::TcpConnect>);
};

#endif