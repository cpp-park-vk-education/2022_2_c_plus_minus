//#include "Server.hpp"
//
//#include <memory>
//
//#include "Channel.h"
//#include "Connection.hpp"
//#include "NewConnection.hpp"
//#include "SessionImpl.hpp"
//
// class ServerImpl {
//   public:
//    ServerImpl(Server& server, unsigned short port, std::string httpMessage)
//        : server{server},
//          endpoint{boost::asio::ip::tcp::v4(), port},
//          ioContext{},
//          acceptor{ioContext, endpoint},
//          httpMessage{std::move(httpMessage)} {
//        listenForConnections();
//    }
//
//    void listenForConnections();
//    void registerChannel(Channel& channel);
//    void reportError(std::string_view message);
//
//    using ChannelMap = std::unordered_map<Connection,
//    std::shared_ptr<Channel>,
//                                          std::hash<Connection>>;
//
//    Server& server;
//    const boost::asio::ip::tcp::endpoint endpoint;
//    boost::asio::io_context ioContext;
//    boost::asio::ip::tcp::acceptor acceptor;
//    boost::beast::http::string_body::value_type httpMessage;
//
//    ChannelMap channels;
//    std::deque<Message> incoming;
//};
//
// void ServerImpl::listenForConnections() {
//    auto session = std::make_shared<HTTPSession>(*this);
//
//    acceptor.async_accept(session->getSocket(),
//                          [this, session](auto errorCode) {
//                              if (!errorCode) {
//                                  session->start();
//                              } else {
//                                  reportError("Fatal error while accepting");
//                              }
//                              this->listenForConnections();
//                          });
//}
//
// void ServerImpl::registerChannel(Channel& channel) {
//    auto connection = channel.getConnection();
//    channels[connection] = channel.shared_from_this();
//    server.connectionHandler->handleConnect(connection);
//}
//
// void ServerImpl::reportError(std::string_view /*message*/) {
//    // Swallow errors....
//}
//
// void Server::update() { impl->ioContext.poll(); }
//
// std::deque<Message> Server::receive() {
//    std::deque<Message> oldIncoming;
//    std::swap(oldIncoming, impl->incoming);
//    return oldIncoming;
//}
//
// void Server::send(const std::deque<Message>& messages) {
//    for (auto& message : messages) {
//        auto found = impl->channels.find(message.connection);
//        if (impl->channels.end() != found) {
//            found->second->send(message.text);
//        }
//    }
//}
//
// void Server::disconnect(Connection connection) {
//    auto found = impl->channels.find(connection);
//    if (impl->channels.end() != found) {
//        connectionHandler->handleDisconnect(connection);
//        found->second->disconnect();
//        impl->channels.erase(found);
//    }
//}
//
// std::unique_ptr<ServerImpl, ServerImplDeleter> Server::buildImpl(
//    Server& server, unsigned short port, std::string httpMessage) {
//    // NOTE: We are using a custom deleter here so that the impl class can be
//    // hidden within the source file rather than exposed in the header. Using
//    // a custom deleter means that we need to use a raw `new` rather than
//    using
//    // `std::make_unique`.
//    auto* impl = new ServerImpl(server, port, std::move(httpMessage));
//    return std::unique_ptr<ServerImpl, ServerImplDeleter>(impl);
//}
