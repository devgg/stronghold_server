#pragma once

#include <boost/asio.hpp>
#include <map>
#include <atomic>

#include "udp_session.hpp"


namespace communication {

	class udp_server2 {
		boost::asio::ip::udp::socket socket;
		boost::asio::streambuf read_buffer;
		boost::asio::ip::udp::endpoint read_endpoint;
		
		void receive();
		void handle_receive(const boost::system::error_code& ec, std::size_t bytes_transferred);
		void send(const boost::asio::ip::udp::endpoint& endpoint, const boost::asio::streambuf& data);
		void handle_send(const boost::system::error_code& ec, std::size_t bytes_transferred);

	public:
		udp_server2(boost::asio::io_service& io_service);
//		void send(const boost::asio::streambuf& data, const client& c);
//		void broadcast(const boost::asio::streambuf& data);
//
//		~udp_server2();
	};
}
