#pragma once

#include <boost/asio.hpp>
#include "object_pool.hpp"
#include "network_gateway.hpp"
#include <atomic>


namespace communication {

	class udp_server: public network_gateway {
		boost::asio::ip::udp::socket receive_socket;
		boost::asio::ip::udp::socket send_socket;
		std::shared_ptr<object_pool<boost::asio::streambuf>> streambuffer_pool;
		boost::asio::ip::udp::endpoint receive_endpoint;

		void receive();
		void handle_receive(boost::asio::streambuf* streambuf, const boost::system::error_code& ec, std::size_t bytes_transferred);
		void handle_unicast(const boost::system::error_code& ec, std::size_t bytes_transferred);
		void handle_multicast(std::atomic<size_t>* counter, size_t num_endpoints, const boost::system::error_code& ec, std::size_t bytes_transferred);

	public:
		udp_server(boost::asio::io_service& io_service, std::shared_ptr<object_pool<boost::asio::streambuf>> streambuffer_pool, unsigned short receive_port, unsigned short send_port);
		udp_server(boost::asio::io_service& io_service, std::shared_ptr<object_pool<boost::asio::streambuf>> streambuffer_pool, std::string host, unsigned short port, unsigned short send_port);

		virtual void unicast(const boost::asio::ip::udp::endpoint& endpoint, boost::asio::streambuf* streambuf) override;
		virtual void multicast(const std::vector<boost::asio::ip::udp::endpoint>& endpoints, boost::asio::streambuf* streambuf) override;

	};
}
