#include "../include/HttpTask.h"

HttpTask::HttpTask(int fd,bool disconnect,EPOLL_EVENTS events):fd(fd),disConnect(disConnect),events(events)
{

}


void HttpTask::setEvents(int events)
{
    this->events = (EPOLL_EVENTS)events;
}

EPOLL_EVENTS HttpTask::getEvents()
{
    return events;
}