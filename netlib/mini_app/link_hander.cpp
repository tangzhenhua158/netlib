
#include <link_hander.h>

using namespace net;
using namespace app;

uint32_t App_Hander::proc_stream(shared_ptr<TcpConnect> tcpConnect, Buffer& buffer)
{
	const void* data = buffer.readIndexPtr();
	int32_t be32 = *static_cast<const int32_t*>(data);
	const int32_t len = be32toh(be32);

	Request request(buffer.readIndexPtr(),x.readableBytes());
	request.head();
	int len = request.getLength()

	if (len < 0 || len > 65536 )
	{
		LogOutput(error) << "Invalid length " << len;
		conn->shutdown();  // FIXME: disable reading
		break;
	}else if (buffer.readableBytes() >= len + HEADER_SIZE)
	{
		buffer.clearReadIndex(len + HEADER_SIZE);
		//requset.setFormHandler();
	}
	else if (buffer.readableBytes() < len + HEADER_SIZE)
	{
		//need more data
		return 1;
	}

	
}