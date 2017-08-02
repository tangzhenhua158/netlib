#ifndef AGILNET_NET_HTTPSERVER
#define AGILNET_NET_HTTPSERVER

#include <net/TcpServer.h>
#include <net/http/HttpResponse.h>
#include <net/http/HttpRequest.h>


namespace net
{

class HttpServer : public TcpServer
{
public:
    HttpServer(IOEventLoop* loop,SocketAddr& addr);
    //HttpServer类总是被继承，虚析构相对安全。
    virtual ~HttpServer();
private:
    virtual void connectCallback(boost::shared_ptr<TcpConnect> tcpConnect);
    virtual void messageCallback(boost::shared_ptr<TcpConnect>, Buffer&);
    virtual void writeCompletCallback(boost::shared_ptr<TcpConnect> tcpConnect) ;
    virtual void connectCloseCallback(boost::shared_ptr<TcpConnect>);

    virtual void httpCallback(const HttpRequest&, HttpResponse*);

    void onRequest( boost::shared_ptr<TcpConnect> conn, const HttpRequest& req);
};

}
#endif // AGILNET_NET_HTTPSERVER
