#include "TcpServer.cpp"

int CTcpServer::THREAD_NUMBER = 8;
CTcpServer::CTcpServer(const char* ip,int port)
:_io_service(THREAD_NUMBER),_acceptor(_io_service),_strand(_io_service){
	_ip = ip;
	_port = port;
	_alloc_connection_func = NULL;
}

CTcpServer::~CTcpServer()
{
	_connection_manager.StopAll();
}

int CTcpServer::Init()
{

	try{
		boost::asio::ip::tcp::resolver::query _query(_ip,_port);
		boost::asio::ip::tcp::resolver        _resolver(_io_service);
		boost::asio::ip::tcp::endpoint        _end_point = *_resolver.resolve(_query);
		
		_acceptor.open(boost::asio::ip::tcp::v4());
		_acceptor.set_option(boost::asio::ip::tcp::reuse_address(true));
		_acceptor.bind(_end_point);
		_acceptor.listen();

	}catch(std::exception& e){
		//log
		return -1;
	}

	return 0;
}

int CTcpServer::Run()
{
	start_accept();
	return 0;
}

int CTcpServer::start_accept()
{
	if(alloc_connection_func == NULL)
		_new_connection.reset(new CTcpConnectoin(_io_service,_connection_manager));
	else
		_new_connection.reset(_alloc_connection_func(_io_service,_connection_manager));
	
	_acceptor.async_accept(_new_connection->Socket(),
		_strand.wrap(boost::bind(&CTcpServer::handle_accept,enable_shared_from_this(),
			boost::asio::placeholders::error)));

	return 0;
}

void CTcpServer::handle_accept(boost::system::error& e)
{
	if(!e){
		_connection_manager.Start(_new_connection);
	}

	start_accept();
}


int CTcpServer::start_thread()
{
	int n = 0;
	boost::shared_ptr<boost::thread> per_thread;
	for(; n < THREAD_NUMBER; ++n){
		per_thread.reset(new boost::thread(
			boost::bind(&boost::asio::io_service::run,_io_service)));
		_thread_pool.push_back(per_thread);
	}

	n = 0;
	
	for(; (std::size_t)n < _thread_pool.size(); ++n){
		(_thread_pool.at(n))->join());
	}

	return 0;
}

void CTcpServer::SetAllocMethod(ALLOC_CONNECTION alloc_connection_func)
{
	_alloc_connection_func = alloc_connection_func;
}


