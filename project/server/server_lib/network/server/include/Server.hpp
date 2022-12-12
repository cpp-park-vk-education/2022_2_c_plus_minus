#pragma once
#include <boost/asio.hpp>
#include <string>

#include "Connection.hpp"
#include "BasicMenu.hpp"
#include "Router.hpp"

class Server {
   public:
    Server(const std::string address, const std::string port,
           std::size_t threadNum);

    void run();

    ~Server() = default;

   private:
    void accept();
    void handleAccept(const boost::system::error_code& err);
    void handleStop();

    std::size_t thread_count_;
    boost::asio::io_context io_ctx_;
    boost::asio::signal_set signals_;
    boost::asio::ip::tcp::acceptor acceptor_;
    boost::shared_ptr<Connection> client_;
    BasicMenu basic_menu_;
    Router router_;
};