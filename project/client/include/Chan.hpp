#pragma once

#include "SafeQueue.hpp"
#include "memory"
#include "string"

class MoveChan {
   public:
    MoveChan() = default;
    int makeMove(const std::string& move);
    SafeQueue<std::string> moves_chan_;
    std::string last_move_ = "";
};