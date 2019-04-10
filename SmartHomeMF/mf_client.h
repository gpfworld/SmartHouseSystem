#ifndef MF_CLIENT_H
#define MF_CLIENT_H

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
#include"clt_common.h"

#include"widget.h"

#define PORT 9000
#define SADDR "192.168.1.10"


#define SIZE 2048 //消息正文的大小
#define MAX_CLIENT 500
#define SCKT_DATA_LEN 1600 //recv data max len

#define SIZE 2048 //消息正文的大小
#define MSG_FILE "../cpp_src/msg_mf_file"


//出错处理提示函数
void err_fun(const char *file_nam, const int line,const char *fun_nam, const int err_no);


void mf_register_fun(MF_Info& t_mfInfo);

void dev_register_fun(DEV_Info & t_devInfo);

void login_fun(MF_Info &t_mfInfo, Pack &snd_pack);

void logout_fun(MF_Info& t_mfInfo);

void signal_fun(int signo);

void show_list(list<DEV_Info*> t_devInfoList,char * t_mac);

void snd_recv_mf_dev(MF_Info & t_mfInfo,Pack & snd_pack,Pack & recv_pack);

void get_dev_data(MF_Glb & t_va,Pack & t_pack);

void get_dev_status(MF_Info & t_mfInfo,Pack & t_pack);

void set_dev_on(MF_Info & t_mfInfo,Pack & t_pack);

void set_dev_off(MF_Info & t_mfInfo,Pack & t_pack);

void set_dev_data(MF_Info & t_mfInfo,Pack & t_pack);

void snd_recv_mf_plat(MF_Info & t_mfInfo, Pack & recv_pack, Pack &snd_pack);

void get_devs_list(list<DEV_Info*> & t_devInfoList,MF_Info & t_mfInfo,Pack & t_pack);

void * pth_sckt_recv_fun(void * arg);

void * pth_msg_fun(void * arg);

void * pth_getUpdateData_fun(void * arg);

void select(int & t_select);

void mk_get_msg(mode_t creatmsg_mode, int proj_id,int & t_msg_skfd);

void flushDevUI(MF_Glb & t_va);

void updateDevInfoList(MF_Glb * t_va);

#endif // MF_CLIENT_H
