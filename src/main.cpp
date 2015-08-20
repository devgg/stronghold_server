//#include <fstream>
//
//// include headers that implement a archive in simple text format
//#include <boost/archive/text_oarchive.hpp>
//#include <boost/archive/text_iarchive.hpp>
//
//#include <sstream>
//#include <boost/serialization/export.hpp>
//
//
//#include "constants.h"
//#include "message.h"
//#include "key.h"
//#include "connect.h"
//#include "disconnect.h"
//#include "hello.h"
//#include "welcome.h"
//
//BOOST_CLASS_EXPORT_GUID(communication::message::key_message, "key_message")
//BOOST_CLASS_EXPORT_GUID(communication::message::connect, "connect")
//BOOST_CLASS_EXPORT_GUID(communication::message::disconnect, "disconnect")
//BOOST_CLASS_EXPORT_GUID(communication::message::hello, "hello")
//BOOST_CLASS_EXPORT_GUID(communication::message::welcome, "welcome")
//BOOST_CLASS_EXPORT_GUID(communication::client, "client")
/*
 int main() {
 using namespace communication::message;
 
 std::cout << "Trying to save message!" << std::endl;
 communication::client client(23);
 message* m = new key_message(constants::key::right, constants::event::pressed, client);
 std::ofstream ofs("filename.txt");
 {
 boost::archive::text_oarchive oa(ofs);
 oa << m;
 }
 
 std::cout << "Trying to load message!" << std::endl;
 message* m2;
 {
 std::ifstream ifs("filename.txt");
 boost::archive::text_iarchive ia(ifs);
 try {
 ia >> m2;
 } catch (boost::archive::archive_exception e) {
 std::cout << e.what() << std::endl;
 }
 }
 return 0;
 }*/


//#include <fstream>
//
//// include headers that implement a archive in simple text format
//#include <boost/archive/text_oarchive.hpp>
//#include <boost/archive/text_iarchive.hpp>
//
//#include <sstream>
//#include <boost/serialization/export.hpp>
//
//
//#include "constants.h"
//#include "message.h"
//#include "key_message.h"
//
//#include "tcp_server_a.h"
//
//BOOST_CLASS_EXPORT_GUID(key_message, "key_message")

//int main() {
//    //client.run(boost::asio::ip::address::from_string("127.0.0.1"));
//
//    message* m = new key_message(constants::key::right, constants::event::press);
//    std::ofstream ofs("build/filename.txt");
//    {
//        boost::archive::text_oarchive oa(ofs);
//        oa << m;
//    }
//
//    message* m2;
//    {
//        std::ifstream ifs("build/filename.txt");
//        boost::archive::text_iarchive ia(ifs);
//        try {
//            ia >> m2;
//        } catch (boost::archive::archive_exception e) {
//            std::cout << e.what() << std::endl;
//        }
//    }
//    return 0;
//}

/*
 #include <ctime>
 #include <iostream>
 #include <string>
 #include <boost/bind.hpp>
 #include <boost/shared_ptr.hpp>
 #include <boost/enable_shared_from_this.hpp>
 #include <boost/asio.hpp>
 
 #include "tcp_server.h"
 
 
 int main() {
 using boost::asio::ip::tcp;
 namespace com = communication;
 
 try {
 boost::asio::io_service io_service;
 com::tcp_server server(io_service);
 io_service.run();
 }
 catch (std::exception& e) {
 std::cerr << e.what() << std::endl;
 }
 
 return 0;
 }
 */



#include <ctime>
#include <iostream>
#include <string>
#include <boost/asio.hpp>

#include "tcp_server.hpp"
#include "proxy.hpp"
#include "game.hpp"
#include "udp_server.hpp"


int main() {
    
//    using boost::asio::ip::tcp;
//    using boost::asio::ip::udp;
    namespace com = communication;
    

//    try {
//        boost::asio::io_service io_service;
//        
//        std::shared_ptr<com::proxy> proxy = std::make_shared<com::proxy>();
//        std::shared_ptr<com::tcp_server> server = std::make_shared<com::tcp_server>(io_service, proxy);
//        std::shared_ptr<game::game> game = std::make_shared<game::game>(proxy);
//        proxy->set_gateway(server);
//        proxy->set_listener(game);
//        
//        
//        io_service.run();
//
//    } catch (std::exception& e) {
//        std::cerr << e.what() << std::endl;
//    }
    
    try {
        boost::asio::io_service io_service;
        
        std::shared_ptr<com::proxy> proxy = std::make_shared<com::proxy>();
        std::shared_ptr<com::udp_server> server = std::make_shared<com::udp_server>(io_service, proxy);
        std::shared_ptr<game::game> game = std::make_shared<game::game>(proxy);
        proxy->set_gateway(server);
        proxy->set_listener(game);
        
        
        io_service.run();
        
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    
//    try {
//        boost::asio::io_service io_service;
//        
//        std::shared_ptr<com::client_proxy> client_proxy;
//        com::tcp_client client(io_service, client_proxy);
////        client_proxy->set_gateway(std::make_shared<com::tcp_client>(client));
//        
//        
//        tcp::resolver r(io_service);
//        client.start(r.resolve(tcp::resolver::query(boost::asio::ip::address::from_string("192.168.178.199").to_string(), "1337")));
//        
//        io_service.run();
//    } catch (std::exception& e) {
//        std::cerr << "Exception: " << e.what() << "\n";
//    }
    
    return 0;
}

//#include "serializer.h"
//#include "deserializer.h"
//#include "key_event.h"
//#include "client.h"
//#include "connect.h"
//
//#include <utility>
//
//namespace com = communication;
//namespace msg = communication::message;
//
//int main() {
//    
//    
//    std::vector<unsigned char> key_message = com::serializer::serialize(msg::key_event(constants::key::right, constants::event::press, communication::client(23)));
//    std::vector<unsigned char> connect_message = com::serializer::serialize(msg::connect(communication::client(23), "Hallo Mikey to kleina SPLASSSHHAA"));
//    
//    
//    boost::asio::streambuf buffer;
//    std::ostream request_stream(&buffer);
//    
//    request_stream.write(reinterpret_cast<char*>(key_message.data()), key_message.size());
//    std::pair<communication::message::message_id, size_t> header = com::deserializer::deserialize_header(buffer);
//    std::shared_ptr<msg::key_event> key_event = std::static_pointer_cast<msg::key_event>(com::deserializer::deserialize(buffer, header.first));
//    
//    request_stream.write(reinterpret_cast<char*>(connect_message.data()), connect_message.size());
//    header = com::deserializer::deserialize_header(buffer);
//    std::shared_ptr<msg::connect> connect = std::static_pointer_cast<msg::connect>(com::deserializer::deserialize(buffer, header.first));
//    
//    return 0;
//}










