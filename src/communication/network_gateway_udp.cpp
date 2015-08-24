#include <iostream>

#include "network_gateway_udp.hpp"
#include "serializer.hpp"
#include "deserializer.hpp"
#include "message.hpp"

namespace communication {

	network_gateway_udp::network_gateway_udp(unsigned short receive_port, unsigned short send_port) :
		streambuffer_pool(std::make_shared<object_pool<boost::asio::streambuf>>(100)),
		udp_receiver(io_service, streambuffer_pool, receive_port),
		udp_sender(io_service, streambuffer_pool, send_port) {}

	boost::asio::streambuf* network_gateway_udp::serialize_message(message::message* message) {
		boost::asio::streambuf* streambuf = streambuffer_pool->pop();
		serializer::serialize(message, streambuf);
		return streambuf;
	}

	void network_gateway_udp::receive(boost::asio::streambuf* streambuf) {
		message::message* message = deserializer::deserialize(streambuf);
		streambuffer_pool->push(streambuf);
		listener->receive(message);
	}

	void network_gateway_udp::send(message::message* message, const boost::asio::ip::udp::endpoint& endpoint) {
		udp_sender.send(serialize_message(message), endpoint);
	}

	void network_gateway_udp::send(message::message* message, const std::vector<boost::asio::ip::udp::endpoint>& endpoints) {
		udp_sender.send(serialize_message(message), endpoints);
	}

	void network_gateway_udp::set_listener(std::shared_ptr<network_listener> listener) {
		this->listener = listener;
	}

	void network_gateway_udp::run() {
		if (listener) {
			io_service.run();
		} else {
			std::cout << "error [listener not set]" << std::endl;
		}
	}

}