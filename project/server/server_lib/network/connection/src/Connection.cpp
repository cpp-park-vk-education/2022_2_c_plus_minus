#include "Connection.hpp"

#include <boost/bind/bind.hpp>
#include <iostream>

Connection::Connection(boost::asio::io_context& ioContext, BasicMenu& menu,
                       Router& router)
    : strand_(boost::asio::make_strand(ioContext)),
      socket_(strand_),
      user_(socket_),
      basic_menu_(menu),
      router_(router) {}

Connection::~Connection() { close(); }

boost::asio::ip::tcp::socket& Connection::getSocket() { return socket_; }

void Connection::start() {
    user_.id = socket_.remote_endpoint().address().to_string() + ":" +
               std::to_string(socket_.remote_endpoint().port());
    user_.nickname = "Unidentified turtle"; // by default
    user_.position = {Location::MainMenu, ""};
    basic_menu_.addClient(user_);

    boost::asio::async_read_until(
        socket_, read_buffer_, separator,
        boost::asio::bind_executor(
            strand_,
            boost::bind(&Connection::handleRead, shared_from_this(),
                        boost::placeholders::_1, boost::placeholders::_2)));
}

void Connection::handleRead(const boost::system::error_code& err,
                            std::size_t bytes_transferred) {
    if (!err) {
        const auto data = read_buffer_.data();
        std::string str(boost::asio::buffers_begin(data),
                        boost::asio::buffers_begin(data) + bytes_transferred -
                            separator.size());
        read_buffer_.consume(bytes_transferred);
        boost::system::error_code socketErr;
        std::cout << "Recieved (" << user_.id << "): " << str << std::endl;

        std::string writeBuffer = router_.process(str, user_, basic_menu_);

        std::cout << "Write (" << user_.id << "): " << writeBuffer << std::endl;

        boost::asio::async_write(
            socket_,
            boost::asio::buffer(writeBuffer.data(), writeBuffer.size()),
            boost::bind(&Connection::handleWrite, shared_from_this(),
                        boost::placeholders::_1, boost::placeholders::_2));
    } else {
        std::cout << "Сonnection was broken: " << user_.id << std::endl;
    }
}

void Connection::handleWrite(const boost::system::error_code& error,
                             std::size_t transferredBytes) {
    boost::asio::async_read_until(
        socket_, read_buffer_, separator,
        boost::asio::bind_executor(
            strand_,
            boost::bind(&Connection::handleRead, shared_from_this(),
                        boost::placeholders::_1, boost::placeholders::_2)));
}

void Connection::close() {
    boost::asio::post(strand_, [this]() {
        //            connection->m_logger->write(LogType::INFO, "Closing the
        //            connection...");
        std::cout << "Closing connection" << std::endl;
        boost::system::error_code error;

        this->socket_.lowest_layer().shutdown(
            boost::asio::ip::tcp::socket::shutdown_both, error);
        //            if (error) {
        //                this->m_logger->write(LogType::ERROR, "(socket
        //                shutdown)", error.message());
        //            }

        this->socket_.lowest_layer().close(error);
        //            if (error) {
        //                connection->m_logger->write(LogType::ERROR, "(closing
        //                the socket)", error.message());
        //            }
        this->basic_menu_.room_manager_.deleteFromRoom(user_.id);
    });
}
