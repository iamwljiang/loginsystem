#ifndef LS_ACPARSER_H_
#define LS_ACPARSER_H_

#include <string>
#include <vector>

#include "common/CEncryptAndDecrypt.h"
#include "AuthenticationCenter.h"

#include <boost/tuple.hpp>

class CACParser:public CRequestParser{
public:
	CACParser(CAuthenticationCenter& auth_center);
	
	~CACParser();

	bool ParserRequest(const char* packet,short len, std::string& key,std::vector<std::string>& out);

	bool ParserTGTKey(const char* packet,short len,std::vector<std::string>& out);

	bool VerifyUserISValid(const char* packet,short len,UserInfo& user_node);

	boost::tuple<std::string,std::string>	HalfString(const std::string& str);
private:

private:
	CEncryptAndDecrypt	   	_crypt;
	CAuthenticationCenter& 	_auth_center;
};

#endif //LS_ACPARSER_H_