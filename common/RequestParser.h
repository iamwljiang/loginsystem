#ifndef LS_REQUESTPARSER_H_
#define LS_REQUESTPARSER_H_

#include <boost/shared_array.hpp>

class CRequestParser{
protected:
	CRequestParser();
	
	virtual ~CRequestParser();

	/*
	Parse packet and call process method
	@return = 0 finish, < 0 failed
	@param 'packet' is content of packet
	@param 'len' is packet length
	@param 'type' is what's type of packet,see data_protocol.h 
	@param 'plaintext' is content of parsed packet ,it will automatic free 
	*/

	//auth service rewrite this method
	virtual int ParseClientToACPacket(const char *packet,short len,short type,boost::shared_array<char>* plaintext )
	{
		return 0;
	}

	//client service rewrite this method
	virtual int ParseAcTOClientPacket(const char *packet,short len,short type,boost::shared_array<char>* plaintext)
	{
		return 0;
	}

	//server service rewrite this method
	virtual int ParseClientTOServerPacket(const char *packet,short len,short type,boost::shared_array<char>* plaintext)
	{
		return 0;
	}
};

#endif //LS_REQUESTPARSER_H_
