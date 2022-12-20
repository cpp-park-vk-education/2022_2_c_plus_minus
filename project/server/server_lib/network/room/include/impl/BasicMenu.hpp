#pragma once

#include <boost/asio/ip/tcp.hpp>
#include <map>

#include "RoomInterface.hpp"
#include "RoomManager.hpp"

class BasicMenu : public RoomInterface {
   public:
    BasicMenu() = default;

    void addClient(const User& user) override;
    const User* removeClient(const std::string& id) override;
    boost::asio::ip::tcp::socket& getClientSocket(
        const std::string& id) override;
    bool haveClient(const std::string& id) override;
    const User* getClient(const std::string& id) override;

    ~BasicMenu() = default;

    RoomManager room_manager_;

   private:
        std::map<std::string, const User*> clients_;
};
