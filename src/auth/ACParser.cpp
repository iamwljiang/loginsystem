#include "CACParser.h"
#include <string.h>
#include <boost/algorithm/string.hpp>

CACParser::CACParser(CAuthenticationCenter& auth_center)
	:_auth_center(auth_center)
{

}

CACParser::~CACParser()
{

}

bool CACParser::VerifyUserISValid(const char* packet,short len,UserInfo& user_node)
{
	char *psecret = strchr(packet,'|');
	std::string username;
	username.assign(packet,psecret-packet);
	
	char *psig    = strrchr(packet,'|');	
	int  sig_len  = strlen(psig+1);
	int secret_len = psig - psecret - 1;
	
	psecret++;
	psig++;

	std::string	md5_passwd;

	int ret = 0;
	ret = _auth_center.GetUserPassword(username,md5_passwd);
	if(ret < 0){
		return false;
	}
	
	boost::tuple<std::string,std::string> two = HalfString(md5_passwd);
	char sig[33] = "";
	Md5FromString(psecret,secret_len,
		two<1>.get().c_str(),two<1>.get().size(),
		sig);

	//signature not match
	if(strcmp(psig,sig) != 0){
		return false;
	}

	//TODO:maybe free by self
	char *orgi = _crypt.DESDecrypt(psecret,secret_len,two<0>.get().c_str());
	if(orgi == NULL){
		return false;
	}

	char * pname = strchr(orgi,"|");
	user_node.name.assign(orgi,pname - orgi);
	user_node.login_server_address.assign(pname+1);

	if(username != user_node.name){
		return false;
	}

	std::string temp = GetServerKey(user_node.login_server_address);
	if(temp.empty()){
		return false;
	}

	user_node.password = md5_passwd;

	return true;
}

bool CACParser::ParserTGTKey(const char *packet,short len,std::vector<std::string>& out)
{
	std::string auth_key = _auth_center.GetAuthCenterKey();
	char *orgi = _crypt.DESDecrypt(packet,len,auth_key);
	if(orgi == NULL){
		return false;
	}

	//orig format:username|starttime|tgtkey
	boost::split(orgi,out,boost::is_any_of("|"));
	if(out.size() < 3) return false;
	return true;
}

bool ParserRequest(const char* packet,short len, std::string& key,std::vector<std::string>& out)
{
	char *orgi = _crypt.DESDecrypt(packet,len,key);
	if(orgi == NULL){
		return false;
	}
	
	//orgi format: username|login_server_address(domain or ip)|etc..
	boost::split(orgi,out,boost::is_any_of("|"));
	if(out.empty()) return false;

	return true;
}

boost::tuple<std::string,std::string>	HalfString(const std::string& str)
{  
	int pos = str.size() / 2;
	return boost::make_tuple(str.substr(0,pos),str.substr(pos));
}