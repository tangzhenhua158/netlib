#include "connect.h"
#include "common/Log.h"
//#include "SocketAddr.h"
#include "SocketOperation.h"

using namespace net;

const int Connector::kMaxRetryDelayMs = 30*1000;
const int Connector::kInitRetryDelayMs = 500;

Connector::Connector(IOEventLoop *loop, const SocketAddr& addr):
	loop_(loop),
	serverAddr_(addr),
	connect_(false),
	state_(kDisconnected),
	retryDelayMs_(kInitRetryDelayMs)
{
	
}

Connector::~Connector()
{

}

void Connector::start()
{
	connect_ = true;
	loop_->runInLoop(boost::bind(&Connector::startInLoop, this)); // FIXME: unsafe
}

void Connector::startInLoop()
{
	//loop_->inThisThread();
	assert(state_ == kDisconnected);
	if (connect_)
	{
		connect();
	}
	else
	{
		LogOutput(debug) << "do not connect";
	}
}

void Connector::stop()
{
	connect_ = false;
	loop_->runInLoop(boost::bind(&Connector::stopInLoop, this)); // FIXME: unsafe
	// FIXME: cancel timer
}

void Connector::stopInLoop()
{
	//loop_->inThisThread();
	if (state_ == kConnecting)
	{
		setState(kDisconnected);
		int sockfd = removeAndResetChannel();
		retry(sockfd);
	}
}

void Connector::connect()
{
	int sockfd = SocketOperation::createNonblockingSocket();
	int ret = SocketOperation::connect(sockfd, (struct sockaddr *)serverAddr_.getAddrPtr());
	int savedErrno = (ret == 0) ? 0 : errno;
	switch (savedErrno)
	{
	case 0:
	case EINPROGRESS:
	case EINTR:
	case EISCONN:
		connecting(sockfd);
		break;

	case EAGAIN:
	case EADDRINUSE:
	case EADDRNOTAVAIL:
	case ECONNREFUSED:
	case ENETUNREACH:
		retry(sockfd);
		break;

	case EACCES:
	case EPERM:
	case EAFNOSUPPORT:
	case EALREADY:
	case EBADF:
	case EFAULT:
	case ENOTSOCK:
		LogOutput(error)<< "connect error in Connector::startInLoop " << savedErrno;
		SocketOperation::close(sockfd);
		break;

	default:
		LogOutput(error)<< "Unexpected error in Connector::startInLoop " << savedErrno;
		SocketOperation::close(sockfd);
		// connectErrorCallback_();
		break;
	}
}

void Connector::restart()
{
	//loop_->inThisThread();
	setState(kDisconnected);
	retryDelayMs_ = kInitRetryDelayMs;
	connect_ = true;
	startInLoop();
}

void Connector::connecting(int sockfd)
{
	LogOutput(debug)<<"socket connected ";
	setState(kConnecting);
	assert(!event_);
	event_.reset(new IOEvent(loop_, sockfd));
	event_->setWriteFunc(
		boost::bind(&Connector::handleWrite, this)); // FIXME: unsafe
	event_->setErrorFunc(
		boost::bind(&Connector::handleError, this)); // FIXME: unsafe

	// event_->tie(shared_from_this()); is not working,
	// as event_ is not managed by shared_ptr
	loop_->addEvent(event_);
	event_->enableWriting(true);
}

int Connector::removeAndResetChannel()
{
	event_->disableAll();
	event_->removeFromLoop();
	int sockfd = event_->getFd();
	// Can't reset event_ here, because we are inside Channel::handleEvent
	loop_->runInLoop(boost::bind(&Connector::resetChannel, this)); // FIXME: unsafe
	return sockfd;
}

void Connector::resetChannel()
{
	event_.reset();
}

void Connector::handleWrite()
{
	LogOutput(info) << "Connector::handleWrite " << state_;
	loop_->removeEvent(event_->getFd());
	if (state_ == kConnecting)
	{
		int sockfd = removeAndResetChannel();
		int err = SocketOperation::getSocketError(sockfd);
		if (err)
		{
			LogOutput(warning) << "Connector::handleWrite - SO_ERROR = "
				<< err << " " << err;
			retry(sockfd);
		}
		else if (SocketOperation::isSelfConnect(sockfd))
		{
			LogOutput(warning) << "Connector::handleWrite - Self connect";
			retry(sockfd);
		}
		else
		{
			setState(kConnected);
			if (connect_)
			{
				newConnectionCallback_(sockfd);
			}
			else
			{
				SocketOperation::close(sockfd);
			}
		}
	}
	else
	{
		// what happened?
		assert(state_ == kDisconnected);
	}
}

void Connector::handleError()
{
	//LOG_ERROR << "Connector::handleError state=" << state_;
	if (m_errCallback)
	{
		m_errCallback();
	}

	if (state_ == kConnecting)
	{
		int sockfd = removeAndResetChannel();
		int err = SocketOperation::getSocketError(sockfd);
		LogOutput(error)<< "SO_ERROR = " << err << " " ;
		retry(sockfd);
	}
}

void Connector::retry(int sockfd)
{
	SocketOperation::close(sockfd);
	setState(kDisconnected);
	if (connect_)
	{
		LogOutput(debug)<< "Connector::retry - Retry connecting to " << serverAddr_.toString()
			<< " in " << retryDelayMs_ << " milliseconds. ";
		loop_->runOniceAfter(
			boost::bind(&Connector::startInLoop, this),retryDelayMs_/1000.0);
		retryDelayMs_ = std::min(retryDelayMs_ * 2, kMaxRetryDelayMs);
	}
	else
	{
		LogOutput(debug) << "do not connect";
	}
}