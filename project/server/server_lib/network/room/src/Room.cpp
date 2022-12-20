#include "Room.hpp"

#include <iostream>

Room::Room(const std::string& id, const std::string& name,
           const std::string& host_id, const figure_color& host_color,
           unsigned int maxClientNumber)
    : id_(id),
      name_(name),
      host_id_(host_id),
      host_color_(host_color),
      max_count_(maxClientNumber) {}

unsigned int Room::getCurrentClientNumber() { return current_count_; }

std::string Room::getId() { return id_; }

std::string Room::getName() { return name_; }

unsigned int Room::getMaxClientNumber() { return max_count_; }

void Room::addClient(const User& user){
    int a = 5;
    a++;
}

void Room::addClient(const User& user, const figure_color& color) {
    clients_.insert({user.id, std::pair<const User*, figure_color>(&user, color)});
    current_count_++;
    boost::json::object object{{"id", user.id}};
    std::string buffer = boost::json::serialize(object);
    broadcast(user.id, QueryType::ON_ENTER_ROOM, buffer);
}

const User* Room::removeClient(const std::string& id) {
    const User* user = clients_[id].first;
    clients_.erase(id);
    current_count_--;
    return user;
}

boost::asio::ip::tcp::socket& Room::getClientSocket(const std::string& id) {
    return clients_[id].first->socket;
}

bool Room::haveClient(const std::string& id) {
    return clients_.find(id) != clients_.end();
}

const User* Room::getClient(const std::string& id) {
    if (haveClient(id)) {
        return clients_[id].first;
    } else {
        return nullptr;
    }
}

const User* Room::getHost() { return getClient(host_id_); }
std::string Room::getHostId() { return host_id_; }
figure_color Room::getHostColor() { return host_color_; }

void Room::write(const std::string& id, QueryType method,
                 const std::string& data) {
    Response response{method, data};
    std::string buffer = response.toJSON();
    auto client = clients_[id].first;
    boost::asio::write(client->socket,
                       boost::asio::buffer(buffer.data(), buffer.size()));
}

void Room::broadcast(const std::string& id, QueryType method,
                     const std::string& data) {
    Response response{method, data};
    std::string buffer = response.toJSON();
    for (auto& client : clients_) {
        if (client.first != id) {
            boost::asio::write(
                client.second.first->socket,
                boost::asio::buffer(buffer.data(), buffer.size()));
        }
    }
}

void Room::startGame() {
    auto white = std::find_if(clients_.begin(), clients_.end(), [](std::pair<std::string , std::pair<const User*, figure_color>> usr){
        return usr.second.second == figure_color::WHITE;
    });
    auto black = std::find_if(clients_.begin(), clients_.end(), [](std::pair<std::string , std::pair<const User*, figure_color>> usr){
        return usr.second.second == figure_color::BLACK;
    });
    game_session_ = new GameSession(id_, white->first, black->first);
}

bool Room::gameStarted() { return game_started_; }

Room::~Room() {
    for (const auto& client : clients_) {
        delete client.second.first;
    }
}

move_response Room::makeAction(const std::string& id,
                                   const std::string& action) {
    return game_session_->makeMove(action, id);
}

void Room::onEnd() {
    // ...

    delete game_session_;
}
