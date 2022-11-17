#pragma once
#include <functional>
#include <vector>
#include <string>
#include <unordered_map>
#include <memory>
#include <iostream>
#include <sys/socket.h>
#include <cstring>
#include "HttpTask.h"
#include "json.h"
#include "Configuration.h"
#include "MysqlConnectPoll.h"
#include "MysqlConnect.h"



class Route{
public:
    Route(const Route &) = delete;
    Route& operator=(const Route &) = delete;
    static void Mapping(HttpTask &task);


private:
    Route();
    void preWork(HttpResponse &response,int code,const char* );
    void sendFile();

private:
    std::unordered_map<std::string,std::function<std::shared_ptr<void>(HttpTask&)>> func_map;
    MysqlConnectPoll *poll; //指向的静态变量 不需要手动释放
};