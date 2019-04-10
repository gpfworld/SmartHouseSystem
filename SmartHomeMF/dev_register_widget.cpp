#include "dev_register_widget.h"

dev_register_widget::dev_register_widget(MF_Glb *t_va, QWidget *parent) :
    QWidget(parent)
{
    m_va=t_va;

    m_gLayout=new QGridLayout(this);

    m_macLabel=new QLabel("mac:",this);
    m_macEdit=new QLineEdit("",this);
    m_nameLabel=new QLabel("name:",this);
    m_nameEdit=new QLineEdit("",this);
    m_pwdLabel=new QLabel("pwd:",this);
    m_pwdEdit=new QLineEdit("",this);
    m_confirePwdLabel=new QLabel("confire pwd:",this);
    m_confirePwdEdit=new QLineEdit("",this);
    m_ipLabel=new QLabel("ip:",this);
    m_ipEdit=new QLineEdit("",this);
    m_portLabel=new QLabel("port",this);
    m_portEdit=new QLineEdit("",this);

    m_cancelBtn=new QPushButton("login",this);
    m_registerBtn=new QPushButton("register",this);

    m_gLayout->addWidget(m_macLabel,0,0);
    m_gLayout->addWidget(m_macEdit,0,1);
    m_gLayout->addWidget(m_nameLabel,1,0);
    m_gLayout->addWidget(m_nameEdit,1,1);
    m_gLayout->addWidget(m_pwdLabel,2,0);
    m_gLayout->addWidget(m_pwdEdit,2,1);
    m_gLayout->addWidget(m_confirePwdLabel,3,0);
    m_gLayout->addWidget(m_confirePwdEdit,3,1);
    m_gLayout->addWidget(m_ipLabel,4,0);
    m_gLayout->addWidget(m_ipEdit,4,1);
    m_gLayout->addWidget(m_portLabel,5,0);
    m_gLayout->addWidget(m_portEdit,5,1);
    m_gLayout->addWidget(m_cancelBtn,6,0);
    m_gLayout->addWidget(m_registerBtn,6,1);

    m_gLayout->setMargin(10);

    connect(m_cancelBtn,SIGNAL(clicked()),this,SLOT(cancelBtn_slot()));
    connect(m_registerBtn,SIGNAL(clicked()),this,SLOT(registerBtn_slot()));
}


bool dev_register_widget::isRegisterSuccess()
{
    Pack snd_pack;
    msg_buf snd_buf;
    int ret=-1;
    snd_buf.m_msgType=MSG_UI_TO_BACK;
    snd_pack.devRegisterE(*m_va->m_devInfo);
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
        return true;
    }
    else if(ACK_LOGIN_FAIL==recv_pack.getMsg())
    {
        return false;
    }
}

void dev_register_widget::cancelBtn_slot()
{
    this->close();
}
//待完成
void dev_register_widget::registerBtn_slot()
{
    m_va->m_devInfo=new DEV_Info;
    bool t_bl=isRegisterSuccess();
    if(t_bl)
    {

    }
    else
    {

    }

}

