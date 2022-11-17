#pragma once 
#include <sys/epoll.h>
struct epollData
{
    epoll_event *events;
    int efd;
};