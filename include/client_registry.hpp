#include "client.hpp"
#include <map>
#include <boost/asio/ip/udp.hpp>
#include <atomic>

class client_registry {
private:
	std::atomic<size_t> id_counter = 0;
	std::map<unsigned int, communication::client*> id_to_client;
	std::map<boost::asio::ip::udp::endpoint, communication::client*> endpoint_to_client;
public:
	void register_client(communication::client client);
	void deregister_client(communication::client client);

	communication::client* get_client(unsigned int id);
	communication::client* get_client(boost::asio::ip::udp::endpoint endpoint);
};