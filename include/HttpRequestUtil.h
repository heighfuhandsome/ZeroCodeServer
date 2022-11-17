#pragma once
#include "HttpTask.h"
#include <arpa/inet.h>
#include <vector>
#include <regex>
#include <iostream>


class HttpRequestUtil
{
public:
    // 解析客户端发送来的http数据封装到HttpRequest 对象中
    static void parseClientDataToHttpRequest(HttpTask &task);
     
    static void split(std::string &&str,std::vector<std::string> &v,std::string &&dilimiter);
};