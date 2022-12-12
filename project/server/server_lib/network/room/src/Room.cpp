#include "Room.hpp"

#include <iostream>

Room::Room(const std::string& id, const std::string& name,
           unsigned int maxClientNumber)
    : id_(id), name_(name), max_count_(maxClientNumber), game_(id_) {}

unsigned int Room::getCurrentClientNumber() { return current_count_; }

std::string Room::getId() { return id_; }

std::string Room::getName() { return name_; }

unsigned int Room::getMaxClientNumber() { return max_count_; }

void Room::addClient(const ClientData& clientData) {
    clients_.insert({clientData.id, &clientData});
    current_count_++;
    boost::json::object object{{"id", clientData.id}};
    std::string buffer = boost::json::serialize(object);
    broadcast(clientData.id, "EnterRoom", buffer);
}

const ClientData* Room::removeClient(const std::string& id) {
    const ClientData* clientData = clients_[id];
    clients_.erase(id);
    current_count_--;
    return clientData;
}

boost::asio::ip::tcp::socket& Room::getClientSocket(const std::string& id) {
    return clients_[id]->socket;
}

bool Room::haveClient(const std::string& id) {
    return clients_.find(id) != clients_.end();
}

const ClientData* Room::getClient(const std::string& id) {
    if (haveClient(id)) {
        return clients_[id];
    } else {
        return nullptr;
    }
}

void Room::broadcast(const std::string& id, const std::string& method,
                     const std::string& data) {
    //    if (method == "GameStart") {
    for (auto& client : clients_) {
        if (client.first != id) {
            boost::json::object object({{"type", method}});
            if (!data.empty()) {
                object["data"] = data;
            } else
                object["data"].emplace_object();
            std::string buffer = boost::json::serialize(object);
            boost::asio::write(
                client.second->socket,
                boost::asio::buffer(buffer.data(), buffer.size()));
        }
    }
}

void Room::startGame() {
    for (auto it = clients_.cbegin(), nextIt = it; it != clients_.cend();
         it = nextIt) {
        ++nextIt;
        game_.addClient(*removeClient(it->second->id));
    }
    game_.start();
}

Game& Room::getGame() { return game_; }

bool Room::gameStarted() { return game_started_; }

Room::~Room() {
    for (const auto& client : clients_) {
        delete client.second;
    }
}
