#pragma once

#include <array>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <memory>

#include "Connection.hpp"
#include "Request.hpp"
#include "SafeQueue.hpp"
#include "SessionImpl.hpp"

class ConnectionImpl : public std::enable_shared_from_this<ConnectionImpl>,
                       public Connection {
   public:
    ConnectionImpl(const ConnectionImpl&) = delete;
    ConnectionImpl& operator=(const ConnectionImpl&) = delete;

    explicit ConnectionImpl(boost::asio::io_context& io_context,
                            boost::asio::ssl::context* sslContext,
                            boost::asio::ip::tcp::socket&& socket,
                            std::uint64_t id,
                            std::shared_ptr<SafeQueue<Request>> r_queue);

    void Start() override;
    void Stop() override;
    void AddSession(std::weak_ptr<SessionImpl> session);

   private:
    void Read() override;
    void ReadHandler() override;

    void Write() override;
    void WriteHandler() override;

    boost::asio::ssl::stream<boost::asio::ip::tcp::socket> socket_;
    boost::asio::io_context::strand strand_;
    std::shared_ptr<SafeQueue<Request>> r_queue_;
    // cycle ref with SessionImpl, use weak_ptr
    std::weak_ptr<SessionImpl> session_;
    const std::uint64_t id_;

    boost::asio::streambuf buffer_;
};
