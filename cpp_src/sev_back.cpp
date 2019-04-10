/*************************************************************************
	> File Name: sev_back.cpp
	> Author: 
	> Mail: 
	> Created Time: 2016年10月18日 星期二 22时32分41秒
 ************************************************************************/

#include<iostream>
using namespace std;

#include"../h_src/sev_back_server.h"


SEV_Glb g_va;

void delete_msgqueue(int msgid)
{
    int ret = msgctl(msgid, IPC_RMID, NULL);
    if(ret<0 && errno!=EIDRM) err_fun(__FILE__, __LINE__, "msgctl", errno);
}
void signal_fun(int signo)
{
    delete_msgqueue(g_va.m_msg_skfd);
    printf("server quit success!\n");
    exit(-1);  //终止进程
}

int main(int argc, char *argv[])
{
    int ret=-1;

    signal(SIGINT,signal_fun);

    srand(unsigned(time(0)));

    g_va.m_mfFile.setFile(mf_file);
    g_va.m_devFile.setFile(dev_file);

    g_va.m_mfInfo=new MF_Info;

    memcpy(g_va.m_mfInfo->m_id,"96632558",strlen("96632558"));

    memcpy(g_va.m_mfInfo->m_name,"server",strlen("server"));

    pthread_t pth_sev;

    signal(SIGINT,signal_fun);

    socket_fun(g_va);

    cout<<"socket success"<<endl;

    g_va.m_mfInfo->m_skfd=g_va.m_skfd;

    mk_get_msg(0664, 'a',&g_va);

    cout<<"main :"<<g_va.m_msg_skfd<<endl;

    g_va.m_devFile.init_DEV_InfoList(g_va.m_devInfoList);

    list<DEV_Info*>::iterator t_it;
    for(t_it=g_va.m_devInfoList.begin();t_it!=g_va.m_devInfoList.end();t_it++)
    {
        cout<<(*t_it)->m_name<<" "<<(*t_it)->m_mac<<endl;
    }

    ret=pthread_create(&pth_sev,NULL,pth_sev_fun,(void*)&g_va);
    if(ret<0) err_fun(__FILE__,__LINE__,"pthread ",ret);

    char t_mac[MAC_LEN];
    char t_id[ID_MAX_LEN];
    bzero(t_id,ID_MAX_LEN);
    bzero(t_mac,MAC_LEN);

    char t_set;
    msg_buf recv_buf;
    Pack snd_pack;
    Pack snd_pack1;
    DEV_Info * t_tpDevInfo;
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
            cout<<"DEV  ON OFF GETDATA"<<endl;
            snd_recv_plat_dev(g_va,snd_pack);

            snd_pack.reqFromMfToDevByPlatD(t_id,t_mac);

            t_tpDevInfo=find_dev_by_mac(t_mac,g_va);
            t_tpDevInfo->m_status=DEV_ON;

            break;
        case DEV_OFF:
            cout<<"DEV  ON OFF GETDATA"<<endl;
            snd_recv_plat_dev(g_va,snd_pack);

            snd_pack.reqFromMfToDevByPlatD(t_id,t_mac);

            t_tpDevInfo=find_dev_by_mac(t_mac,g_va);
            t_tpDevInfo->m_status=DEV_OFF;

            break;
        case DEV_SET:
            cout<<"DEV  ON OFF GETDATA"<<endl;
            snd_recv_plat_dev(g_va,snd_pack);

            snd_pack.reqSetDevDataByPlatD(t_id,t_mac,t_set);

            t_tpDevInfo=find_dev_by_mac(t_mac,g_va);
            t_tpDevInfo->m_data=t_set;

            break;
        case GET_DEV_DATA:
            cout<<"DEV  ON OFF GETDATA"<<endl;
            snd_recv_plat_dev(g_va,snd_pack);
            break;

        case GET_DEVS_INFO:

            cout<<"GET_DEVS_INFO"<<endl;
            snd_pack1.ackGetDevsFromPlatE(g_va.m_devInfoList);
            snd_pack_to_ui(g_va,snd_pack1);

            break;
        case DEV_REGISTER:

            cout<<"GET_DEVS_INFO"<<endl;
            snd_recv_plat_dev(g_va,snd_pack);

            break;
        }
    }

    return 0;

}

