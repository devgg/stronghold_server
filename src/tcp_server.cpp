//
//  tcp_server.cpp
//  stronghold_server
//
//  Created by Florian Gauger on 17/08/15.
//
//


#include "tcp_server.hpp"
#include <ctime>
#include <iostream>
#include <string>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/range/adaptor/map.hpp>
#include "serializer.hpp"

namespace communication {
    
    namespace com = communication;
    using con = communication::tcp_connection;
    using con_pointer = boost::shared_ptr<tcp_connection>;
    using boost::asio::ip::tcp;
    
    tcp_server::tcp_server(boost::asio::io_service& io_service, std::shared_ptr<com::relay> relay) : acceptor(io_service, tcp::endpoint(tcp::v4(), 1337)), relay(relay) {
        start_accept();
    }
    
    void tcp_server::start_accept() {
        client c(counter++);
        con_pointer new_connection = con::create(acceptor.get_io_service(), relay, c);
        acceptor.async_accept(new_connection->socket(),
                              boost::bind(&tcp_server::handle_accept, this, new_connection,
                                          boost::asio::placeholders::error));
    }
    
    void tcp_server::handle_accept(con_pointer connection, const boost::system::error_code& error) {
        std::cout << "connect" << std::endl;
        if (!error) {
            clients[connection->get_client()] = connection;
            connection->start();
            connection->socket().set_option(tcp::no_delay(true));
        }
        
        start_accept();
    }
    
    void tcp_server::send(std::shared_ptr<msg::message> message, communication::client c) {
        con_pointer connection = clients[c];
        connection->write(serializer::serialize(message));
    }
    
    void tcp_server::broadcast(std::shared_ptr<msg::message> message) {
        std::vector<unsigned char> bytes = serializer::serialize(message);
        for(auto&& connection : clients | boost::adaptors::map_values) {
            connection->write(bytes);
        }
    }
}