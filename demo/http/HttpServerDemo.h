#ifndef AGILNET_DOME_HTTPSERVERDOME
#define AGILNET_DOME_HTTPSERVERDOME

#include <net/http/HttpServer.h>



class HttpServerDemo : public net::HttpServer
{
public:
    HttpServerDemo(net::IOEventLoop* loop,net::SocketAddr& addr);

    virtual void httpCallback(const net::HttpRequest&, net::HttpResponse*);

};

#endif
