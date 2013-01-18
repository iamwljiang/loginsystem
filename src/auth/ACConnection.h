//重新实现TcpConnection中的接口

#ifndef LS_ACCONNECTION_H_
#define LS_ACCONNECTION_H_

#include "common/TcpConnection.h"

class CACConnection:public CTcpConnection{
public:
	CACConnection(boost::asio::io_service& io_service,CConnectionManager& manager);
	
	~CACConnection();

protected:
	virtual int ProcessPacket(const char* packet,short type,short len);

private:
	int check_client_info();

	int creat_client_TGT();

		
};

#endif //LS_ACCONNECTION_H_
