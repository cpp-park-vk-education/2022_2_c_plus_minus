#pragma once
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/write.hpp>
#include <map>
#include <string>

#include "ClientData.hpp"
#include "GameSession.hpp"
#include "RoomInterface.hpp"

class Game : public RoomInterface {
   public:
    Game(const std::string& id);

    void addClient(const ClientData& clientData) override;
    const ClientData* removeClient(const std::string& id) override;
    boost::asio::ip::tcp::socket& getClientSocket(
        const std::string& id) override;
    bool haveClient(const std::string& id) override;
    const ClientData* getClient(const std::string& id) override;

    void broadcast(const std::string& id, const std::string& response);
    return_after_move makeAction(const std::string& id,
                                 const std::string& action);
    void start();
    void onEnd();

    ~Game() = default;

   private:
    const std::string id_;
    GameSession* game_session_;
    std::map<const std::string, const ClientData*> clients_;
};