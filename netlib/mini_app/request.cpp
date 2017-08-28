
#include "request.h"

using namespace app;

Sender::Sender():pb(), hpk(pb), pk(pb, HEADER_SIZE){

}

Sender::Sender(const Sender &s): pb(),  hpk(pb), pk(pb, HEADER_SIZE){
	length = s.length;
	uri = s.uri;
	resCode = s.resCode;
	pk.push(s.pk.data(), s.pk.size());
}

Sender::Sender(URI_TYPE u, const sox::Marshallable &m) :
pb(), hpk(pb), pk(pb, HEADER_SIZE){
	marshall(u, m);
}


void Sender::endPack() {
	hpk.replace_uint32(0, length);

	hpk.replace_uint32(4, uri);
	hpk.replace_uint16(8, resCode);

	hpk.replace_uint32(0, headerSize() + bodySize());
}

const char *Sender::body() {
	return pk.data();
}

uint32_t Sender::bodySize() {
	return pk.size();
}

const char *Sender::header(){
	return hpk.data();
}

uint32_t Sender::headerSize(){
	return HEADER_SIZE;
}

void Sender::clear(){
	// be care paket offset
	pb.resize(0 + HEADER_SIZE);
	resCode = RES_SUCCESS;
}

void Sender::marshall(const sox::Marshallable &m){
	pk << m;
}

void Sender::marshall(URI_TYPE u, const sox::Marshallable &m){
	setUri(u);
	m.marshal(pk);
}

void Sender::marshall(const char *data, size_t sz){
	pk.push(data, sz);
}

Sender & Sender::operator= (const Sender& sender){
	this->length = sender.length;
	this->uri = sender.uri;
	this->resCode = sender.resCode;
//	pb.resize(0);
	pb.resize(HEADER_SIZE);
	
	pk.push(sender.pk.data(), sender.pk.size());
	return *this;
}



Request::Request(const char *data, uint32_t sz) :
	up(data, sz), od(data), os(sz),  handler(NULL), cmd(NULL), connectType(0){
}

Request::~Request() {
	if (handler && cmd) {
		handler->destroyForm(cmd);
	}
}

void Request::head(){
	length = up.pop_uint32();

	uri = up.pop_uint32();
	resCode = up.pop_uint16();

}

bool Request::setFormHandler(IFormHandle *h) {
	if (h) {
		if(getResCode() == RES_SUCCESS){
			handler = h;
			cmd = h->handlePacket(up);
		}
		return true;
	}else{
		return false;
	}
}

void Request::forceHandler(IFormHandle *h){
	if (h) {
		handler = h;
		cmd = h->handlePacket(up);
	}else{
		handler = NULL;
	}
}

void Request::leftPack(std::string &out){
	out.assign(up.data(), up.size());
}

uint32_t Request::peeklen(const void * d) {
	uint32_t i = *((uint32_t*)d);
	return XHTONL(i);
}

std::string Request::getKey(){
	return key;
}

void Request::setKey(const std::string &k){
	key = k;
}


bool Request::ifSuccess(uint32_t res)
{
	return res == RES_SUCCESS;
}


void Request::packOrgin(sox::Pack &pk) const{
	pk.push(od, os);
}
