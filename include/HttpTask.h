#pragma once
#include <sys/epoll.h>
#include "HttpRequest.h"
#include "HttpResponse.h"

class HttpTask{
    friend class EpollUtil;
    friend class EventLoop;
    friend class Route;
    friend class HttpRequestUtil;

public:
    HttpTask(int fd,bool disconnect = false,EPOLL_EVENTS events = EPOLLIN);
    HttpTask(const HttpTask &o) = delete;
    HttpTask& operator=(const HttpTask &o) = delete;

    void setEvents(int events);

    EPOLL_EVENTS getEvents();
private:
    int fd;
    HttpRequest request;
    HttpResponse response;
    EPOLL_EVENTS events;
    char buff[4096];
    bool disConnect; 
};