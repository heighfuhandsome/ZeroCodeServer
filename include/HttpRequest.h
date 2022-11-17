#pragma once
#include <unordered_map>
#include <string>
#include <memory>


// 请求方式
enum class ReqMethod{get,post};
// http请求解析状态
enum class ReqParseStatu{ nostart,line_done,headers_done,body_done }; //{未开始,请求行解析完成,请求头解析完成,请求体解析完成}
class HttpRequest{
public:
    HttpRequest() ;
    HttpRequest(const HttpRequest &o) = delete;
    HttpRequest& operator=(const HttpRequest &o) = delete;

    void setReq_method(std::string &request);

    void setReq_path(std::string &path);

    void setHtp_version(std::string &httpVersion);

    void setReqParseStatu(ReqParseStatu stat);

    std::unordered_map<std::string,std::string>& getHeaders();

    // 获取缓存
    const std::string& getCache() ;

    std::string getReq_path() const;

    const ReqParseStatu& getStatu() const;

    ReqMethod getReq_method();

    // 释放缓存中已读取数据
    void freeCache(size_t size);

     // 将客户端发来的数据放入缓存
    void addToCache(const char *data,size_t size);
private:
    // 请求方法
    ReqMethod req_method;
    // 请求路径
    std::string req_path;
    // http版本
    std::string http_version;
    // 请求头
    std::unordered_map<std::string,std::string> headers;

    // 用于缓存client发送来的http除了请求体以外的数据，便于解析
    std::string cache;

    // 解析状态
    ReqParseStatu statu;

    
};