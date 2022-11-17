#include "../include/EpollUtil.h"

EpollUtil* EpollUtil::get()
{
    static EpollUtil e;
    return &e;
}

void EpollUtil::init(struct epollData &data)
{
    data.efd = epoll_create(1);
    data.events = new epoll_event[1024];
}

void EpollUtil::add(struct epollData &data,HttpTask &task)
{
    mod(data,task,EPOLLIN,EPOLL_CTL_ADD);
}


void EpollUtil::del(struct epollData &data,HttpTask &task)
{
    mod(data,task,EPOLLIN,EPOLL_CTL_DEL);
}

//type -> epoll_wait() 需要检测的事件类型
void EpollUtil::mod(struct epollData &data,HttpTask &task,int type)
{
    mod(data,task,type,EPOLL_CTL_MOD);
}

void EpollUtil::mod(struct epollData &data,HttpTask &task,int type,int op)
{
    epoll_event event;
    event.data.fd = task.fd;
    event.data.ptr = &task;
    event.events = type;
    epoll_ctl(data.efd,op,task.fd,&event);

    //断开与客户端的连接并释放
    if(op == EPOLL_CTL_DEL)
    {
        shutdown(task.fd,SHUT_RDWR);
        delete &task;
    }
}
