#pragma once

#include "Connection.h"

// interface
class ConnectionHandler {
public:
    virtual ~ConnectionHandler() = default;
    virtual void handleConnect(Connection) = 0;
    virtual void handleDisconnect(Connection) = 0;
};
