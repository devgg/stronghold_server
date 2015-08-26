#include "udp_receiver.hpp"
#include <iostream>
#include <boost/bind/bind.hpp>


namespace communication {

	udp_receiver::udp_receiver(network_gateway<false>* gateway, boost::asio::io_service& io_service, std::shared_ptr<object_pool<boost::asio::streambuf>> streambuffer_pool, unsigned short port) :
		socket(io_service, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port)),
		streambuffer_pool(streambuffer_pool), gateway(gateway) {}

	udp_receiver::udp_receiver(network_gateway<false>* gateway, boost::asio::io_service& io_service, std::shared_ptr<object_pool<boost::asio::streambuf>> streambuffer_pool, std::string host, unsigned short port) :
		socket(io_service, boost::asio::ip::udp::endpoint(boost::asio::ip::address::from_string(host), port)),
		streambuffer_pool(streambuffer_pool), gateway(gateway) {}

	void udp_receiver::receive() {
		boost::asio::streambuf* streambuf = streambuffer_pool->pop();
		boost::asio::ip::udp::endpoint* endpoint = endpoint_pool->pop();
		socket.async_receive_from(streambuf->prepare(1000), *endpoint, boost::bind(&udp_receiver::handle_receive, this, streambuf, endpoint, _1, _2));
	}


	void udp_receiver::handle_receive(boost::asio::streambuf* streambuf, boost::asio::ip::udp::endpoint* endpoint, const boost::system::error_code& error, size_t bytes_transferred) {
		if (!error) {
			std::cout << "read [bytes '" << bytes_transferred << "', #streambuffer '" << streambuffer_pool->num_available_objects() << "']" << std::endl;
			streambuf->commit(bytes_transferred);
			gateway->receive(streambuf, endpoint);
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

	void udp_receiver::set_endpoint_pool(std::shared_ptr<object_pool<boost::asio::ip::udp::endpoint>> endpoint_pool) {
		this->endpoint_pool = endpoint_pool;
	}
}

