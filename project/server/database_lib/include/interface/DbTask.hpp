#pragma once

#include "Task.hpp"

template <class C>
class DbTask {
   public:
    virtual int GetId(const Task &task, uint64_t *Task_id,
                      C *connection) const = 0;

    virtual int GetTask(const Task &task,
                        std::vector<std::vector<uint8_t>> *task__,
                        C *connection) const = 0;

    virtual int UpdateTask(const uint64_t &task_id, C *connection) const = 0;

    virtual int Delete(const Task &task, C *connection) const = 0;

    virtual int NewTask(const Task &task, uint64_t *new_task_id,
                        C *connection) const = 0;

    virtual ~DbTask() = default;
};