#ifndef LS_REQUESTPARSER_H_
#define LS_REQUESTPARSER_H_

class CRequestParser{
public:
	CRequestParser();
	
	virtual ~CRequestParser();

	/*
	Parse packet and call process method
	@return = 0 finish, < 0 failed
	@param 'packet' is content of packet
	@param 'len' is packet length
	@param 'type' is what's type of packet,see data_protocol.h 
	*/

	virtual int ParseClientToACPacket(const char *packet,short len,short type);

	virtual int ParseAcTOClientPacket(const char *packet,short len,short type);

	virtual int ParseClientTOServerPacket(const char *packet,short len,short type);
};

#endif //LS_REQUESTPARSER_H_
