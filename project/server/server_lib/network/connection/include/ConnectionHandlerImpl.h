#pragma once

#include <memory>
#include "ConnectionHandler.h"

template <typename C, typename D>
class ConnectionHandlerImpl final : public ConnectionHandler {
public:
    ConnectionHandlerImpl(C onConnect, D onDisconnect)
            : onConnect{std::move(onConnect)},
              onDisconnect{std::move(onDisconnect)}
    { }
    ~ConnectionHandlerImpl() override = default;
    void handleConnect(Connection c)    override { onConnect(c);    }
    void handleDisconnect(Connection c) override { onDisconnect(c); }
private:
    C onConnect;
    D onDisconnect;
};