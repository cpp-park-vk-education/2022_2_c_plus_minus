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
    time_ = std::chrono::system_clock::now();
    user_.id = socket_.remote_endpoint().address().to_string() + ":" +
               std::to_string(socket_.remote_endpoint().port());
    std::stringstream ss;
    ss << "connection_" << user_.id << "_log.txt";
    logger_ = std::make_shared<Log>(ss.str().c_str());
    user_.nickname = "Unidentified turtle";  // by default
    user_.position = {Location::MainMenu, ""};
    basic_menu_.addClient(user_);
    read();
}


void Connection::read() {
    const boost::system::error_code err;
    std::size_t bytes_transferred =boost::asio::read_until(socket_, read_buffer_, separator);
    const auto data = read_buffer_.data();
    std::string str(boost::asio::buffers_begin(data),
                    boost::asio::buffers_begin(data) + bytes_transferred -
                    separator.size());
    read_buffer_.consume(bytes_transferred);
    boost::system::error_code socketErr;
    logger_->Write(LogType::info, "Recieved (", user_.id, "): ", str, "\n");
    std::string writeBuffer = router_.process(str, user_, basic_menu_);
    logger_->Write(LogType::info, "Write (", user_.id, "): ", writeBuffer,
                   "\n");
    boost::asio::write(
            socket_,
            boost::asio::buffer(writeBuffer.data(), writeBuffer.size()));
    read();
}


void Connection::close() {
    boost::asio::post(strand_, [this]() {
        logger_->Write(LogType::info, "Closing the connection...\n");
        boost::system::error_code error;

        this->socket_.lowest_layer().shutdown(
            boost::asio::ip::tcp::socket::shutdown_both, error);
        if (error) {
            logger_->Write(LogType::error, "(socket shutdown) ",
                           error.message(), "\n");
        }

        this->socket_.lowest_layer().close(error);
        if (error) {
            logger_->Write(LogType::error, "(closing the socket) ",
                           error.message(), "\n");
        }
        this->basic_menu_.room_manager_.deleteFromRoom(user_.id);
    });
}
