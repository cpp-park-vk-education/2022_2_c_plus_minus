#include "Client.hpp"
#include "Connection.hpp"

#include <iostream>

Client::Client(
    std::shared_ptr<boost::asio::io_context> io
    )
    : io_ctx_{ io } , connection_ { nullptr }, signals_(*io_ctx_)
{
    signals_.add(SIGINT);
    signals_.add(SIGTERM);
    signals_.async_wait(boost::bind(&Client::CloseConnection, this));
}

Client::~Client() {
    if(state_ != State::CLOSED) {
        this->CloseConnection();
    }
}

void Client::GetAllRooms() {
    GetRoomsRequest req;
    auto string = req.toJSON();
    Write(string);
}

void Client::Authorise(const std::string& nick) {
    AuthRequest req(nick);
    auto string = req.toJSON();
    Write(string);
}

void Client::EnterRoom(const std::string& id) {
    EnterRoomRequest req(id);
    auto string = req.toJSON();
    Write(string);
}

void Client::MoveFigure(const std::string& fromTo) {
    MoveFigureRequest req(fromTo);
    auto string = req.toJSON();
    Write(string);
}

void Client::CreateRoom(const std::string& name) {
    CreateRoomRequest req(name);
    auto string = req.toJSON();
    Write(string);
}

void Client::Connect(std::string_view path, std::string_view port) {
    connection_ = std::make_shared<client::Connection>(this->weak_from_this(), io_ctx_);
    connection_->Connect(path, port);
}

void Client::Write(std::string text) {
    connection_->Write(std::move(text));
}

void Client::SetState(State state) noexcept {
    std::lock_guard<std::mutex> lock{ mutex_ };
    state_ = state;
}

Client::State Client::GetState() const noexcept {
    std::lock_guard<std::mutex> lock{ mutex_ };
    return state_;
}

void Client::CloseConnection() {
    connection_->Close();
}

void Client::HandleMessage(Response&& response) {
    response_ = std::move(response);
    std::lock_guard<std::mutex> lock{ mutex_ };
    switch(response_.resp_type) {
        case QueryType::UNDEFINED:
            std::cout << "undefined";
            break ;
        case QueryType::CREATE_ROOM:
            handleCreateRoom(response_.resp_data);
            break ;
        case QueryType::MOVE_FIGURE:
            handleMoveFigure(response.resp_data);
            break ;
        case QueryType::START_GAME:
            handleStartGame(response.resp_data);
            break;
        case QueryType::LEAVE_ROOM:
            std::cout << "leave";
            break ;
        case QueryType::ENTER_ROOM:
            handleEnterRoom(response_.resp_data);
            break ;
        case QueryType::AUTHORISE:
            handleAuth(response_.resp_data);
            break ;
        case QueryType::GET_ROOMS:
            handleGetAllRooms(response_.resp_data);
            break;
        default: break;
    }
}

void Client::handleCreateRoom(const std::string& data) {
    std::cout << "handling creating room ... " << std::endl;
}

void Client::handleAuth(const std::string& data) {
    std::cout << "handling authorising  ... " << std::endl;
    AuthResponse response;
    response.parse(data);
    std::cout << "Message: " << response.message
              << " Status: " << response.status << std::endl;
}

void Client::handleEnterRoom(const std::string& data) {
    std::cout << "handling entering room ... " << std::endl;
}
void Client::handleGetAllRooms(const std::string& data) {
    std::cout << "handling getting rooms ... " << std::endl;
}

void Client::handleMoveFigure(const std::string& data) {
    std::cout << "handling moving figure ... " << std::endl;
    GameResponse response;
    response.parse(data);
    if (response.moveStatus != MOVE_ERROR) {
        if (response.moveStatus == MOVE_CHECK_WHITE) {
            std::cout << "Check white" << std::endl;
        }
        if (response.moveStatus == MOVE_CHECKMATE_WHITE) {
            std::cout << "Check and Mate white" << std::endl;
        }
        if (response.moveStatus == MOVE_CHECK_BLACK) {
            std::cout << "Check black" << std::endl;
        }
        if (response.moveStatus == MOVE_CHECKMATE_BLACK) {
            std::cout << "Check mate black" << std::endl;
        }
        if (response.moveStatus == MOVE_OK) {
            std::cout << "Successful move" << std::endl;
        }
    } else if (response.moveStatus = MOVE_ERROR) {
        std::cout << "Wrong move" << std::endl;
    }
}

void Client::handleStartGame(const std::string& data) {
    std::cout << "handling starting game... " << std::endl;
}

//#include "Client.hpp"
//
//#include <boost/bind.hpp>
//#include <iostream>
//
//Client::Client(boost::asio::io_context& io_context, const std::string& server,
//               const std::string& port)
//    : resolver_(io_context), strand_(io_context), socket_(io_context), mutable_buffer_(read_buffer_.prepare(1024))
//{
////    read_buffer_.prepare(1024);
//
//    resolver_.async_resolve(server, port,
//                            boost::bind(&Client::handleResolve, this,
//                                        boost::asio::placeholders::error,
//                                        boost::asio::placeholders::results));
//}
//
//Client::~Client() {
//    close();
//}
//
//void Client::write() {
//
//}
//
//void Client::getAllRooms() {
//    GetRoomsRequest req;
//    auto string = req.toJSON();
//    boost::asio::async_write(socket_,
//                             boost::asio::buffer(string.data(), string.size()),
//                             boost::asio::bind_executor(
//                                 strand_,
//                                 std::bind(&Client::handleWrite,
//                                           this,
//                                           std::placeholders::_1,
//                                           std::placeholders::_2
//                                           )
//                                     )
//    );
////                             boost::bind(&Client::handleWrite, this,
////                                         boost::asio::placeholders::error));
//}
//
//void Client::authorise(const std::string& nick) {
//    AuthRequest req(nick);
//    auto string = req.toJSON();
//    boost::asio::async_write(socket_,
//                             boost::asio::buffer(string.data(), string.size()),
//                             boost::asio::bind_executor(
//                                 strand_,
//                                 std::bind(&Client::handleWrite,
//                                           this,
//                                           std::placeholders::_1,
//                                           std::placeholders::_2
//                                           )
//                                     )
//    );
////                             boost::bind(&Client::handleWrite, this,
////                                         boost::asio::placeholders::error));
//}
//
//void Client::enterRoom(const std::string& id) {
//    EnterRoomRequest req(id);
//    auto string = req.toJSON();
//    boost::asio::async_write(socket_,
//                             boost::asio::buffer(string.data(), string.size()),
//                             boost::asio::bind_executor(
//                                 strand_,
//                                 std::bind(&Client::handleWrite,
//                                           this,
//                                           std::placeholders::_1,
//                                           std::placeholders::_2
//                                           )
//                                     )
//    );
//
////                             boost::bind(&Client::handleWrite, this,
////                                         boost::asio::placeholders::error));
//}
//
//void Client::moveFigure(const std::string& fromTo) {
//    MoveFigureRequest req(fromTo);
//    auto string = req.toJSON();
//    boost::asio::async_write(socket_,
//                             boost::asio::buffer(string.data(), string.size()),
//                             boost::asio::bind_executor(
//                                 strand_,
//                                 std::bind(&Client::handleWrite,
//                                           this,
//                                           std::placeholders::_1,
//                                           std::placeholders::_2
//                                           )
//                                     )
//    );
////                             boost::bind(&Client::handleWrite, this,
////                                         boost::asio::placeholders::error));
//}
//
//void Client::createRoom(const std::string& name) {
//    CreateRoomRequest req(name);
//    auto string = req.toJSON();
//    boost::asio::async_write(socket_,
//                             boost::asio::buffer(string.data(), string.size()),
//                             boost::asio::bind_executor(
//                                 strand_,
//                                 std::bind(&Client::handleWrite,
//                                           this,
//                                           std::placeholders::_1,
//                                           std::placeholders::_2
//                                           )
//                                     )
//    );
////                             boost::bind(&Client::handleWrite, this,
////                                         boost::asio::placeholders::error));
//}
//
//void Client::handleWrite(const boost::system::error_code& err, std::size_t bytes_transferred) {
//
//    boost::asio::async_read_until(
//        socket_, read_buffer_, separator,
//        boost::asio::bind_executor(
//            strand_,
//            std::bind(&Client::handleRead,
//                      this,
//                      std::placeholders::_1,
//                      std::placeholders::_2
//                      )));
////        boost::bind(&Client::handleRead, this, boost::asio::placeholders::error,
////                    boost::asio::placeholders::bytes_transferred));
//}
//
//void Client::handleResolve(
//    const boost::system::error_code& err,
//    const boost::asio::ip::tcp::resolver::results_type& endpoints) {
//    if (!err) {
//        boost::asio::async_connect(
//            socket_, endpoints,
//            boost::bind(&Client::handleConnect, this,
//                        boost::asio::placeholders::error));
//    } else {
//        std::cout << "Error: " << err.message() << "\n";
//    }
//}
//
//void Client::handleConnect(const boost::system::error_code& err) {
//    if (!err) {
//        boost::asio::async_read_until(
//            socket_, read_buffer_, separator,
//            boost::asio::bind_executor(
//            strand_,
//            std::bind(&Client::handleRead,
//                this,
//                std::placeholders::_1,
//                std::placeholders::_2
//            )));
////            boost::bind(&Client::handleRead, this,
////                        boost::asio::placeholders::error,
////                        boost::asio::placeholders::bytes_transferred));
//    } else {
//        std::cout << "Error: " << err.message() << "\n";
//    }
//}
//
//void Client::handleRead(const boost::system::error_code& err,
//                        std::size_t bytes_transferred) {
//    if (!err) {
//        const auto data = read_buffer_.data();
//        std::string str;
//        {
//            std::lock_guard<std::mutex> lock(queue_mutex_);
//            str = std::string(boost::asio::buffers_begin(data),
//                            boost::asio::buffers_begin(data) +
//                                bytes_transferred - separator.size());
//            read_buffer_.consume(bytes_transferred);
//        }
////        read_buffer_.commit(bytes_transferred);
//
//        boost::system::error_code socketErr;
//        std::cout << "Recieved: " << str << std::endl;
//        boost::json::object json_object = boost::json::parse(str).get_object();
//        std::string method = json_object["type"].as_string().c_str();
//        std::string responseData;
//        try {
//            responseData = json_object["data"].as_string().c_str();
//        } catch (...) {
//            // in some cases data is empty
//        }
//
//        if (method == "CreateRoom") {
//            handleCreateRoom(responseData);
//        } else if (method == "GetRooms") {
//            handleGetAllRooms(responseData);
//        } else if (method == "Authorise") {
//            handleAuth(responseData);
//        } else if (method == "EnterRoom") {
//            handleEnterRoom(responseData);
//        } else if (method == "StartGame") {
//            handleStartGame(responseData);
//        } else if (method == "MoveFigure") {
//            handleMoveFigure(responseData);
//        } else if (method == "NewAction") {
//            // handle
//        } else {
//            std::cout << "Error: undefined response" << std::endl;
//        }
//
//        boost::asio::async_read_until(
//            socket_, read_buffer_, separator,
//            boost::asio::bind_executor(
//                strand_,
//                std::bind(&Client::handleRead,
//                          this,
//                          std::placeholders::_1,
//                          std::placeholders::_2
//                          )));
////            boost::bind(&Client::handleRead, this,
////                        boost::asio::placeholders::error,
////                        boost::asio::placeholders::bytes_transferred));
//    } else {
//        std::cout << "Error: " << err.message() << "\n";
//    }
//}
//
//void Client::handleCreateRoom(const std::string& data) {
//    std::cout << "handling creating room ... " << std::endl;
//}
//
//void Client::handleAuth(const std::string& data) {
//    std::cout << "handling authorising  ... " << std::endl;
//    AuthResponse response;
//    response.parse(data);
//    std::cout << "Message: " << response.message
//              << " Status: " << response.status << std::endl;
//}
//
//void Client::handleEnterRoom(const std::string& data) {
//    std::cout << "handling entering room ... " << std::endl;
//}
//void Client::handleGetAllRooms(const std::string& data) {
//    std::cout << "handling getting rooms ... " << std::endl;
//}
//void Client::handleMoveFigure(const std::string& data) {
//    std::cout << "handling moving figure ... " << std::endl;
//    game_response_.parse(data);
//    if (game_response_.moveStatus != MOVE_ERROR) {
//        if (game_response_.moveStatus == MOVE_CHECK_WHITE) {
//            std::cout << "Check white" << std::endl;
//        }
//        if (game_response_.moveStatus == MOVE_CHECKMATE_WHITE) {
//            std::cout << "Check and Mate white" << std::endl;
//        }
//        if (game_response_.moveStatus == MOVE_CHECK_BLACK) {
//            std::cout << "Check black" << std::endl;
//        }
//        if (game_response_.moveStatus == MOVE_CHECKMATE_BLACK) {
//            std::cout << "Check mate black" << std::endl;
//        }
//        if (game_response_.moveStatus == MOVE_OK) {
//            std::cout << "Successful move" << std::endl;
//        }
//    } else if (game_response_.moveStatus = MOVE_ERROR) {
//        std::cout << "Wrong move" << std::endl;
//    }
//}
//void Client::handleStartGame(const std::string& data) {
//    std::cout << "handling starting game... " << std::endl;
//}
//
//void Client::close() {
//    std::cout << "Closing connection" << std::endl;
//    boost::system::error_code error;
//
//    this->socket_.lowest_layer().shutdown(boost::asio::ip::tcp::socket::shutdown_both, error);
//    //            if (error) {
//    //                this->m_logger->write(LogType::ERROR, "(socket shutdown)", error.message());
//    //            }
//
//    this->socket_.lowest_layer().close(error);
//}