#ifndef LS_USERTABLE_H_
#define LS_USERTABLE_H_

class CUserTable{
public:
	CUserTable(const std::string& tablename);

	int LoadToMemory();

	/*
	*return =0, in the table have a couple of 'username' and 'passwd'
	*@param 'username' 
	*@param 'passwd' user's password
	*/
	int CheckAccount(const std::string& username,const std::string& passwd);

	/*
	*return  <0,user not exist ; =0 user passwd save 'out_passwd'
	*@param 'username' is register username
	*@param 'out_passwd' is save user password
	*/
	int GetPassword(const std::string& username,std::string& out_passwd);

private:
	std::string _table_name;
	std::map<std::string,std::string> _register_user_map;
};
#endif //LS_USERTABLE_H_