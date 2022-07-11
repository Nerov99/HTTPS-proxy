#pragma once
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>
#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>
#include <iostream>
#include <string>

typedef boost::shared_ptr<boost::asio::ip::tcp::socket> socket_ptr;
typedef boost::shared_ptr<boost::asio::io_service> io_service_ptr;


