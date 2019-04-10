#ifndef SERVER_H
#define SERVER_H


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

#include"sev_common.h"

#include<list>
#include<fstream>
#include<iostream>
using namespace std;
#define SIZE 2048 //消息正文的大小

#define MSG_FILE "../cpp_src/msg_sev_file"

#define SHM_FILE "./shm_file"
#define SHM_SIZE 1024
#define MAX_CLIENT 500

#define mf_file "sys_user.txt"
#define dev_file "../cpp_src/sys_dev.txt"

#define NAME_MAX_LEN   20
#define MAC_LEN        11



void err_fun(const char *file_nam, const int line, \
             const char *fun_nam, const int err_no);

void random(char * t_token,char t_tokenLen);


void mk_get_msg(mode_t creatmsg_mode, int proj_id,SEV_Glb * g_va);

void signal_fun(int signo);


int login_register_fun(SEV_Glb &t_va);


void socket_fun(SEV_Glb &t_va);

//void socket_fun();


bool find_skfd_by_mac(char * t_mac,SEV_Glb & t_va,int & t_skfd);


bool delete_dev_by_mac(char * t_mac,SEV_Glb & t_va);

bool find_skfd_by_id(char * t_id,SEV_Glb & t_va,int & t_skfd);

bool delete_mf_by_id(char * t_id,SEV_Glb & t_va);

void send_data_to_allmf(SEV_Glb & t_va,char * t_data);

void *pth_mf_fun(void *arg);

void *pth_dev_fun(void *arg);

void select(int & t_select);

void show_list(list<DEV_Info*> t_devInfoList,char * t_mac);

void snd_recv_plat_dev(SEV_Glb & t_va, Pack &snd_pack);

void *pth_sev_fun(void *arg);

void * pth_msg_fun(void * arg);

void updateDevInfoList(SEV_Glb * t_va);

void * pth_getUpdateData_fun(void * arg);

#endif // SERVER_H
