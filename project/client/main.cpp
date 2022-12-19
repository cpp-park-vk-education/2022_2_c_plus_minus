#include <time.h>

#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include "Client.hpp"

#define DEBUG

#ifdef DEBUG
#include <iostream>
#endif

int main() {
    std::shared_ptr<boost::asio::io_context> io {
        std::make_shared<boost::asio::io_context>()
    };

    auto client = std::make_shared<Client>(io);
    client->Connect("127.0.0.1", "8080");
    std::vector<std::thread> ts;
    for (int i = 0; i < 2; i++) {
        ts.emplace_back([io]() {
            for (;;) {
                try {
                    io->run();
                    break; // run() exited normally
                }
                catch (...) {
                    // Deal with exception as appropriate.
                }
            }
        });
    }
    sleep(1);
    bool stop = false;
    while (!stop) {
        std::cout << "1 - GET ROOMS" << std::endl
                  << "2 - CREATE ROOM" << std::endl
                  << "3 - AUTHORISE"  << std::endl
                  << "4 - MOVE FIGURE" <<std::endl
                  << "5 - ENTER ROOM " << std::endl;
        int choice;
        std::cin >> choice;
        switch (choice) {
            case 1:
            {
//                for (int i = 0; i < 100; i++) {
//                    std::cout << i << std::endl;
                    client->GetAllRooms();
//                }
                break ;
            }
            case 2:
            {
                std::string name;
                std::cout << "enter room name" << std::endl;
                std::cin >> name;
                client->CreateRoom(name);
                break ;
            }
            case 3:
            {
                std::string name;
                std::cout << "enter nickname" << std::endl;
                std::cin >> name;
                client->Authorise(name);
                break ;
            }
            case 4:
            {
                std::string move;
                std::cout << "enter your move,  format: \"E2E4\"" << std::endl;
                std::cin >> move;
                client->MoveFigure(move);
                break ;
            }
            case 5:
            {
                std::string id;
                std::cout << "enter id of room" << std::endl;
                std::cin >> id;
                client->EnterRoom("127.0.0.1:" + id);
                break ;
            }
            case 6:
                stop = true;
        }
    }
    return 0;
}

//        client->Authorise("CHESS_PLAYER");
//        client->CreateRoom("MYROOOM");
//        for (int i = 0; i < 100; i++) {
//            std::cout << i << std::endl;
//            client->GetAllRooms();
//        }

//    while (true) {
//        string str = "MESSAGE";
//        //		getline(cin, str);
//        client->Write(std::move(str));
//    }
//    boost::asio::io_context io_context;
//    Client c(io_context, "127.0.0.1", "8080");
//    std::thread thread(boost::bind(&boost::asio::io_context::run, &io_context));
//    sleep(1);
//    c.authorise("Petr");
//    c.createRoom("MyRoom");
//    for (int i=0 ; i < 10000; i++){
//        sleep(0.2);
//        c.getAllRooms();
//    }
//    c.getAllRooms();
//    char str[5];
//    scanf("%s", str);
//    c.enterRoom("127.0.0.1:" + std::string(str));
//    sleep(1);
//    while (1) {
//        char move[4];
//        scanf("%s", move);
//        c.moveFigure(std::string(move));
//    }
//
//    thread.join();
