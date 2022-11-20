#pragma once

#include <stdint.h>

// Logic of creating id of user (perhaps increment, hash, other difficult logic
struct NewConnection {
    inline static uint64_t cur_num_ = 0;
    uint64_t operator()() const {
        return ++cur_num_;
    }
};
