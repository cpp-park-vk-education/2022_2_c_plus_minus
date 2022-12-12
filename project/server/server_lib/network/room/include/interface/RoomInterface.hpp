#pragma once

#include <boost/asio/ip/tcp.hpp>

#include "ClientData.hpp"

class RoomInterface {
   public:
    virtual void addClient(const ClientData& clientData) = 0;
    virtual const ClientData* removeClient(const std::string& id) = 0;
    virtual boost::asio::ip::tcp::socket& getClientSocket(
        const std::string& id) = 0;
    virtual bool haveClient(const std::string& id) = 0;
    virtual const ClientData* getClient(const std::string& id) = 0;
};