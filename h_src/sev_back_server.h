#ifndef SEV_BACK_SERVER_H
#define SEV_BACK_SERVER_H


#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
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
#include <sys/shm.h>


#define PORT 9000
#define SADDR "192.168.1.10"
#define SCKT_DATA_LEN 1600
#define TOKEN_LEN 13
#include"../h_src/mf_info.h"
#include"../h_src/dev_info.h"

#include"../h_src/common.h"
#include"../h_src/pack.h"
#include"../h_src/protocal.h"
#include"../h_src/file_opt.h"


#include<list>
#include<fstream>
#include<iostream>
using namespace std;
#define SIZE 2048 //消息正文的大小
#define MSG_FILE "msg_sev_file"

#define SHM_FILE "./shm_file"
#define SHM_SIZE 1024
#define MAX_CLIENT 500

#define mf_file "sys_user.txt"
#define dev_file "sys_dev.txt"

#define NAME_MAX_LEN   20
#define MAC_LEN        11



class SEV_Glb
{
    public:
    int m_skfd;
    int m_clt_skfd;
    file_opt m_mfFile;
    file_opt m_devFile;
    list<MF_Info*> m_mfInfoList;
    int m_msg_skfd;
    list<DEV_Info *> m_devInfoList;
    MF_Info * m_mfInfo;
};


void err_fun(const char *file_nam, const int line, \
             const char *fun_nam, const int err_no);

void random(char * t_token,char t_tokenLen);  //随机产生torken值


void mk_get_msg(mode_t creatmsg_mode, int proj_id,SEV_Glb * g_va);//创建消息队列

void signal_fun(int signo);//信号处理函数


int login_register_fun(SEV_Glb &t_va);//客户\设备的注册登录


void socket_fun(SEV_Glb &t_va);//socket网络连接函数

//void socket_fun();


bool find_skfd_by_mac(char * t_mac,SEV_Glb & t_va,int & t_skfd);//查找对应设备的skfd

DEV_Info * find_dev_by_mac(char * t_mac,SEV_Glb & t_va);//通过mac查找对应设备

bool delete_dev_by_mac(char * t_mac,SEV_Glb & t_va);//通过mac删除对应设备

bool find_skfd_by_id(char * t_id,SEV_Glb & t_va,int & t_skfd);//通过id找到对应客户端的skfd

bool delete_mf_by_id(char * t_id,SEV_Glb & t_va);//通过id删除对应客户

void send_data_to_allmf(SEV_Glb & t_va,char * t_data);//向所有客户端发送消息

void snd_pack_to_ui(SEV_Glb & t_va,Pack & snd_pack);//向QT端发pack包

void *pth_mf_fun(void *arg);//客户端服务线程


void *pth_dev_fun(void *arg);//设备端服务线程

void select(int & t_select);//


void show_list(list<DEV_Info*> t_devInfoList,char * t_mac);//


void snd_recv_plat_dev(SEV_Glb & t_va, Pack &snd_pack);//中控向设备发送消息函数


void *pth_sev_fun(void *arg);//服务器工作线程

void * pth_msg_fun(void * arg);//

#endif //SEV_BACK_SERVER_H

