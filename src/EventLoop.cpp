#include "../include/EventLoop.h"



EventLoop::EventLoop()
{
    epollUtil = EpollUtil::get();
    epollUtil->init(edata);

    tid=std::this_thread::get_id();
    socketpair(AF_LOCAL,SOCK_STREAM,0,socketPair);
    addTask(new HttpTask(socketPair[1]),ExecType::add);


}

struct epollData &EventLoop::getEpollData()
{
    return edata;
}


void EventLoop::addTask(HttpTask *task,ExecType type)
{
    std::lock_guard<std::mutex> guard(mutex);
    task_que.emplace(task,type);
    //主线程添加的任务
    if(!(std::this_thread::get_id() == tid))
    {
        send(socketPair[0],"GET /socketPair HTTP/1.1\r\nContent-Length:0\r\n\r\n",strlen("GET /socketPair HTTP/1.1\r\nContent-Length:0\r\n\r\n"),0);
    }
    
}

void EventLoop::execTask_que()
{
    while (!task_que.empty())
    {
        struct Task &t = task_que.front();
        switch(t.type)
        {
            case ExecType::add:
                epollUtil->add(edata,*t.task);
                break;
            case ExecType::del:
                epollUtil->del(edata,*t.task);
                break;
            case ExecType::mod:
                epollUtil->mod(edata,*t.task,t.task->getEvents());
                break;
        }
        task_que.pop();
    }
    
}

void EventLoop::dispatch()
{
    int size = epoll_wait(edata.efd,edata.events,1024,-1);
    for(int i=0;i<size ;i++)
    {
        HttpTask &task = *(HttpTask*)edata.events[i].data.ptr;
        if(edata.events[i].events & EPOLLIN)
        {
            HttpRequestUtil::parseClientDataToHttpRequest(task);
            if(task.disConnect)
            {
                addTask(&task,ExecType::del);
            }

            if( !task.disConnect &&  !(task.getEvents() & EPOLLOUT) && task.request.getStatu() == ReqParseStatu::body_done && task.fd != socketPair[1])
            {
                task.setEvents(task.getEvents()|EPOLLOUT);
                addTask(&task,ExecType::mod);
            }
        }

        if( (edata.events[i].events & EPOLLOUT)  && task.response.getResBuildStatu() != ResBuildStatu::body_done && !task.disConnect)
        {
            Route::Mapping(task);
            if(task.response.getResBuildStatu() == ResBuildStatu::body_done)
            {
                addTask(&task,ExecType::del);
            }
        }
    }
}

void EventLoop::run()
{
    while (true)
    {
        execTask_que(); 
        dispatch();
    }
}