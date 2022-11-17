#pragma once
#include <queue>
#include <thread>
#include <mutex>
#include <sys/epoll.h>
#include <sys/socket.h>
#include "HttpRequestUtil.h"
#include "Route.h"
#include "EpollUtil.h"
#include "HttpTask.h" 

enum class ExecType{add,mod,del}; 
class EventLoop
{
    
//公共成员方法
public:
    EventLoop();
    EventLoop(const EventLoop &o) = delete;
    EventLoop& operator=(const EventLoop &o) = delete;
    void run();
    struct epollData& getEpollData();
    void addTask(HttpTask *task,ExecType type);

//私有成员方法
private:
    void execTask_que();
    void dispatch();

private:
    struct epollData edata;
    struct Task
    {
        Task(HttpTask *task,ExecType type)
        {
            this->task = task;
            this->type = type;
        }       
        HttpTask* task;
        ExecType type;
    };
    std::queue<struct Task> task_que;
    std::mutex mutex;
    EpollUtil *epollUtil;
    std::thread::id tid;
    int socketPair[2];
};