#pragma once

#include <boost/asio.hpp>
#include "object_pool.hpp"


namespace communication {

	class udp_receiver {
		boost::asio::ip::udp::socket socket;
		boost::asio::ip::udp::endpoint receive_endpoint; //might need more?!
		std::shared_ptr<object_pool<boost::asio::streambuf>> streambuffer_pool;


		void receive();
		void handle_receive(boost::asio::streambuf* streambuf, const boost::system::error_code& ec, std::size_t bytes_transferred);

	public:
		udp_receiver(boost::asio::io_service& io_service, std::shared_ptr<object_pool<boost::asio::streambuf>> streambuffer_pool, unsigned short port);
		udp_receiver(boost::asio::io_service& io_service, std::shared_ptr<object_pool<boost::asio::streambuf>> streambuffer_pool, std::string host, unsigned short port);
	};
}
