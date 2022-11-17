#include "../include/MysqlConnectPoll.h"
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include "json.h"


MysqlConnectPoll* MysqlConnectPoll::getMysqlConnectPoll(const std::string &path)
{
    static MysqlConnectPoll poll(path);
    return &poll;
}

MysqlConnectPoll::MysqlConnectPoll(std::string configPath)
{
    int fd = open(configPath.c_str(),O_RDONLY);
    if(fd<0)
    {
        perror("MysqlConnect::MysqlConnect() open");
    }
    std::string s;
    char buff[4096];
    int ret=0;
    while ((ret=read(fd,buff,4096)))
    {
        s.append(buff,ret);
    }

    Json::Reader r;
    Json::Value root;
    r.parse(s,root,false);

    host = root["host"].asString();
    user = root["user"].asString();
    password = root["password"].asString();
    dbname = root["dbname"].asString();
    port = root["port"].asUInt();
    connect_poll_maxSize = root["connect_poll_maxSize"].asUInt();
    connect_poll_minSize = root["connect_poll_minSize"].asUInt();
    free_time = root["free_time"].asUInt();

    while (connect_poll.size() != connect_poll_maxSize)
    {
        addConnect();
    }

    t_produce = new std::thread(&MysqlConnectPoll::produce,this);
    t_recovery = new std::thread(&MysqlConnectPoll::recovery,this);
}

size_t MysqlConnectPoll::getPollSize()
{
    std::lock_guard<std::mutex> guard(mutex);
    return connect_poll.size();
}

void MysqlConnectPoll::addConnect()
{
    MysqlConnect *conn = new MysqlConnect(); 
    if(conn->connect(host,user,password,dbname,port))
    {
        connect_poll.push(conn);
    } else
    {
        delete conn;
    }
}


void MysqlConnectPoll::produce()
{
    while (true)
    {
        std::lock_guard<std::mutex> guard(mutex);
        while (connect_poll.size() > connect_poll_minSize)
        {
            produce_condition.wait(mutex);
            if(!isRunning)
                return;
        }
        std::cout << "添加连接\n" <<'\n';
        addConnect();
        consumer_condition.notify_all();   
    }
}

std::shared_ptr<MysqlConnect> MysqlConnectPoll::getConnect()
{
    std::lock_guard<std::mutex> guard(mutex);
    while (connect_poll.size() == 0)
    {
        mutex.unlock();
        produce_condition.notify_one();
        std::cv_status status = consumer_condition.wait_for(mutex,std::chrono::seconds(10));
        if(status == std::cv_status::timeout)
        {
            return nullptr;
        }
    }
    std::shared_ptr<MysqlConnect> p(connect_poll.front(),[this](MysqlConnect *p){
        std::lock_guard<std::mutex> guard(mutex);
        connect_poll.push(p);
    });
    connect_poll.pop();
    std::cout <<"获取连接\n";
    return p;
}


void MysqlConnectPoll::recovery()
{
    while (isRunning)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::lock_guard<std::mutex> guard(mutex);
        for(size_t i=0;i<connect_poll.size();i++)
        {
            if(connect_poll.size() <= connect_poll_minSize)
                break;
            MysqlConnect *p = connect_poll.front();
            if(p->getfreeTime() >= free_time)
            {
                delete p;
                connect_poll.pop();
                std::cout << "回收连接\n";
            }
        }
    }
}


MysqlConnectPoll::~MysqlConnectPoll()
{
    std::cout <<"销毁连接池\n";
    isRunning = false;
    t_recovery->join();
    produce_condition.notify_one();
    t_produce->join();
    delete t_recovery;
    delete t_produce;
    while (connect_poll.size())
    {
        delete connect_poll.front();
        connect_poll.pop();
    }
    
}