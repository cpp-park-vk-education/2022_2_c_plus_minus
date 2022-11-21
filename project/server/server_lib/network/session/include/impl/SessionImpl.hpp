#pragma once

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <memory>

#include "Request.hpp"
#include "SafeQueue.hpp"
#include "User.hpp"
#include "Session.hpp"
#include "Task.hpp"

class Connection;
class GameRoom;
class Room;
class RoomController;

class SessionImpl : public std::enable_shared_from_this<SessionImpl>, public Session{
   public:
    SessionImpl(std::shared_ptr<boost::asio::io_context> io_ctx,
                std::shared_ptr<boost::asio::ssl::context> ssl_ctx,
                boost::asio::ip::tcp::socket&& socket,
                std::shared_ptr<RoomController> room_ctrl);

    ~SessionImpl();

    void BindConnection() override;

    std::uint64_t GetId() const;
    const User& GetUser() const;
    const Room& GetRoom() const;
    const GameRoom& GetGameRoom() const;
    std::vector<std::shared_ptr<Room>> GetAllRooms() const;

    // for every request in queue call private method HandleRequest
    void HandleRequests() override;

    void Send(std::string message) override;
    void SendAllExcept(const std::string& message) override;

    std::uint64_t NewRoom(const std::string& name, std::size_t max_user_count) override;

    std::vector<uint64_t> StartGame();
    std::int64_t Move(std::uint64_t figureId, std::uint64_t pos_x,
                          std::uint64_t pos_y);

    void DeleteFromRoomController();

    void StopConnection() override;

   private:
    void HandleRequest(Request&& request) override;

   private:
    // bind session with user, room and connection
    User user_;
    std::shared_ptr<RoomController> room_ctrl_;
    std::shared_ptr<SafeQueue<Request>> req_queue_;
    std::shared_ptr<boost::asio::io_context> io_ctx_;
    std::shared_ptr<Connection> connection_;
};
