#ifndef __app_hander_h__
#define __app_hander_h__

#include "Buffer.h"
#include <endian.h>
#include "/usr/include/endian.h"

namespace app
{

	class App_Hander
	{
	public:
		uint32_t proc_stream(shared_ptr<TcpConnect> tcpConnect, Buffer& buffer);
		//uint32_t 
	};
}

#endif