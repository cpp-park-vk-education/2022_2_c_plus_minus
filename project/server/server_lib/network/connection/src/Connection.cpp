#include "../include/Connection.hpp"

#include <boost/bind.hpp>
#include <iostream>

Connection::Connection(boost::asio::io_context& ioContext, BasicMenu& menu,
                       Router& router)
    : strand_(boost::asio::make_strand(ioContext)),
      socket_(strand_),
      client_data_(socket_),
      basic_menu_(menu),
      router_(router) {}

boost::asio::ip::tcp::socket& Connection::getSocket() { return socket_; }

void Connection::start() {
    client_data_.id = socket_.remote_endpoint().address().to_string() + ":" +
                      std::to_string(socket_.remote_endpoint().port());
    // client_data_.nickname = client_data_.id;
    client_data_.position = {Location::MainMenu, ""};
    basic_menu_.addClient(client_data_);

    socket_.async_read_some(
        boost::asio::buffer(read_buffer_),
        boost::bind(&Connection::handleRead, shared_from_this(),
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
}

void Connection::handleRead(const boost::system::error_code& err,
                            std::size_t bytes_transferred) {
    if (!err) {
        read_buffer_[bytes_transferred] = '\0';
        std::cout << "Read (" << client_data_.id << "): " << read_buffer_.data()
                  << std::endl;

        std::string writeBuffer = router_.process(
            std::string(read_buffer_.data()), client_data_, basic_menu_);

        std::cout << "Write (" << client_data_.id << "): " << writeBuffer
                  << std::endl;

        boost::asio::async_write(
            socket_,
            boost::asio::buffer(writeBuffer.data(), writeBuffer.size()),
            boost::bind(&Connection::handleWrite, shared_from_this(),
                        boost::asio::placeholders::error));
    } else {
        std::cout << "Сonnection was broken: " << client_data_.id << std::endl;
    }
}

void Connection::handleWrite(const boost::system::error_code& err) {
    socket_.async_read_some(
        boost::asio::buffer(read_buffer_),
        boost::bind(&Connection::handleRead, shared_from_this(),
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
}
