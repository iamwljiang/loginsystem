#ifndef LS_DATATYPE_H_	
#define LS_DATATYPE_H_

#include <boost/asio.hpp>

class CTcpConnection;
class CConnectionManager;

typedef CTcpConnection* (*ALLOC_CONNECTION)(boost::asio::io_service&,CConnectionManager&);

#endif