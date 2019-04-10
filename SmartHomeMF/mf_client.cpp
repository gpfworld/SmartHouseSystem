#include"mf_client.h"
#include<QMap>
//test



void err_fun(const char *file_nam, const int line,const char *fun_nam, const int err_no)
{
    fprintf(stderr, "in %s %d, %s is fail: %s\n", file_nam, line, fun_nam, strerror(err_no));
    exit(-1);
}

void flushDevUI(MF_Glb & t_va)
{
    list<DEV_Info*>::iterator t_iter;
    QString * t_macStr;
    for(t_iter=t_va.m_devInfoList.begin();t_iter!=t_va.m_devInfoList.end();t_iter++)
    {
        t_macStr=new QString((*t_iter)->m_mac);
        t_va.m_widget->m_devCntlMap[*t_macStr]->m_devInfo->m_isOnline=(*t_iter)->m_isOnline;
    }
    QMap<QString,Dev *>::Iterator t_it;
    for(t_it=t_va.m_widget->m_devCntlMap.begin();t_it!=t_va.m_widget->m_devCntlMap.end();t_it++)
    {
        (*t_it)->flushDev();
    }
}

void updateDevInfoList(MF_Glb * t_va)
{
    msg_buf snd_buf;
    Pack snd_pack;
    int ret=-1;

    snd_buf.m_msgType=MSG_UI_TO_BACK;
    snd_pack.reqFromMfToPlatE(GET_DEVS_INFO);

    memcpy(snd_buf.m_msgData,(void*)&snd_pack,sizeof(snd_pack));
    ret = msgsnd(t_va->m_msg_skfd, (void *)&snd_buf, SIZE, 0); //阻塞发送消息
    if(ret < 0) err_fun(__FILE__, __LINE__, "msgsnd", errno);
}

void * pth_msg_fun(void * arg)
{
    cout<<"pth_msg_fun"<<endl;

    MF_Glb  * t_va=(MF_Glb*)arg;
    
    int ret=-1;
    char t_mac[MAC_LEN];
    bzero(t_mac,MAC_LEN);

    msg_buf recv_buf;
    Pack recv_pack;

    list<DEV_Info*>::iterator t_it;


     char t_id[ID_MAX_LEN];
     float t_data;
     bzero(t_mac,MAC_LEN);
     bzero(t_id,ID_MAX_LEN);
     unsigned char t_lightStatus;
     char t_light_r;
    QString *t_macStr;
    while(1)
    {
        cout<<"msgrcving.."<<endl;
        ret = msgrcv(t_va->m_msg_skfd, (void *)&recv_buf, SIZE,MSG_BACK_TO_UI, 0);
        if(ret<0) err_fun(__FILE__,__LINE__,"msgrecv ",ret);
        memcpy(&recv_pack,recv_buf.m_msgData,sizeof(recv_pack));
        cout<<"msgrcv finish"<<endl;
        switch(recv_pack.getMsg())
        {
            case GET_DEV_DATA:
                 cout<<"GET_DEV_DATA"<<endl;

                 recv_pack.ackGetDataFromDevsByPlatD(t_mac,t_id,t_data);

                 cout<<"GET_DEV_DATA t_mac "<<t_mac<<endl;

                 t_macStr=new QString(t_mac);
                 if(t_va->m_widget->m_devCntlMap.contains(*t_macStr))
                 {
                     t_va->m_widget->m_devCntlMap[*t_macStr]->m_devInfo->m_light=(int)t_data;
                     t_va->m_widget->m_devCntlMap[*t_macStr]->flushDev();
                 }
                 cout<<"t_src "<<t_mac<<" t_dest "<<t_id<<" light:"<<t_data<<endl;

                 break;
            case GET_DEV_STATUS:


             cout<<"GET_DEV_STATUS"<<endl;
                 recv_pack.ackLightStatusByPlatD(t_mac,t_id,t_lightStatus,t_light_r);
                 t_macStr=new QString(t_mac);
                 if(t_va->m_widget->m_devCntlMap.contains(*t_macStr))
                 {
                     t_va->m_widget->m_devCntlMap[*t_macStr]->m_devInfo->m_status=t_lightStatus;
                     t_va->m_widget->m_devCntlMap[*t_macStr]->m_devInfo->m_data=t_light_r;
                     t_va->m_widget->m_devCntlMap[*t_macStr]->flushDev();
                 }
                break;

            case GET_DEVS_INFO:
                recv_pack.ackGetDevsFromPlatD(t_va->m_devInfoList);

                cout<<"get msg fun info list"<<endl;

                cout<<"dev info list size  "<<t_va->m_devInfoList.size()<<endl;
                for(t_it=t_va->m_devInfoList.begin();t_it!=t_va->m_devInfoList.end();t_it++)
                {
                    cout<<(*t_it)->m_name<<" "<<(*t_it)->m_mac<<" "<<(int)(*t_it)->m_devType<<" "<<(int)(*t_it)->m_isOnline<<endl;
                }
                flushDevUI(*t_va);

                break;
            case ACK_LOGIN_SUCCESS:
                cout<<"ACK_LOGIN_SUCCESS"<<endl;
                recv_pack.ackLoginD(t_va->m_mfInfo->m_token);
                break;
            case ACK_LOGIN_FAIL:
                cout<<"ACK_LOGIN_FAIL"<<endl;
                recv_pack.ackLoginD(t_va->m_mfInfo->m_token);
                break;
             default:
                cout<<(int)recv_pack.getMsg()<<endl;
                   break;
        }
    }
}

void * pth_getUpdateData_fun(void * arg)
{
    cout<<"getupdate data fun"<<endl;

    MF_Glb  * t_va=(MF_Glb*)arg;
     int ret=-1;
     Pack snd_pack;
     Pack snd_pack1;
     msg_buf snd_buf;
     snd_buf.m_msgType=MSG_UI_TO_BACK;
     list<DEV_Info*>::iterator t_it;
     while(1)
     {
         for(t_it=t_va->m_devInfoList.begin();t_it!=t_va->m_devInfoList.end();t_it++)
         {
             if(DEV_ONLINE==(*t_it)->m_isOnline)
             {
                snd_pack.reqFromMfToDevByPlatE(t_va->m_mfInfo->m_id,(*t_it)->m_mac,GET_DEV_DATA);
                memcpy(snd_buf.m_msgData,(void*)&snd_pack,sizeof(snd_pack));

                cout<<"pth get update data fun msgsnding..."<<endl;
                ret = msgsnd(t_va->m_msg_skfd, (void *)&snd_buf, SIZE, 0); //阻塞发送消息
                if(ret < 0) err_fun(__FILE__, __LINE__, "msgsnd", errno);
                cout<<"pth get update data fun msgsnd finish"<<endl;

                snd_pack1.reqFromMfToDevByPlatE(t_va->m_mfInfo->m_id,(*t_it)->m_mac,GET_DEV_STATUS);
                memcpy(snd_buf.m_msgData,(void*)&snd_pack1,sizeof(snd_pack1));

                cout<<"pth get update data fun msgsnding..."<<endl;
                ret = msgsnd(t_va->m_msg_skfd, (void *)&snd_buf, SIZE, 0); //阻塞发送消息
                if(ret < 0) err_fun(__FILE__, __LINE__, "msgsnd", errno);
                cout<<"pth get update data fun msgsnd finish"<<endl;

             }
         }

         updateDevInfoList(t_va);

         sleep(1);
     }
}

void mk_get_msg(mode_t creatmsg_mode, int proj_id,int & t_msg_skfd)
{
    int fd = -1;
    key_t key = -1;

    // 创建一个新文件，起文件的路径名用于生成key值，如果文件已经存在
    // 就不必再创建新文件，也不必报文件存在的错误
    fd = open(MSG_FILE, O_CREAT, 0664);
    if(fd<0 && EEXIST!=errno) err_fun(__FILE__, __LINE__, "open", errno);

    // 利用文件路径和课题ID获取key值
    key = ftok(MSG_FILE, proj_id);
    if(key < -1) err_fun(__FILE__, __LINE__, "ftok", errno);
    // 如果key没有被用，则创建新的消息队列，否则直接打开已有队列
    t_msg_skfd= msgget(key, IPC_CREAT|creatmsg_mode);
    if(t_msg_skfd< 0) err_fun(__FILE__, __LINE__, "msgget", errno);
}
