#include "Game.hpp"

Game::Game(const std::string& id) : id_(id) {}

void Game::addClient(const ClientData& clientData) {
    clients_.insert({clientData.id, &clientData});
}

const ClientData* Game::removeClient(const std::string& id) {
    const ClientData* clientData = clients_[id];
    clients_.erase(id);
    return clientData;
}

boost::asio::ip::tcp::socket& Game::getClientSocket(const std::string& id) {
    return clients_[id]->socket;
}

bool Game::haveClient(const std::string& id) {
    return clients_.find(id) != clients_.end();
}

const ClientData* Game::getClient(const std::string& id) {
    if (haveClient(id)) {
        return clients_[id];
    } else {
        return nullptr;
    }
}

void Game::broadcast(const std::string& id, const std::string& response) {
    for (auto& client : clients_) {
        if (client.first != id) {
            boost::json::object object({{"type", "MoveFigure"}});
            object["data"] = response;
            std::string buffer = boost::json::serialize(object);
            boost::asio::write(
                client.second->socket,
                boost::asio::buffer(buffer.data(), buffer.size()));
        }
    }
}

return_after_move Game::makeAction(const std::string& id,
                                   const std::string& action) {
    return game_session_->makeMove(action, id);
}

void Game::start() {
    game_session_ = new GameSession(id_, (*clients_.begin()).first,
                                    (*(++clients_.begin())).first);
}

void Game::onEnd() {
    // ...

    delete game_session_;
}
