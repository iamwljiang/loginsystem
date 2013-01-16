#ifndef LS_TCPCONNECTION_H_
#define LS_TCPCONNECTION_H_

#include <iostream>
#include <string>

#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <boost/enable_shared_from_this.hpp>

//every service need inherit this class and rewrite virtual method
class CTcpConnection 
	:public boost::enable_shared_from_this<CTcpConnection> ,
	private boost::noncopyable{
public:
	CTcpConnection(boost::asio::io_service& io_service,CConnectionManager& manager);
	
	//close
	virtual ~CTcpConnection();

	//start read data from client when conntected
	void Start();

	//shutdown
	void Stop();

	boost::asio::ip::tcp::socket& Socket();	

protected:
	virtual void handle_read (boost::system::error& e,std::size_t bytes_transferred);

	virtual void handle_write(boost::system::error& e,std::size_t bytes_transferred);

protected:
	boost::asio::io_service 		_io_service;

	boost::asio::io_service::strand _strand;
	
	boost::asio::ip::tcp::socket    _socket;

	CConnectionManager& 			_connection_manager;	
};

#endif //LS_TCPCONNECTION_H_
