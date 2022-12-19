#include "Room.hpp"

#include <iostream>

Room::Room(const std::string& id, const std::string& name, const std::string host_id,
           unsigned int maxClientNumber)
    : id_(id), name_(name), host_id_(host_id), max_count_(maxClientNumber) {}

unsigned int Room::getCurrentClientNumber() { return current_count_; }

std::string Room::getId() { return id_; }

std::string Room::getName() { return name_; }

unsigned int Room::getMaxClientNumber() { return max_count_; }

void Room::addClient(const User& user) {
    clients_.insert({user.id, &user});
    current_count_++;
    boost::json::object object{{"id", user.id}};
    std::string buffer = boost::json::serialize(object);
    broadcast(user.id, QueryType::ON_ENTER_ROOM, buffer);
}

const User* Room::removeClient(const std::string& id) {
    const User* user = clients_[id];
    clients_.erase(id);
    current_count_--;
    return user;
}

boost::asio::ip::tcp::socket& Room::getClientSocket(const std::string& id) {
    return clients_[id]->socket;
}

bool Room::haveClient(const std::string& id) {
    return clients_.find(id) != clients_.end();
}

const User* Room::getClient(const std::string& id) {
    if (haveClient(id)) {
        return clients_[id];
    } else {
        return nullptr;
    }
}

const User* Room::getHost() {
    return getClient(host_id_);
}

std::string Room::getHostId() {
    return host_id_;
}

void Room::broadcast(const std::string& id, QueryType method, const std::string& data) {
//    if (method == "GameStart") {
    Response response {method, data};
    std::string buffer = response.toJSON();
    for (auto& client : clients_) {
        if (client.first != id) {
//
//            boost::json::object object({{"type", AsString()method}});
//            if (!data.empty()) {
//                object["data"] = data;
//            }
//            else object["data"].emplace_object();
//            std::string buffer = boost::json::serialize(object) + separator;
            boost::asio::write(client.second->socket,
                               boost::asio::buffer(buffer.data(), buffer.size()));
        }
    }
}

void Room::startGame() {
//    for (auto it = clients_.cbegin(), nextIt = it; it != clients_.cend();
//         it = nextIt) {
//        ++nextIt;
//        game_.addClient(*removeClient(it->second->id));
//    }
    game_session_ = new GameSession(id_, (*clients_.begin()).first,
                                    (*(++clients_.begin())).first);
//    game_.start();
}

bool Room::gameStarted() { return game_started_; }

Room::~Room() {
    for (const auto& client : clients_) {
        delete client.second;
    }
}

return_after_move Room::makeAction(const std::string& id,
                             const std::string& action) {
    return game_session_->makeMove(action, id);
}

void Room::onEnd(){
    // ...

    delete game_session_;
}