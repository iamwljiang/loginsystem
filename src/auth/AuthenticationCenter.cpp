#include "AuthenticationCenter.h"
/*boost::function <CTcpConnection* (boost::asio::io_service&,
	CConnectionManager& )> AllocConnectionFunction;

AllocConnectionFunction = &CAuthenticationCenter::AllocConnection;
*/

CTcpConnection* CAuthenticationCenter::AllocConnection(boost::asio::io_service& io_service,CConnectionManager& manager)
{
	return new CACConnection(io_service,manager,*this,_request_parser);
}

void CAuthenticationCenter::PostErrorMessage(boost::shared_ptr<CTcpConnection> conn,
		boost::asio::io_service::strand& strand,
		const std::string& errmsg)
{
	strand.post(boost::bind(
		&CAuthenticationCenter::ReplyErrorMessage,
		this,conn,errmsg
		));
}

void CAuthenticationCenter::ReplyErrorMessage(boost::shared_ptr<CTcpConnection> conn,
	const std::string& errmsg)
{
	int len = errmsg.size();
	boost::array<char,len+4> buffer;
	*((short*)buffer.data()) = htons(AC_REPLY_ERROR);
	*((short*)(buffer.data()+2)) = htons(len+4);
	memcpy(buffer.data()+4,errmsg.c_str(),len);
	conn->ReplySyncMessage(buffer.data(),len+4);
}


void CAuthenticationCenter::PostRandomTGT(boost::shared_ptr<CTcpConnection> conn,
		boost::asio::io_service::strand& strand,
		UserInfo &unode)
{
	strand.post(boost::bind(&CAuthenticationCenter::ProcessTGTMessage,this,conn,unode));
}

void CAuthenticationCenter::ProcessTGTMessage(boost::shared_ptr<CTcpConnection> conn,UserInfo &unode)
{
	unsigned int rand_value ;
	rand_r(&rand_value);
	char rand_str[20];
	sprintf(rand_str,"%d",rand_value);
	int  key_len = strlen(rand_str);

	ReplySecretWithUserMasterKey(rand_str,key_len,conn,UserInfo& unode);
	ReplySecretWithACMasterKey(rand_str,key_len,conn,UserInfo& unode);
}

void CAuthenticationCenter::ReplySecretWithUserMasterKey(const char* rand_key,int key_len,
	boost::shared_ptr<CTcpConnection> conn,UserInfo& unode)
{
	//AC_REPLY_SAC
	char *secret = _crypt.DESEncrypt(rand_str,key_len,unode.password.c_str());

	int content_len = strlen(secret);
	boost::array<char,content_len+4> sac_buffer;
	*((short*)sac_buffer.data()) = htons(AC_REPLY_SAC);
	*((short*)(sac_buffer.data()+2)) = htons(content_len+4);
	memcpy(sac_buffer.data()+4,secret_key,content_len);
	conn->PostAsyncMessage(sac_buffer.data(),content_len+4);
}

void CAuthenticationCenter::ReplySecretWithACMasterKey(const char* rand_key,int key_len,
	boost::shared_ptr<CTcpConnection> conn,UserInfo& unode)
{
	//AC_REPLY_TGT
	std::ostringstream oss;
	oss << unode.name << "|" 
		<< time(NULL) << "|"
		<< rand_key;

	char *secret  = _crypt.DESEncrypt(oss.str().c_str(),oss.str().size(),_auth_key);
	int content_len = strlen(secret);
	boost::array<char,content_len+4> auth_buffer;
	*((short*)auth_buffer.data()) = htons(AC_REPLY_TGT);
	*((short*)(auth_buffer.data()+2)) = htons(content_len+4);
	memcpy(auth_buffer.data()+4,secret,content_len);
	conn->PostAsyncMessage(auth_buffer.data(),content_len+4);
}

void CAuthenticationCenter::PostRandomTicket(boost::shared_ptr<CTcpConnection> conn,
		boost::asio::io_service::strand& strand,
		const std::string& secret_key,std::vector<std::string> command_vec)
{
	strand.post(boost::bind(&CAuthenticationCenter::ProcessTicketMessage,this,
		conn,secret_key,command_vec));
}


void CAuthenticationCenter::ProcessTicketMessage(boost::shared_ptr<CTcpConnection> conn,
		const std::string& secret_key,std::vector<std::string> command_vec)
{
	unsigned int rand_value;
	rand_r(&rand_value);
	char client_server_key[20] = "";
	sprintf(client_server_key,"%d",rand_value);
	
	std::ostringstream oss;
	oss << client_server_key << "|" << command_vec[0];
	//使用先前的密钥加密server和client之间给client的密文
	ReplyTicketWithSecretKey(oss.str().c_str(),oss.str().size(),secret_key.c_str(),conn);
	
	oss.str("");
	oss << client_server_key << "|" << command_vec[0];
	//使用server的密钥加密server和client之间给server的密文
	ReplyTicketWithServerKey(oss.str().c_str(),oss.str().size(),server_address,conn);
}


void CAuthenticationCenter::ReplyTicketWithSecretKey(const char* packet,int len,
	const char* secret_key,boost::shared_ptr<CTcpConnection> conn)
{
	char* secret = _crypt.DESEncrypt(packet,len,secret_key);
	int   content_len = strlen(secret);
	boost::array<char,content_len+4> client_ticket;
	*((short*)client_ticket.data())     = htons(AC_REPLY_SSC);
	*((short*)(client_ticket.data()+2)) = htons(content_len + 4);
	memcpy(client_ticket.data()+4,secret,content_len);
	conn->PostAsyncMessage(client_ticket.data(),content_len+4);
}

void CAuthenticationCenter::ReplyTicketWithServerKey(const char* packet,int len,
	const char* server_address,boost::shared_ptr<CTcpConnection> conn)
{
	std::string server_key = GetServerKey(server_addres);
	char* secret = _crypt.DESEncrypt(packet,len,server_key.c_str());
	int   content_len = strlen(secret);
	boost::array<char,content_len+4> client_ticket;
	*((short*)client_ticket.data())     = htons(AC_REPLY_SSC);
	*((short*)(client_ticket.data()+2)) = htons(content_len + 4);
	memcpy(client_ticket.data()+4,secret,content_len);
	conn->PostAsyncMessage(client_ticket.data(),content_len+4);
}

