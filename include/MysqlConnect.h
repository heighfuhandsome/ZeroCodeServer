#pragma once
#include <mysql/mysql.h>
#include <string>

#include <time.h>
class MysqlConnect{
    friend class MysqlConnectPoll;
public:
    MysqlConnect(const MysqlConnect &o) = delete;
    MysqlConnect& operator=(MysqlConnect &o) = delete;

    bool connect(std::string &host,std::string &user,std::string &password,std::string &dbname,unsigned short port);
    bool update(const char* sql);
    bool query(const char* sql);
    bool next();
    void setAutoSubmit(bool autoSubmit);
    bool submit();
    bool rollback();
    const char* index(int index);
    // 单位是毫秒
    unsigned long getfreeTime();
    ~MysqlConnect();
private:
    void freeRes();
    MysqlConnect();
    void refushTimePoint();
private:
    MYSQL *m_mysql;
    MYSQL_RES *m_res;
    MYSQL_ROW m_row;

    // 最后一次被放入连接池等待被使用的时间
    time_t t;
};