//
//  communication_proxy.cpp
//  stronghold_server
//
//  Created by Mikael Beyene on 16/08/15.
//
//

#include "proxy.hpp"
#include "key_event.hpp"
#include "hello.hpp"
#include "welcome.hpp"
#include "connect.hpp"
#include "disconnect.hpp"

#include "gateway.hpp"
#include "listener.hpp"

#include <iostream>

using namespace std;
using namespace communication;
namespace msg = communication::message;

auto proxy::relay_message(shared_ptr<msg::message> m, client c) -> void {
    switch (m->get_id()) {
            
        case msg::message_id::hello: {
            shared_ptr<msg::hello> hello = static_pointer_cast<msg::hello>(m);
            std::cout << *hello << std::endl;
            listener.lock()->client_connected(hello->get_username(), c);
//            gateway.lock()->send(make_shared<msg::welcome>(c), c);
            break;
        }
            
        case msg::message_id::welcome: {
            // server must not receive welcome message
            break;
        }
            
        case msg::message_id::connect: {
            shared_ptr<msg::connect> connect = static_pointer_cast<msg::connect>(m);
            std::cout << *connect << std::endl;
            listener.lock()->client_connected(connect->get_username(), connect->get_client());
            break;
        }
            
        case msg::message_id::disconnect: {
            shared_ptr<msg::disconnect> disconnect = static_pointer_cast<msg::disconnect>(m);
            std::cout << *disconnect << std::endl;
            listener.lock()->client_disconnected(disconnect->get_client());
            break;
        }
            
        case msg::message_id::key_event: {
            shared_ptr<msg::key_event> key_event = static_pointer_cast<msg::key_event>(m);
            std::cout << *key_event << std::endl;
            listener.lock()->key_event(key_event->get_key(), key_event->get_event(), key_event->get_client());
            break;
        }
            
        default:
            break;
            
    }
}

auto proxy::key_event(constants::key key, constants::event event, client c) -> void {
//    gateway.lock()->broadcast(make_shared<msg::key_event>(key, event, c));
}

auto proxy::update_listener() -> void {
    // TODO transfer events to proxy
}

void proxy::set_gateway(std::weak_ptr<communication::gateway> gateway) {
    this->gateway = gateway;
}

void proxy::set_listener(std::weak_ptr<communication::listener> listener) {
    this->listener = listener;
}