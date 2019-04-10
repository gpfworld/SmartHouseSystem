/*************************************************************************
	> File Name: mf_back_client.cpp
	> Author: 
	> Mail: 
	> Created Time: 2016年10月19日 星期三 00时01分31秒
 ************************************************************************/

#include<iostream>
using namespace std;


#include"../h_src/mf_back_client.h"


/*向QT端发送pack数据包*/
void snd_pack_to_ui(MF_Glb & t_va,Pack & snd_pack)
{
    int ret=-1;
    msg_buf snd_buf;
    snd_buf.m_msgType=MSG_BACK_TO_UI;
    memcpy(snd_buf.m_msgData,(void*)&snd_pack,sizeof(snd_pack));
    cout<<"snd pack to ui msgsnding..."<<endl;
    //cout<<"cntl  mf id "<<m_va->m_mfInfo->m_id<<"  mac "<<m_devInfo->m_mac<<endl;
    ret = msgsnd(t_va.m_msg_skfd, (void *)&snd_buf, SIZE, 0); //阻塞发送消息
    if(ret < 0) err_fun(__FILE__, __LINE__, "msgsnd", errno);
    cout<<"snd pack to ui  msgsnd finishs"<<endl;
}

void err_fun(const char *file_nam, const int line,const char *fun_nam, const int err_no)
{
    fprintf(stderr, "in %s %d, %s is fail: %s\n", file_nam, line, fun_nam, strerror(err_no));
    exit(-1);
}
/*网络连接函数*/
void socket_fun(int & t_sckt)
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

    t_sckt=skfd;
}
/*客户端用户注册函数*/
void mf_register_fun(MF_Glb & t_va)
{
    int ret=-1;
    char t_ptlData[SCKT_DATA_LEN];
    Protocal t_ptl;
    Pack snd_pack;
    Pack recv_pack;
    bzero(t_ptlData,SCKT_DATA_LEN);
    
    snd_pack.mfRegisterE(*t_va.m_mfInfo);
    t_ptl.setPack(snd_pack);
    int t_i=0; 
    t_ptl.encode(t_ptlData,t_i);

    ret = send(t_va.m_mfInfo->m_skfd, t_ptlData,t_i, 0);
    if(ret < 0) err_fun(__FILE__, __LINE__, "sned", errno);
    bzero(t_ptlData, SCKT_DATA_LEN);
    t_i=0;
    cout<<"mf wait recv"<<endl;
    ret = recv(t_va.m_mfInfo->m_skfd,t_ptlData,SCKT_DATA_LEN, 0);
    if(ret < 0) err_fun(__FILE__, __LINE__, "sned", errno);
    else if(ret > 0)
    {
        t_ptl.decode(t_ptlData,t_i);
        recv_pack=t_ptl.getPack();

        if(ACK_LOGIN_SUCCESS ==recv_pack.getMsg())
        {
            snd_pack_to_ui(t_va,recv_pack);
            printf("mf register success!\n");
        }
        else if(ACK_LOGIN_FAIL==recv_pack.getMsg())
        {
            snd_pack_to_ui(t_va,recv_pack);
            printf("mf register fail, plrase try again!\n");
        }
    }
}
/*客户端用户登录函数*/
void login_fun(MF_Glb & t_va,Pack & snd_pack)
{
    int ret=-1;
    char t_ptlData[SCKT_DATA_LEN];
    Protocal t_ptl;
    Pack recv_pack;
    bzero(t_ptlData,SCKT_DATA_LEN);
    
    t_ptl.setPack(snd_pack);
    int t_i=0; 
    t_ptl.encode(t_ptlData,t_i);
    cout<<"login fun sending..."<<endl;
    ret = send(t_va.m_mfInfo->m_skfd, t_ptlData,t_i, 0);
    if(ret < 0) err_fun(__FILE__, __LINE__, "sned", errno);
     cout<<"login fun send finish"<<endl;
    bzero(t_ptlData, SCKT_DATA_LEN);
    t_i=0;
    cout<<"login fun recving.."<<endl;
    ret = recv(t_va.m_mfInfo->m_skfd,t_ptlData,SCKT_DATA_LEN, 0);
    cout<<"login fun recv finish"<<endl;
    cout<<"login wait recv"<<endl;
    if(ret < 0) err_fun(__FILE__, __LINE__, "sned", errno);
    else if(ret > 0)
    {
        t_ptl.decode(t_ptlData,t_i);
        recv_pack=t_ptl.getPack();


        snd_pack_to_ui(t_va,recv_pack);
    }
}
/*客户端用户登出函数*/
void logout_fun(MF_Info& t_mfInfo)
{
    int ret=-1;
    char t_ptlData[SCKT_DATA_LEN];
    Protocal t_ptl;
    Pack snd_pack;
    Pack recv_pack;
    bzero(t_ptlData,SCKT_DATA_LEN);
    
    snd_pack.logoutE(t_mfInfo.m_id,MF_LOGOUT);
    t_ptl.setPack(snd_pack);
    int t_i=0; 
    t_ptl.encode(t_ptlData,t_i);
    ret = send(t_mfInfo.m_skfd, t_ptlData,t_i, 0);
    if(ret < 0) err_fun(__FILE__, __LINE__, "sned", errno);
    bzero(t_ptlData, SCKT_DATA_LEN);
    t_i=0;
    ret = recv(t_mfInfo.m_skfd,t_ptlData,SCKT_DATA_LEN, 0);
    cout<<"logout wait recv"<<endl;
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
        else cout<<"logout fail!"<<endl;
    }
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
/*客户端向设备端发送控制信息*/
void snd_recv_mf_dev(MF_Info & t_mfInfo,Pack & snd_pack, Pack & recv_pack)
{

    int ret=-1;
    char t_ptlData[SCKT_DATA_LEN];
    Protocal t_ptl;
    //Pack snd_pack;
    bzero(t_ptlData,SCKT_DATA_LEN);

    t_ptl.setPack(snd_pack);
    int t_i=0; 
    
    t_ptl.encode(t_ptlData,t_i);
    
    ret = send(t_mfInfo.m_skfd, t_ptlData,t_i, 0);
    if(ret < 0) err_fun(__FILE__, __LINE__, "send", errno);
    bzero(t_ptlData, SCKT_DATA_LEN);
    t_i=0;
    ret = recv(t_mfInfo.m_skfd,t_ptlData,SCKT_DATA_LEN, 0);
    if(ret < 0) err_fun(__FILE__, __LINE__, "send", errno);

    t_ptl.decode(t_ptlData,t_i);
    
    recv_pack=t_ptl.getPack();
}
/*获取设备数据*/
void get_dev_data(MF_Glb & t_va,Pack & t_pack)
{
    Pack recv_pack;

    snd_recv_mf_dev(*t_va.m_mfInfo,t_pack,recv_pack);

    if(GET_DEV_DATA ==recv_pack.getMsg())
    {
        cout<<"send dev data to UI"<<endl;
        snd_pack_to_ui(t_va,recv_pack);
        cout<<"send dev data to UI"<<endl;
    }
}
/*获取设备状态信息*/
void get_dev_status(MF_Glb & t_va,Pack & t_pack)
{
    Pack recv_pack;
    
    snd_recv_mf_dev(*t_va.m_mfInfo,t_pack,recv_pack);

        if(GET_DEV_STATUS ==recv_pack.getMsg())
        {
            cout<<"send dev data to UI"<<endl;
            snd_pack_to_ui(t_va,recv_pack);
            cout<<"send dev data to UI"<<endl;
        }
}
/*开启设备函数*/
void set_dev_on(MF_Info & t_mfInfo,Pack & t_pack)
{
    Pack recv_pack;
    snd_recv_mf_dev(t_mfInfo,t_pack,recv_pack);

    if(ACK_SUCCESS ==recv_pack.getMsg())
        {
            cout<<"dev on"<<endl;
        }
}
/*关闭设备函数*/
void set_dev_off(MF_Info & t_mfInfo,Pack & t_pack)
{
    Pack recv_pack;
    snd_recv_mf_dev(t_mfInfo,t_pack,recv_pack);

        if(ACK_SUCCESS ==recv_pack.getMsg())
        {
            cout<<"dev off"<<endl;
        }
}
/*设置设备数据*/
void set_dev_data(MF_Info & t_mfInfo,Pack & t_pack)
{
    Pack recv_pack;
    //char t_data=23;
    snd_recv_mf_dev(t_mfInfo,t_pack,recv_pack);
        if(ACK_SUCCESS ==recv_pack.getMsg())
        {
            cout<<"dev set"<<endl;
        }
}
/*向中控发出控制信息*/
void snd_recv_mf_plat(MF_Info & t_mfInfo,Pack & recv_pack,Pack & snd_pack)
{
    int ret=-1;
    char t_ptlData[SCKT_DATA_LEN];
    Protocal t_ptl;
    //Pack snd_pack;
    bzero(t_ptlData,SCKT_DATA_LEN);
    
    //snd_pack.reqFromMfToPlatE(GET_DEVS_INFO);
    t_ptl.setPack(snd_pack);
    int t_i=0; 
    t_ptl.encode(t_ptlData,t_i);
    ret = send(t_mfInfo.m_skfd, t_ptlData,t_i, 0);
    if(ret < 0) err_fun(__FILE__, __LINE__, "send", errno);
    bzero(t_ptlData, SCKT_DATA_LEN);
    t_i=0;
    cout<<"recving .."<<endl;
    ret = recv(t_mfInfo.m_skfd,t_ptlData,SCKT_DATA_LEN, 0);
    cout<<"recv finish!"<<endl;


    if(ret < 0) err_fun(__FILE__, __LINE__, "send", errno);
    t_ptl.decode(t_ptlData,t_i);
    recv_pack=t_ptl.getPack();
}
/*获取在线设备列表*/
void get_devs_list(MF_Glb & t_va,Pack & t_pack)
{
    Pack recv_pack;

    msg_buf snd_buf;
    snd_buf.m_msgType=MSG_BACK_TO_UI;

    snd_recv_mf_plat(*t_va.m_mfInfo,recv_pack,t_pack);

    recv_pack.ackGetDevsFromPlatD(t_va.m_devInfoList);

    list<DEV_Info*>::iterator t_it;

    for(t_it=t_va.m_devInfoList.begin();t_it!=t_va.m_devInfoList.end();t_it++)
    {
        cout<<(*t_it)->m_name<<" "<<(*t_it)->m_mac<<" "<<(int)(*t_it)->m_devType<<" "<<(int)(*t_it)->m_isOnline<<endl;
    }

    if(GET_DEVS_INFO ==recv_pack.getMsg())
    {
        snd_pack_to_ui(t_va,recv_pack);
    }
}

/*创建消息队列*/
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



