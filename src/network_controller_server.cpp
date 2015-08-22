#include "network_controller_server.hpp"
#include "serializer.hpp"

namespace communication {

	void network_controller_server::receive(boost::asio::streambuf* streambuf) {

		streambuffer_pool->push(streambuf);
	}

	void network_controller_server::send_reliable(message::message* message, client* client) {

	}

	void network_controller_server::send_reliable(message::message* message, std::vector<client*> clients) {

	}

	void network_controller_server::send_unreliable(message::message* message, client* client) {
		udp_server->unicast(, serialize_message(message));
	}

	void network_controller_server::send_unrelieable(message::message* message, std::vector<client*> clients) {
		udp_server->multicast(, serialize_message(message));
	}

	boost::asio::streambuf* network_controller_server::serialize_message(message::message* message) {
		boost::asio::streambuf* streambuf = streambuffer_pool->pop();
		serializer::serialize(message, streambuf);
		return streambuf;
	}

}