#include "../include/HttpResponse.h"

HttpResponse::HttpResponse():res_build_statu(ResBuildStatu::nostart)
{
    
}

void HttpResponse::setRes_code(int code)
{
    this->res_code = code;
}

void HttpResponse::setRes_des(std::string &&des)
{
    res_des = des;
}

void HttpResponse::setHttp_version(std::string &&version)
{
    http_version = version;
}


ResBuildStatu HttpResponse::getResBuildStatu()
{
    return res_build_statu;
}

void HttpResponse::setResBuildStatu(ResBuildStatu statu)
{
    res_build_statu = statu;
}