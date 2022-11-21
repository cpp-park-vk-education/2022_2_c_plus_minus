#include "Task.hpp"

bool Task::IsEmpty() const noexcept {
    return !(this->id_) && this->task_.empty();
}

void Task::SetId(const uint64_t &id) noexcept { this->id_ = id; }

void Task::SetTask(const std::vector<std::vector<uint8_t>> &task) noexcept {
    this->task_ = task;
}

uint64_t &Task::GetId() noexcept { return this->id_; }

std::vector<std::vector<uint8_t>> &Task::GetTask() noexcept {
    return this->task_;
}
