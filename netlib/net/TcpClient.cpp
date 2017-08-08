#include "TcpClient.h"
using namespace net;
using namespace std;

TcpClient::TcpClient(IOEventLoop* loop,
	const SocketAddr& serverAddr,
	const string& name):
	loop_(loop),
	connector_(new Connector(loop, serverAddr)),
	name_(name),
	connect_(true),
	nextConnId_(1)
{
	connector_->setNewConnectionCallback(
		boost::bind(&TcpClient::newConnection, this, _1));
}

TcpClient::~TcpClient()
{
	//LOG_INFO << "TcpClient::~TcpClient[" << name_
	//	<< "] - connector " << get_pointer(connector_);
	
	connector_->stop();
}

void TcpClient::connect()
{
	// FIXME: check state
	//LOG_INFO << "TcpClient::connect[" << name_ << "] - connecting to "
	//	<< connector_->serverAddress().toIpPort();
	connect_ = true;
	connector_->start();
}

void TcpClient::disconnect()
{
	connect_ = false;

	{
		if (connection_)
		{
			connection_->shutdownWrite();
		}
	}
}

void TcpClient::stop()
{
	connect_ = false;
	connector_->stop();
}

void TcpClient::newConnection(int sockfd)
{
	SocketAddr peerAddr(SocketOperation::getPeerAddr(sockfd));
	char buf[32];
	snprintf(buf, sizeof buf, ":%s#%d", peerAddr.toString().c_str(), nextConnId_);
	++nextConnId_;
	string connName = name_ + buf;

	SocketAddr localAddr(SocketOperation::getLocalAddr(sockfd));
	// FIXME poll with zero timeout to double confirm the new connection
	// FIXME use make_shared if necessary

	boost::shared_ptr<TcpConnect> tcpConnect(new TcpConnect(loop_,localAddr.getAddr(),sockfd));
	tcpConnect->setMessageCallback(boost::bind(&TcpClient::messageCallback,this,_1,_2));
	tcpConnect->setCloseCallback(boost::bind(&TcpClient::connectCloseEvent,this,_1));
	tcpConnect->connectedHandle();

	{
		connection_ = tcpConnect;
	}
}

void TcpClient::connectCloseEvent(boost::shared_ptr<TcpConnect> connect)
{
	connectCloseCallback(connect);
	removeConnection(connect);
}

void TcpClient::removeConnection(const TcpConnectionPtr& conn)
{
	//assert(loop_ == conn->getLoop());

	{
		assert(connection_ == conn);
		connection_.reset();
	}
}

void TcpClient::seterrCallback(const EventCallback &cb)
{
	if(connector_.get())
	{
		connector_->seterrCallback(cb);
	} 
}

void TcpClient::write(void* data,uint32_t length)
{
	if(connection_)
		connection_->writeInLoop(data,length);
}