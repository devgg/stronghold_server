#include "udp_server2.hpp"
#include <iostream>


namespace communication {

	udp_server2::udp_server2(boost::asio::io_service& io_service) :
		socket(io_service, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), 1337)) {
		receive();
	}


	void udp_server2::receive() {
		socket.async_receive_from(read_buffer.prepare(10000), read_endpoint, boost::bind(&udp_server2::handle_receive, this, _1, _2));
	}

	void udp_server2::handle_receive(const boost::system::error_code& error, std::size_t bytes_transferred) {
		if (!error) {
			read_buffer.commit(14);
			send(read_endpoint, read_buffer);
		} else if ((boost::asio::error::connection_reset == error) || (boost::asio::error::eof == error)) {
			std::cout << "error during read [" << error.message() << "]" << std::endl;
		} else {
			std::cout << "error during read [" << error.message() << "]" << std::endl;
		}
		receive();
	}

	void udp_server2::send(const boost::asio::ip::udp::endpoint& endpoint, const boost::asio::streambuf& data) {
		socket.async_send_to(data.data(), endpoint, boost::bind(&udp_server2::handle_send, this, _1, _2));
	}

	void udp_server2::handle_send(const boost::system::error_code& error, std::size_t bytes_transferred) {
		if (!error) {
			read_buffer.consume(14);
			//        std::cout << "bytes: " << bytes_transferred << std::endl;
		}
		else {
			std::cout << "error during write [" << error.message() << "]" << std::endl;
		}
	}

}