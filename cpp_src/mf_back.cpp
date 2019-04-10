/*************************************************************************
> File Name: mf_back.cpp
> Author: 
> Mail: 
> Created Time: 2016年10月18日 星期二 22时35分27秒
************************************************************************/

#include<iostream>
using namespace std;
#include"../h_src/mf_info.h"
#include"../h_src/dev_info.h"
#include"../h_src/file_opt.h"
#include"../h_src/common.h"
#include"../h_src/mf_back_client.h"


MF_Glb g_va;

void delete_msgqueue(int msgid)
{
    int ret = msgctl(msgid, IPC_RMID, NULL);
    if(ret<0 && errno!=EIDRM) err_fun(__FILE__, __LINE__, "msgctl", errno);
}

void signal_fun(int signo)
{
    delete_msgqueue(g_va.m_msg_skfd);
    printf("mf exit success!\n");
    exit(-1);
}

int main()
{
    int ret = -1;

    int t_skfd;

    signal(SIGINT,signal_fun);

    socket_fun(t_skfd);

    g_va.m_mfInfo=new MF_Info;

    mk_get_msg(0664, 'b',g_va.m_msg_skfd);


    g_va.m_mfInfo->m_skfd=t_skfd;

    msg_buf recv_buf;
    Pack snd_pack;
    while(1)
    {
        cout<<"back main recving "<<endl;
        ret = msgrcv(g_va.m_msg_skfd, (void *)&recv_buf, SIZE,MSG_UI_TO_BACK, 0);
        if(ret<0) err_fun(__FILE__,__LINE__,"msgrecv ",ret);
        cout<<"back mian msgrcv finish"<<endl;
        memcpy(&snd_pack,recv_buf.m_msgData,sizeof(snd_pack));
        int t_msg=snd_pack.getMsg();
        switch(t_msg)
        {

        case DEV_ON:
            cout<<"DEV_ON"<<endl;
            cout<<"pth msg fun msg data "<<snd_pack.getMsgData()<<endl;
            set_dev_on(*g_va.m_mfInfo,snd_pack);
            break;

        case DEV_OFF:
            cout<<"DEV_ON"<<endl;
            cout<<"main data "<<snd_pack.getMsgData()<<endl;
            set_dev_off(*g_va.m_mfInfo,snd_pack);
            break;

        case DEV_SET:
            cout<<"DEV_SET"<<endl;
            cout<<"main data "<<snd_pack.getMsgData()<<endl;
            set_dev_data(*g_va.m_mfInfo,snd_pack);
            break;
        case MF_REGISTER:
            cout<<"MF_REGISTER"<<endl;
            cout<<"mian msg data "<<snd_pack.getMsgData()<<endl;
            mf_register_fun(g_va);

            break;

        case GET_DEV_DATA:
            cout<<"GET_DEV_DATA"<<endl;
            cout<<"pth msg fun msg data "<<snd_pack.getMsgData()<<endl;
            get_dev_data(g_va,snd_pack);

            break;
        case MF_LOGIN:
            cout<<"MF_LOGIN"<<endl;
            login_fun(g_va,snd_pack);
            break;

        case GET_DEVS_INFO:
            cout<<"GET_DEVS_INFO"<<endl;
            get_devs_list(g_va,snd_pack);
            break;
        case GET_DEV_STATUS:
            cout<<"GET_DEVS_STATUS"<<endl;
            get_dev_status(g_va,snd_pack);
            break;
        }
    }

    return 0;    
}
