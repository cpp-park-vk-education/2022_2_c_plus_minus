#pragma once

#include "DbTask.hpp"

static enum result_new { NOT_NEW = 1, ERROR_NEW } NEW;

static enum result_get_task_id {
    NOT_GET_TASK_ID = 3,
    ERROR_GET_TASK_ID
} GET_TASK_ID;

static enum result_get_task_task {
    NOT_GET_TASK_TASK = 6,
    ERROR_GET_TASK_TASK
} GET_TASK;

static enum result_delete_task {
    NOT_DELETE_TASK = 9,
    ERROR_DELETE_TASK
} TASK_DELETE;

static enum result_update_task_task {
    NOT_UPDATE_TASK_TASK = 12,
    ERROR_UPDATE_TASK_TASK
} UPDATE_TASK_TASK;

template <class C>
class DbTaskImpl : public DbTask<C> {
   public:
    int GetId(const Task &task, uint64_t *Task_id,
              C *connection) const override;

    int GetTask(const Task &task, std::vector<std::vector<uint8_t>> *task__,
                C *connection) const override;

    int UpdateTask(const uint64_t &task_id, C *connection) const override;

    int Delete(const Task &task, C *connection) const override;

    int NewTask(const Task &task, uint64_t *new_task_id,
                C *connection) const override;

    DbTaskImpl() = default;
    ~DbTaskImpl() = default;
};