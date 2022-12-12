#include "Client.hpp"

#include <boost/bind.hpp>
#include <iostream>

Client::Client(boost::asio::io_context& io_context, const std::string& server,
               const std::string& port)
    : resolver_(io_context), socket_(io_context) {
    resolver_.async_resolve(server, port,
                            boost::bind(&Client::handleResolve, this,
                                        boost::asio::placeholders::error,
                                        boost::asio::placeholders::results));
}

void Client::getAllRooms() {
    GetRoomsRequest req;
    auto string = req.toJSON();
    boost::asio::async_write(socket_,
                             boost::asio::buffer(string.data(), string.size()),
                             boost::bind(&Client::handleWrite, this,
                                         boost::asio::placeholders::error));
}
void Client::enterRoom(const std::string& id) {
    EnterRoomRequest req(id);
    auto string = req.toJSON();
    boost::asio::async_write(socket_,
                             boost::asio::buffer(string.data(), string.size()),
                             boost::bind(&Client::handleWrite, this,
                                         boost::asio::placeholders::error));
}

void Client::moveFigure(const std::string& fromTo) {
    MoveFigureRequest req(fromTo);
    auto string = req.toJSON();
    boost::asio::async_write(socket_,
                             boost::asio::buffer(string.data(), string.size()),
                             boost::bind(&Client::handleWrite, this,
                                         boost::asio::placeholders::error));
}

void Client::createRoom(const std::string& name) {
    CreateRoomRequest req(name);
    auto string = req.toJSON();
    boost::asio::async_write(socket_,
                             boost::asio::buffer(string.data(), string.size()),
                             boost::bind(&Client::handleWrite, this,
                                         boost::asio::placeholders::error));
}

void Client::handleWrite(const boost::system::error_code& err) {
    socket_.async_read_some(
        boost::asio::buffer(m_readBuffer),
        boost::bind(&Client::handleRead, this, boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
}

void Client::handleResolve(
    const boost::system::error_code& err,
    const boost::asio::ip::tcp::resolver::results_type& endpoints) {
    if (!err) {
        boost::asio::async_connect(
            socket_, endpoints,
            boost::bind(&Client::handleConnect, this,
                        boost::asio::placeholders::error));
    } else {
        std::cout << "Error: " << err.message() << "\n";
    }
}

void Client::handleConnect(const boost::system::error_code& err) {
    if (!err) {
        socket_.async_read_some(
            boost::asio::buffer(m_readBuffer),
            boost::bind(&Client::handleRead, this,
                        boost::asio::placeholders::error,
                        boost::asio::placeholders::bytes_transferred));
    } else {
        std::cout << "Error: " << err.message() << "\n";
    }
}

void Client::handleRead(const boost::system::error_code& err,
                        std::size_t bytes_transferred) {
    if (!err) {
        m_readBuffer[bytes_transferred] = '\0';
        std::cout << "Read: " << m_readBuffer.data() << std::endl;
        boost::json::object json_object =
            boost::json::parse(m_readBuffer.data()).get_object();
        std::string method = json_object["type"].as_string().c_str();
        std::string responseData;
        try {
            responseData = json_object["data"].as_string().c_str();
        } catch (...) {
            // in some cases data is empty
        }

        if (method == "CreateRoom") {
            handleCreateRoom(responseData);
        } else if (method == "GetRooms") {
            handleGetAllRooms(responseData);
        } else if (method == "EnterRoom") {
            handleEnterRoom(responseData);
        } else if (method == "StartGame") {
            handleStartGame(responseData);
        } else if (method == "MoveFigure") {
            handleMoveFigure(responseData);
        } else if (method == "NewAction") {
            game_response_.parse(responseData);
            // handle
        } else {
            std::cout << "Error: undefined response" << std::endl;
        }

        socket_.async_read_some(
            boost::asio::buffer(m_readBuffer),
            boost::bind(&Client::handleRead, this,
                        boost::asio::placeholders::error,
                        boost::asio::placeholders::bytes_transferred));
    } else {
        std::cout << "Error: " << err.message() << "\n";
    }
}

void Client::handleCreateRoom(const std::string& data) {
    std::cout << "handling creating room ... " << std::endl;
}

void Client::handleEnterRoom(const std::string& data) {
    std::cout << "handling entering room ... " << std::endl;
}
void Client::handleGetAllRooms(const std::string& data) {
    std::cout << "handling getting rooms ... " << std::endl;
}
void Client::handleMoveFigure(const std::string& data) {
    std::cout << "handling moving figure ... " << std::endl;
    game_response_.parse(data);
    if (game_response_.moveStatus != MOVE_ERROR) {
        if (game_response_.moveStatus == MOVE_CHECK_WHITE) {
            std::cout << "Check white" << std::endl;
        }
        if (game_response_.moveStatus == MOVE_CHECKMATE_WHITE) {
            std::cout << "Check and Mate white" << std::endl;
        }
        if (game_response_.moveStatus == MOVE_CHECK_BLACK) {
            std::cout << "Check black" << std::endl;
        }
        if (game_response_.moveStatus == MOVE_CHECKMATE_BLACK) {
            std::cout << "Check mate black" << std::endl;
        }
        if (game_response_.moveStatus == MOVE_OK) {
            std::cout << "Successful move" << std::endl;
        }
    } else if (game_response_.moveStatus = MOVE_ERROR) {
        std::cout << "Wrong move" << std::endl;
    }
}
void Client::handleStartGame(const std::string& data) {
    std::cout << "handling starting game... " << std::endl;
}
