#include"../h_src/mf_info.h"
#include<string>
#include<string.h>
#define TOKEN_LEN 13


MF_Info::MF_Info()
{
    memset(this->m_id,0,ID_MAX_LEN);
    memset(this->m_pwd,0,PWD_MAX_LEN);
    memset(this->m_name,0,NAME_MAX_LEN);
    memset(this->m_mailBox,0,MAILBOX_MAX_LEN);

    this->m_token=new char[TOKEN_LEN];
    this->m_tokenLen=TOKEN_LEN-1;
    bzero(this->m_token,TOKEN_LEN);
}
MF_Info::MF_Info(const char * t_id,const char* t_pwd,const char * t_name,const char *t_mailBox)
{
    memset(this->m_id,0,ID_MAX_LEN);
    memset(this->m_pwd,0,PWD_MAX_LEN);
    memset(this->m_name,0,NAME_MAX_LEN);
    memset(this->m_mailBox,0,MAILBOX_MAX_LEN);
    
    memcpy(this->m_id,t_id,strlen(t_id));
    memcpy(this->m_pwd,t_pwd,strlen(t_pwd));
    memcpy(this->m_name,t_name,strlen(t_name));
    memcpy(this->m_mailBox,t_mailBox,strlen(t_mailBox));
    this->m_token=new char[TOKEN_LEN];
    this->m_tokenLen=TOKEN_LEN-1;
    bzero(this->m_token,TOKEN_LEN);
}
MF_Info& MF_Info::operator=(MF_Info &t_mfInfo) 
{
    if(this == &t_mfInfo) return *this;

    memset(this->m_id,0,ID_MAX_LEN);
    memset(this->m_pwd,0,PWD_MAX_LEN);
    memset(this->m_name,0,NAME_MAX_LEN);
    memset(this->m_mailBox,0,MAILBOX_MAX_LEN);

    memcpy(this->m_id,t_mfInfo.m_id,strlen(t_mfInfo.m_id));
    memcpy(this->m_pwd,t_mfInfo.m_pwd,strlen(t_mfInfo.m_pwd));
    memcpy(this->m_name,t_mfInfo.m_name,strlen(t_mfInfo.m_name));
    memcpy(this->m_mailBox,t_mfInfo.m_mailBox,strlen(t_mfInfo.m_mailBox));

    this->m_tokenLen=t_mfInfo.m_tokenLen;
    memcpy(this->m_token,t_mfInfo.m_token,TOKEN_LEN);
    return *this;
} 
MF_Info::MF_Info(MF_Info &t_mfInfo) 
{
    memset(this->m_id,0,ID_MAX_LEN);
    memset(this->m_pwd,0,PWD_MAX_LEN);
    memset(this->m_name,0,NAME_MAX_LEN);
    memset(this->m_mailBox,0,MAILBOX_MAX_LEN);

    memcpy(this->m_id,t_mfInfo.m_id,strlen(t_mfInfo.m_id));
    memcpy(this->m_pwd,t_mfInfo.m_pwd,strlen(t_mfInfo.m_pwd));
    memcpy(this->m_name,t_mfInfo.m_name,strlen(t_mfInfo.m_name));
    memcpy(this->m_mailBox,t_mfInfo.m_mailBox,strlen(t_mfInfo.m_mailBox));

    this->m_tokenLen=t_mfInfo.m_tokenLen;
    memcpy(this->m_token,t_mfInfo.m_token,TOKEN_LEN);
}
