#pragma once
#include "common.h"
#include "proxy-conn.hpp"
#include <deque>

typedef std::deque<io_service_ptr> ios_deque;

class server {
public:
	server(const ios_deque& io_services, int port=10001, std::string interface_address = "");

private:
	std::string get_password() const;
	void start_accept();
	void handle_accept(connection::pointer new_connection, const boost::system::error_code& error);
	
	ios_deque io_services_;
	const boost::asio::ip::tcp::endpoint endpoint_;   /**< object, that points to the connection endpoint */
	boost::asio::ip::tcp::acceptor acceptor_;         /**< object, that accepts new connections */
	boost::asio::ssl::context context_;
};
