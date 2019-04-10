#include "login.h"



login::login(MF_Glb * t_va, QWidget *parent) :
    QWidget(parent)
{
    m_va=t_va;

    m_registerWidget=NULL;
    m_mainWidget=NULL;


    m_gLayout=new QGridLayout(this);

    m_idLabel=new QLabel("id:",this);
    m_idEdit=new QLineEdit("",this);

    m_pwdLabel=new QLabel("pwd:",this);
    m_pwdEdit=new QLineEdit("",this);

    m_pwdEdit->setEchoMode(QLineEdit::Password);

    m_loginBtn=new QPushButton("login",this);
    m_registerBtn=new QPushButton("register",this);

    m_gLayout->addWidget(m_idLabel,1,0);
    m_gLayout->addWidget(m_idEdit,1,1);

    m_gLayout->addWidget(m_pwdLabel,3,0);
    m_gLayout->addWidget(m_pwdEdit,3,1);



    m_gLayout->addWidget(m_loginBtn,6,0);
    m_gLayout->addWidget(m_registerBtn,6,1);

    m_gLayout->setMargin(40);

    connect(m_loginBtn,SIGNAL(clicked()),this,SLOT(loginBtn_slot()));
    connect(m_registerBtn,SIGNAL(clicked()),this,SLOT(registerBtn_slot()));

    this->resize(300,200);

}

bool login::isLoginSuccess()
{
    Pack snd_pack;
    msg_buf snd_buf;
    int ret=-1;
    snd_buf.m_msgType=MSG_UI_TO_BACK;
    snd_pack.loginE(m_va->m_mfInfo->m_id,m_va->m_mfInfo->m_pwd,MF_LOGIN);
    memcpy(snd_buf.m_msgData,(void*)&snd_pack,sizeof(snd_pack));

    ret = msgsnd(m_va->m_msg_skfd, (void *)&snd_buf, SIZE, 0); //阻塞发送消息
    if(ret < 0) err_fun(__FILE__, __LINE__, "msgsnd", errno);

    msg_buf recv_buf;
    Pack recv_pack;

    ret = msgrcv(m_va->m_msg_skfd, (void *)&recv_buf, SIZE,MSG_BACK_TO_UI, 0);
    if(ret<0) err_fun(__FILE__,__LINE__,"msgrecv ",ret);
    memcpy(&recv_pack,recv_buf.m_msgData,sizeof(recv_pack));

    if(ACK_LOGIN_SUCCESS==recv_pack.getMsg())
    {
        recv_pack.ackLoginD(m_va->m_mfInfo->m_token);
        return true;
    }
    else if(ACK_LOGIN_FAIL==recv_pack.getMsg())
    {
        return false;
    }
}

void login::getDevInfoList()
{
    int ret = -1;

    msg_buf recv_buf;
    Pack recv_pack;

    updateDevInfoList(m_va);

    cout<<"main msgrcving.."<<endl;
    ret = msgrcv(m_va->m_msg_skfd, (void *)&recv_buf, SIZE,MSG_BACK_TO_UI, 0);
    if(ret<0) err_fun(__FILE__,__LINE__,"msgrecv ",ret);
    memcpy(&recv_pack,recv_buf.m_msgData,sizeof(recv_pack));
    cout<<"main msgrcv finish"<<endl;

    recv_pack.ackGetDevsFromPlatD(m_va->m_devInfoList);

    cout<<"show dev info list"<<endl;
    cout<<"recv msgData:"<<recv_pack.getMsgData()<<endl;
    list<DEV_Info*>::iterator t_it;
    cout<<"dev info list size  "<<m_va->m_devInfoList.size()<<endl;
    for(t_it=m_va->m_devInfoList.begin();t_it!=m_va->m_devInfoList.end();t_it++)
    {
        cout<<(*t_it)->m_name<<" "<<(*t_it)->m_mac<<" "<<(int)(*t_it)->m_devType<<" "<<(int)(*t_it)->m_isOnline<<endl;
    }
}

void login::loginBtn_slot()
{

    m_va->m_mfInfo=new MF_Info;

    string t_idstr=m_idEdit->text().toStdString();
    string t_pwdstr=m_pwdEdit->text().toStdString();

    memcpy(m_va->m_mfInfo->m_id,t_idstr.c_str(),ID_MAX_LEN);
    memcpy(m_va->m_mfInfo->m_pwd,t_pwdstr.c_str(),PWD_MAX_LEN);


    bool t_bl=isLoginSuccess();


    if(t_bl)
    {
        getDevInfoList();

        pthread_t pth_msg;

        pthread_t pth_getUpdateData;

        int ret=-1;

        m_mainWidget=new Widget(m_va);
        m_mainWidget->show();

        ret=pthread_create(&pth_msg,NULL,pth_msg_fun,(void*)m_va);
        if(ret<0) err_fun(__FILE__,__LINE__,"pthread ",ret);

        ret=pthread_create(&pth_getUpdateData,NULL,pth_getUpdateData_fun,(void*)m_va);
        if(ret<0) err_fun(__FILE__,__LINE__,"pthread ",ret);

        this->hide();
    }
    else
    {
        QMessageBox::information(this,"Tip",tr("ip or pwd is wrong,plessase try again."));
        return;
    }
}

void login::registerBtn_slot()
{
    m_registerWidget=new mf_register_widget(m_va);
    m_registerWidget->show();
}
