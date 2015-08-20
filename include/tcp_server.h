//
//  tcp_server.h
//  stronghold_server
//
//  Created by Florian Gauger on 17/08/15.
//
//

#ifndef __stronghold_server__tcp_server__
#define __stronghold_server__tcp_server__

#include <boost/asio.hpp>
#include <map>
#include <atomic>
#include <memory>

#include "tcp_connection.h"
#include "gateway.h"
#include "relay.h"


namespace communication {
    
    
    class tcp_server: public virtual gateway {
    private:
        tcp::acceptor acceptor;
        std::shared_ptr<communication::relay> relay;
        
        std::map<client, boost::shared_ptr<tcp_connection>> clients;
        std::atomic<size_t> counter;
        
        void start_accept();
        void handle_accept(boost::shared_ptr<tcp_connection> connection, const boost::system::error_code& error);
        
    public:
        tcp_server(boost::asio::io_service& io_service, std::shared_ptr<communication::relay> relay);
        virtual void send(std::shared_ptr<msg::message> message, client c) override;
        virtual void broadcast(std::shared_ptr<msg::message> message) override;
    };
    
}

#endif /* defined(__stronghold_server__tcp_server__) */