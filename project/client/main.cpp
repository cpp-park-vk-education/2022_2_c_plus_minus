#include <time.h>

#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include "Client.hpp"

#define DEBUG

#ifdef DEBUG
#include <iostream>
#endif

void PrintGameState(std::shared_ptr<Client> client){
    std::string position = client->GetFENState();
    if (client->GetColor() == figure_color::WHITE){
        int data_cursor = 0;
        std::cout << "   | A | B | C | D | E | F | G | H |" << std::endl << " 8 ";
        int counter = 7;
        while (position[data_cursor] != ' ') {  // считываем нотацию доски
            switch (position[data_cursor]) {
                case '1' ... '8':
                    for (int i = 0; i < position[data_cursor] - '0'; i++){
                        std::cout << "|   ";
                    }
                    break;
                case '/':
                    std::cout << "|" << std::endl << " " << counter-- << " ";
                    break;
                default:
                    std::cout << "| "<< position[data_cursor] << " " ;
                    break;
            }
            data_cursor++;
        }
        std::cout << std::endl;
    }
    else{
        std::cout << "   | A | B | C | D | E | F | G | H |" << std::endl << " 1 ";
        int data_cursor = 0;
        int counter = 2;
        while (position[data_cursor+1] != ' '){
            data_cursor++;
        }
        while (data_cursor >= 0) {  // считываем нотацию доски
            switch (position[data_cursor]) {
                case '1' ... '8':
                    for (int i = 0; i < position[data_cursor] - '0'; i++){
                        std::cout << "|   ";
                    }
                    break;
                case '/':
                    if (position[data_cursor+1] == ' '){
                        data_cursor--;
                        continue;
                    }
                    std::cout << "|" << std::endl << " " << counter++ << " ";
                    break;
                default:
                    std::cout << "| "<< position[data_cursor] << " " ;
                    break;
            }
            data_cursor--;
        }
        std::cout << std::endl;
    }

}

void PlayGame(std::shared_ptr<Client> client){
    while(!client->IsGameStarted()){
        // wait on front .....
    }
    std::cout << "Game started! " << std::endl;
    while (!client->IsGameFinished()){
        system("clear");
        std::cout << "Wait for enemy ..." << std::endl;
        PrintGameState(client);
        while (!client->IsYourTurn()){
            // wait on front ...
        }
        system("clear");
        PrintGameState(client);
        std::string move;
        std::cout << "Your move,  format: \"E2E4\"" << std::endl;
        std::cin >> move;
        client->MoveFigure(move);
    }
}


int main() {
    std::shared_ptr<boost::asio::io_context> io{
        std::make_shared<boost::asio::io_context>()};

    auto client = std::make_shared<Client>(io);
    client->Connect("127.0.0.1", "8080");
    std::vector<std::thread> ts;
    for (int i = 0; i < 2; i++) {
        ts.emplace_back([io]() {
            for (;;) {
                try {
                    io->run();
                    break;  // run() exited normally
                } catch (...) {
                    // Deal with exception as appropriate.
                }
            }
        });
    }
    sleep(1);
    bool stop = false;
    while (!stop) {
        std::cout << "1 - GET ROOMS " << "2 - CREATE ROOM " << "3 - AUTHORISE "
                  << "4 - ENTER ROOM " << "5 - START GAME (FOR HOST)" << std::endl;
        int choice;
        std::cin >> choice;
        switch (choice) {
            case 1: {
                //                for (int i = 0; i < 100; i++) {
                //                    std::cout << i << std::endl;
                client->GetAllRooms();
                //                }
                break;
            }
            case 2: {
                std::string name;
                std::cout << "enter room name" << std::endl;
                std::cin >> name;
                std::cout << "your color: w or b?" << std::endl;
                char color;
                std::cin >> color;
                figure_color p_color = (color == 'w' ? figure_color::WHITE : figure_color::BLACK);
                client->CreateRoom(name, p_color);
                //PlayGame(client);
                break;
            }
            case 3: {
                std::string name;
                std::cout << "enter nickname" << std::endl;
                std::cin >> name;
                client->Authorise(name);
                break;
            }

            case 4: {
                std::string id;
                std::cout << "enter id of room" << std::endl;
                std::cin >> id;
                client->EnterRoom("127.0.0.1:" + id);
                PlayGame(client);
                break;
            }
            case 5:
                client->StartGame();
                PlayGame(client);
                break;
            case 6:
                stop = true;
        }
    }
    return 0;
}
