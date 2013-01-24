#ifndef LS_TCPCONNECTION_H_
#define LS_TCPCONNECTION_H_

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

	//wrap
	void ReplySyncMessage(const char* data, int data_len);
	
	//post
	void PostAsyncMessage(const char* data, int data_len);

	//wrap
	void ReplyAsyncMessage(const char* data, int data_len);

protected:
	void handle_read_header (boost::system::error& e,std::size_t bytes_transferred,char *header);

	void handle_read_content (boost::system::error& e,std::size_t bytes_transferred);

	void handle_write(boost::system::error& e,std::size_t bytes_transferred);

	//in real service you can rewrite this method
	/*
	*return <0,packet process error;=0,suceess
	*@param 'packet' real transfer content
	*@param 'type'   type of this packet
	*@param 'len'    len  of this packet
	*/
	virtual int ProcessPacket(const char* packet,short len,short type) = 0;

protected:
	boost::asio::io_service 		_io_service;

	boost::asio::io_service::strand _strand;
	
	boost::asio::ip::tcp::socket    _socket;

	CConnectionManager& 			_connection_manager;	

	short							_content_buf_len;

	short							_packet_len;

	short 							_packet_type;

	boost::shared_array<char>       _packet_content;	
};

#endif //LS_TCPCONNECTION_H_
