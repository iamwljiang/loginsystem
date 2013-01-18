/*
date:2013-01-14
author:iamwljiang@gmail.com
description:
tcpserver base class
*/
#ifndef LS_TCPSERVER_H_
#define LS_TCPSERVER_H_

#include <vector>
#include <string>

#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "ConnectionManager.h"
#include "TcpConnection.h"
#include "data_type.h"

class CTcpServer
	:public boost::enable_shared_from_this<CTcpServer> ,
	private boost::noncopyable{
public:
	explicit CTcpServer(const char* ip,int port);
	
	~CTcpServer();

	//bind and listen
	int Init();

	//call start_accept and new threads
	int Run();

	//if set alloc func that use it to alloc new connection else use default
	void SetAllocMethod(ALLOC_CONNECTION alloc_connection_func);
private:
	
	//new connection and push to manager
	int start_accept();

	//check acceptor status
	void handle_accept(const boost::system::error& e);

	//start threads
	int start_thread();

	ALLOC_CONNECTION  _alloc_connection_func;

private:
	boost::asio::io_service 			_io_service;

	boost::asio::ip::tcp::acceptor 		_acceptor;

	boost::asio::io_service::strand 	_strand;

	boost::shared_ptr<CTcpConnection> 	_new_connection;

	std::vector<boost::shared_ptr<boost::thread> > _thread_pool;
	CConnectionManager		_connectoin_manager;

	std::string 			_ip;

	std::string 			_port;

	static 	int				THREAD_NUMBER;
};

#endif //LS_TCPSERVER_H_
