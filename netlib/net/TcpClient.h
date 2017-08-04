#ifndef __TCP_CLIENT_H__
#define __TCP_CLIENT_H__

namespace net
{
	class TcpClient
	{
	public:
		TcpClient(SocketAddr& addr);
	private:
		TcpConnect * m_pTcpConnent;
	};
}

#endif