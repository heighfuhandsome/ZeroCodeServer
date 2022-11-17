#include "../include/WorkThread.h"

WorkThread::WorkThread(int index)
{
    tname.append("sub thread "+ std::to_string(index));
    m_thrad=std::thread(&WorkThread::run,this);
    condition.wait(mutex);
    mutex.unlock();
}

void WorkThread::run()
{
    tid = std::this_thread::get_id();
    eventloop = std::make_shared<EventLoop>();
    condition.notify_one();
    eventloop->run();
}

std::shared_ptr<EventLoop> WorkThread::getEventLoop()
{
    return eventloop;
}