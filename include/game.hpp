//
//  game.h
//  stronghold_server
//
//  Created by Mikael Beyene on 18/08/15.
//
//

#ifndef __stronghold_server__game__
#define __stronghold_server__game__

#include "proxy.hpp"
#include "listener.hpp"
#include "player.hpp"
#include <map>

namespace game {
    
    class game: public virtual communication::listener {
    public:
        game(std::shared_ptr<communication::proxy> proxy): proxy(proxy) {}
        
        virtual void key_event(constants::key key, constants::event event, communication::client c) override;
        virtual void client_connected(std::string user_name, communication::client c) override;
        virtual void client_disconnected(communication::client c) override;
    private:
        std::shared_ptr<communication::proxy> proxy;
        std::map<communication::client, player> players;
    };
    
}

#endif /* defined(__stronghold_server__game__) */