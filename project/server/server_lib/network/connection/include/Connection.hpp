//#ifndef Connection_H
//#define Connection_H
#pragma once

#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/noncopyable.hpp>

#include "ClientData.hpp"
#include "Router.hpp"

#define MAX_MESSAGE_SIZE 2048

class Connection : public boost::enable_shared_from_this<Connection> {
   public:
    Connection(boost::asio::io_context& ioContext, BasicMenu& mainMenu,
               Router& router);

    boost::asio::ip::tcp::socket& getSocket();

    void start();

    ~Connection() = default;

   private:
    void handleRead(const boost::system::error_code& err,
                    std::size_t bytes_transferred);
    void handleWrite(const boost::system::error_code& err);

    boost::asio::strand<boost::asio::io_context::executor_type> strand_;
    boost::asio::ip::tcp::socket socket_;

    ClientData client_data_;
    BasicMenu& basic_menu_;
    Router& router_;
    boost::array<char, MAX_MESSAGE_SIZE> read_buffer_;
};

//#endif // Connection_H
