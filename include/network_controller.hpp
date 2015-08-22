#pragma once

#include <boost/asio.hpp>
#include "message.hpp"
#include "client.hpp"

namespace communication {

	class network_controller {
	public:
		virtual void receive(boost::asio::streambuf* streambuf) = 0;
		virtual void send_reliable(message::message* message, client* cient) = 0;
		virtual void send_reliable(message::message* message, std::vector<client*> clients) = 0;
		virtual void send_unreliable(message::message* message, client* cient) = 0;
		virtual void send_unrelieable(message::message* message, std::vector<client*> clients) = 0;
	};

}