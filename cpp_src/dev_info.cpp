#include"../h_src/dev_info.h"
#include<string>
#include<string.h>
#define TOKEN_LEN 13


DEV_Info::DEV_Info()
{
    memset(this->m_mac,0,MAC_LEN);
    memset(this->m_pwd,0,PWD_MAX_LEN);
    memset(this->m_name,0,NAME_MAX_LEN);
    memset(this->m_ip,0,IP_LEN);
    this->m_port=-1;
    this->m_token=new char[TOKEN_LEN];
    this->m_tokenLen=TOKEN_LEN-1;
    bzero(this->m_token,TOKEN_LEN);

    this->m_status=DEV_OFF;
    this->m_data=0;
    this->m_light=0;
    this->m_devType=-1;
    this->m_isOnline=DEV_OFFLINE;
}
DEV_Info::DEV_Info(const char * t_mac,const char* t_pwd,const char * t_name,const char *t_ip,short t_port)
{
    memset(this->m_mac,0,MAC_LEN);
    memset(this->m_pwd,0,PWD_MAX_LEN);
    memset(this->m_name,0,NAME_MAX_LEN);
    memset(this->m_ip,0,IP_LEN);
    
    memcpy(this->m_mac,t_mac,strlen(t_mac));
    memcpy(this->m_pwd,t_pwd,strlen(t_pwd));
    memcpy(this->m_name,t_name,strlen(t_name));
    memcpy(this->m_ip,t_ip,strlen(t_ip));
    this->m_port=t_port;
    this->m_token=new char[TOKEN_LEN];
    this->m_tokenLen=TOKEN_LEN-1;
    bzero(this->m_token,TOKEN_LEN);
    this->m_status=DEV_OFF;
    this->m_data=0;
    this->m_light=0;
    this->m_devType=-1;
     this->m_isOnline=DEV_OFFLINE;
}
DEV_Info& DEV_Info::operator=(DEV_Info &t_devInfo) 
{
    if(this == &t_devInfo) return *this;

    memset(this->m_mac,0,MAC_LEN);
    memset(this->m_pwd,0,PWD_MAX_LEN);
    memset(this->m_name,0,NAME_MAX_LEN);
    memset(this->m_ip,0,IP_LEN);

    memcpy(this->m_mac,t_devInfo.m_mac,strlen(t_devInfo.m_mac));
    memcpy(this->m_pwd,t_devInfo.m_pwd,strlen(t_devInfo.m_pwd));
    memcpy(this->m_name,t_devInfo.m_name,strlen(t_devInfo.m_name));
    memcpy(this->m_ip,t_devInfo.m_ip,strlen(t_devInfo.m_ip));
    this->m_port=t_devInfo.m_port;
    this->m_tokenLen=t_devInfo.m_tokenLen;
    memcpy(this->m_token,t_devInfo.m_token,TOKEN_LEN);
    this->m_status=t_devInfo.m_status;
    this->m_data=t_devInfo.m_data;
    this->m_devType=t_devInfo.m_devType;
    this->m_light=t_devInfo.m_light;
    this->m_light=t_devInfo.m_isOnline;
    return *this;
} 
DEV_Info::DEV_Info(DEV_Info &t_devInfo) 
{
    memset(this->m_mac,0,MAC_LEN);
    memset(this->m_pwd,0,PWD_MAX_LEN);
    memset(this->m_name,0,NAME_MAX_LEN);
    memset(this->m_ip,0,IP_LEN);

    memcpy(this->m_mac,t_devInfo.m_mac,strlen(t_devInfo.m_mac));
    memcpy(this->m_pwd,t_devInfo.m_pwd,strlen(t_devInfo.m_pwd));
    memcpy(this->m_name,t_devInfo.m_name,strlen(t_devInfo.m_name));
    memcpy(this->m_ip,t_devInfo.m_ip,strlen(t_devInfo.m_ip));
    this->m_port=t_devInfo.m_port;
    this->m_tokenLen=t_devInfo.m_tokenLen;
    memcpy(this->m_token,t_devInfo.m_token,TOKEN_LEN);
    this->m_status=t_devInfo.m_status;
    this->m_data=t_devInfo.m_data;
    this->m_devType=t_devInfo.m_devType;
    this->m_light=t_devInfo.m_light;
    this->m_isOnline=t_devInfo.m_isOnline;

}
