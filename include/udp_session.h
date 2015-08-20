//
//  udp_session.h
//  stronghold_server
//
//  Created by Florian Gauger on 19/08/15.
//
//

#ifndef __stronghold_server__udp_session__
#define __stronghold_server__udp_session__

#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "message.h"
#include "relay.h"
#include "client.h"

namespace communication {
    
    class udp_server; // forward declaration
    
    class udp_session: public boost::enable_shared_from_this<udp_session> {
    public:
        udp_session(std::shared_ptr<communication::relay> relay, communication::client client) : relay(relay), client(client) {}
        void handle_sent(const boost::system::error_code& error, size_t bytes_transferred);
        void handle_request(const boost::system::error_code& error, std::size_t bytes_transferred);
        
    private:
        boost::asio::ip::udp::endpoint remote_endpoint_;
        boost::asio::streambuf input_buffer;
        std::pair<communication::message::message_id, size_t> header;
        
        std::shared_ptr<communication::relay> relay;
        communication::client client;
        
        friend class udp_server;
    };
}

#endif /* defined(__stronghold_server__udp_session__) */
