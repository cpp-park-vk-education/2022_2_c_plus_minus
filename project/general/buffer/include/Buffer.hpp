#pragma once

#include <vector>
#include <string>
#include <array>
#include <boost/asio.hpp>

namespace asio = boost::asio;

/**
 * This type provide functionality for scatter-gether I/O.
 * It wrapped two buffer sequences and switched between them updating
 * view buffer sequence which is used by write/write_some/async_write/async_write_some
 * operations as data source const buffers.
 *
 * Not thread safe.
 */
class Buffers final {
   public:

    Buffers(std::size_t reserved = 10);

    /**
     * Queue data to passibe buffer.
     */
    void Enque(std::string&& data);

    /**
     * Swap buffers and update @m_bufferSequence.
     */
    void SwapBuffers();

    std::size_t GetQueueSize() const noexcept {
        return m_buffers[m_activeBuffer ^ 1].size();
    }

    const std::vector<asio::const_buffer>& GetBufferSequence() const noexcept {
        return m_bufferSequence;
    }

   private:

    using DoubleBuffer = std::array<std::vector<std::string>, 2>;

    /**
     * Represent two sequences of some buffers
     * One sequence is active, another one is passive.
     * They can be swapped when needed.
     **/
    DoubleBuffer m_buffers;

    /**
     * View const buffer sequence used by write operations.
     */
    std::vector<asio::const_buffer> m_bufferSequence;

    std::size_t m_activeBuffer { 0 };
};
