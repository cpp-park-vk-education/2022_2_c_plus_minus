#pragma once

#include "ServerImpl.h"

struct ServerImplDeleter {
    void operator()(ServerImpl* serverImpl);
};