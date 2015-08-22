#pragma once

#include "network_controller.hpp"
#include "udp_server.hpp"


namespace communication {
	class network_controller_server : public network_controller {

		std::shared_ptr<udp_server> udp_server;
		std::shared_ptr<object_pool<boost::asio::streambuf>> streambuffer_pool;

		boost::asio::streambuf* serialize_message(message::message* message);

	public:
		virtual void receive(boost::asio::streambuf* streambuf) override;
		virtual void send_reliable(message::message* message, client* cient) override;
		virtual void send_reliable(message::message* message, std::vector<client*> clients) override;
		virtual void send_unreliable(message::message* message, client* cient) override;
		virtual void send_unrelieable(message::message* message, std::vector<client*> clients) override;
	};
}