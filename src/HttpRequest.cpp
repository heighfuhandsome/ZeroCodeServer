#include "../include/HttpRequest.h"

HttpRequest::HttpRequest():statu(ReqParseStatu::nostart)
{
    
}

std::string HttpRequest::getReq_path() const
{
    return req_path;
}

const ReqParseStatu& HttpRequest::getStatu() const
{
    return statu;
}

void HttpRequest::addToCache(const char *data,size_t size)
{
    cache.append(data,size);
}

const std::string& HttpRequest::getCache() 
{
    return cache;
}


void HttpRequest::setReq_method(std::string &method)
{
    if((method == "GET") || (method == "get"))
    {
        req_method = ReqMethod::get;
    }
    else
    {
        req_method = ReqMethod::post;
    }
}


void HttpRequest::setReq_path(std::string &path)
{
    req_path = path;
}

void HttpRequest::setHtp_version(std::string &httpVersion)
{
    http_version = httpVersion;
}


std::unordered_map<std::string,std::string>& HttpRequest::getHeaders()
{
    return headers;
}

void HttpRequest::freeCache(size_t size)
{
    cache = cache.substr(size);
}

ReqMethod HttpRequest::getReq_method()
{
    return req_method;
}

void HttpRequest::setReqParseStatu(ReqParseStatu stat)
{
    statu = stat;
}