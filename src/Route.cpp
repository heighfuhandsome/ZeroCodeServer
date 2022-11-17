#include "../include/Route.h"
#include <sys/sendfile.h>
void Route::Mapping(HttpTask &task)
{
    static Route route;
    if(route.func_map.find(task.request.getReq_path()) == route.func_map.end())
    {
        task.response.setResBuildStatu(ResBuildStatu::body_done);
    }
    else
    {
        route.func_map[task.request.getReq_path()](task);
    }
}

void Route::preWork(HttpResponse &o,int code,const char* des)
{
    o.setRes_code(code);
    o.setRes_des(des);
    o.setHttp_version ("HTTP.1.1");
}


Route::Route()
{    
    poll = MysqlConnectPoll::getMysqlConnectPoll("");

    func_map.insert({"/",[this](HttpTask &o){
        const char *body = "HTTP/1.1 200 OK\r\nContent-Length:11\r\n\r\nhello world";
        send(o.fd,body,strlen(body),0);
        o.response.setResBuildStatu(ResBuildStatu::body_done);
        return nullptr;
    }});

    //应为拿的是局部静态变量 所以config已经是初始化过的了
    const Configuration& config= Configuration::getConfiguration("");
    const Json::Value &methods = config.getMethods();
    for(int i=0;i<methods.size();i++)
    {
        const Json::Value &v = methods[i];
        std::vector<std::string> keys = v.getMemberNames();
        if(keys[0] == "sql")
        {
            //验证配置文件中的sql是否正确
            std::shared_ptr<MysqlConnect> ptr = poll->getConnect();
            bool success = ptr->query(v["content"].asString().c_str());
            if(!success)
            {
                continue;
            }

            
        }else if(keys[0] == "file")
        {
            //验证配置文件中的文件是否存在
            int ret = open(v["content"].asString().c_str(),O_RDONLY);
            if(ret < 0)
            {
                continue;
            }
        }
    }
}