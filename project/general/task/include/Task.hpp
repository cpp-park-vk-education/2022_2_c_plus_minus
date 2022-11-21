#pragma once

#include <cstdint>
#include <string>
#include <vector>

const uint8_t BoardSize = 8;

class Task {
   private:
    uint64_t id_;
    std::vector<std::vector<uint8_t>> task_ = std::vector<std::vector<uint8_t>>(
        BoardSize, std::vector<uint8_t>(BoardSize, 0));

   public:
    Task() = default;
    virtual ~Task() = default;
    bool IsEmpty() const noexcept;

    void SetId(const uint64_t &) noexcept;
    void SetTask(const std::vector<std::vector<uint8_t>> &) noexcept;

    uint64_t &GetId() noexcept;
    std::vector<std::vector<uint8_t>> &GetTask() noexcept;
};
