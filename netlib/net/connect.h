#ifndef __TCP_CONNECT_H__
#define __TCP_CONNECT_H__

#include "IOEventLoop.h"
#include "SocketAddr.h"
#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>

namespace net
{
	class Connector
	{
	public:
		typedef boost::function<void()> EventCallback;
		typedef boost::function<void (int sockfd)> NewConnectionCallback;

		Connector(IOEventLoop *loop, const SocketAddr& addr);
		~Connector();

		void start();
		void restart();
		void stop();


		void setNewConnectionCallback(const NewConnectionCallback& cb)
		{ newConnectionCallback_ = cb; }

		void seterrCallback(const EventCallback &cb)
		{
			m_errCallback = cb;
		}

		const SocketAddr& serverAddress() const { return serverAddr_; }

	private:
		enum States { kDisconnected, kConnecting, kConnected };
		static const int kMaxRetryDelayMs ;
		static const int kInitRetryDelayMs ;

	private:
		void setState(States s) { state_ = s; }
		void startInLoop();
		void stopInLoop();
		void connect();
		void connecting(int sockfd);
		void handleWrite();
		void handleError();
		void retry(int sockfd);
		int removeAndResetChannel();
		void resetChannel();

	private:
		IOEventLoop* loop_;
		boost::shared_ptr<IOEvent> event_;
		NewConnectionCallback newConnectionCallback_;
		EventCallback   m_errCallback;
		SocketAddr serverAddr_;
		bool connect_; // atomic
		States state_;  // FIXME: use atomic variable
		int retryDelayMs_;
	};
}

#endif