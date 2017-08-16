#ifndef __TCP_CLIENT_H__
#define __TCP_CLIENT_H__

#include "TcpConnect.h"
#include "connect.h"
#include "IOEventLoop.h"
#include <string>
#include <boost/shared_ptr.hpp>

namespace net
{
	typedef boost::shared_ptr<Connector> ConnectorPtr;
	typedef boost::shared_ptr<TcpConnect> TcpConnectionPtr;
	class TcpClient
	{
	public:
		typedef boost::function<void()> EventCallback;
		TcpClient(IOEventLoop* loop,
				  const SocketAddr& addr,
				  const std::string& name);

		~TcpClient();

		void connect();
		void disconnect();
		void stop();

		virtual void connectCallback(boost::shared_ptr<TcpConnect> tcpConnect)=0;
		virtual void messageCallback(boost::shared_ptr<TcpConnect>, Buffer&)=0;
		virtual void writeCompletCallback(boost::shared_ptr<TcpConnect> tcpConnect) = 0;
		virtual void connectCloseCallback(boost::shared_ptr<TcpConnect>)=0;
		void seterrCallback(const EventCallback &cb);
		
		void write(void* data,uint32_t length);
		void newConnection(int sockfd);
		void removeConnection(const TcpConnectionPtr& conn);
		void connectCloseEvent(boost::shared_ptr<TcpConnect> connect);
	private:
		IOEventLoop* loop_;
		ConnectorPtr connector_;
		const std::string name_;
		bool connect_;
		uint32_t nextConnId_;
		boost::shared_ptr<TcpConnect> connection_;
		
		
	};
}

#endif