#pragma once

#include <boost/asio/ip/tcp.hpp>

#include "User.hpp"

class RoomInterface {
   public:
    virtual void addClient(const User& user) = 0;
    virtual const User* removeClient(const std::string& id) = 0;
    virtual boost::asio::ip::tcp::socket& getClientSocket(
        const std::string& id) = 0;
    virtual bool haveClient(const std::string& id) = 0;
    virtual const User* getClient(const std::string& id) = 0;
};