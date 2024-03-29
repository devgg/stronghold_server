#include "udp_sender.hpp"
#include <iostream>
#include <boost/bind/bind.hpp>


namespace communication {

	udp_sender::udp_sender(boost::asio::io_service& io_service, std::shared_ptr<object_pool<boost::asio::streambuf>> streambuffer_pool, unsigned short port) :
		socket(io_service, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port)),
		streambuffer_pool(streambuffer_pool) {}

	udp_sender::udp_sender(boost::asio::io_service& io_service, std::shared_ptr<object_pool<boost::asio::streambuf>> streambuffer_pool, std::string host, unsigned short port) :
		socket(io_service, boost::asio::ip::udp::endpoint(boost::asio::ip::address::from_string(host), port)),
		streambuffer_pool(streambuffer_pool) {}

	void udp_sender::send(boost::asio::streambuf* streambuf, const boost::asio::ip::udp::endpoint* endpoint) {
		socket.async_send_to(streambuf->data(), *endpoint, boost::bind(&udp_sender::handle_unicast, this, streambuf, _1, _2));
	}

	void udp_sender::send(boost::asio::streambuf* streambuf, const std::vector<boost::asio::ip::udp::endpoint*> endpoints) {
		std::atomic<size_t>* counter = new std::atomic<size_t>(0);
		for (const boost::asio::ip::udp::endpoint* endpoint : endpoints) {
			socket.async_send_to(streambuf->data(), *endpoint, boost::bind(&udp_sender::handle_multicast, this, streambuf, counter, endpoints.size(), _1, _2));
		}
	}


	void udp_sender::handle_unicast(boost::asio::streambuf* streambuf, const boost::system::error_code& error, std::size_t bytes_transferred) {
		if (!error) {

			//        std::cout << "bytes: " << bytes_transferred << std::endl;
		} else {
			std::cout << "error during write [" << error.message() << "]" << std::endl;
		}
		streambuffer_pool->push(streambuf);
	}

	void udp_sender::handle_multicast(boost::asio::streambuf* streambuf, std::atomic<size_t>* counter, size_t num_endpoints, const boost::system::error_code& error, std::size_t bytes_transferred) {
		if (!error) {

			//        std::cout << "bytes: " << bytes_transferred << std::endl;
		} else {
			std::cout << "error during write [" << error.message() << "]" << std::endl;
		}
		++(*counter);
		if (*counter == num_endpoints) {
			// release streambuf back in pool
			delete counter;
		}
		streambuffer_pool->push(streambuf);
	}


}