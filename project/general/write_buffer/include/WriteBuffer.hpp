#pragma once

#include <array>

#include "boost/asio.hpp"
#include "string"
#include "vector"

class WriteBuffer {
   public:
    WriteBuffer() = default;

    ~WriteBuffer() = default;

    void push(std::string&& data) {
        m_buffers[m_currentBuffer].emplace_back(std::move(data));
    }

    std::vector<boost::asio::const_buffer> popAll() {
        m_buffers[1 - m_currentBuffer].clear();

        std::vector<boost::asio::const_buffer> bufferSequence;
        for (const auto& string : m_buffers[m_currentBuffer]) {
            bufferSequence.emplace_back(
                boost::asio::const_buffer(string.data(), string.size()));
        }

        m_currentBuffer = 1 - m_currentBuffer;

        return bufferSequence;
    }

    std::size_t size() const { return m_buffers[m_currentBuffer].size(); }

   private:
    std::array<std::vector<std::string>, 2> m_buffers;

    std::size_t m_currentBuffer = 0;
};
