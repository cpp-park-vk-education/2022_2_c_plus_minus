#include "Server.hpp"

#include <boost/bind.hpp>
#include <iostream>

#include "GameHandler.hpp"
#include "RoomHandler.hpp"

Server::Server(const std::string address, const std::string port,
               std::size_t threadNum)
    : thread_count_(threadNum),
      io_ctx_(),
      signals_(io_ctx_),
      acceptor_(io_ctx_),
      client_(),
      basic_menu_() {
    signals_.add(SIGINT);
    signals_.add(SIGTERM);
    signals_.async_wait(boost::bind(&Server::handleStop, this));

    router_.addRoute(
        QueryType::CREATE_ROOM,
        Router::Route{
            new CreateRoomRequest(),
            new CreateRoomHandler(),
            new CreateRoomResponse()});

    router_.addRoute(
        QueryType::ENTER_ROOM,
        Router::Route{new EnterRoomRequest(), new EnterRoomHandler(),
                      new EnterRoomResponse()});
    router_.addRoute(
        QueryType::LEAVE_ROOM,
        Router::Route{new LeaveRoomRequest(), new LeaveRoomHandler(),
                      new LeaveRoomResponse()});
    router_.addRoute(QueryType::GET_ROOMS,
                     Router::Route{new GetRoomsRequest(), new GetRoomsHandler(),
                                   new GetRoomsResponse()});
    router_.addRoute(
        QueryType::START_GAME,
        Router::Route{new StartGameRequest(), new StartGameHandler(),
                      new StartGameResponse()});

    router_.addRoute(QueryType::MOVE_FIGURE,
                     Router::Route{
                        new MoveFigureRequest(),
                        new GameHandler(),
                        new GameResponse()});

    router_.addRoute(QueryType::AUTHORISE,
                     Router::Route{new AuthRequest(), new AuthHandler(),
                                   new AuthResponse()});

    boost::asio::ip::tcp::resolver resolver(io_ctx_);
    boost::asio::ip::tcp::endpoint endpoint =
        *resolver.resolve(address, port).begin();
    acceptor_.open(endpoint.protocol());
    acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
    acceptor_.bind(endpoint);
    acceptor_.listen();

    accept();
    logger_.Write(LogType::info, "Started accept\n");
}

void Server::run() {
    std::vector<boost::shared_ptr<std::thread>> threads;
    for (std::size_t i = 0; i < thread_count_; ++i) {
        boost::shared_ptr<std::thread> thread(new std::thread(
            boost::bind(&boost::asio::io_context::run, &io_ctx_)));
        threads.push_back(thread);
    }

    for (std::size_t i = 0; i < threads.size(); ++i) {
        threads[i]->join();
    }
}

void Server::accept() {
    client_.reset(new Connection(io_ctx_, basic_menu_, router_));
    acceptor_.async_accept(client_->getSocket(),
                           boost::bind(&Server::handleAccept, this,
                                       boost::asio::placeholders::error));
}

void Server::handleAccept(const boost::system::error_code& err) {
    if (!err) {
        boost::asio::ip::tcp::socket& socket = client_->getSocket();
        logger_.Write(LogType::info, "Started handling: ",
                      socket.remote_endpoint().address().to_string(), ":",
                      std::to_string(socket.remote_endpoint().port()), "\n");
        client_->start();
    }

    accept();
}

void Server::handleStop() {
    logger_.Write(LogType::info, "Stopped connection\n");
    io_ctx_.stop();
}