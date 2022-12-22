#include <table.hpp>
#include "Client.hpp"

using namespace tabulate;
using Row_t = Table::Row_t;


int main() {
    std::shared_ptr<boost::asio::io_context> io{ std::make_shared<boost::asio::io_context>() };

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
    client->Run();
    for (auto& t: ts) {
        t.join();
    }
//    system("tput setaf 52");
}
