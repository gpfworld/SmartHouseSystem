#ifndef DEV_INFO_H
#define DEV_INFO_H
using namespace std;
#include<iostream>
#include"../h_src/common.h"

#define MAC_LEN 11
#define PWD_MAX_LEN 12
#define NAME_MAX_LEN 20
#define IP_LEN 15

class DEV_Info
{
    public:
        char m_mac [MAC_LEN] ;			//设备mac地址
        char m_pwd [PWD_MAX_LEN];		//设备密码
        char m_name[NAME_MAX_LEN];		//设备名称
        char m_ip[IP_LEN];				//设备ip
        short m_port;					//设备端口
        char * m_token;					//设备口令
        char m_tokenLen;				//设备口令长度
        int m_skfd;						//设备socket文件描述符
        unsigned char m_status;			//设备状态
        char m_data;					//设备的数据，比如R值，比如温度
        char m_devType;					//设备类型，比如灯，空调
    public:
		/*默认构造函数*/
        DEV_Info();
		/*带参构造函数
		*t_mac:设备mac地址
		*t_pwd:设备密码
		*t_name:设备名称
		*t_ip:设备ip
		*t_port:设备端口
		*/
        DEV_Info(const char * t_mac,const char* t_pwd,const char * t_name,const char *t_ip,short t_port);
        /*复制构造函数*/
		DEV_Info(DEV_Info & t_devInfo);
        /*等号重载*/
		DEV_Info & operator=(DEV_Info & t_devInfo);
};


#endif // PACK_H
