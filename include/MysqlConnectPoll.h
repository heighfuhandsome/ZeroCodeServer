#pragma once
#include <memory>
#include <thread>
#include <queue>
#include "MysqlConnect.h"
#include <string>
#include <mutex>
#include <condition_variable>


class MysqlConnectPoll
{
public:
    static MysqlConnectPoll* getMysqlConnectPoll(const std::string &path);
    MysqlConnectPoll(const MysqlConnectPoll &o) = delete;
    MysqlConnectPoll& operator=(const MysqlConnectPoll &o) = delete;
    std::shared_ptr<MysqlConnect> getConnect();
    size_t getPollSize();
    ~MysqlConnectPoll();
private:
    MysqlConnectPoll(std::string configPath);
    void addConnect();
    void produce();
    void recovery();
    

private:
    std::queue<MysqlConnect*> connect_poll;

    // mysql 连接信息
    std::string host;
    std::string user;
    std::string password;
    std::string dbname;
    unsigned short port;
    unsigned int connect_poll_maxSize;
    unsigned int connect_poll_minSize;
    unsigned int free_time;

    // 互斥锁和条件变量
    std::mutex mutex;
    std::condition_variable_any produce_condition;
    std::condition_variable_any consumer_condition;

    //连接池当前状态
    bool isRunning;

    // 生产者线程和回收线程
    std::thread *t_produce;
    std::thread *t_recovery;
};