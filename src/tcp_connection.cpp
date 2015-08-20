//
//  tcp_connection.cpp
//  stronghold_server
//
//  Created by Florian Gauger on 17/08/15.
//
//

#include "tcp_connection.hpp"
#include "key_event.hpp"
#include "client.hpp"
#include "deserializer.hpp"

namespace cm = communication::message;
using namespace communication;




void tcp_connection::handle_read(const boost::system::error_code& error, std::size_t bytes_transferred) {
    
    if (!error) {
        
        std::shared_ptr<cm::message> message = deserializer::deserialize(input_buffer, header.first);
        input_buffer.consume(header.second - serialization::header_size);
        
        relay->relay_message(message, client);
        
        header.second = 0;
        start_read();
    } else if ((boost::asio::error::connection_reset == error) || (boost::asio::error::eof == error)) {
        std::cout << "error during read [" << error.message() << "]" << std::endl;
    } else {
        std::cout << "error during read [" << error.message() << "]" << std::endl;
    }
}

bool tcp_connection::completion_condition(const boost::system::error_code& error, std::size_t bytes_transferred) {
    if ((header.second == 0) && (bytes_transferred >= serialization::header_size)) {
        input_buffer.commit(serialization::header_size);
        header = deserializer::deserialize_header(input_buffer);
        input_buffer.consume(serialization::header_size);
    }
    bool is_complete = (header.second > 0) && (bytes_transferred >= header.second);
    if (is_complete) {
        input_buffer.commit(header.second - serialization::header_size);
    }
    return is_complete;
}

boost::shared_ptr<tcp_connection> tcp_connection::create(boost::asio::io_service& io_service, std::shared_ptr<communication::relay> relay, communication::client client) {
    return boost::shared_ptr<tcp_connection>(new tcp_connection(io_service, relay, client));
}

tcp::socket& tcp_connection::socket() {
    return socket_;
}

void tcp_connection::start() {
    start_read();
}

void tcp_connection::start_read() {
    boost::asio::async_read(socket_, boost::asio::buffer(input_buffer.prepare(200)),
                            boost::bind(&tcp_connection::completion_condition, shared_from_this(), _1, _2),
                            boost::bind(&tcp_connection::handle_read, shared_from_this(), _1, _2));
}

void tcp_connection::write(std::vector<unsigned char> bytes) {
    boost::asio::streambuf buffer;
    std::ostream request_stream(&buffer);
    request_stream.write(reinterpret_cast<char*>(bytes.data()), bytes.size());
    
    boost::asio::async_write(socket_, buffer,
                             boost::bind(&tcp_connection::handle_write, shared_from_this(),
                                         boost::asio::placeholders::error,
                                         boost::asio::placeholders::bytes_transferred));
}

void tcp_connection::handle_write(const boost::system::error_code& error, size_t bytes_transferred) {
    if (!error) {
//        std::cout << "bytes: " << bytes_transferred << std::endl;
    } else {
        std::cout << "error during write [" << error.message() << "]" << std::endl;
    }
    
    // bytes_transfered == end of file after disconnect
}

communication::client tcp_connection::get_client() {
    return client;
}