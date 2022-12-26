#pragma once
#include <boost/asio.hpp>
#include <string>

#include "BasicMenu.hpp"
#include "Connection.hpp"
#include "Logger.hpp"
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
    boost::shared_ptr<Connection> connection_;
    BasicMenu basic_menu_;
    Router router_;
    Log logger_{"server_log.txt"};
};