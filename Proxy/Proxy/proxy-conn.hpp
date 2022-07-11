#pragma once
#include "common.h"
#include <boost/unordered_map.hpp>

class connection : public boost::enable_shared_from_this<connection> {
public:
	typedef boost::shared_ptr<connection> pointer;

	static pointer create(boost::asio::io_service& io_service) {
		return pointer(new connection(io_service));
	}

	boost::asio::ip::tcp::socket& socket() {
		return bsocket_;
	}

	/// Start read data of request from browser
	void start();

private:
	connection(boost::asio::io_service& io_service);

	/// Read header of HTTP request from browser
	void handle_browser_read_headers(const boost::system::error_code& err, size_t len);
	
	/// Start connecting to the web-server, initially to resolve the DNS-name of Web server into the IP address
	void start_connect();
	void handle_resolve(const boost::system::error_code& err,
		boost::asio::ip::tcp::resolver::iterator endpoint_iterator);
	void handle_connect(const boost::system::error_code& err,
		boost::asio::ip::tcp::resolver::iterator endpoint_iterator, const bool first_time);

	/// Write data to the web-server
	void start_write_to_server();
	void handle_server_write(const boost::system::error_code& err, size_t len);

	/// Read header of data returned from the web-server
	void handle_server_read_headers(const boost::system::error_code& err, size_t len);

	/// Reading data from a Web server, and writing it to the browser
	void handle_browser_write(const boost::system::error_code& err, size_t len);
	void handle_server_read_body(const boost::system::error_code& err, size_t len);

	/// Close both sockets: for browser and web-server
	void shutdown();



	boost::asio::io_service& io_service_;
	boost::asio::ip::tcp::socket bsocket_;
	boost::asio::ip::tcp::socket ssocket_;
	boost::asio::ip::tcp::resolver resolver_;
	bool proxy_closed;
	bool isPersistent;
	int32_t RespLen;
	int32_t RespReaded;

	boost::array<char, 8192> bbuffer;
	boost::array<char, 8192> sbuffer;

	std::string fURL;
	std::string fHeaders;
	std::string fNewURL;
	std::string fMethod;
	std::string fReqVersion;
	std::string fServer;
	std::string fPort;
	bool isOpened;

	std::string fReq;

	typedef boost::unordered_map<std::string,std::string> headersMap;
	headersMap reqHeaders, respHeaders;

	void parseHeaders(const std::string& h, headersMap& hm);
};