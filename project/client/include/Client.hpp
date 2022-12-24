#pragma once

#include <boost/bind/bind.hpp>
#include <boost/asio.hpp>
#include <memory>
#include <mutex>
#include <string>
#include <string_view>
#include <queue>

#include "GameRequest.hpp"
#include "GameResponse.hpp"
#include "GameState.hpp"
#include "Response.hpp"
#include "RoomRequest.hpp"
#include "RoomResponse.hpp"
#include "TextDrawer.hpp"
#include "Logger.hpp"
//#include "GameUi.hpp"
#include "SafeQueue.hpp"
#include "Chan.hpp"

namespace client {
class Connection;
}

class GameUi;

class Client final : public std::enable_shared_from_this<Client> {
   public:
    enum class State {
        CLOSED,
        CONNECTED,
        PERFORMING_ACTION,
        READY
    };

   public:
    Client(std::shared_ptr<boost::asio::io_context> io);

    Client(const Client&) = delete;
    Client& operator=(const Client&) = delete;

    ~Client();
    void Run();
    void HandleMessage(Response&&);
    void Write(std::string text);
    void GetAllRooms();
    void CreateRoom(const std::string& name, const figure_color& color = figure_color::WHITE);
    void EnterRoom(const std::string& name);
    void LeaveRoom();
    void StartGame();
    move_status MoveFigure(const std::string& fromTo);
    void Authorise();

    void Connect(std::string_view path, std::string_view port);
    void CloseConnection();
    void SetState(State state) noexcept;
    State GetState() const noexcept;

   private:
    void handleCreateRoom(const std::string& data);
    void handleEnterRoom(const std::string& data);
    void handleGetAllRooms(const std::string& data);
    void handleMoveFigure(const std::string& data);
    void handleStartGame(const std::string& data);
    void handleAuth(const std::string& data);
    void handleLeaveRoom(const std::string& data);
    std::shared_ptr<boost::asio::io_context> io_ctx_{nullptr};
    std::shared_ptr<client::Connection> connection_{nullptr};
    Response response_;
    State state_{State::CLOSED};
    mutable std::mutex mutex_;
    boost::asio::signal_set signals_;
    GameState game_;
    std::string nick_ = "Unidentified turtle";
    bool is_authorised = false;
    std::map<std::string, RoomData> rooms_;
    std::atomic_int64_t waiting_responses_ = 0;
    MoveChan chan_;
    std::shared_ptr<GameUi> gameUI_;
};

