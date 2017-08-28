#ifndef __APP_REQUEST_H__
#define __APP_REQUEST_H__

#include "res_code.h"
#include "packet.h"

namespace app
{
	struct IFormHandle {
		virtual ~IFormHandle(){}
		virtual void *handlePacket(sox::Unpack &up) = 0;
		virtual void destroyForm(void *form) = 0;
	};

	template<class T> class FormHandleT : public IFormHandle {
	public:
		virtual void *handlePacket(sox::Unpack &up) {
			T *obj = new T();
			up >> *obj;
			return obj;
		}
		virtual void destroyForm(void *cmd) {
			T *obj = (T *)cmd;
			if(obj)
				delete obj;
		}

	};

	#define HEADER_SIZE 14
	typedef uint64_t URI_TYPE;
	struct Header
	{
		uint32_t length;
		URI_TYPE uri;
		uint16_t resCode;

		Header():length(0),uri(0),resCode(E_CODE_OK)
		{

		}
		URI_TYPE getUri()
		{
			return uri;
		}

		void setUri(URI_TYPE u)
		{
			uri = u;
		}

		uint16_t getResCode() const {
			return resCode;
		}

		void setResCode(uint16_t res){
			resCode = res;
		}

		bool isSuccess() const{
			return resCode == E_CODE_OK;
		}

		uint32_t getLength(){
			return length;
		}
	};


	struct Sender: public Header{
	protected:
		PackBuffer pb;
		Pack hpk;
		Pack pk;
	public:
		Sender();

		Sender(URI_TYPE, const sox::Marshallable &);
		Sender & operator = (const Sender& sender);
		Sender(const Sender &);

		const char *body();

		size_t bodySize();

		const char *header();

		size_t headerSize();

		void marshall(const sox::Marshallable &);

		void marshall(URI_TYPE, const sox::Marshallable &);

		void marshall(const char *, size_t sz);

		void endPack();

		void clear();
	};


	class Request: public Header{
		Unpack up;
		const char *od; //origin data
		uint32_t os;	//origin size
	public:
		std::string key;	

		IFormHandle *handler;
		void *cmd;

		std::string to; 
	public:
		static uint32_t peeklen(const void * d);

		Request(const char *data, uint32_t sz);

		void head();

		virtual ~Request();

		bool setFormHandler(IFormHandle *h);

		void forceHandler(IFormHandle *h);


		void setKey(const std::string &u);

		std::string getKey();

		void *getCmd() const{
			return cmd;
		}

		sox::Unpack& getPackData(){ return up; };

		static bool ifSuccess(uint32_t);

		void packOrgin(sox::Pack &pk) const;

		void leftPack(std::string &out);
	};
}
#endif