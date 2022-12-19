#include "BasicMenu.hpp"

void BasicMenu::addClient(const User& user) {
    clients_.insert({user.id, &user});
}

const User* BasicMenu::removeClient(const std::string& id) {
    const User* user = clients_[id];
    clients_.erase(id);
    return user;
}

boost::asio::ip::tcp::socket& BasicMenu::getClientSocket(
    const std::string& id) {
    return clients_[id]->socket;
}

bool BasicMenu::haveClient(const std::string& id) {
    return clients_.find(id) != clients_.end();
}

const User* BasicMenu::getClient(const std::string& id) {
    if (haveClient(id)) {
        return clients_[id];
    } else {
        return nullptr;
    }
}
