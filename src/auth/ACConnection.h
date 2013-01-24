//重新实现TcpConnection中的接口

#ifndef LS_ACCONNECTION_H_
#define LS_ACCONNECTION_H_


#include <boost/asio.hpp>

#include "ACParser.h"
#include "AuthenticationCenter.h"
#include "common/TcpConnection.h"
#include "common/ConnectionManager.h"


class CACConnection:public CTcpConnection{
public:
	CACConnection(boost::asio::io_service& io_service,CConnectionManager& manager,
		CAuthenticationCenter& auth_center);
	
	~CACConnection();

protected:
	virtual int ProcessPacket(const char* packet,short len,short type);

private:
	enum {
		NONE    =1,
		AUTH_KEY=2,
		REQUEST =3,
	};
private:
	CAuthenticationCenter& 	_auth_center;

	CRequestParser			_request_parser;

	int 					_state;	

	int						_count_verify;
	
	//use keep server side key ,it recv from client because server side not save it
	//boost::shared_array<char> _secret_key_buffer;

	std::string				  _secret_key;
};

#endif //LS_ACCONNECTION_H_
