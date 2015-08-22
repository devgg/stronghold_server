//
//  udp_server.cpp
//  stronghold_server
//
//  Created by Florian Gauger on 18/08/15.
//
//

#include <boost/range/adaptor/map.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/bind.hpp>
#include <boost/make_shared.hpp>

#include <boost/asio.hpp>
#include <memory>

#include "udp_session.hpp"
#include "udp_server.hpp"
#include "serializer.hpp"

#include <boost/asio/buffer.hpp>

using boost::asio::ip::udp;

namespace communication {
    
    
    udp_server::udp_server(boost::asio::io_service& io_service, std::shared_ptr<communication::relay> relay):
    socket_(io_service, udp::endpoint(udp::v4(), 1337)), strand_(io_service), relay(relay), counter(100) {
        receive_session();
        
        sessions = std::map<boost::shared_ptr<udp_session>, client,
            std::function<bool(const boost::shared_ptr<udp_session>&, const boost::shared_ptr<udp_session>&)>> {
            [](const boost::shared_ptr<udp_session>& a, const boost::shared_ptr<udp_session>& b) {
                return a->remote_endpoint_ < b->remote_endpoint_;
            }
        };
    }

    void udp_server::receive_session() {
        boost::shared_ptr<udp_session> session = boost::make_shared<udp_session>(relay, client(-1));
        socket_.async_receive_from(session->input_buffer.prepare(200), session->remote_endpoint_,
                                   strand_.wrap(boost::bind(&udp_server::handle_receive, this,
                                                     session, // keep-alive of buffer/endpoint
                                                     boost::asio::placeholders::error,
                                                     boost::asio::placeholders::bytes_transferred)));
    }
    
    void udp_server::handle_receive(boost::shared_ptr<udp_session> const& session, const boost::system::error_code &ec, std::size_t bytes_transferred) {
        boost::shared_ptr<udp_session> current_session = session;
        
        auto session_iter = sessions.find(session);
        if (session_iter != sessions.end()) {
            current_session = session_iter->first;
            session->client = current_session->client;
        } else {
            client c(counter++);
            sessions[session] = c;
            session->client = c;
        }
        
        
        // now, handle the current session on any available pool thread
        socket_.get_io_service().post(bind(&udp_session::handle_request, session, ec, bytes_transferred));
        
        // immediately accept new datagrams
        receive_session();
    }
    
    void udp_server::enqueue_response(boost::shared_ptr<udp_session> const& session, const std::vector<unsigned char>& bytes) {
		std::vector<unsigned char>* bytes2 = new std::vector<unsigned char>();
		for (int i = 0; i < bytes.size(); i++) {
			bytes2->push_back(bytes[i]);
		}

        socket_.async_send_to(boost::asio::buffer(*bytes2), session->remote_endpoint_, bind(&udp_session::handle_sent, session, _1, _2));
    }
    
    void udp_server::send(std::shared_ptr<msg::message> message, client c) {
        for(auto&& session : sessions) {
            if (session.second.get_id() == c.get_id()) {
                enqueue_response(session.first, serializer::serialize(message));
            }
        }
    }
    
    void udp_server::broadcast(std::shared_ptr<msg::message> message) {
		std::vector<unsigned char> bytes = serializer::serialize(message);
        for(auto&& session : sessions | boost::adaptors::map_keys) {
            enqueue_response(session, bytes);
        }
    }
}