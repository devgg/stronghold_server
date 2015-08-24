#pragma once

#include "network_gateway.hpp"
#include "udp_receiver.hpp"
#include "udp_sender.hpp"
#include "message.hpp"
#include "object_pool.hpp"
#include "network_listener.hpp"


namespace communication {
	class network_gateway_udp : public network_gateway<false> {

		boost::asio::io_service io_service;
		std::shared_ptr<object_pool<boost::asio::streambuf>> streambuffer_pool;

		udp_receiver udp_receiver;
		udp_sender udp_sender;
		std::shared_ptr<network_listener> listener = nullptr;

		boost::asio::streambuf* serialize_message(message::message* message);

	public:
		network_gateway_udp(unsigned short receive_port, unsigned short send_port);

		virtual void receive(boost::asio::streambuf* streambuf) override;
		virtual void send(message::message* message, const boost::asio::ip::udp::endpoint& endpoint) override;
		virtual void send(message::message* message, const std::vector<boost::asio::ip::udp::endpoint>& endpoints) override;

		virtual void set_listener(std::shared_ptr<network_listener> listener) override;
		virtual void run() override;
	};
}