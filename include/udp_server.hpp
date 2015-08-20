//
//  udp_server.h
//  stronghold_server
//
//  Created by Florian Gauger on 18/08/15.
//
//

#ifndef __stronghold_server__udp_server__
#define __stronghold_server__udp_server__

#include <string>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>
#include <map>
#include <atomic>

#include "gateway.hpp"
#include "udp_session.hpp"

namespace communication {
    
    class udp_server: public virtual gateway
    {
    public:
        udp_server(boost::asio::io_service& io_service, std::shared_ptr<communication::relay> relay);
        virtual void send(std::shared_ptr<msg::message> message, client c) override;
        virtual void broadcast(std::shared_ptr<msg::message> message) override;
    private:
        boost::asio::ip::udp::socket socket_;
        boost::asio::strand strand_;
        std::shared_ptr<communication::relay> relay;
        
        std::map<boost::shared_ptr<udp_session>, client, std::function<bool(const boost::shared_ptr<udp_session>&, const boost::shared_ptr<udp_session>&)>> sessions;
        std::atomic<size_t> counter;
        
        void receive_session();
        void handle_receive(const boost::shared_ptr<udp_session>& session, const boost::system::error_code& ec, std::size_t bytes_transferred);
        void enqueue_response(boost::shared_ptr<udp_session> const& session, std::vector<unsigned char> bytes);
    };
    
}
#endif /* defined(__stronghold_server__udp_server__) */