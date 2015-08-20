//
//  communication_proxy.h
//  stronghold_server
//
//  Created by Mikael Beyene on 16/08/15.
//
//

#ifndef __stronghold_server__communication_proxy__
#define __stronghold_server__communication_proxy__

#include "relay.h"
#include "mediator.h"
#include "listener.h"
#include "gateway.h"
#include <memory>

namespace communication {
    
    class proxy: public virtual mediator, virtual public relay {
    public:
        proxy() {}
        virtual void relay_message(std::shared_ptr<msg::message> m, client c) override;
        virtual void key_event(constants::key key, constants::event event, client c) override;
        virtual void update_listener() override;
        
        void set_gateway(std::weak_ptr<gateway> gateway);
        void set_listener(std::weak_ptr<listener>);
    private:
        std::weak_ptr<gateway> gateway;
        std::weak_ptr<listener> listener;
        
    };
    
}

#endif /* defined(__stronghold_server__communication_proxy__) */