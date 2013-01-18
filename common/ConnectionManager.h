#ifndef LS_CONNECTION_MANAGER_H_
#define LS_CONNECTION_MANAGER_H_

#include <set>

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "TcpConnection.h"

//Very simple manager connection
class CConnectionManager{
public:
	CConnectionManager();

	~CConnectionManager();

	void Start(boost::shared_ptr<CTcpConnection> new_conn);

	void Stop (boost::shared_ptr<CTcpConnection> conn);

	void StopAll();

private:
	std::set<boost::weak_ptr<CTcpConnection> > connections;
};

#endif //LS_CONNECTION_MANAGER_H_