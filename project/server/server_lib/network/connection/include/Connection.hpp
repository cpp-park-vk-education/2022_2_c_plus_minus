#pragma once

#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/noncopyable.hpp>

#include "Logger.hpp"
#include "Router.hpp"
#include "SafeQueue.hpp"
#include "User.hpp"

class Connection : public boost::enable_shared_from_this<Connection> {
   public:
    Connection(boost::asio::io_context& ioContext, BasicMenu& mainMenu,
               Router& router);

    boost::asio::ip::tcp::socket& getSocket();

    void start();
    void close();

    ~Connection();

   private:
    void read();

    boost::asio::strand<boost::asio::io_context::executor_type> strand_;
    boost::asio::ip::tcp::socket socket_;

    User user_;
    BasicMenu& basic_menu_;
    Router& router_;
    boost::asio::streambuf read_buffer_;
    std::shared_ptr<Log> logger_{nullptr};
    std::chrono::time_point<std::chrono::system_clock> time_;
};
