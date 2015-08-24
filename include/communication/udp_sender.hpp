#pragma once

#include <boost/asio.hpp>
#include "object_pool.hpp"
#include <atomic>


namespace communication {

	class udp_sender {
		boost::asio::ip::udp::socket socket;
		std::shared_ptr<object_pool<boost::asio::streambuf>> streambuffer_pool;

		void handle_unicast(const boost::system::error_code& error, std::size_t bytes_transferred);
		void handle_multicast(std::atomic<size_t>* counter, size_t num_endpoints, const boost::system::error_code& error, std::size_t bytes_transferred);

	public:
		udp_sender(boost::asio::io_service& io_service, std::shared_ptr<object_pool<boost::asio::streambuf>> streambuffer_pool, unsigned short port);
		udp_sender(boost::asio::io_service& io_service, std::shared_ptr<object_pool<boost::asio::streambuf>> streambuffer_pool, std::string host, unsigned short port);

		void send(boost::asio::streambuf* streambuf, const boost::asio::ip::udp::endpoint& endpoint);
		void send(boost::asio::streambuf* streambuf, const std::vector<boost::asio::ip::udp::endpoint>& endpoints);

	};
}
