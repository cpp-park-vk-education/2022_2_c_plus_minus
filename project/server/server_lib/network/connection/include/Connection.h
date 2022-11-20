#pragma once

#include "NewConnection.h"

struct Connection {
    uint64_t id_;

    bool operator==(Connection other) const {
        return id_ == other.id_;
    }

    ~Connection(){
        NewConnection::cur_num_--;
    }
};

