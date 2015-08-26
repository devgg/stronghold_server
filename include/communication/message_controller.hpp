#pragma once

#include <network_listener.hpp>
#include <message.hpp>
#include <network_gateway.hpp>

namespace communication {
	class message_controller : public network_listener, public std::enable_shared_from_this<message_controller> {
		std::shared_ptr<network_gateway<false>> gateway;
		std::shared_ptr<object_pool<boost::asio::ip::udp::endpoint>> endpoint_pool;

	public:
		message_controller(std::shared_ptr<network_gateway<false>> gateway);
		virtual void receive(message::message* message, boost::asio::ip::udp::endpoint* endpoint) override;
		virtual std::shared_ptr<object_pool<boost::asio::ip::udp::endpoint>> get_endpoint_pool() override;
	};
};