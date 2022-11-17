#pragma once
#include <thread>
#include <string>
#include <condition_variable>
#include <mutex>
#include "EventLoop.h"

class WorkThread{
public:
    WorkThread(int index);
    WorkThread(const WorkThread &o) = delete;
    WorkThread& operator=(const WorkThread &o) = delete;
    
    std::shared_ptr<EventLoop> getEventLoop();

//私有成员方法
private:
    void run();
private:
    std::thread m_thrad;
    std::thread::id tid;
    std::string tname;
    std::mutex mutex;
    std::shared_ptr<EventLoop> eventloop;
    std::condition_variable_any condition;

};