#pragma once

#include <boost/asio.hpp>
#include "object_pool.hpp"
#include "network_gateway.hpp"


namespace communication {

	class udp_receiver {
		boost::asio::ip::udp::socket socket;
		std::shared_ptr<object_pool<boost::asio::streambuf>> streambuffer_pool;
		std::shared_ptr<object_pool<boost::asio::ip::udp::endpoint>>  endpoint_pool;
		network_gateway<false>* gateway;
		
		void handle_receive(boost::asio::streambuf* streambuf, boost::asio::ip::udp::endpoint* endpoint, const boost::system::error_code& error, std::size_t bytes_transferred);

	public:
		udp_receiver(network_gateway<false>* gateway, boost::asio::io_service& io_service, std::shared_ptr<object_pool<boost::asio::streambuf>> streambuffer_pool, unsigned short port);
		udp_receiver(network_gateway<false>* gateway, boost::asio::io_service& io_service, std::shared_ptr<object_pool<boost::asio::streambuf>> streambuffer_pool, std::string host, unsigned short port);
		void set_endpoint_pool(std::shared_ptr<object_pool<boost::asio::ip::udp::endpoint>> endpoint_pool);
		void receive();
	};
}
