#include <time.h>

#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include "Client.hpp"

#define DEBUG

#ifdef DEBUG
#include <iostream>
#endif

int main() {
    boost::asio::io_context io_context;
    Client c(io_context, "127.0.0.1", "8080");
    std::thread thread(boost::bind(&boost::asio::io_context::run, &io_context));
    sleep(1);
    //    c.createRoom("MyRoom");
    //    sleep(1);
    c.getAllRooms();
    sleep(1);
    char str[5];
    scanf("%s", str);
    c.enterRoom("127.0.0.1:" + std::string(str));
    sleep(1);
    while (1) {
        char move[4];
        scanf("%s", move);
        c.moveFigure(std::string(move));
    }

    thread.join();

    return 0;
}