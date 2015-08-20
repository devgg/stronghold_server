//
//  player.cpp
//  stronghold_server
//
//  Created by Mikael Beyene on 18/08/15.
//
//

#include "player.h"

namespace game {
    
    auto player::get_username() -> std::string {
        return user_name;
    }
    
    auto player::get_client() -> communication::client {
        return client;
    }
}