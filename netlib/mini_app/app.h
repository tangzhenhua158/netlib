#include "baseserver.h"
#include <iostream>
#include <common/Log.h>
#include <net/IOEventLoop.h>
#include <net/IOEventLoopThread.h>

using namespace std;
using namespace net;

//int main()
//{
//	//������ѭ���¼��ַ���
//	IOEventLoop loop;
//
//	//������ַ�Ͷ˿�
//	SocketAddr addr(true,10002);
//
//	//����������
//	EchoServer server(&loop,addr);
//
//	IOEventLoopThread bussThread;
//	bussThread.start();
//
//	//�����̳߳ش�С
//	server.setThreadPoolSize(3);
//
//	//��������ʼ����
//	server.start();
//
//	cout<<"run"<<endl;
//	//�¼�ѭ����������
//	loop.run();
//}

template<
typename SERVER,
typename SERVER_CONFIG,
typename APP_CTX
>
struct MiniApp_Template
{
	string m_strConfigName;
	SERVER	__server;
	SERVER_CONFIG __config;
	APP_CTX __appContext;

	MiniApp_Template(const std::string & serverName):
	m_strConfigName(name),
	__server(),
	__config(m_strConfigName),
	__appContext()
	{

	}

	~MiniApp_Template()
	{

	}

	virtual void init()
	{

	}

	virtual void run()
	{

	}
};

typedef
<

>