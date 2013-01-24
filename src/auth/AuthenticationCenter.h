//调度封装
#ifndef LS_AUTHENTICATION_CENTER_H_
#define LS_AUTHENTICATION_CENTER_H_

class CAuthenticationCenter{
public:
	CAuthenticationCenter();
	
	~CAuthenticationCenter();

	//bind boost function 
	CTcpConnection* AllocConnection(boost::asio::io_service&,CConnectionManager&);

	int Init();

	int Run();

	void PostErrorMessage(boost::shared_ptr<CTcpConnection> conn,
		boost::asio::io_service::strand& strand,
		const std::string& errmsg);

	void PostRandomTGT(boost::shared_ptr<CTcpConnection> conn,
		boost::asio::io_service::strand& strand,
		UserInfo &unode);

	/*
	*@param 'secret_key' save server side random key and other info
	*@param 'command_vec' save client request command
	*/
	void PostRandomTicket(boost::shared_ptr<CTcpConnection> conn,
		boost::asio::io_service::strand& strand,
		const std::string& secret_key,std::vector<std::string> command_vec);

	void ReplyErrorMessage(boost::shared_ptr<CTcpConnection> conn,
		const std::string& errmsg);

	//TGT
	void ProcessTGTMessage(boost::shared_ptr<CTcpConnection> conn,UserInfo &unode);

	void ReplySecretWithUserMasterKey(const char* rand_key,int key_len,
		boost::shared_ptr<CTcpConnection> conn,UserInfo& unode);

	void ReplySecretWithACMasterKey(const char* rand_key,int key_len,
		boost::shared_ptr<CTcpConnection> conn,UserInfo& unode);

	//Ticket
	void ProcessTicketMessage(boost::shared_ptr<CTcpConnection> conn,
		const std::string& secret_key,std::vector<std::string> command_vec);

	void ReplyTicketWithSecretKey(const char* packet,int len,const char* secret_key,boost::shared_ptr<CTcpConnection> conn);

	void ReplyTicketWithServerKey(const char* packet,int len,const char* server_address,boost::shared_ptr<CTcpConnection> conn);

	//user account and get interface keep reentrant
	inline int CheckUserAccount(const std::string& username,const std::string& passwd);

	inline int GetUserPassword (const std::string& username,std::string& out_passwd);

	inline std::string GetAuthCenterKey();

	inline std::string GetServerKey(const std::string& server_name);

private:
	int load_config(const std::string& config);

private:
	CUserTable		_utable;

	std::string 	_auth_key;

	std::map<std::string,std::string> 	_server_key_map;

	CEncryptAndDecrypt					_crypt;
};
#endif //LS_AUTHENTICATION_CENTER_H_
