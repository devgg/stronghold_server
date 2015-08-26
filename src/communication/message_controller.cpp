#include "message_controller.hpp"
#include "message.hpp"
#include "key_event.hpp"
#include "hello.hpp"
#include "welcome.hpp"
#include "connect.hpp"
#include "disconnect.hpp"
#include <iostream>


namespace communication {

	message_controller::message_controller(std::shared_ptr<network_gateway<false>> gateway) : gateway(gateway), endpoint_pool(std::make_shared<object_pool<boost::asio::ip::udp::endpoint>>(100)) {
		gateway->set_listener(this);
	}

	void message_controller::receive(message::message* message, boost::asio::ip::udp::endpoint* endpoint) {
//		client client;
//		if (endpoint is client already) {
			endpoint_pool->push(endpoint);
//			client =
//		} else {
//			client =

//		}

		switch(message->get_id()) {
			case message::message_id::hello: {
				message::hello* hello = static_cast<message::hello*>(message);
				std::cout << *hello << std::endl;
				gateway->send(new message::welcome(100), endpoint);
//				listener.lock()->client_connected(hello->get_username(), c);
				break;
			}
			case message::message_id::welcome: {
				// server must not receive welcome message
				break;
			}

			case message::message_id::connect: {
				message::connect* connect = static_cast<message::connect*>(message);
				std::cout << *connect << std::endl;
//				listener.lock()->client_connected(connect->get_username(), connect->get_client());
				break;
			}

			case message::message_id::disconnect: {
				message::disconnect* disconnect = static_cast<message::disconnect*>(message);
				std::cout << *disconnect << std::endl;
//				listener.lock()->client_disconnected(disconnect->get_client());
				break;
			}
			case message::message_id::key_event: {
				message::key_event* key_event = static_cast<message::key_event*>(message);
				std::cout << *key_event << std::endl;
				gateway->send(message, endpoint);
				break;
			};
			default: break;
		}
		

		//		delete message;
	}

	std::shared_ptr<object_pool<boost::asio::ip::udp::endpoint>> message_controller::get_endpoint_pool() {
		return endpoint_pool;
	}
}
