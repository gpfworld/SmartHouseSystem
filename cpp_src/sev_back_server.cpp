#include"../h_src/sev_back_server.h"



void err_fun(const char *file_nam, const int line, \
             const char *fun_nam, const int err_no)
{
    fprintf(stderr, "in %s %d, %s is fail: %s\n", file_nam,\
            line, fun_nam, strerror(err_no));
    exit(-1);
}
/*随机产生torken值*/
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
/*创建消息队列*/
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

/*修改设备参数*/
void alter_dev_line_status(SEV_Glb & t_va,DEV_Info & t_devInfo)
{
    list<DEV_Info*>::iterator t_it;
    msg_buf snd_buf;

    int ret=-1;
    snd_buf.m_msgType=FLUSH_DEV_UI;
    for(t_it=t_va.m_devInfoList.begin();t_it!=t_va.m_devInfoList.end();t_it++)
    {
        if(0==strcmp(t_devInfo.m_mac,(*t_it)->m_mac))
        {
            (*t_it)->m_isOnline=DEV_ONLINE;
            (*t_it)->m_skfd=t_devInfo.m_skfd;

            memcpy(snd_buf.m_msgData,(void*)t_devInfo.m_mac,MAC_LEN);
        

            ret = msgsnd(t_va.m_msg_skfd, (void *)&snd_buf, SIZE, 0); //阻塞发送消息
            if(ret < 0) err_fun(__FILE__, __LINE__, "msgsnd", errno);

            break;
        }
    }
}
/*向QT端发送pack包*/
void snd_pack_to_ui(SEV_Glb & t_va,Pack & snd_pack)
{
    int ret=-1;
    msg_buf snd_buf;
    snd_buf.m_msgType=MSG_BACK_TO_UI;
    memcpy(snd_buf.m_msgData,(void*)&snd_pack,sizeof(snd_pack));

    ret = msgsnd(t_va.m_msg_skfd, (void *)&snd_buf, SIZE, 0); //阻塞发送消息
    if(ret < 0) err_fun(__FILE__, __LINE__, "msgsnd", errno);

}
/*客户\设备的注册登录*/
int login_register_fun(SEV_Glb & t_va)
{

    int ret = -1;
    Pack recv_pack, send_pack;

    Protocal t_ptl;
    char t_data[SCKT_DATA_LEN];
    bzero(t_data,SCKT_DATA_LEN);

    ret = recv(t_va.m_clt_skfd,t_data,SCKT_DATA_LEN, 0);
    if(ret < 0) err_fun(__FILE__, __LINE__, "recv", errno);
    else if(ret > 0)
    {
        int t_i=0;
        t_ptl.decode(t_data,t_i);
        recv_pack=t_ptl.getPack();
        if((LINK_OPERATE==recv_pack.getMsgType())&&(MF_REGISTER==recv_pack.getMsg()))
        {
            MF_Info *t_tpMfInfo=new MF_Info();
            t_tpMfInfo->m_skfd=t_va.m_clt_skfd;

            recv_pack.mfRegisterD(*t_tpMfInfo);
            bool t_bret=t_va.m_mfFile.is_MF_Registersuccess(*t_tpMfInfo);

            if(!t_bret)
            {
                send_pack.ackLoginE(t_tpMfInfo->m_token,ACK_LOGIN_FAIL);
                t_ptl.setPack(send_pack);
                bzero(t_data,SCKT_DATA_LEN);
                t_i=0;
                t_ptl.encode(t_data,t_i);
                send(t_tpMfInfo->m_skfd, t_data,t_i, 0);
                goto lab0;
            }    
            random(t_tpMfInfo->m_token,t_tpMfInfo->m_tokenLen);

            send_pack.ackLoginE(t_tpMfInfo->m_token,ACK_LOGIN_SUCCESS);
            t_ptl.setPack(send_pack);
            bzero(t_data,SCKT_DATA_LEN);
            t_i=0;
            t_ptl.encode(t_data,t_i);
            send(t_tpMfInfo->m_skfd, t_data,t_i, 0);
            t_va.m_mfInfoList.push_back(t_tpMfInfo);
            return MF_LOGIN;
        }
        else if((LINK_OPERATE==recv_pack.getMsgType())&&(MF_LOGIN==recv_pack.getMsg()))
        {   
            MF_Info *t_tpMfInfo=new MF_Info();
            t_tpMfInfo->m_skfd=t_va.m_clt_skfd;

            recv_pack.loginD(t_tpMfInfo->m_id,t_tpMfInfo->m_pwd);

            bool t_bl=t_va.m_mfFile.is_MF_Loginsuccess(*t_tpMfInfo);
            if(!t_bl)
            {
                send_pack.ackLoginE(t_tpMfInfo->m_token,ACK_LOGIN_FAIL);

            }

            random(t_tpMfInfo->m_token,t_tpMfInfo->m_tokenLen);

            send_pack.ackLoginE(t_tpMfInfo->m_token,ACK_LOGIN_SUCCESS);
            t_ptl.setPack(send_pack);
            bzero(t_data,SCKT_DATA_LEN);
            t_i=0;
            t_ptl.encode(t_data,t_i);
            send(t_tpMfInfo->m_skfd, t_data,t_i, 0);
            t_va.m_mfInfoList.push_back(t_tpMfInfo);
            return MF_LOGIN;
        }
        else if((LINK_OPERATE==recv_pack.getMsgType())&&(DEV_LOGIN==recv_pack.getMsg()))
        {
            DEV_Info *t_tpDevInfo=new DEV_Info();
            t_tpDevInfo->m_skfd=t_va.m_clt_skfd;

            recv_pack.loginD(t_tpDevInfo->m_mac,t_tpDevInfo->m_pwd);

            bool t_bl=t_va.m_devFile.is_DEV_Loginsuccess(*t_tpDevInfo);
            if(!t_bl)
            {
                cout<<"dev login fail!"<<endl;
                send_pack.ackLoginE(t_tpDevInfo->m_token,ACK_LOGIN_FAIL);
                t_ptl.setPack(send_pack);
                bzero(t_data,SCKT_DATA_LEN);
                t_i=0;
                t_ptl.encode(t_data,t_i);
                send(t_tpDevInfo->m_skfd, t_data,t_i, 0);
                goto lab0;
            }

            random(t_tpDevInfo->m_token,t_tpDevInfo->m_tokenLen);

            send_pack.ackLoginE(t_tpDevInfo->m_token,ACK_LOGIN_SUCCESS);
            t_ptl.setPack(send_pack);
            bzero(t_data,SCKT_DATA_LEN);
            t_i=0;
            t_ptl.encode(t_data,t_i);
            send(t_tpDevInfo->m_skfd, t_data,t_i, 0);

            alter_dev_line_status(t_va,*t_tpDevInfo);

            cout<<"mac:"<<t_tpDevInfo->m_mac<<"pwd:"<<t_tpDevInfo->m_pwd<<"type:"<<(int)t_tpDevInfo->m_devType<<endl;
            cout<<"dev login!"<<endl;
            return DEV_LOGIN;
        }
        else goto lab0;
    }
    lab0:	return ACK_LOGIN_FAIL;
}
/*socket网络连接函数*/
void socket_fun(SEV_Glb &t_va)
{
    int ret=-1,skfd=-1;
    struct sockaddr_in ser_addr = {0};

    /* 套接字函数
    * 参数1：地址协议族
    * 	AF_UNIX, AF_LOCAL：与套接子，本地通信用的
    *	AF_INET：IPV4
    *	AF_INET6：ipv6
    * 参数2：套接字类型
    * 	SOCK_STREAM：流式套接字，对应TCP
    * 	SOCK_DGRAM：数据报套接字，对应udp
    * 	SOCK_RAW：原始套接字，对应ip
    * 参数3：子协议编号，没有其它子协议则写0
    * 返回值：成功返回一个套接字文件描述符
    * 	   失败返回-1，errno被设置 */
    skfd = socket(AF_INET, SOCK_STREAM, 0);
    if(skfd < 0) err_fun(__FILE__, __LINE__, "socket", errno);

    ser_addr.sin_family = AF_INET; //协议族
    ser_addr.sin_port   = htons(PORT); //将port转为网络端序
    /* 1.将字符ip地址转为32无符号整形数的IP
    * 2.32无符号整形数的IP，转为网络大小端序 */
    ser_addr.sin_addr.s_addr = inet_addr(SADDR);
    //inet_aton(SADDR, &ser_addr.sin_addr);
    /* 绑定函数，将端口，ip地址和网卡绑定
    * 参数1：套接字描述符
    * 参数2：填写由端口和IP地址的结构体变量地址，拿去绑定
    * 参数3：结构体大小 */
    ret =bind(skfd, (struct sockaddr *)&ser_addr, sizeof(ser_addr));
    if(ret < 0) err_fun(__FILE__, __LINE__, "bind", errno);

    /* 将套接字描述符变为被动描述符，用于accept用
    * 参数1：套接字描述符
    * 参数2：在某时刻，能够被同时监听的客户数量 */
    ret = listen(skfd, MAX_CLIENT);
    if(ret < 0) err_fun(__FILE__, __LINE__, "listen", errno);
    t_va.m_skfd=skfd;
}
/*查找对应设备的skfd*/
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
/*通过mac查找对应设备*/
DEV_Info * find_dev_by_mac(char * t_mac,SEV_Glb & t_va)
{
    list<DEV_Info*>::iterator t_it;
    for(t_it=t_va.m_devInfoList.begin();t_it!=t_va.m_devInfoList.end();t_it++)
    {
        if(0==strcmp(t_mac,(*t_it)->m_mac))
        {
            return * t_it;
        }
    }
    return NULL;
}
/*通过mac删除对应设备*/
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
/*通过id找到对应客户端的skfd*/
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
/*通过id删除对应客户*/
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
/*向所有客户端发送消息*/
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
/*客户端服务线程*/
void *pth_mf_fun(void *arg)
{
    SEV_Glb *t_va=(SEV_Glb*)arg;
    Pack recv_pack, send_pack;
    int t_i=0;

    char t_mac[MAC_LEN];
    char t_id[ID_MAX_LEN];
    bzero(t_mac,MAC_LEN);
    bzero(t_id,ID_MAX_LEN);


    int t_skfd=t_va->m_clt_skfd;
    int t_dev_skfd;
    Protocal t_ptl;
    char t_data[SCKT_DATA_LEN];
    char t_data_copy[SCKT_DATA_LEN];

    int ret=-1;
    bool t_bl=false;
    DEV_Info * t_tpDevInfo;


    while(1)
    {
        bzero(t_data,SCKT_DATA_LEN);
        bzero(t_data_copy,SCKT_DATA_LEN);    
        t_i=0;
        ret=recv(t_skfd,t_data,SCKT_DATA_LEN,0);
        memcpy(t_data_copy,t_data,SCKT_DATA_LEN);

        if(ret  <0)err_fun(__FILE__,__LINE__,"recv",ret);

        t_ptl.decode(t_data,t_i);
        recv_pack=t_ptl.getPack();
        cout<<"mf fun recv data:"<<recv_pack.getMsgData()<<endl;
        unsigned char t_msg=recv_pack.getMsg();
        switch(t_msg)
        {

        case GET_DEV_DATA:
            cout<<"cha kan msg type:"<<endl;
            recv_pack.reqFromMfToDevByPlatD(t_id,t_mac);

            t_bl=find_skfd_by_mac(t_mac,*t_va,t_dev_skfd);
            cout<<"find finish!"<<endl;
            cout<<"t_dest:"<<t_mac<<"t_dev_skfd:"<<t_dev_skfd<<endl;
            if(t_bl)
            {
                cout<<"sending...."<<endl;
                ret=send(t_dev_skfd,t_data_copy,ret,0);
                //show_protocal_data(t_data);

                if(ret<0)err_fun(__FILE__,__LINE__,"send",ret);
                cout<<"send finish!"<<endl;
            }
            else cout<<"can not find the dev!"<<endl;
            break;

        case DEV_REGISTER:
            t_tpDevInfo=new DEV_Info();

            recv_pack.devRegisterD(*t_tpDevInfo);
            t_bl=t_va->m_devFile.is_DEV_Registersuccess(*t_tpDevInfo);
            if(!t_bl)
            {
                send_pack.ackE(ACK_LOGIN_FAIL);
                t_ptl.setPack(send_pack);
                bzero(t_data,SCKT_DATA_LEN);
                t_i=0;
                t_ptl.encode(t_data,t_i);
                send(t_skfd, t_data,t_i, 0);
                cout<<t_tpDevInfo->m_name<<":dev register fail!"<<endl;
            }    
            random(t_tpDevInfo->m_token,t_tpDevInfo->m_tokenLen);

            send_pack.ackE(ACK_LOGIN_SUCCESS);
            t_ptl.setPack(send_pack);
            bzero(t_data,SCKT_DATA_LEN);
            t_i=0;
            t_ptl.encode(t_data,t_i);
            send(t_skfd, t_data,t_i, 0);
            cout<<t_tpDevInfo->m_name<<":dev register success!"<<endl;
            break;

        case GET_DEVS_INFO:
            cout<<"get devs info!"<<endl;
            cout<<"online dev size "<<t_va->m_devInfoList.size()<<endl;
            send_pack.ackGetDevsFromPlatE(t_va->m_devInfoList);
            t_ptl.setPack(send_pack);
            bzero(t_data,SCKT_DATA_LEN);
            t_i=0;
            t_ptl.encode(t_data,t_i);
            //show_protocal_data(t_data);
            ret=send(t_skfd,t_data,t_i,0);
			if(ret<0) err_fun(__FILE__,__LINE__,"send",errno);
			break;

        case MF_LOGOUT:
                recv_pack.logoutD(t_id);
                t_bl=delete_mf_by_id(t_id,*t_va);
                if(t_bl)
                {
                    cout<<t_id<<":logout!"<<endl;

                    send_pack.ackE(ACK_SUCCESS);
                    t_ptl.setPack(send_pack);
                    bzero(t_data,SCKT_DATA_LEN);
                    t_i=0;
                    t_ptl.encode(t_data,t_i);
                    ret=send(t_skfd,t_data,t_i,0);
                    if(ret<0) err_fun(__FILE__,__LINE__,"send",errno);
                }
                goto lab0;
                break;

        case GET_DEV_STATUS:
            cout<<"cha kan msg type:"<<endl;
            recv_pack.reqFromMfToDevByPlatD(t_id,t_mac);
            t_bl=find_skfd_by_mac(t_mac,*t_va,t_dev_skfd);
            cout<<"find finish!"<<endl;
            cout<<"t_dest:"<<t_mac<<"t_dev_skfd:"<<t_dev_skfd<<endl;
            if(t_bl)
            {
                cout<<"sending...."<<endl;
                ret=send(t_dev_skfd,t_data_copy,ret,0);
                //show_protocal_data(t_data);

                if(ret<0)err_fun(__FILE__,__LINE__,"send",ret);
                cout<<"send finish!"<<endl;
            }
            else cout<<"can not find the dev!"<<endl;

            //snd_pack_to_ui(*t_va,recv_pack);

            break;
        case DEV_ON:

            recv_pack.reqFromMfToDevByPlatD(t_id,t_mac);

            t_tpDevInfo=find_dev_by_mac(t_mac,*t_va);
            t_tpDevInfo->m_status=DEV_ON;

            t_bl=find_skfd_by_mac(t_mac,*t_va,t_dev_skfd);

            if(t_bl)
            {
                ret=send(t_dev_skfd,t_data_copy,ret,0);

                if(ret<0)err_fun(__FILE__,__LINE__,"send",ret);
            }
            else cout<<"can not find the dev!"<<endl;

            snd_pack_to_ui(*t_va,recv_pack);

            break;
        case DEV_OFF:

            recv_pack.reqFromMfToDevByPlatD(t_id,t_mac);

            t_tpDevInfo=find_dev_by_mac(t_mac,*t_va);
            t_tpDevInfo->m_status=DEV_ON;

            t_bl=find_skfd_by_mac(t_mac,*t_va,t_dev_skfd);

            if(t_bl)
            {          
                ret=send(t_dev_skfd,t_data_copy,ret,0);
                if(ret<0)err_fun(__FILE__,__LINE__,"send",ret);
            }
            else cout<<"can not find the dev!"<<endl;

            snd_pack_to_ui(*t_va,recv_pack);

            break;
        case DEV_SET:
            char t_set;
            recv_pack.reqSetDevDataByPlatD(t_id,t_mac,t_set);

            t_tpDevInfo=find_dev_by_mac(t_mac,*t_va);
            t_tpDevInfo->m_data=t_set;

            t_bl=find_skfd_by_mac(t_mac,*t_va,t_dev_skfd);

            if(t_bl)
            {
                ret=send(t_dev_skfd,t_data_copy,ret,0);
                if(ret<0)err_fun(__FILE__,__LINE__,"send",ret);
            }
            else cout<<"can not find the dev!"<<endl;

            snd_pack_to_ui(*t_va,recv_pack);

            break;
        }

        bzero(t_mac,MAC_LEN);
        bzero(t_id,ID_MAX_LEN);
        ret=-1;
        t_bl=false;
    }                
    lab0: return NULL;
}


/*设备端服务线程*/
void *pth_dev_fun(void *arg)
{
    SEV_Glb *t_va=(SEV_Glb*)arg;
    int ret=-1;
    Protocal t_ptl;
    Pack recv_pack,send_pack;
    int t_i=0;

    char t_mac[MAC_LEN];
    char t_id[ID_MAX_LEN];
    float t_tpData;
    bool t_bl=false;
    bzero(t_mac,MAC_LEN);
    bzero(t_id,ID_MAX_LEN);

    char t_data[SCKT_DATA_LEN];
    char t_data_copy[SCKT_DATA_LEN];
    int t_skfd=t_va->m_clt_skfd;
    int t_mf_skfd;
    unsigned char t_dev_status=255;
    char t_dev_data=-1;
    while(1)
    {
        bzero(t_data,SCKT_DATA_LEN);
        bzero(t_data_copy,SCKT_DATA_LEN);
        t_i=0;
        ret=recv(t_skfd,t_data,SCKT_DATA_LEN,0);
        if(ret<0)err_fun(__FILE__,__LINE__,"recv",errno);
        memcpy(t_data_copy,t_data,SCKT_DATA_LEN);
        t_ptl.decode(t_data,t_i);
        recv_pack=t_ptl.getPack();
        unsigned char t_msg=recv_pack.getMsg();
        switch(t_msg)
        {

        case GET_DEV_DATA:
                recv_pack.ackGetDataFromDevsByPlatD(t_mac,t_id,t_tpData);
                cout<<"pth dev fun : "<<"t_mac "<<t_mac<<"t_id "<<t_id<<endl;
                cout<<"pth dev fun : "<<"t_sev_info "<<t_va->m_mfInfo->m_id<<endl;
                if(0!=strcmp(t_id,t_va->m_mfInfo->m_id))
                {
                    t_bl=find_skfd_by_id(t_id,*t_va,t_mf_skfd);
                    if(t_bl)
                    {
                        ret=send(t_mf_skfd,t_data_copy,ret,0);
                        if(ret<0) err_fun(__FILE__,__LINE__,"send",ret);
                     }
                    else cout<<"can not find the "<<t_id<<" mf!"<<endl;
                }
                cout<<"pth dev fun get dev data send dev data to ui "<<endl;
                snd_pack_to_ui(*t_va,recv_pack);
            break;

        case DEV_LOGOUT:
            recv_pack.logoutD(t_mac);
            t_bl=delete_dev_by_mac(t_mac,*t_va);
            if(t_bl)
            {
                cout<<t_mac<<":logout!"<<endl;
            }
            send_pack.ackE(ACK_SUCCESS);
            t_ptl.setPack(send_pack);
            bzero(t_data,SCKT_DATA_LEN);
            t_i=0;
            t_ptl.encode(t_data,t_i);
            ret=send(t_skfd,t_data,t_i,0);
            if(ret<0) err_fun(__FILE__,__LINE__,"send",errno);
            break;

        case GET_DEV_STATUS:

            recv_pack.ackLightStatusByPlatD(t_mac,t_id,t_dev_status,t_dev_data);
            cout<<"t_src"<<t_mac<<"t_dest"<<t_id<<endl;
            t_bl=find_skfd_by_id(t_id,*t_va,t_mf_skfd);
            if(t_bl)
            {
                ret=send(t_mf_skfd,t_data_copy,ret,0);
                if(ret<0) err_fun(__FILE__,__LINE__,"send",ret);
            }
            else cout<<"can not find the "<<t_id<<" mf!"<<endl;

            snd_pack_to_ui(*t_va,recv_pack);

            break;

        case ACK_SUCCESS:
            recv_pack.ackD(t_id);
            cout<<"pth dev fun :"<<"t_dest"<<t_id<<endl;
            t_bl=find_skfd_by_id(t_id,*t_va,t_mf_skfd);
            if(t_bl)
            {
                ret=send(t_mf_skfd,t_data_copy,ret,0);
                if(ret<0) err_fun(__FILE__,__LINE__,"send",ret);
            }
            else cout<<"can not find the "<<t_id<<" mf!"<<endl;
            break;
        }
        bzero(t_mac,MAC_LEN);
        bzero(t_id,ID_MAX_LEN);
        ret=-1;
        t_bl=false;
    }
    return 0;
}



void show_list(list<DEV_Info*> t_devInfoList,char * t_mac)
{
    cout<<"please select the num:"<<endl;
    int t_tp=0;
    list<DEV_Info*>::iterator t_it;
    for(t_it=t_devInfoList.begin();t_it!=t_devInfoList.end();t_it++)
    {
        cout<<t_tp++<<":"<<(*t_it)->m_name<<" "<<(*t_it)->m_mac<<endl;

    }
    int t_i=0;
    int t_select;
    cin>>t_select;
    t_it=t_devInfoList.begin();
    for(t_i=0;t_i<t_select;t_i++)
    {
        t_it++;

    }
    strcpy(t_mac,(*t_it)->m_mac);

}
/*中控向设备发送消息函数*/
void snd_recv_plat_dev(SEV_Glb & t_va,Pack & snd_pack)
{
    char t_mac[MAC_LEN];
    char t_id[ID_MAX_LEN];
    bzero(t_id,ID_MAX_LEN);
    bzero(t_mac,MAC_LEN);
   
    int t_dev_skfd=-1;

    snd_pack.reqFromMfToDevByPlatD(t_id,t_mac);
    //show_list(t_va.m_devInfoList,t_mac);
    
    find_skfd_by_mac(t_mac,t_va,t_dev_skfd);
    
    int ret=-1;
    char t_ptlData[SCKT_DATA_LEN];
    Protocal t_ptl;
    //Pack snd_pack;
    bzero(t_ptlData,SCKT_DATA_LEN);
    t_ptl.setPack(snd_pack);
    int t_i=0; 
    cout<<"encodeing..."<<endl;
    t_ptl.encode(t_ptlData,t_i);
    cout<<"sending.."<<endl;
    

    cout<<"show data"<<endl;
    ret = send(t_dev_skfd, t_ptlData,t_i, 0);
    if(ret < 0) err_fun(__FILE__, __LINE__, "send", errno);

}
/*服务器工作线程*/
void *pth_sev_fun(void *arg)
{
    SEV_Glb *g_va=(SEV_Glb*)arg;

    int ret=-1;
    pthread_t pth_sev_mf;
    pthread_t pth_sev_dev;

    while(1)
    {
        lab0:g_va->m_clt_skfd = accept(g_va->m_skfd, NULL, NULL);
        if(g_va->m_clt_skfd < 0) err_fun(__FILE__, __LINE__,"accept", errno);

        int rt=login_register_fun(*g_va);
        if(rt == ACK_LOGIN_FAIL) goto lab0;

        if(rt==MF_LOGIN)
        {
            ret = pthread_create(&pth_sev_mf, NULL, pth_mf_fun,(void *)g_va);
            if(ret != 0) err_fun(__FILE__, __LINE__,"pthread_create", ret);
        }
        else if(rt==DEV_LOGIN)
        {
            ret= pthread_create(&pth_sev_dev, NULL, pth_dev_fun,(void *)g_va);
            if(ret != 0) err_fun(__FILE__, __LINE__,"pthread_create", ret);
        }
    }
}




