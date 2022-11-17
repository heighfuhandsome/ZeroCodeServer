#pragma once
#include <sys/epoll.h>
#include <sys/socket.h>
#include "HttpTask.h"
#include "Route.h"
#include "HttpRequestUtil.h"
#include "EpollData.h"



class EpollUtil{
public:
    static EpollUtil* get();

    void init(struct epollData &data);

    void add(struct epollData &data,HttpTask &task);

    void del(struct epollData &data,HttpTask &task);

    void mod(struct epollData &data,HttpTask &task,int type);

    // void dispatch(EventLoop &eventloop);
private:
    void mod(struct epollData &data,HttpTask &task,int type,int op);
    EpollUtil() = default;
 };



