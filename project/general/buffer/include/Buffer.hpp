#pragma once

#include <array>
#include <boost/asio.hpp>
#include <string>
#include <vector>

namespace asio = boost::asio;

class Buffers final {
   public:
    Buffers(std::size_t reserved = 10);
    void Enque(std::string&& data);
    void SwapBuffers();

    std::size_t GetQueueSize() const noexcept {
        return m_buffers[m_activeBuffer ^ 1].size();
    }

    const std::vector<asio::const_buffer>& GetBufferSequence() const noexcept {
        return m_bufferSequence;
    }

   private:
    using DoubleBuffer = std::array<std::vector<std::string>, 2>;
    DoubleBuffer m_buffers;
    std::vector<asio::const_buffer> m_bufferSequence;
    std::size_t m_activeBuffer{0};
};
