/*************************************************************************
	> File Name: mf_back_client.h
	> Author: 
	> Mail: 
	> Created Time: 2016年10月19日 星期三 00时01分59秒
 ************************************************************************/

#ifndef _MF_BACK_CLIENT_H
#define _MF_BACK_CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/msg.h>



#include<string>
#include<iostream>
using namespace std;

#include"../h_src/pack.h"
#include"../h_src/protocal.h"
#include"../h_src/file_opt.h"
#include"../h_src/dev_info.h"


#define PORT 9000
#define SADDR "192.168.1.10"


#define SIZE 2048 //消息正文的大小
#define MAX_CLIENT 500
#define SCKT_DATA_LEN 1600 //recv data max len

#define SIZE 2048 //消息正文的大小
#define MSG_FILE "msg_mf_file"


class MF_Glb
{ 
    public:
    int m_msg_skfd;
    list<DEV_Info *> m_devInfoList;
    MF_Info * m_mfInfo;
};


void err_fun(const char *file_nam, const int line,const char *fun_nam, const int err_no);

void socket_fun(int & t_sckt);//网络连接函数

void mf_register_fun(MF_Glb &t_va);//客户端注册函数

void dev_register_fun(MF_Glb &t_va, DEV_Info & t_devInfo);//设备注册函数

void login_fun(MF_Glb & t_va, Pack &snd_pack);//客户登陆函数

void logout_fun(MF_Info& t_mfInfo);//客户登出函数

void signal_fun(int signo);//信号处理函数

void show_list(list<DEV_Info*> t_devInfoList,char * t_mac);//

void snd_recv_mf_dev(MF_Info & t_mfInfo,Pack & snd_pack,Pack & recv_pack);//客户端向设备发送控制命令

//void get_dev_data(MF_Glb & t_va,Pack & t_pack);
void get_dev_data(MF_Glb & t_va,Pack & t_pack);//获取设备数据
void get_dev_status(MF_Glb & t_va,Pack & t_pack);//获取设备工作状态

void set_dev_on(MF_Info & t_mfInfo,Pack & t_pack);//打开设备

void set_dev_off(MF_Info & t_mfInfo,Pack & t_pack);//关闭设备
void set_dev_data(MF_Info & t_mfInfo,Pack & t_pack);//设置设备数据

void snd_recv_mf_plat(MF_Info & t_mfInfo, Pack & recv_pack, Pack &snd_pack);//客户端向中控发送消息

void get_devs_list(MF_Glb & t_va,Pack & t_pack);//获取设备在线列表

void * pth_sckt_recv_fun(void * arg);//接收数据线程

void * pth_msg_fun(void * arg);//发送消息线程

void select(int & t_select);//

void mk_get_msg(mode_t creatmsg_mode, int proj_id,int & t_msg_skfd);//设置\打开消息队列



#endif
