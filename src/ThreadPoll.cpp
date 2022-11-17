#include "../include/ThreadPoll.h"

ThreadPoll::ThreadPoll(int size)
{
    index = 0;
    while (size--)
    {
        poll.push_back(std::make_shared<WorkThread>(index++));
    }
    index =0;
}

size_t ThreadPoll::getPollSize()
{
    return poll.size();
}

void ThreadPoll::addTask(int fd)
{
    HttpTask *task=new HttpTask(fd);
    poll[index]->getEventLoop()->addTask(task,ExecType::add);
    index++;
    index = index % poll.size();

}