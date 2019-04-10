#include"server.h"
#include"widget.h"


void err_fun(const char *file_nam, const int line, \
             const char *fun_nam, const int err_no)
{
    fprintf(stderr, "in %s %d, %s is fail: %s\n", file_nam,\
            line, fun_nam, strerror(err_no));
    exit(-1);
}
void random(char * t_token,char t_tokenLen)
{
    int t_len=(int)t_tokenLen;
    int t_i=0;
    for(;t_i<t_len-1;t_i++)
    {
        t_token[t_i]=rand()%9+1;
    }
    t_token[t_i]='\0';
}

void mk_get_msg(mode_t creatmsg_mode, int proj_id,SEV_Glb * g_va)
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
    g_va->m_msg_skfd = msgget(key, IPC_CREAT|creatmsg_mode);
    if(g_va->m_msg_skfd < 0) err_fun(__FILE__, __LINE__, "msgget", errno);
}

void signal_fun(int signo)
{
    printf("server quit success!\n");
    exit(-1);  //终止进程

}

bool find_skfd_by_mac(char * t_mac,SEV_Glb & t_va,int & t_skfd)
{
    list<DEV_Info*>::iterator t_it;
    for(t_it=t_va.m_devInfoList.begin();t_it!=t_va.m_devInfoList.end();t_it++)
    {
        if(0==strcmp(t_mac,(*t_it)->m_mac))
        {
            t_skfd=(*t_it)->m_skfd;
            return true;
        }
    }
    return false;
}

bool delete_dev_by_mac(char * t_mac,SEV_Glb & t_va)
{
    list<DEV_Info*>::iterator t_it;
    for(t_it=t_va.m_devInfoList.begin();t_it!=t_va.m_devInfoList.end();t_it++)
    {
        if(0==strcmp(t_mac,(*t_it)->m_mac))
        {
            t_va.m_devInfoList.erase(t_it);
            return true;
        }
    }
    return false;

}

bool find_skfd_by_id(char * t_id,SEV_Glb & t_va,int & t_skfd)
{
    list<MF_Info*>::iterator t_it;
    for(t_it=t_va.m_mfInfoList.begin();t_it!=t_va.m_mfInfoList.end();t_it++)
    {
        if(0==strcmp(t_id,(*t_it)->m_id))
        {
            t_skfd=(*t_it)->m_skfd;
            return true;
        }
    }
    return false;
}

bool delete_mf_by_id(char * t_id,SEV_Glb & t_va)
{
    list<MF_Info*>::iterator t_it;
    for(t_it=t_va.m_mfInfoList.begin();t_it!=t_va.m_mfInfoList.end();t_it++)
    {
        if(0==strcmp(t_id,(*t_it)->m_id))
        {
            t_va.m_mfInfoList.erase(t_it);
            return true;
        }
    }
    return false;

}

void send_data_to_allmf(SEV_Glb & t_va,char * t_data)
{
    int ret=-1;
    list<MF_Info*>::iterator t_it;
    for(t_it=t_va.m_mfInfoList.begin();t_it!=t_va.m_mfInfoList.end();t_it++)
    {
        ret=send((*t_it)->m_skfd,t_data,SCKT_DATA_LEN,0);
        if(ret<0) err_fun(__FILE__,__LINE__,"send",errno);
    }
}


void flushDevUI(SEV_Glb & t_va)
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

void * pth_msg_fun(void * arg)
{

    SEV_Glb  * t_va=(SEV_Glb*)arg;

    int ret=-1;
    char t_mac[MAC_LEN];
    char t_id[ID_MAX_LEN];
    float t_data;
    char t_set_data;
    bzero(t_id,ID_MAX_LEN);
    bzero(t_mac,MAC_LEN);

    msg_buf recv_buf;
    Pack recv_pack;


    QString *t_macStr;
    list<DEV_Info*>::iterator t_it;
    while(1)
    {
        ret = msgrcv(t_va->m_msg_skfd, (void *)&recv_buf, SIZE,MSG_BACK_TO_UI, 0);
        if(ret<0) err_fun(__FILE__,__LINE__,"msgrecv ",ret);
        memcpy(&recv_pack,recv_buf.m_msgData,sizeof(recv_pack));

        switch(recv_pack.getMsg())
        {
        case DEV_ON:
            cout<<"pth msg fun DEV ON"<<endl;
            recv_pack.reqFromMfToDevByPlatD(t_id,t_mac);
            t_macStr=new QString(t_mac);
            if(t_va->m_widget->m_devCntlMap.contains(*t_macStr))
            {
                t_va->m_widget->m_devCntlMap[*t_macStr]->m_devInfo->m_status=DEV_ON;
                t_va->m_widget->m_devCntlMap[*t_macStr]->flushDev();
            }
            cout<<"pth_msg_fun set ok"<<endl;
            break;
        case DEV_OFF:
            cout<<"pth msg fun DEV OFF"<<endl;
            recv_pack.reqFromMfToDevByPlatD(t_id,t_mac);
            t_macStr=new QString(t_mac);
            if(t_va->m_widget->m_devCntlMap.contains(*t_macStr))
            {
                cout<<"set ing ...."<<endl;
                t_va->m_widget->m_devCntlMap[*t_macStr]->m_devInfo->m_status=DEV_OFF;
                t_va->m_widget->m_devCntlMap[*t_macStr]->flushDev();
            }
            cout<<"pth_msg_fun set ok"<<endl;
            break;
        case DEV_SET:
            cout<<"pth msg fun DEV SET DATA"<<endl;

            recv_pack.reqSetDevDataByPlatD(t_id,t_mac,t_set_data);

            t_macStr=new QString(t_mac);
            if(t_va->m_widget->m_devCntlMap.contains(*t_macStr))
            {
                t_va->m_widget->m_devCntlMap[*t_macStr]->m_devInfo->m_data=(int)t_set_data;
                t_va->m_widget->m_devCntlMap[*t_macStr]->flushDev();
            }
            cout<<"pth_msg_fun set ok"<<endl;
            break;
        case GET_DEV_DATA:
            cout<<"pth msg fun DEV DATA"<<endl;

            recv_pack.ackGetDataFromDevsByPlatD(t_mac,t_id,t_data);
            t_macStr=new QString(t_mac);

            if(t_va->m_widget->m_devCntlMap.contains(*t_macStr))
            {
                cout<<"set ing ...."<<endl;
                t_va->m_widget->m_devCntlMap[*t_macStr]->m_devInfo->m_light=(int)t_data;
                t_va->m_widget->m_devCntlMap[*t_macStr]->flushDev();
            }


            break;
        case GET_DEVS_INFO:
            recv_pack.ackGetDevsFromPlatD(t_va->m_devInfoList);

            cout<<"get msg fun info list"<<endl;

            flushDevUI(*t_va);
            break;
        }
        bzero(t_mac,MAC_LEN);
    }
}

void updateDevInfoList(SEV_Glb * t_va)
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

void * pth_getUpdateData_fun(void * arg)
{
    SEV_Glb  * t_va=(SEV_Glb*)arg;
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
        sleep(5);
    }
}

