#include "../include/Server.hpp"

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

    router_.addRoute("CreateRoom",
                     Route{new CreateRoomRequest(), new CreateRoomHandler(),
                           new CreateRoomResponse()});
    router_.addRoute("EnterRoom",
                     Route{new EnterRoomRequest(), new EnterRoomHandler(),
                           new EnterRoomResponse()});
    router_.addRoute("LeaveRoom",
                     Route{new LeaveRoomRequest(), new LeaveRoomHandler(),
                           new LeaveRoomResponse()});
    router_.addRoute("GetRooms",
                     Route{new GetRoomsRequest(), new GetRoomsHandler(),
                           new GetRoomsResponse()});
    router_.addRoute("StartGame",
                     Route{new StartGameRequest(), new StartGameHandler(),
                           new StartGameResponse()});
    router_.addRoute(
        "MoveFigure",
        Route{new MoveFigureRequest(), new GameHandler(), new GameResponse()});

    boost::asio::ip::tcp::resolver resolver(io_ctx_);
    boost::asio::ip::tcp::endpoint endpoint =
        *resolver.resolve(address, port).begin();
    acceptor_.open(endpoint.protocol());
    acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
    acceptor_.bind(endpoint);
    acceptor_.listen();

    accept();
    std::cout << "Started accept" << std::endl;
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
        std::cout << "Started handling: "
                  << socket.remote_endpoint().address().to_string() + ":" +
                         std::to_string(socket.remote_endpoint().port())
                  << std::endl;
        client_->start();
    }

    accept();
}

void Server::handleStop() {
    std::cout << "Stopped connection" << std::endl;
    io_ctx_.stop();
}
