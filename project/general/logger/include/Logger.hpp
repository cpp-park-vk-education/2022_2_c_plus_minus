#pragma once

#include <chrono>
#include <fstream>
#include <mutex>

enum class LogType { error, warning, info };

class Log final {
   public:
    Log(const char* filename) {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_os.open(filename, std::ofstream::out);
    }

    ~Log() {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_os.is_open()) {
            m_os.close();
        }
    }

    template <class... Args>
    void Write(const LogType type, Args&&... args) {
        const auto now = std::chrono::high_resolution_clock::now();
        const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            now.time_since_epoch());

        std::lock_guard<std::mutex> lock(m_mutex);
        m_os << ms.count() << ' ';
        switch (type) {
            case LogType::info: {
                m_os << "[INFO] : ";
            } break;
            case LogType::error: {
                m_os << "[ERROR] : ";
            } break;
            case LogType::warning: {
                m_os << "[WARNING] : ";
            } break;
            default:
                break;
        }
        ((m_os << " " << std::forward<Args>(args)), ...);
    }

   private:
    std::ofstream m_os{};
    std::mutex m_mutex;
};
