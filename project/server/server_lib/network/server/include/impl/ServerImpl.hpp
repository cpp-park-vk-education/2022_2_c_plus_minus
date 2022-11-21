#pragma once

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <memory>
#include <optional>

#include "Controller.hpp"
#include "Server.hpp"

class ServerImpl : public Server {
   public:
    ServerImpl(std::shared_ptr<boost::asio::io_context> ioContext,
               std::uint16_t port);
    ~ServerImpl();

    void Run() override;
    void Stop() override;

   private:
    std::shared_ptr<boost::asio::io_context> io_ctx_;
    std::shared_ptr<boost::asio::ssl::context> ssl_ctx_;
    boost::asio::ip::tcp::acceptor acceptor_;
    std::optional<boost::asio::ip::tcp::socket> socket_;
    std::shared_ptr<Controller> room_ctrl_;
};
