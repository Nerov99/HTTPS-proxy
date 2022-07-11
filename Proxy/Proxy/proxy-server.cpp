#include "proxy-server.hpp"

server::server(const ios_deque& io_services, int port, std::string interface_address)
	: io_services_(io_services),
	  endpoint_(interface_address.empty()?	
				(boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)): // INADDR_ANY for v4 (in6addr_any if the fix to v6)
		  boost::asio::ip::tcp::endpoint(boost::asio::ip::address().from_string(interface_address), port) ),	// specified ip address
	  acceptor_(*io_services.front(), endpoint_),	// By default set option to reuse the address (i.e. SO_REUSEADDR)
	context_(boost::asio::ssl::context::sslv23)
{
	context_.set_options(
		boost::asio::ssl::context::default_workarounds
		| boost::asio::ssl::context::no_sslv2
		| boost::asio::ssl::context::single_dh_use);
	context_.set_password_callback(std::bind(&server::get_password, this));
	context_.use_certificate_chain_file("cert.pem");
	context_.use_private_key_file("key.pem", boost::asio::ssl::context::pem);
	context_.use_tmp_dh_file("dh4096.pem");

	std::cout << endpoint_.address().to_string() << ":" << endpoint_.port() << std::endl;
//	std::cout << "server::server" << std::endl;
	start_accept();
}

std::string server::get_password() const
{
	return "test";
}

void server::start_accept() {
//	std::cout << "server::start_accept" << std::endl;
	// Round robin.
	io_services_.push_back(io_services_.front());
	io_services_.pop_front();
	connection::pointer new_connection = connection::create(*io_services_.front());

	acceptor_.async_accept(new_connection->socket(),
						   boost::bind(&server::handle_accept, this, new_connection,
							   boost::asio::placeholders::error));
}

void server::handle_accept(connection::pointer new_connection, const boost::system::error_code& error) {
//	std::cout << "server::handle_accept" << std::endl;
	if (!error) {
		new_connection->start();
		start_accept();
	}
}