#pragma once

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/write.hpp>
#include <map>
#include <string>

#include "ClientData.hpp"
#include "Game.hpp"

class Room : public RoomInterface {
   public:
    Room(const std::string& roomId, const std::string& name,
         unsigned int maxClientNumber);

    unsigned int getCurrentClientNumber();
    unsigned int getMaxClientNumber();
    std::string getId();
    std::string getName();

    void addClient(const ClientData& clientData) override;
    const ClientData* removeClient(const std::string& id) override;
    boost::asio::ip::tcp::socket& getClientSocket(
        const std::string& id) override;
    bool haveClient(const std::string& id) override;
    const ClientData* getClient(const std::string& id) override;

    void broadcast(const std::string& id, const std::string& method,
                   const std::string& data = "");
    void startGame();
    Game& getGame();
    bool gameStarted();

    ~Room();

   private:
    std::string id_;
    std::string name_;

    std::map<const std::string, const ClientData*> clients_;
    unsigned int current_count_ = 0;
    unsigned int max_count_;
    Game game_;
    bool game_started_ = false;
};