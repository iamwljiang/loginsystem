#ifndef LS_DATATYPE_H_	
#define LS_DATATYPE_H_

#include <boost/asio.hpp>

class CTcpConnection;
class CConnectionManager;

typedef CTcpConnection* (*ALLOC_CONNECTION)(boost::asio::io_service&,CConnectionManager&);

typedef struct _tag_UserVerifyInfo{
	std::string name;
	std::string login_server_address;
	std::string password;
}UserInfo;

#endif