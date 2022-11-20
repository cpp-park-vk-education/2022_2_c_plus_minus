#include "Connection.h"
#include <string>
#include <memory>
#include "ServerImpl.h"
#include "ServerImplDeleter.h"
#include "ConnectionHandlerImpl.h"

class Server {
public:
    template <typename C, typename D>
    Server(unsigned short port,
           std::string httpMessage,
           C onConnect,
           D onDisconnect)
            : connectionHandler{std::make_unique<ConnectionHandlerImpl<C,D>>(onConnect, onDisconnect)},
              impl{buildImpl(*this, port, std::move(httpMessage))}
    { }

    void update();
//    TODO: implement Message interface
//    void send(const std::deque<Message>& messages);

//    [[nodiscard]] std::deque<Message> receive();


    void disconnect(Connection connection);

private:
    friend class ServerImpl;

    static std::unique_ptr<ServerImpl,ServerImplDeleter>
    buildImpl(Server& server, unsigned short port, std::string httpMessage);

    std::unique_ptr<ConnectionHandler> connectionHandler;
    std::unique_ptr<ServerImpl,ServerImplDeleter> impl;
};