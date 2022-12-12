#pragma once

#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "GameRequest.hpp"
#include "GameResponse.hpp"
#include "RoomRequest.hpp"

const unsigned MaxMessageSize = 2048;

class Client : public boost::enable_shared_from_this<Client> {
   public:
    Client(boost::asio::io_context& io_context, const std::string& server,
           const std::string& port);
    void getAllRooms();
    void createRoom(const std::string& name);
    void enterRoom(const std::string& id);
    void moveFigure(const std::string& fromTo);
    ~Client() {
        //        close();
    }

   private:
    void handleResolve(
        const boost::system::error_code& err,
        const boost::asio::ip::tcp::resolver::results_type& endpoints);
    void handleConnect(const boost::system::error_code& err);
    void handleWrite(const boost::system::error_code& err);
    void handleRead(const boost::system::error_code& err,
                    std::size_t bytes_transferred);

    void handleCreateRoom(const std::string& data);
    void handleEnterRoom(const std::string& data);
    void handleGetAllRooms(const std::string& data);
    void handleMoveFigure(const std::string& data);
    void handleStartGame(const std::string& data);
    void close();

    GameResponse game_response_;
    boost::asio::ip::tcp::resolver resolver_;
    boost::asio::ip::tcp::socket socket_;
    mutable std::mutex queue_mutex_;
    boost::array<char, MaxMessageSize> m_readBuffer;
};
