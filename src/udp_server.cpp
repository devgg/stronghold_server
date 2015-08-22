#include "udp_server.hpp"
#include <iostream>
#include <boost/bind/bind.hpp>


namespace communication {

	udp_server::udp_server(boost::asio::io_service& io_service, std::shared_ptr<object_pool<boost::asio::streambuf>> streambuffer_pool, unsigned short receive_port, unsigned short send_port) :
		receive_socket(io_service, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), receive_port)),
		send_socket(io_service, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), send_port)),
		streambuffer_pool(streambuffer_pool) {
		receive();
	}

	udp_server::udp_server(boost::asio::io_service& io_service, std::shared_ptr<object_pool<boost::asio::streambuf>> streambuffer_pool, std::string host, unsigned short receive_port, unsigned short send_port) :
		receive_socket(io_service, boost::asio::ip::udp::endpoint(boost::asio::ip::address::from_string(host), receive_port)), 
		send_socket(io_service, boost::asio::ip::udp::endpoint(boost::asio::ip::address::from_string(host), send_port)),
		streambuffer_pool(streambuffer_pool) {
		receive();
	}

	void udp_server::receive() {
		boost::asio::streambuf* streambuf = streambuffer_pool->pop();
		receive_socket.async_receive_from(streambuf->prepare(1000), receive_endpoint, boost::bind(&udp_server::handle_receive, this, streambuf, _1, _2));
	}

	void udp_server::unicast(const boost::asio::ip::udp::endpoint& endpoint, const boost::asio::streambuf& data) {
		send_socket.async_send_to(data.data(), endpoint, boost::bind(&udp_server::handle_unicast, this, _1, _2));
	}

	void udp_server::multicast(const std::vector<boost::asio::ip::udp::endpoint>& endpoints, const boost::asio::streambuf& data) {
		std::atomic<size_t>* counter = new std::atomic<size_t>(0);
		for (const boost::asio::ip::udp::endpoint& endpoint : endpoints) {
			send_socket.async_send_to(data.data(), endpoint, boost::bind(&udp_server::handle_multicast, this, counter, endpoints.size(), _1, _2));
		}
	}

	void udp_server::handle_receive(boost::asio::streambuf* streambuf, const boost::system::error_code& error, std::size_t bytes_transferred) {
		if (!error) {
			std::cout << "read [bytes '" << bytes_transferred << "', #streambuffer '" << streambuffer_pool->num_available_objects() << "']" << std::endl;
			streambuf->commit(14);
			streambuffer_pool->push(streambuf);
//			boost::asio::buffers_begin(receive_buffer);
		}
		else if ((boost::asio::error::connection_reset == error) || (boost::asio::error::eof == error)) {
			std::cout << "error during read [" << error.message() << "]" << std::endl;
		}
		else {
			std::cout << "error during read [" << error.message() << "]" << std::endl;
		}
		receive();
	}


	void udp_server::handle_unicast(const boost::system::error_code& error, std::size_t bytes_transferred) {
		if (!error) {

			//        std::cout << "bytes: " << bytes_transferred << std::endl;
		}
		else {
			std::cout << "error during write [" << error.message() << "]" << std::endl;
		}
	}

	void udp_server::handle_multicast(std::atomic<size_t>* counter, size_t num_endpoints, const boost::system::error_code& error, std::size_t bytes_transferred) {
		if (!error) {

			//        std::cout << "bytes: " << bytes_transferred << std::endl;
		}
		else {
			std::cout << "error during write [" << error.message() << "]" << std::endl;
		}
		(*counter)++;
		if (*counter == num_endpoints) {
			// release streambuf back in pool
			delete counter;
		}
	}
}