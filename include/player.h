//
//  player.h
//  stronghold_server
//
//  Created by Mikael Beyene on 18/08/15.
//
//

#ifndef __stronghold_server__player__
#define __stronghold_server__player__

#include "client.h"
#include <string>

namespace game {
    
    class player {
    public:
        player() {}
        player(std::string user_name, communication::client client): user_name(user_name), client(client) {}
        auto get_username() -> std::string;
        auto get_client() -> communication::client;
    private:
        std::string user_name;
        communication::client client;
    };
    
}

#endif /* defined(__stronghold_server__player__) */