#pragma once

#include <string>

class Message {
   public:
    Message() = default;
    virtual ~Message() = default;
    // functions to work with Message as JSON
    virtual void ToJSON(std::string& json) const = 0;
    virtual void FromJSON(const std::string& json) = 0;
};
