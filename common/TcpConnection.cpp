CTcpConnection::CTcpConnection(boost::asio::io_service& io_service,CConnectionManager& manager)
	:_io_service(io_service),_manager(manager),_strand(io_service),_socket(io_service)

{
	_packet_len  = 0;
	_packet_type = 0;
}

CTcpConnection::~CTcpConnection()
{

}

void CTcpConnection::Start()
{
	char header[5]="";
	boost::asio::async_read(_socket,boost::asio::buffer(header,4),
		boost::asio::transfer_at_least(4),
		_strand.wrap(boost::bind(&CTcpConnection::handle_read_header,this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred,
			header)));

}

void CTcpConnection::Stop()
{
	_socket.close();
}

boost::asio::ip::tcp::socket&  CTcpConnection::Socket()
{
	return _socket;
}

void CTcpConnection::handle_read_header (boost::system::error& e,std::size_t bytes_transferred,char *header)
{
	if(bytes_transferred != 4){
		_manager.Stop(boost::shared_from_this());
		return;
	}

	if(!e){
		_manager.Stop(boost::shared_from_this());
		return;	
	}

	_packet_type = ntohs(*((short*)header));
	_packet_len  = ntohs(*((short*)(header+2)));

	if(_content_buf_len < _packet_len){
		_packet_content.reset(new char[_packet_len]);
	}

	memset(_packet_content.get(),_content_buf_len,0);
	boost::asio::async_read(_socket,boost::asio::buffer(_packet_content.get(),_packet_len),
		_strand.wrap(boost::bind(&CTcpConnection::handle_read_content,this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred)));
}

void CTcpConnection::handle_read_content (boost::system::error& e,std::size_t bytes_transferred)
{
	if(!e){
		_manager.Stop(boost::shared_from_this());
		return;
	}

	int ret = ProcessPacket(_packet_content.get(),_packet_type,_packet_len);
	if(ret == 0){
		Start();
	}else{
		_manager.Stop(boost::shared_from_this());
	}
}

void CTcpConnection::handle_write(boost::system::error& e,std::size_t bytes_transferred)
{
	if(!e){
		_manager.Stop(boost::shared_from_this());
		return;
	}
}
void ReplySyncMessage(const char* data, int data_len)
{
	_strand.dispatch(boost::asio::write(_socket,boost::asio::buffer(data,data_len)));
}

void CTcpConnection::PostAsyncMessage(const char* data, int data_len)
{
	_strand.post(boost::bind(
		&CTcpConnection::ReplyAsyncMessage,
		this,data,data_len));
}

void CTcpConnection::ReplyAsyncMessage(const char* data, int data_len)
{
	boost::asio::async_write(_socket,boost::asio::buffer(data,data_len),
		_strand.wrap(boost::bind(&CTcpConnection::handle_write,this,
			boost::asio::placeholders::error));
}