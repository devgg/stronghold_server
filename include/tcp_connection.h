//
//  tcp_connection.h
//  stronghold_server
//
//  Created by Florian Gauger on 17/08/15.
//
//

#ifndef __stronghold_server__tcp_connection__
#define __stronghold_server__tcp_connection__

#include <ctime>
#include <iostream>
#include <string>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <utility>
#include <fstream>
#include <sstream>
#include <memory>

#include "message.h"
#include "relay.h"
#include "client.h"
#include "communication.h"

namespace communication {
    namespace msg = communication::message;
    
    using boost::asio::ip::tcp;
    
    class tcp_connection : public boost::enable_shared_from_this<tcp_connection> {
    private:
        tcp::socket socket_;
        boost::asio::streambuf input_buffer;
        std::pair<communication::message::message_id, size_t> header;
        
        std::shared_ptr<communication::relay> relay;
        communication::client client;
        
        tcp_connection(boost::asio::io_service& io_service, std::shared_ptr<communication::relay> relay, communication::client client) : socket_(io_service), relay(relay), client(client) {}
        
        void handle_write(const boost::system::error_code& error, size_t bytes_transferred);
        void handle_read(const boost::system::error_code& error, std::size_t bytes_transferred);
        bool completion_condition(const boost::system::error_code& error, std::size_t bytes_transferred);
        
    public:
        static boost::shared_ptr<tcp_connection> create(boost::asio::io_service& io_service, std::shared_ptr<communication::relay> relay, communication::client client);
        tcp::socket& socket();
        communication::client get_client();
        void start();
        void start_read();
        void write(std::vector<unsigned char> bytes);
    };
    
}

#endif /* defined(__stronghold_server__tcp_connection__) */