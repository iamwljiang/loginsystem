#include <vector>
#include <stdlib.h>
#include "ACConnection.h"

CACConnection::CACConnection(boost::asio::io_service& io_service,CConnectionManager& manager,
		CAuthenticationCenter& auth_center,CRequestParser& request_parser)
		:CTcpConnection(io_service,manager),_auth_center(auth_center)
{
	_state = NONE;
	_secret_key = "";
}

CACConnection::~CACConnection()
{

}


int CACConnection::ProcessPacket(const char* packet,short len,short type)
{
	if(!is_client_type(type) || _count_verify >= AC_TRY_MAX_VERIFY){
		_auth_center.PostErrorMessage(boost::shared_from_this(),
			_strand,"Packet type is invalid or verify times to much");
		return -1;
	}
	int ret = 0;
	bool bret = true;

	std::vector<string> command_vec;
	UserInfo user_node;

	if(type == CLIENT_KEY_APPLY_TGT){	
		ret = _request_parser.VerifyUserISValid(packet,len,user_node);
	}else if(type == CLIENT_AC_TGT){
		ret = _request_parser.ParserTGTKey(packet,len,command_vec);
	}else if (type == CLIENT_SAC_APPLY_TICKET){
		ret = _request_parser.ParserRequest(packet,len,_secret_key,command_vec);
	}

	if(ret != 0){
		_auth_center.PostErrorMessage(boost::shared_from_this(),
			_strand,"Packet parser error");
		return -2;
	}
	
	if(state == NONE){
		_state = REQUEST;
	}

	switch(type){
		case CLIENT_KEY_APPLY_TGT:
			_auth_center.PostRandomTGT(boost::shared_from_this(),
				_strand,user_node);
			_count_verify += 1;
			_state = NONE;
			break;
		case CLIENT_AC_TGT:
			_count_verify += 1;
			time_t now = time(NULL);
			if(now - atol(command_vec[1]) > 5 * 60){
				_auth_center.PostErrorMessage(boost::shared_from_this(),
					_strand,"TGT timeout");
				ret = -3
			}else{
				_secret_key = command_vec[1];
				_state = AUTH_KEY;
			}
			break;
		case CLIENT_SAC_APPLY_TICKET:
			if(_state != AUTH_KEY){
				_auth_center.PostErrorMessage(boost::shared_from_this(),
					_strand,"Packet order error");
				ret = -3
			}else{
				_auth_center.PostRandomTicket(boost::shared_from_this,
					_strand,_secret_key,command_vec);
				_state = NONE;
			}
			break;
		default:
			_auth_center.PostErrorMessage(boost::shared_from_this(),
				_strand,"Unknown error");
			ret = -4;
			break;
	}
	
	return ret;
}