#include "../include/HttpRequestUtil.h"
void HttpRequestUtil::parseClientDataToHttpRequest(HttpTask &task)
{
    size_t size = recv(task.fd,task.buff,4096,0);
    if(size == 0)
    {
        task.disConnect = true;
        printf("%s\n","客户端请求断开连接\n");
        return;
    }

    //如果请求数据已经全部接收则跳过
    if(task.request.getStatu() == ReqParseStatu::body_done)
        return;

    task.request.addToCache(task.buff,size);
    if(task.request.getStatu()==ReqParseStatu::nostart)
    {
        const std::string &cache = task.request.getCache();
        size_t pos1,pos2; //pos1 是请求行结束标志位  pos2 是请求头结束标志位
        pos2 = cache.find("\r\n\r\n") ;
        if(pos2 == std::string::npos)
        {
            return;
        }
        else
        {
            // 解析请求行
            std::vector<std::string> v;
            pos1 = cache.find("\r\n");
            split(cache.substr(0,pos1),v," ");
            task.request.setReq_method(v[0]);
            task.request.setReq_path(v[1]);
            task.request.setHtp_version(v[2]);
            
            // 解析请求头
            std::regex r(".*+\r\n");
            for(std::sregex_iterator start(cache.begin()+pos1+2,cache.begin()+pos2+2,r),end;start!=end;start++)
            {

                const std::string &s = start->str();
                size_t pos = s.find(":");
                std::unordered_map<std::string,std::string> &headers = task.request.getHeaders();
                headers.insert({ s.substr(0,pos),s.substr(pos+1)});
            }
            
            // 释放cache 中请求体以外的数据
            task.request.freeCache(pos2+4);
            if(task.request.getReq_method() == ReqMethod::get)
            {
                task.request.setReqParseStatu(ReqParseStatu::body_done);
            }
            else
            {
                task.request.setReqParseStatu(ReqParseStatu::headers_done);
            }
        }
    }
    else
    {
        // for(auto &pair:task.request.getHeaders())
        // {
        //     std::cout << pair.first <<": " << pair.second<<"\n";
        // }
        
        size_t content_length = std::stoul(task.request.getHeaders()["Content-Length"]);
        if(task.request.getCache().size() == content_length)
        {
            task.request.setReqParseStatu(ReqParseStatu::body_done);
        }
    }


}

void HttpRequestUtil::split(std::string &&str,std::vector<std::string> &v,std::string &&delimiter)
{
    size_t pos1=0,pos2=0;
    int delimiter_size = delimiter.size();
    while ((pos2 = str.find(delimiter,pos1))!= std::string::npos)
    {
        if(pos1 != pos2)
        {
            v.emplace_back(str.substr(pos1,pos2-pos1));
        }

        pos2 += delimiter_size;
        pos1 = pos2;
    }

    if(pos1 < str.size())
    {
        v.emplace_back(str.substr(pos1,str.size() - pos1));
    }
    
}