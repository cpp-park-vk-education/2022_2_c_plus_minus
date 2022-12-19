#pragma once

#include <memory>
#include <string_view>
#include <string>
#include <mutex>
#include "Response.hpp"
#include <boost/asio.hpp>
#include "RoomRequest.hpp"
#include "GameRequest.hpp"
#include "GameResponse.hpp"
#include "RoomResponse.hpp"


namespace client {
    class Connection;
}


class Client final
    : public std::enable_shared_from_this<Client> {
   public:
    enum class State {
        CLOSED,
        CONNECTED,
    };

   public:

    Client(
        std::shared_ptr<boost::asio::io_context> io
    );

    Client(const Client&) = delete;
    Client& operator=(const Client&) = delete;

    ~Client();
    void HandleMessage(Response&&);
    void Write(std::string text);
    void GetAllRooms();
    void CreateRoom(const std::string& name);
    void EnterRoom(const std::string& id);
    void MoveFigure(const std::string& fromTo);
    void Authorise(const std::string& nick);

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

    std::shared_ptr<boost::asio::io_context>    io_ctx_ { nullptr };
    std::shared_ptr<client::Connection>       connection_ { nullptr };
    Response response_;
    State state_ { State::CLOSED };
    mutable std::mutex mutex_;
    boost::asio::signal_set signals_;
};

//#pragma once
//
//#include <boost/array.hpp>
//#include <boost/asio.hpp>
//#include <boost/enable_shared_from_this.hpp>
//
//#include "GameRequest.hpp"
//#include "GameResponse.hpp"
//#include "RoomRequest.hpp"
//#include "Buffer.hpp"
//
//
//
//class Client : public boost::enable_shared_from_this<Client> {
//   public:
//    Client(boost::asio::io_context& io_context, const std::string& server,
//           const std::string& port);
//    void getAllRooms();
//    void createRoom(const std::string& name);
//    void enterRoom(const std::string& id);
//    void moveFigure(const std::string& fromTo);
//    void authorise(const std::string& nick);
//    ~Client();
//
//   private:
//    void handleResolve( const boost::system::error_code& err,
//        const boost::asio::ip::tcp::resolver::results_type& endpoints);
//    void handleConnect(const boost::system::error_code& err);
//    void handleWrite(const boost::system::error_code& err,
//                     std::size_t bytes_transferred);
//    void handleRead(const boost::system::error_code& err,
//                    std::size_t bytes_transferred);
//
//    void handleCreateRoom(const std::string& data);
//    void handleEnterRoom(const std::string& data);
//    void handleGetAllRooms(const std::string& data);
//    void handleMoveFigure(const std::string& data);
//    void handleStartGame(const std::string& data);
//    void handleAuth(const std::string& data);
//    void close();
//    void write();
//
//    GameResponse game_response_;
//    boost::asio::ip::tcp::resolver resolver_;
//    boost::asio::io_context::strand strand_;
//    boost::asio::ip::tcp::socket socket_;
//    mutable std::mutex queue_mutex_;
//    boost::asio::streambuf read_buffer_;
//    boost::asio::streambuf::mutable_buffers_type mutable_buffer_;
//    Buffers m_outbox;
//};
