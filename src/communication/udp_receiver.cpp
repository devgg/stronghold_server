#include "udp_receiver.hpp"
#include <iostream>
#include <boost/bind/bind.hpp>


namespace communication {

	udp_receiver::udp_receiver(boost::asio::io_service& io_service, std::shared_ptr<object_pool<boost::asio::streambuf>> streambuffer_pool, unsigned short port) :
		socket(io_service, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port)),
		streambuffer_pool(streambuffer_pool) {
		receive();
	}

	udp_receiver::udp_receiver(boost::asio::io_service& io_service, std::shared_ptr<object_pool<boost::asio::streambuf>> streambuffer_pool, std::string host, unsigned short port) :
		socket(io_service, boost::asio::ip::udp::endpoint(boost::asio::ip::address::from_string(host), port)),
		streambuffer_pool(streambuffer_pool) {
		receive();
	}

	void udp_receiver::receive() {
		boost::asio::streambuf* streambuf = streambuffer_pool->pop();
		socket.async_receive_from(streambuf->prepare(1000), receive_endpoint, boost::bind(&udp_receiver::handle_receive, this, streambuf, _1, _2));
	}

	void udp_receiver::handle_receive(boost::asio::streambuf* streambuf, const boost::system::error_code& error, std::size_t bytes_transferred) {
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
}