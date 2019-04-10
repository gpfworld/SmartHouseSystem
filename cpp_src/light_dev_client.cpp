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

#include<string>
#include<iostream>
using namespace std;

#include"../h_src/pack.h"
#include"../h_src/protocal.h"
#include"../h_src/dev_info.h"
#include"../h_src/file_opt.h"

#include"../h_src/common.h"

#define PORT 9000
#define SADDR "192.168.1.10"


#define SIZE 2048 //消息正文的大小
#define MAX_CLIENT 500
#define SCKT_DATA_LEN 1600 //recv data max len


void err_fun(const char *file_nam, const int line,const char *fun_nam, const int err_no)
{
    fprintf(stderr, "in %s %d, %s is fail: %s\n", file_nam, line, fun_nam, strerror(err_no));
    exit(-1);
}
/*网络连接函数*/
void socket_fun(DEV_Info & t_devInfo)
{
    int ret=-1,skfd=-1;

    struct sockaddr_in ser_addr = {0};
    skfd = socket(AF_INET, SOCK_STREAM, 0);
    if(skfd < 0) err_fun(__FILE__, __LINE__, "socket", errno);

    ser_addr.sin_family = AF_INET; //协议族
    ser_addr.sin_port   = htons(PORT); //将port转为网络端序
    //ser_addr.sin_addr.s_addr = inet_addr(SADDR);
    inet_aton(SADDR, &ser_addr.sin_addr);
    ret =connect(skfd, (struct sockaddr *)&ser_addr, sizeof(ser_addr));
    if(ret < 0) err_fun(__FILE__, __LINE__, "bind", errno);

    t_devInfo.m_skfd=skfd;
}
/*设备登录函数*/
void login_fun(DEV_Info& t_devInfo)
{
    int ret=-1;
    char t_ptlData[SCKT_DATA_LEN];
    Protocal t_ptl;
    Pack snd_pack;
    Pack recv_pack;
    bzero(t_ptlData,SCKT_DATA_LEN);
    
    snd_pack.loginE(t_devInfo.m_mac,t_devInfo.m_pwd,DEV_LOGIN);
    t_ptl.setPack(snd_pack);
    int t_i=0; 
    t_ptl.encode(t_ptlData,t_i);

    ret = send(t_devInfo.m_skfd, t_ptlData,t_i, 0);
    if(ret < 0) err_fun(__FILE__, __LINE__, "sned", errno);
    bzero(t_ptlData, SCKT_DATA_LEN);
    t_i=0;
    ret = recv(t_devInfo.m_skfd,t_ptlData,SCKT_DATA_LEN, 0);
    if(ret < 0) err_fun(__FILE__, __LINE__, "sned", errno);
    else if(ret > 0)
    {
        t_ptl.decode(t_ptlData,t_i);
        recv_pack=t_ptl.getPack();

        if(ACK_LOGIN_SUCCESS ==recv_pack.getMsg())
        {
            recv_pack.ackLoginD(t_devInfo.m_token);
            printf("login success!\n");
        }
        else if(ACK_LOGIN_FAIL==recv_pack.getMsg())
        {
            printf("login fail, plrase try again!\n");
            close(t_devInfo.m_skfd);
            exit(-1);
        }
    }
}
/*设备登出函数*/
void logout_fun(DEV_Info& t_devInfo)
{
    int ret=-1;
    char t_ptlData[SCKT_DATA_LEN];
    Protocal t_ptl;
    Pack snd_pack;
    Pack recv_pack;
    bzero(t_ptlData,SCKT_DATA_LEN);
    
    snd_pack.logoutE(t_devInfo.m_mac,DEV_LOGIN);
    t_ptl.setPack(snd_pack);
    int t_i=0; 
    t_ptl.encode(t_ptlData,t_i);

    ret = send(t_devInfo.m_skfd, t_ptlData,t_i, 0);
    if(ret < 0) err_fun(__FILE__, __LINE__, "sned", errno);
    bzero(t_ptlData, SCKT_DATA_LEN);
    t_i=0;
    ret = recv(t_devInfo.m_skfd,t_ptlData,SCKT_DATA_LEN, 0);
    if(ret < 0) err_fun(__FILE__, __LINE__, "sned", errno);
    else if(ret > 0)
    {
        t_ptl.decode(t_ptlData,t_i);
        recv_pack=t_ptl.getPack();

        if(ACK_SUCCESS ==recv_pack.getMsg())
        {
            recv_pack.ackD();
            printf("logout success!\n");
            exit(-1);
        }
    }
}

void signal_fun(int signo)
{
    printf("logout ok!\n");
    exit(0);
}

float light_random()
{
    return (float)(rand()%100);
}

/*发送设备运行数据*/
void send_light_data(DEV_Info & t_devInfo,char * t_id)
{
    float t_light;
    t_light=light_random();

    Protocal t_ptl;
    Pack send_pack;
    char t_data[SCKT_DATA_LEN];
    bzero(t_data,SCKT_DATA_LEN);
    int t_i=0;
    int ret=-1;
    send_pack.ackGetDataFromDevsByPlatE(t_devInfo.m_mac,t_id,GET_DEV_DATA,t_light);
    t_ptl.setPack(send_pack);

    
    t_ptl.encode(t_data,t_i);
    
    ret=send(t_devInfo.m_skfd,t_data,t_i,0);
    if(ret<0) err_fun(__FILE__,__LINE__,"send",ret);

}
/*发送确认信息*/
void send_ok(DEV_Info & t_devInfo,char * t_id)
{
    Protocal t_ptl;
    Pack send_pack;
    char t_data[SCKT_DATA_LEN];
    bzero(t_data,SCKT_DATA_LEN);
    int t_i=0;
    int ret=-1;
    send_pack.ackE(t_id,ACK_SUCCESS);
    t_ptl.setPack(send_pack);

    
    t_ptl.encode(t_data,t_i);
    
    ret=send(t_devInfo.m_skfd,t_data,t_i,0);
    if(ret<0) err_fun(__FILE__,__LINE__,"send",ret);

}
/*发送设备状态信息*/
void send_light_status(DEV_Info & t_devInfo,char * t_id)
{
    Protocal t_ptl;
    Pack send_pack;
    char t_data[SCKT_DATA_LEN];
    bzero(t_data,SCKT_DATA_LEN);
    int t_i=0;
    int ret=-1;

    send_pack.ackLightStatusByPlatE(t_devInfo.m_mac,t_id,t_devInfo.m_status,t_devInfo.m_data);
    t_ptl.setPack(send_pack);

    
    t_ptl.encode(t_data,t_i);
    
    ret=send(t_devInfo.m_skfd,t_data,t_i,0);
    if(ret<0) err_fun(__FILE__,__LINE__,"send",ret);
    //show_protocal_data(t_data);
}


int main(int argc, char **argv)
{
    int ret = -1;

    pthread_t pth_clt;

    if(3 != argc)
	{
        printf("dev  mac password:\n");
		exit(-1);
	}
    srand(unsigned(time(0)));
    DEV_Info t_devInfo;
    signal(SIGINT, signal_fun);
	
    strcpy(t_devInfo.m_mac, argv[1]);
    strcpy(t_devInfo.m_pwd,argv[2]);
    
    socket_fun(t_devInfo);
    login_fun(t_devInfo);

    ret=pthread_create(&pth_clt,NULL,pth_clt_fun,(void*)&t_devInfo);
    if(ret!=0) err_fun(__FILE__,__LINE__,"pthread_create",ret);

    char t_data[SCKT_DATA_LEN];
    bzero(t_data,SCKT_DATA_LEN);
    Protocal t_ptl;
    Pack recv_pack;
    
    char t_id[ID_MAX_LEN];
    char t_mac[MAC_LEN];
    bzero(t_id,ID_MAX_LEN);
    bzero(t_mac,MAC_LEN);
    char t_set_data=-1;
    int t_i=0;
    while(1)
    {
        t_i=0;
        bzero(t_data,SCKT_DATA_LEN);

        ret=recv(t_devInfo.m_skfd,t_data,SCKT_DATA_LEN,0);
        if(ret<0) err_fun(__FILE__,__LINE__,"recv",ret);

        t_ptl.decode(t_data,t_i);
        recv_pack=t_ptl.getPack();

        unsigned char t_msg=recv_pack.getMsg();
        switch(t_msg)
        {
            case GET_DEV_DATA:
                recv_pack.reqFromMfToDevByPlatD(t_id,t_mac);
                cout<<"main "<<"t_id "<<t_id<<endl;
                cout<<"main "<<"t_mac "<<t_mac<<endl;
                send_light_data(t_devInfo,t_id); 
                break;
            case GET_DEV_STATUS:
                recv_pack.reqFromMfToDevByPlatD(t_id,t_mac);
                cout<<"main "<<"t_id "<<t_id<<endl;
                cout<<"mian "<<"t_mac "<<t_mac<<endl;
                send_light_status(t_devInfo,t_id); 
                break;
            case DEV_ON:
                recv_pack.reqFromMfToDevByPlatD(t_id,t_mac);
                cout<<"mian "<<"t_id "<<t_id<<endl;
                cout<<"mian "<<"t_mac "<<t_mac<<endl;
                cout<<"main "<<"light on"<<endl;
                t_devInfo.m_status=DEV_ON;
                send_ok(t_devInfo,t_id);
                break;    
            case DEV_OFF:
                recv_pack.reqFromMfToDevByPlatD(t_id,t_mac);
                cout<<"main "<<"t_id "<<t_id<<endl;
                cout<<"main "<<"t_mac "<<t_mac<<endl;
                t_devInfo.m_status=DEV_OFF;
                send_ok(t_devInfo,t_id);
                cout<<"main "<<"light off"<<endl;
                break;
            case DEV_SET:
                recv_pack.reqSetDevDataByPlatD(t_id,t_mac,t_set_data);
                cout<<"main "<<"t_id "<<t_id<<endl;
                cout<<"main "<<"t_mac "<<t_mac<<endl;
                cout<<"mian "<<"set data:"<<(int)t_set_data<<endl;
                send_ok(t_devInfo,t_id);
                t_devInfo.m_data=t_set_data;
                cout<<"main "<<"light set"<<endl;
                break;
            default:
                cout<<"main "<<"msgtype:"<<recv_pack.getMsg()<<endl;
                break;

        }
        ret=-1;
        bzero(t_id,ID_MAX_LEN);
        bzero(t_mac,MAC_LEN);
    }
    return 0;
}
