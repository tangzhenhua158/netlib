#include "TcpClient.h"
#include <iostream>
#include <common/Log.h>
#include <net/IOEventLoop.h>
#include <net/IOEventLoopThread.h>

using namespace std;
using namespace net;

class EchoClient:public TcpClient
{
public:
	EchoClient(IOEventLoop* loop,
		const SocketAddr& addr,
		const std::string& name):
	 TcpClient(loop,addr,name)
	 {

	 }

	~EchoClient(){};

	virtual void connectCallback(boost::shared_ptr<TcpConnect> tcpConnect)
	{
		LogOutput(debug)<< "new tcp conn";
		cout<<"new tcp CONN"<<endl;
		tcpConnect->write("hello!");
	};
	virtual void messageCallback(boost::shared_ptr<TcpConnect>tcpConnect, Buffer&buf)
	{
		string data;
		buf.readAllAsString(data);
		cout<<data<<endl;
		tcpConnect->write(data);
	}
	virtual void writeCompletCallback(boost::shared_ptr<TcpConnect> tcpConnect) 
	{
		cout<<"thread id:"<<boost::this_thread::get_id()<<endl;
		string addr = tcpConnect->getAddr().toString();
		cout<<addr<<":"<<"write complet "<<endl;
		LogOutput(info)<<addr<<":"<<"write complet "<<endl;
	}
	virtual void connectCloseCallback(boost::shared_ptr<TcpConnect>)
	{
		cout<<"tcp CONN CLOSE"<<endl;
	};
};


int main()
{
    //创建主循环事件分发器
   IOEventLoop loop;

   SocketAddr addr("127.0.0.1",10002);
   EchoClient client(&loop,addr,"echo");
   client.connect();

    //事件循环机制运行
   loop.run();
}
