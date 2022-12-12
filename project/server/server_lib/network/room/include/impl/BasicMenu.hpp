#pragma once

#include <boost/asio/ip/tcp.hpp>
#include <map>

#include "ClientData.hpp"
#include "RoomInterface.hpp"
#include "RoomManager.hpp"

class BasicMenu : public RoomInterface {
   public:
    BasicMenu() = default;

    void addClient(const ClientData& clientData) override;
    const ClientData* removeClient(const std::string& id) override;
    boost::asio::ip::tcp::socket& getClientSocket(
        const std::string& id) override;
    bool haveClient(const std::string& id) override;
    const ClientData* getClient(const std::string& id) override;

    ~BasicMenu() = default;

   public:
    RoomManager roomController;

   private:
    std::map<std::string, const ClientData*> clients_;
};
