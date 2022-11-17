#include <iostream>
#include "../include/ThreadPoll.h"
#include "../include/Configuration.h"
int main(int argc,char** argv)
{
    if(argc <2 )
    {
        printf("请指定配置文件路径\n");
        exit(0);
    }
    
    //初始化配置文件
    const Configuration& config=Configuration::getConfiguration(argv[1]);

    //初始化数据库连接池
    MysqlConnectPoll::getMysqlConnectPoll(config.getConfigPath());
    
    int fd = socket(AF_INET,SOCK_STREAM,0);
    if(fd<0)
    {
        perror("socket");
    }
    sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(config.getPort());
    inet_pton(AF_INET,config.getIp().c_str(),&saddr.sin_addr.s_addr);

    sockaddr_in caddr;

    int ret = bind(fd,(sockaddr*)&saddr,sizeof(saddr));
    if(ret < 0)
    {
        perror("bind");
    }
    ret = listen(fd,1024);
    if(ret < 0)
    {
        perror("listen");
    }

    ThreadPoll poll(config.getWorkThreadCount());
    while (true)
    {
        socklen_t len;
        int cfd = accept(fd,(sockaddr*)&caddr,&len);
        // std::cout <<"客户端请求地址：" << inet_ntoa(caddr.sin_addr) <<"\n";
        poll.addTask(cfd);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    
    
    return 0;
}