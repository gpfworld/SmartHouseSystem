/*************************************************************************
	> File Name: file_opt.cpp
	> Author: 
	> Mail: 
	> Created Time: 2016年09月27日 星期二 20时35分49秒
 ************************************************************************/
#include"../h_src/file_opt.h"
#include<stdlib.h>

void err_fun(const char *file_nam, const int line,const char *fun_nam)
{
    fprintf(stderr, "in %s %d, %s is fail!\n", file_nam, line, fun_nam);
    exit(-1);
}
file_opt::file_opt(const char * t_file_name)
{
    this->m_count=0;
    sem_init(&m_sem_mutex,0,1);
    sem_init(&m_sem_rw,0,1);
    sem_init(&m_sem_w,0,1);
    bzero(m_file_name,MAX_FILE_NAME_LEN);
    strcpy(m_file_name,t_file_name);
}
file_opt::file_opt()
{
    this->m_count=0;
    sem_init(&m_sem_mutex,0,1);
    sem_init(&m_sem_rw,0,1);
    sem_init(&m_sem_w,0,1);
    bzero(m_file_name,MAX_FILE_NAME_LEN);
}
void file_opt::setFile(const char * t_file_name)
{
    bzero(m_file_name,MAX_FILE_NAME_LEN);
    strcpy(m_file_name,t_file_name);
}
file_opt::~file_opt()
{
    sem_destroy(&m_sem_mutex);
    sem_destroy(&m_sem_rw);
    sem_destroy(&m_sem_w);
}
void file_opt::read_sync_lock()
{
    sem_wait(&m_sem_w);
    sem_wait(&m_sem_mutex);
        if(m_count==0)
            sem_wait(&m_sem_rw);
        m_count++;
    sem_post(&m_sem_mutex);
    sem_post(&m_sem_w);
}
void file_opt::read_sync_unlock()
{
    sem_wait(&m_sem_mutex);
        m_count--;
        if(m_count==0)
            sem_post(&m_sem_rw);
    sem_post(&m_sem_mutex);
}
void file_opt::write_sync_lock()
{
    sem_wait(&m_sem_w);
    sem_wait(&m_sem_rw);
}
void file_opt::write_sync_unlock()
{
    sem_post(&m_sem_rw);
    sem_post(&m_sem_w);
}
bool file_opt::is_MF_Registersuccess(MF_Info & t_mfInfo)
{
    MF_Info tt_info;

    read_sync_lock();
    m_fInOut.open(m_file_name,ios::in|ios::out|ios::app);
    while(!m_fInOut.eof())
    {
        m_fInOut>>tt_info.m_id>>tt_info.m_pwd>>tt_info.m_name>>tt_info.m_mailBox;    
        if(0==strcmp(t_mfInfo.m_id,tt_info.m_id))
        {
            this->m_fInOut.close();
            read_sync_unlock();
            return false;
        }
    }
    this->m_fInOut.close();
    read_sync_unlock();
    //写互斥
    write_sync_lock();
    m_fInOut.open(m_file_name,ios::in|ios::out|ios::app);
    
    m_fInOut<<t_mfInfo.m_id<<" "<<t_mfInfo.m_pwd<<" "<<t_mfInfo.m_name<<" "<<t_mfInfo.m_mailBox<<endl;
    this->m_fInOut.close();
    write_sync_unlock();
    return true;
}
bool file_opt::is_MF_Loginsuccess(MF_Info & t_mfInfo)
{
    MF_Info tt_info;
    
    read_sync_lock();
        this->m_fInOut.open(m_file_name,ios::in|ios::out|ios::app);
    while(!m_fInOut.eof())
    {
        m_fInOut>>tt_info.m_id>>tt_info.m_pwd>>tt_info.m_name>>tt_info.m_mailBox;    
        if(((0==strcmp(t_mfInfo.m_id,tt_info.m_id))&(0==strcmp(t_mfInfo.m_pwd,tt_info.m_pwd))))
        {
            memcpy(t_mfInfo.m_name,tt_info.m_name,strlen(tt_info.m_name));
            memcpy(t_mfInfo.m_mailBox,tt_info.m_mailBox,strlen(tt_info.m_mailBox));
            this->m_fInOut.close();
            read_sync_unlock();

            return true;
        }    
    }
    //读访问控制
    this->m_fInOut.close();
    read_sync_unlock();
    return false;
}
bool file_opt::is_DEV_Registersuccess(DEV_Info & t_devInfo)
{
    DEV_Info tt_info;
    
    read_sync_lock();
    this->m_fInOut.open(m_file_name,ios::in|ios::out|ios::app);
    
    while(!m_fInOut.eof())
    {
        m_fInOut>>tt_info.m_devType>>tt_info.m_mac>>tt_info.m_pwd>>tt_info.m_name>>tt_info.m_ip>>tt_info.m_port;
        if(0==strcmp(t_devInfo.m_mac,tt_info.m_mac))
        {
            this->m_fInOut.close();
            read_sync_unlock();
            return false;
        }
    }
    this->m_fInOut.close();
    read_sync_unlock();
    
    
    //写互斥
    write_sync_lock();
    this->m_fInOut.open(m_file_name,ios::in|ios::out|ios::app);
    m_fInOut<<t_devInfo.m_devType<<" "<<t_devInfo.m_mac<<" "<<t_devInfo.m_pwd<<" "<<t_devInfo.m_name<<" "<<t_devInfo.m_ip<<" "<<t_devInfo.m_port<<endl;
    
    this->m_fInOut.close();
    write_sync_unlock();
    
    return true;
}

bool file_opt::is_DEV_Loginsuccess(DEV_Info & t_devInfo)
{
    cout<<t_devInfo.m_mac<<" "<<t_devInfo.m_pwd<<endl;
    DEV_Info tt_info;
    read_sync_lock();
    if(!this->m_fInOut.is_open()) 
    this->m_fInOut.open(m_file_name,ios::in|ios::out|ios::app);
    //读访问控制
    while(!m_fInOut.eof())
    {
        cout<<"read file"<<endl;
        m_fInOut>>tt_info.m_devType>>tt_info.m_mac>>tt_info.m_pwd>>tt_info.m_name>>tt_info.m_ip>>tt_info.m_port;
        cout<<tt_info.m_mac<<"  "<<tt_info.m_pwd<<endl;
        if(((0==strcmp(t_devInfo.m_mac,tt_info.m_mac))&(0==strcmp(t_devInfo.m_pwd,tt_info.m_pwd))))
        {
            memcpy(t_devInfo.m_name,tt_info.m_name,strlen(tt_info.m_name));
            memcpy(t_devInfo.m_ip,tt_info.m_ip,strlen(tt_info.m_ip));
            t_devInfo.m_port=tt_info.m_port;
            t_devInfo.m_devType=tt_info.m_devType;


            this->m_fInOut.close();
            read_sync_unlock();

            return true;
        }
    }

    this->m_fInOut.close();
    read_sync_unlock();
    //读访问控制
    
    return false;
}
void file_opt::init_DEV_InfoList(list<DEV_Info*>& t_devInfoList)
{
    if(!this->m_fInOut.is_open())
    this->m_fInOut.open(m_file_name,ios::in|ios::out|ios::app);

    DEV_Info * tt_info;
    read_sync_lock();

    while(!m_fInOut.eof())
    {
        tt_info=new DEV_Info;
        m_fInOut>>tt_info->m_devType>>tt_info->m_mac>>tt_info->m_pwd>>tt_info->m_name>>tt_info->m_ip>>tt_info->m_port;
        if(strlen(tt_info->m_mac)!=0)
        t_devInfoList.push_back(tt_info);
    }
    this->m_fInOut.close();
    read_sync_unlock();
}
void file_opt::init_MF_InfoList(list<MF_Info*>& t_mfInfoList)
{

}
