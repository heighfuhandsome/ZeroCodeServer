//
// Created by Administrator on 2022/11/16.
//
#include "../include/Configuration.h"

Configuration::Configuration(std::string jsonPath):path(jsonPath){

    int len;
    std::string cache;
    char buff[1024];
    int fd = open(path.c_str(),O_RDONLY);
    if(fd<0)
    {
        perror("Configuration::Configuration(std::string jsonPath) 16");
    }

    while ( (len = read(fd,buff,1024)) >0)
    {
        cache.append(buff,len);
    }


    Json::Value root;
    Json::Reader r;
    r.parse(cache,root, false);

    ip = root["ip"].asString();
    port = root["port"].asUInt();
    workThreadCount = root["workThreadCount"].asInt();

    methods = root["methods"];
}

const std::string &Configuration::getConfigPath() const{
    return path;
}

int Configuration::getPort() const{
    return port;
}

const std::string &Configuration::getIp() const{
    return ip;
}

const Configuration &Configuration::getConfiguration(std::string path) {
    static Configuration config(path);
    return config;
}

int Configuration::getWorkThreadCount() const {
    return workThreadCount;
}


const Json::Value& Configuration::getMethods() const{
    return methods;
}