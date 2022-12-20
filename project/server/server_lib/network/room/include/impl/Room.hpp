#pragma once

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/write.hpp>
#include <map>
#include <string>

#include "Base.hpp"
#include "GameSession.hpp"
#include "RoomInterface.hpp"

class Room : public RoomInterface {
   public:
    Room(const std::string& roomId, const std::string& name,
         const std::string& host_id, const figure_color& host_color,
         unsigned int maxClientNumber);

    unsigned int getCurrentClientNumber();
    unsigned int getMaxClientNumber();
    std::string getId();
    std::string getName();

    void addClient(const User& user, const figure_color& color);
    void addClient(const User& user) override;
    const User* removeClient(const std::string& id) override;
    boost::asio::ip::tcp::socket& getClientSocket(
        const std::string& id) override;
    bool haveClient(const std::string& id) override;
    const User* getClient(const std::string& id) override;

    std::string getHostId();
    figure_color getHostColor();
    const User* getHost();
    void broadcast(const std::string& id, QueryType method,
                   const std::string& data = "");
    void write(const std::string& id, QueryType method,
               const std::string& data = "");

    void startGame();
    bool gameStarted();
    move_response makeAction(const std::string& id,
                                 const std::string& action);
    void onEnd();

    ~Room();

   private:
    std::string id_;
    std::string name_;
    std::string host_id_;
    figure_color host_color_;

    std::map<const std::string, std::pair<const User*, figure_color>> clients_;
    GameSession* game_session_;
    unsigned int current_count_ = 0;
    unsigned int max_count_;
    bool game_started_ = false;
};