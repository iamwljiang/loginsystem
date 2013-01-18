#include "ConnectionManager.h"

CConnectionManager::CConnectionManager()
{
	connections.clear();
}

CConnectionManager::~CConnectionManager()
{

}

void CConnectionManager::Start(boost::shared_ptr<CTcpConnection> new_conn)
{
	boost::weak_ptr<CTcpConnection> weak_conn(new_conn);
	if(!weak_conn->expired()){
		new_conn->Start();
		connections.insert(weak_conn);
	}
}

void CConnectionManager::Stop (boost::shared_ptr<CTcpConnection> conn)
{
	boost::weak_ptr<CtcpConnection> weak_conn(conn);
	connections.erase(weak_conn);
	if(!weak_conn->expired()){
		conn->Stop();
	}
}

void CConnectionManager::StopAll()
{
	std::set<boost::weak_ptr<CTcpConnection> >::iterator conn_iter,conn_end;
	conn_end = connections.end();
	for(conn_iter = connections.begin(); conn_iter != conn_end; ++conn_iter){
		if(!(*conn_iter)->expired()){
			boost::shared_ptr<CTcpConnection> temp_conn(*conn_iter);
			temp_conn->Stop();
		}
	}

	connections.clear();
}