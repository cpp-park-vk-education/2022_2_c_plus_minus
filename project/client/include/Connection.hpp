#pragma once

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <memory>
#include <string_view>

#include "Buffer.hpp"
#include "Client.hpp"
#include "Logger.hpp"
#include "iostream"

namespace client {

class Connection : public std::enable_shared_from_this<Connection> {
   public:
    Connection(std::weak_ptr<Client> client,
               std::shared_ptr<boost::asio::io_context> context);

    ~Connection();

    void Connect(std::string_view path, std::string_view port);
    void Write(std::string text);
    void WriteLog(LogType type, const std::string& text);
    void Close();

   private:
    void handleResolve(
        const boost::system::error_code& err,
        const boost::asio::ip::tcp::resolver::results_type& endpoints);
    void handleConnect(const boost::system::error_code& err);
    void Read();
    void OnRead(const boost::system::error_code& error,
                size_t transferredBytes);

    void Write();
    void OnWrite(const boost::system::error_code& error,
                 size_t transferredBytes);

   private:
    Log m_logger{"log.txt"};

    std::weak_ptr<Client> m_client;
    std::shared_ptr<boost::asio::io_context> m_io;

    boost::asio::io_context::strand m_strand;
    boost::asio::ip::tcp::socket m_stream;

    bool m_isWriting{false};
    boost::asio::streambuf m_inbox;
    Buffers m_outbox;
    boost::asio::ip::tcp::resolver resolver_;
};

Connection::Connection(std::weak_ptr<Client> client,
                       std::shared_ptr<boost::asio::io_context> context)
    : m_client{client},
      m_io{context},
      m_strand{*context},
      m_stream{*context},
      resolver_(*context) {}

Connection::~Connection() {
    m_logger.Write(LogType::info, "Connection closed through destructor\n");
}

void Connection::Connect(std::string_view path, std::string_view port) {
    resolver_.async_resolve(path, port,
                            boost::bind(&Connection::handleResolve, this,
                                        boost::placeholders::_1,
                                        boost::placeholders::_2));
}

void Connection::handleResolve(
    const boost::system::error_code& err,
    const boost::asio::ip::tcp::resolver::results_type& endpoints) {
    if (!err) {
        boost::asio::async_connect(
            m_stream, endpoints,
            boost::bind(&Connection::handleConnect, this,
                        boost::placeholders::_1));
    } else {
        m_logger.Write(LogType::error,
                       "Resolving failed with error: ", err.message(), "\n");
    }
}

void Connection::handleConnect(const boost::system::error_code& err) {
    if (!err) {
        m_logger.Write(LogType::info, "Connection connected successfully!\n");
        Read();
    } else {
        m_logger.Write(
            LogType::error,
            "Connection failed to connect with error: ", err.message(), "\n");
    }
}

void Connection::Close() {
    boost::asio::post(m_strand, [self = this->shared_from_this()]() {
        boost::system::error_code error;
        self->m_stream.lowest_layer().shutdown(
            asio::ip::tcp::socket::shutdown_both, error);
        if (error) {
            self->m_logger.Write(
                LogType::error,
                "Connection's socket called shutdown with error: ",
                error.message(), '\n');
            error.clear();
        }

        self->m_stream.lowest_layer().close(error);
        if (error) {
            self->m_logger.Write(
                LogType::error,
                "Connection's socket is being closed with error: ",
                error.message(), '\n');
        }
        self->m_isWriting = false;

        if (auto model = self->m_client.lock(); model) {
            model->SetState(Client::State::CLOSED);
        }
        std::cout << "Successfully closed connection" << std::endl;
    });
}

void Connection::Read() {
    boost::asio::async_read_until(
        m_stream, m_inbox, separator,
        boost::asio::bind_executor(
            m_strand, std::bind(&Connection::OnRead, this->shared_from_this(),
                                std::placeholders::_1, std::placeholders::_2)));
}

void Connection::Write() {
    m_isWriting = true;
    m_outbox.SwapBuffers();
    boost::asio::async_write(
        m_stream, m_outbox.GetBufferSequence(),
        boost::asio::bind_executor(
            m_strand, std::bind(&Connection::OnWrite, this->shared_from_this(),
                                std::placeholders::_1, std::placeholders::_2)));
}

void Connection::Write(std::string text) {
    boost::asio::post(m_strand, [text = std::move(text),
                                 self = this->shared_from_this()]() mutable {
        self->m_outbox.Enque(std::move(text));
        if (!self->m_isWriting) {
            self->Write();
        }
    });
}

void Connection::WriteLog(LogType type, const std::string& text){
    m_logger.Write(type, text);
}

void Connection::OnRead(const boost::system::error_code& error,
                        size_t transferredBytes) {
    if (!error) {
        m_logger.Write(LogType::info,
                       "Connection just recive:", transferredBytes, "bytes.\n");

        const auto data{m_inbox.data()};
        std::string received{
            asio::buffers_begin(data),
            asio::buffers_begin(data) + transferredBytes - separator.size()};
        m_inbox.consume(transferredBytes);
//        std::cout << received << std::endl;
        Response incomingResponse;
        incomingResponse.parse(received);

        boost::system::error_code error;
        m_logger.Write(LogType::info,
                       m_stream.lowest_layer().remote_endpoint(error), ':',
                       received, '\n');

        if (auto model = m_client.lock(); model) {
            /* if it's ACK, we resolve the State in synchronous way, otherwise
             * resolve asyncronously */
            model->HandleMessage(std::move(incomingResponse));
        }
        this->Read();
    } else {
        m_logger.Write(LogType::error,
                       "Connection failed to read with error:", error.message(),
                       "\n");
        this->Close();
    }
}

void Connection::OnWrite(const boost::system::error_code& error,
                         size_t transferredBytes) {
    if (!error) {
        m_logger.Write(LogType::info, "Connection just sent:", transferredBytes,
                       "bytes\n");
        if (m_outbox.GetQueueSize()) {
            // we need to send other data
            this->Write();
        } else {
            m_isWriting = false;
        }
    } else {
        std::cout << "ERROR" << error.message() << std::endl;
        m_isWriting = false;
        m_logger.Write(LogType::error,
                       "Connection has error on writting:", error.message(),
                       '\n');
        this->Close();
    }
}

}  // namespace client