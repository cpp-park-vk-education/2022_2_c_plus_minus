#include "BasicMenu.hpp"

void BasicMenu::addClient(const ClientData& clientData) {
    clients_.insert({clientData.id, &clientData});
}

const ClientData* BasicMenu::removeClient(const std::string& id) {
    const ClientData* clientData = clients_[id];
    clients_.erase(id);
    return clientData;
}

boost::asio::ip::tcp::socket& BasicMenu::getClientSocket(
    const std::string& id) {
    return clients_[id]->socket;
}

bool BasicMenu::haveClient(const std::string& id) {
    return clients_.find(id) != clients_.end();
}

const ClientData* BasicMenu::getClient(const std::string& id) {
    if (haveClient(id)) {
        return clients_[id];
    } else {
        return nullptr;
    }
}
