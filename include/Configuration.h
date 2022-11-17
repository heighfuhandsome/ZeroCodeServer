//
// Created by Administrator on 2022/11/16.
//

#ifndef ZEROCODESERVER_CONFIGURATION_H
#define ZEROCODESERVER_CONFIGURATION_H
//全局配置类  单例

#include <string>
#include "json.h"
#include <fcntl.h>
#include <unistd.h>
class Configuration {
public:
    Configuration(const Configuration &o) =delete;
    Configuration& operator=(const Configuration &o) = delete;
    const static Configuration& getConfiguration(std::string path);


    const std::string&  getConfigPath() const;
    const std::string& getIp() const;
    int getPort() const;
    int getWorkThreadCount() const;
    const Json::Value& getMethods() const;
private:
    Configuration(std::string configPath);
private:
    std::string  path;
    std::string ip;
    ushort port;
    int workThreadCount ;
    Json::Value methods;
};


#endif //ZEROCODESERVER_CONFIGURATION_H
