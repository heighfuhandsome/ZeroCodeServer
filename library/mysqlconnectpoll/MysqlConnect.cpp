#include "MysqlConnect.h"

MysqlConnect::MysqlConnect():m_res(nullptr),m_row(nullptr)
{
    m_mysql = mysql_init(nullptr);
    if(!m_mysql)
    {
        perror("mysql_init");
        exit(0);
    }
    int ret = mysql_set_character_set(m_mysql,"utf8");
    if(ret < 0)
    {
        perror("mysql_set_character_set");
        exit(0);
    }
   
}

bool MysqlConnect::connect(std::string &host,std::string &user,std::string &password,std::string &dbname,unsigned short port)
{
    if(mysql_real_connect(m_mysql,host.c_str(),user.c_str(),password.c_str(),dbname.c_str(),port,nullptr,0))
    {
        return true;
    }
    perror("mysql_real_connect");
    return false;
}

void MysqlConnect::freeRes()
{
    if(m_res)
    {
        mysql_free_result(m_res);
    }
}

bool MysqlConnect::update(const char* sql)
{
    
    if(mysql_query(m_mysql,sql))
    {
        return false;
    }
    return true;
}

bool MysqlConnect::query(const char* sql)
{
    freeRes();
    if(mysql_query(m_mysql,sql))
    {
        return false;
    }
    m_res = mysql_store_result(m_mysql);
    return true;
}

bool MysqlConnect::next()
{
    if(!m_res)
        return false;
    m_row = mysql_fetch_row(m_res);
    if(!m_row)
        return false;
    return true;
}

const char* MysqlConnect::index(int index)
{
    unsigned int field_num = mysql_num_fields(m_res);
    if(index<0 || index >= field_num)
    {
        return "";
    }
    return m_row[index];
}

 void MysqlConnect::setAutoSubmit(bool autoSubmit)
 {
    mysql_autocommit(m_mysql,autoSubmit);
 }

 bool MysqlConnect::submit()
 {
    return mysql_commit(m_mysql);
 }

bool MysqlConnect::rollback()
{
    return mysql_rollback(m_mysql);
}

void MysqlConnect::refushTimePoint()
{
    time(&t);
}

unsigned long MysqlConnect::getfreeTime()
{
    time_t tt;
    time(&tt);
    return difftime(tt,t);
}

MysqlConnect::~MysqlConnect()
{
    freeRes();
    if(m_mysql)
        mysql_close(m_mysql);
}



