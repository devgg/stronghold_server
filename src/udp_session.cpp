//
//  udp_session.cpp
//  stronghold_server
//
//  Created by Florian Gauger on 19/08/15.
//
//

#include <iostream>

#include "udp_session.hpp"
#include "key_event.hpp"
#include "client.hpp"
#include "deserializer.hpp"

namespace cm = communication::message;
using namespace communication;

namespace communication {
    
    void udp_session::handle_sent(const boost::system::error_code& error, size_t bytes_transferred) {
        if (!error) {
            //        std::cout << "bytes: " << bytes_transferred << std::endl;
        } else {
            std::cout << "error during write [" << error.message() << "]" << std::endl;
        }
        
        // bytes_transfered == end of file after disconnect
    }
    
    void udp_session::handle_request(const boost::system::error_code& error, std::size_t bytes_transferred) {
        
        if (!error) {
            input_buffer.commit(serialization::header_size);
            std::pair<cm::message_id, size_t> header = communication::deserializer::deserialize_header(input_buffer);
            input_buffer.consume(serialization::header_size);
            
            input_buffer.commit(header.second - serialization::header_size);
            std::shared_ptr<cm::message> message = deserializer::deserialize(input_buffer, header.first);
            input_buffer.consume(header.second - serialization::header_size);
            
            relay->relay_message(message, client);
            
            header.second = 0;
        } else if ((boost::asio::error::connection_reset == error) || (boost::asio::error::eof == error)) {
            std::cout << "error during read [" << error.message() << "]" << std::endl;
        } else {
            std::cout << "error during read [" << error.message() << "]" << std::endl;
        }
    }
    
}