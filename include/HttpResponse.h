#pragma once
#include <string>
#include <unordered_map>

enum class ResBuildStatu{nostart,headers_done,body_done};
class HttpResponse{
public:
    HttpResponse();
    HttpResponse(const HttpResponse &o) = delete;
    HttpResponse& operator=(const HttpResponse &o) = delete;


    void setRes_code(int code);
    void setRes_des(std::string &&des);
    void setHttp_version(std::string &&version);
    void setResBuildStatu(ResBuildStatu statu);

    ResBuildStatu getResBuildStatu();

private:
    int res_code;
    std::string res_des;
    std::string http_version;
    std::unordered_map<std::string,std::string> headers;
    ResBuildStatu res_build_statu;

    unsigned long send_over;  //响应体已发送字节数

};