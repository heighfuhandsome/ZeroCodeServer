#pragma once
#include "WorkThread.h"
#include <vector>
#include <memory>



class ThreadPoll{
public:
    ThreadPoll(int size);
    ThreadPoll(const ThreadPoll &o) = delete;
    ThreadPoll& operator=(const ThreadPoll&o) = delete;

    size_t getPollSize();

    void addTask(int fd);
private:
    std::vector<std::shared_ptr<WorkThread>> poll;
    int index;
};