#include "../h_src/dev.h"
#include<QDebug>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include<errno.h>
#include<QDebug>

Dev::Dev(DEV_Info * t_devInfo,QWidget *parent) :
    QWidget(parent)
{
    m_devInfo=t_devInfo;
    m_model=MANUL;
    m_gLayout=new QGridLayout(this);
    m_rLayout=new QGridLayout;
    m_hLayout=new QHBoxLayout;
    m_vLayout=new QVBoxLayout;

    m_switchBtn=new QPushButton("open",this);
    m_modelBtn=new QPushButton("manul",this);
    m_iconLabel=new QLabel("",this);



    switch(m_devInfo->m_devType)
    {
    case DEV_LIGHT:

 
        m_nameLabel=new QLabel(m_devInfo->m_name,this);
        m_nameLabel->setFixedHeight(20);

        m_lightLabel=new QLabel("light:",this);
        m_lightValueLabel=new QLabel("",this);
        m_lightValueLabel->setNum((int)m_devInfo->m_light);
        m_dataLabel=new QLabel("R:",this);
        m_dataValueLabel=new QLabel("",this);
        m_dataValueLabel->setNum((int)m_devInfo->m_data);
        m_iconPixmap=new QPixmap("://image/light_off.png");
        m_iconLabel->setPixmap(*m_iconPixmap);

        m_sliderLabel=new QLabel("R: ",this);
        m_dataSlider=new QSlider(Qt::Horizontal);
        m_dataSlider->setMinimum(0);
        m_dataSlider->setMaximum(100);


        if(DEV_OFFLINE==m_devInfo->m_isOnline)
        {
            m_isOnlineLabel=new QLabel("offline",this);
        }
        else if(DEV_ONLINE==m_devInfo->m_isOnline)
        {      
            m_isOnlineLabel=new QLabel("online",this);
        }

        m_hLayout->addWidget(m_sliderLabel);
        m_hLayout->addWidget(m_dataSlider);

         connect(m_dataSlider,SIGNAL(valueChanged(int)),this,SLOT(dataSlider_slot(int)));

        break;
    case DEV_AIRCONDITIONER:


        m_nameLabel=new QLabel(m_devInfo->m_name,this);
        m_nameLabel->setFixedHeight(20);

        m_lightLabel=new QLabel("temperature:",this);
        m_lightValueLabel=new QLabel("",this);
        m_lightValueLabel->setNum((int)m_devInfo->m_light);
        m_dataLabel=new QLabel("air temperature:",this);
        m_dataValueLabel=new QLabel("",this);
        m_dataValueLabel->setNum((int)m_devInfo->m_data);
        m_iconPixmap=new QPixmap("://image/air_off.png");

        m_iconLabel->setPixmap(*m_iconPixmap);

        m_sliderLabel=new QLabel("air temperature: ",this);
        m_dataSlider=new QSlider(Qt::Horizontal);
        m_dataSlider->setMinimum(0);
        m_dataSlider->setMaximum(100);


        if(DEV_OFFLINE==m_devInfo->m_isOnline)
        {
          
            m_isOnlineLabel=new QLabel("offline",this);

        }
        else if(DEV_ONLINE==m_devInfo->m_isOnline)
        {
        
            m_isOnlineLabel=new QLabel("online",this);

        }

        m_hLayout->addWidget(m_sliderLabel);
        m_hLayout->addWidget(m_dataSlider);

       connect(m_dataSlider,SIGNAL(valueChanged(int)),this,SLOT(dataSlider_slot(int)));


        break;
    case DEV_FAN:


        m_nameLabel=new QLabel(m_devInfo->m_name,this);
        m_nameLabel->setFixedHeight(20);

        m_lightLabel=new QLabel("",this);
        m_lightValueLabel=new QLabel("",this);
        m_lightValueLabel->setNum((int)m_devInfo->m_light);
        m_dataLabel=new QLabel("",this);
        m_dataValueLabel=new QLabel("",this);
        m_dataValueLabel->setNum((int)m_devInfo->m_data);
        m_iconPixmap=new QPixmap("://image/fan_off.png");

        m_iconLabel->setPixmap(*m_iconPixmap);

        m_sliderLabel=new QLabel("",this);

  

        if(DEV_OFFLINE==m_devInfo->m_isOnline)
        {
          
            m_isOnlineLabel=new QLabel("offline",this);

        }
        else if(DEV_ONLINE==m_devInfo->m_isOnline)
        {
          
            m_isOnlineLabel=new QLabel("online",this);

        }

        break;
    case DEV_CURTAIN:


        m_nameLabel=new QLabel(m_devInfo->m_name,this);
        m_nameLabel->setFixedHeight(20);

        m_lightLabel=new QLabel("light:",this);
        m_lightValueLabel=new QLabel("",this);
        m_lightValueLabel->setNum((int)m_devInfo->m_light);
        m_dataLabel=new QLabel("curtain half:",this);
        m_dataValueLabel=new QLabel("",this);
        m_dataValueLabel->setNum((int)m_devInfo->m_data);
        m_iconPixmap=new QPixmap("://image/curtain_off.png");

        m_iconLabel->setPixmap(*m_iconPixmap);

        m_sliderLabel=new QLabel("curtain half: ",this);
        m_dataSlider=new QSlider(Qt::Horizontal);
        m_dataSlider->setMinimum(0);
        m_dataSlider->setMaximum(100);


        if(DEV_OFFLINE==m_devInfo->m_isOnline)
        {

            m_isOnlineLabel=new QLabel("offline",this);

        }
        else if(DEV_ONLINE==m_devInfo->m_isOnline)
        {
    
            m_isOnlineLabel=new QLabel("online",this);

        }

        m_hLayout->addWidget(m_sliderLabel);
        m_hLayout->addWidget(m_dataSlider);

        connect(m_dataSlider,SIGNAL(valueChanged(int)),this,SLOT(dataSlider_slot(int)));


        break;
    }

    m_rLayout->addWidget(m_lightLabel,0,0);
    m_rLayout->addWidget(m_lightValueLabel,0,1);
    m_rLayout->addWidget(m_dataLabel,1,0);
    m_rLayout->addWidget(m_dataValueLabel,1,1);

    m_vLayout->addLayout(m_rLayout);
    m_vLayout->addWidget(m_switchBtn);
    m_vLayout->addWidget(m_modelBtn);

    m_gLayout->addWidget(m_nameLabel,0,0);
    m_gLayout->addWidget(m_isOnlineLabel,0,1);
    m_gLayout->addWidget(m_iconLabel,1,0);
    m_gLayout->addLayout(m_vLayout,1,1,1,1);
    m_gLayout->addLayout(m_hLayout,2,0,1,2);


    connect(m_switchBtn,SIGNAL(clicked()),this,SLOT(switchBtn_slot()));
    connect(m_modelBtn,SIGNAL(clicked()),this,SLOT(modelBtn_slot()));


}

void Dev::flushDev()
{
    cout<<"in flushDEV fun "<<endl;


    switch(m_devInfo->m_devType)
    {
        case DEV_LIGHT:

        this->m_isOnlineLabel->setText("online");
        if(DEV_ON==m_devInfo->m_status)
        {
            delete m_iconPixmap;
            m_iconPixmap=new QPixmap("://image/light_on.png");
            m_iconLabel->setPixmap(*m_iconPixmap);
            m_switchBtn->setText("close");
        }
        else if(DEV_OFF)
        {
            delete m_iconPixmap;
            m_iconPixmap=new QPixmap("://image/light_off.png");
            m_iconLabel->setPixmap(* m_iconPixmap);
            m_switchBtn->setText("open");
        }
        m_lightValueLabel->setNum((int)m_devInfo->m_light);
        m_dataValueLabel->setNum((int)m_devInfo->m_data);
        m_dataSlider->setValue((int)m_devInfo->m_data);
        break;
    case DEV_AIRCONDITIONER:
        this->m_isOnlineLabel->setText("online");
        if(DEV_ON==m_devInfo->m_status)
        {
            delete m_iconPixmap;
            m_iconPixmap=new QPixmap("://image/air_on.png");
            m_iconLabel->setPixmap(*m_iconPixmap);
            m_switchBtn->setText("close");
        }
        else if(DEV_OFF)
        {
            delete m_iconPixmap;
            m_iconPixmap=new QPixmap("://image/air_off.png");
            m_iconLabel->setPixmap(* m_iconPixmap);
            m_switchBtn->setText("open");
        }
        m_lightValueLabel->setNum((int)m_devInfo->m_light);
        m_dataValueLabel->setNum((int)m_devInfo->m_data);
        m_dataSlider->setValue((int)m_devInfo->m_data);
        break;

       case DEV_CURTAIN:
        this->m_isOnlineLabel->setText("online");
        if(DEV_ON==m_devInfo->m_status)
        {
            delete m_iconPixmap;
            m_iconPixmap=new QPixmap("://image/curtain_on.png");
            m_iconLabel->setPixmap(*m_iconPixmap);
            m_switchBtn->setText("close");
        }
        else if(DEV_OFF)
        {
            delete m_iconPixmap;
            m_iconPixmap=new QPixmap("://image/curtain_off.png");
            m_iconLabel->setPixmap(* m_iconPixmap);
            m_switchBtn->setText("open");
        }
        m_lightValueLabel->setNum((int)m_devInfo->m_light);
        m_dataValueLabel->setNum((int)m_devInfo->m_data);
        m_dataSlider->setValue((int)m_devInfo->m_data);
        break;
      case DEV_FAN:
        this->m_isOnlineLabel->setText("online");
        if(DEV_ON==m_devInfo->m_status)
        {
            delete m_iconPixmap;
            m_iconPixmap=new QPixmap("://image/fan_on.png");
            m_iconLabel->setPixmap(*m_iconPixmap);
            m_switchBtn->setText("close");
        }
        else if(DEV_OFF)
        {
            delete m_iconPixmap;
            m_iconPixmap=new QPixmap("://image/fan_off.png");
            m_iconLabel->setPixmap(* m_iconPixmap);
            m_switchBtn->setText("open");
        }

        break;

    }
}

void Dev::setGlb(Glb *t_va)
{
    this->m_va=t_va;
}

void Dev::switchBtn_slot()
{
    msg_buf snd_buf;
    Pack snd_pack;
    int ret=-1;

    cout<<"dev msg skfd "<<m_va->m_msg_skfd<<endl;
    snd_buf.m_msgType=MSG_UI_TO_BACK;

    switch(m_devInfo->m_devType)
    {
    case DEV_LIGHT:
        if(DEV_OFF==m_devInfo->m_status)
        {

            snd_pack.reqFromMfToDevByPlatE(m_va->m_mfInfo->m_id,m_devInfo->m_mac,DEV_ON);
            memcpy(snd_buf.m_msgData,(void*)&snd_pack,sizeof(snd_pack));

            qDebug()<<"msgsnding..."<<endl;
            cout<<"cntl  mf id "<<m_va->m_mfInfo->m_id<<"  mac "<<m_devInfo->m_mac<<endl;
            ret = msgsnd(m_va->m_msg_skfd, (void *)&snd_buf, SIZE, 0); //阻塞发送消息
            if(ret < 0) err_fun(__FILE__, __LINE__, "msgsnd", errno);
            qDebug()<<"msgsnd finish"<<endl;
            m_devInfo->m_status=DEV_ON;
            delete m_iconPixmap;
            m_iconPixmap=new QPixmap("://image/light_on.png");
            m_iconLabel->setPixmap(*m_iconPixmap);
            m_switchBtn->setText("close");
        }
        else if(DEV_ON)
        {
            snd_pack.reqFromMfToDevByPlatE(m_va->m_mfInfo->m_id,m_devInfo->m_mac,DEV_OFF);
            memcpy(snd_buf.m_msgData,(void*)&snd_pack,sizeof(snd_pack));

            cout<<"msgsnding..."<<endl;
            ret = msgsnd(m_va->m_msg_skfd, (void *)&snd_buf, SIZE, 0); //阻塞发送消息
            if(ret < 0) err_fun(__FILE__, __LINE__, "msgsnd", errno);
            cout<<"msgsnd finish"<<endl;
            m_devInfo->m_status=DEV_OFF;
            m_dataSlider->setValue(0);
            delete m_iconPixmap;
            m_iconPixmap=new QPixmap("://image/light_off.png");
            m_iconLabel->setPixmap(* m_iconPixmap);
            m_switchBtn->setText("open");
        }
        break;
    case DEV_AIRCONDITIONER:
        if(DEV_OFF==m_devInfo->m_status)
        {

            snd_pack.reqFromMfToDevByPlatE(m_va->m_mfInfo->m_id,m_devInfo->m_mac,DEV_ON);
            memcpy(snd_buf.m_msgData,(void*)&snd_pack,sizeof(snd_pack));

            qDebug()<<"msgsnding..."<<endl;
            cout<<"cntl  mf id "<<m_va->m_mfInfo->m_id<<"  mac "<<m_devInfo->m_mac<<endl;
            ret = msgsnd(m_va->m_msg_skfd, (void *)&snd_buf, SIZE, 0); //阻塞发送消息
            if(ret < 0) err_fun(__FILE__, __LINE__, "msgsnd", errno);
            qDebug()<<"msgsnd finish"<<endl;

            m_devInfo->m_status=DEV_ON;
            delete m_iconPixmap;
            m_iconPixmap=new QPixmap("://image/air_on.png");
            m_iconLabel->setPixmap(*m_iconPixmap);
            m_switchBtn->setText("close");
        }
        else if(DEV_ON)
        {
            snd_pack.reqFromMfToDevByPlatE(m_va->m_mfInfo->m_id,m_devInfo->m_mac,DEV_OFF);
            memcpy(snd_buf.m_msgData,(void*)&snd_pack,sizeof(snd_pack));

            cout<<"msgsnding..."<<endl;
            ret = msgsnd(m_va->m_msg_skfd, (void *)&snd_buf, SIZE, 0); //阻塞发送消息
            if(ret < 0) err_fun(__FILE__, __LINE__, "msgsnd", errno);
            cout<<"msgsnd finish"<<endl;
            m_devInfo->m_status=DEV_OFF;
            m_dataSlider->setValue(0);
            delete m_iconPixmap;
            m_iconPixmap=new QPixmap("://image/air_off.png");
            m_iconLabel->setPixmap(* m_iconPixmap);
            m_switchBtn->setText("open");
        }
        break;
    case DEV_FAN:
        if(DEV_OFF==m_devInfo->m_status)
        {

            snd_pack.reqFromMfToDevByPlatE(m_va->m_mfInfo->m_id,m_devInfo->m_mac,DEV_ON);
            memcpy(snd_buf.m_msgData,(void*)&snd_pack,sizeof(snd_pack));

            qDebug()<<"msgsnding..."<<endl;
            cout<<"cntl  mf id "<<m_va->m_mfInfo->m_id<<"  mac "<<m_devInfo->m_mac<<endl;
            ret = msgsnd(m_va->m_msg_skfd, (void *)&snd_buf, SIZE, 0); //阻塞发送消息
            if(ret < 0) err_fun(__FILE__, __LINE__, "msgsnd", errno);
            qDebug()<<"msgsnd finish"<<endl;
            m_devInfo->m_status=DEV_ON;
            delete m_iconPixmap;
            m_iconPixmap=new QPixmap("://image/fan_on.png");
            m_iconLabel->setPixmap(*m_iconPixmap);
            m_switchBtn->setText("close");
        }
        else if(DEV_ON)
        {
            snd_pack.reqFromMfToDevByPlatE(m_va->m_mfInfo->m_id,m_devInfo->m_mac,DEV_OFF);
            memcpy(snd_buf.m_msgData,(void*)&snd_pack,sizeof(snd_pack));

            cout<<"msgsnding..."<<endl;
            ret = msgsnd(m_va->m_msg_skfd, (void *)&snd_buf, SIZE, 0); //阻塞发送消息
            if(ret < 0) err_fun(__FILE__, __LINE__, "msgsnd", errno);
            cout<<"msgsnd finish"<<endl;
            m_devInfo->m_status=DEV_OFF;
            m_dataSlider->setValue(0);
            delete m_iconPixmap;
            m_iconPixmap=new QPixmap("://image/fan_off.png");
            m_iconLabel->setPixmap(* m_iconPixmap);
            m_switchBtn->setText("open");
        }
        break;
    case DEV_CURTAIN:
        if(DEV_OFF==m_devInfo->m_status)
        {

            snd_pack.reqFromMfToDevByPlatE(m_va->m_mfInfo->m_id,m_devInfo->m_mac,DEV_ON);
            memcpy(snd_buf.m_msgData,(void*)&snd_pack,sizeof(snd_pack));

            qDebug()<<"msgsnding..."<<endl;
            cout<<"cntl  mf id "<<m_va->m_mfInfo->m_id<<"  mac "<<m_devInfo->m_mac<<endl;
            ret = msgsnd(m_va->m_msg_skfd, (void *)&snd_buf, SIZE, 0); //阻塞发送消息
            if(ret < 0) err_fun(__FILE__, __LINE__, "msgsnd", errno);
            qDebug()<<"msgsnd finish"<<endl;
            m_devInfo->m_status=DEV_ON;
            delete m_iconPixmap;
            m_iconPixmap=new QPixmap("://image/curtain_on.png");
            m_iconLabel->setPixmap(*m_iconPixmap);
            m_switchBtn->setText("close");
        }
        else if(DEV_ON)
        {
            snd_pack.reqFromMfToDevByPlatE(m_va->m_mfInfo->m_id,m_devInfo->m_mac,DEV_OFF);
            memcpy(snd_buf.m_msgData,(void*)&snd_pack,sizeof(snd_pack));

            cout<<"msgsnding..."<<endl;
            ret = msgsnd(m_va->m_msg_skfd, (void *)&snd_buf, SIZE, 0); //阻塞发送消息
            if(ret < 0) err_fun(__FILE__, __LINE__, "msgsnd", errno);
            cout<<"msgsnd finish"<<endl;
            m_devInfo->m_status=DEV_OFF;
            m_dataSlider->setValue(0);
            delete m_iconPixmap;
            m_iconPixmap=new QPixmap("://image/curtain_off.png");
            m_iconLabel->setPixmap(* m_iconPixmap);
            m_switchBtn->setText("open");
        }
        break;
    }
}

void Dev::modelBtn_slot()
{
       qDebug() << "model";
       if(MANUL==m_model)
       {
           m_model=AUTO;
           m_modelBtn->setText("manul");
       }
       else if(AUTO==m_model)
       {
           m_model=MANUL;
           m_modelBtn->setText("auto");
       }
}

void Dev::dataSlider_slot(int value)
{
       qDebug() <<value;
       int ret=-1;
       msg_buf snd_buf;
       Pack snd_pack;
       snd_buf.m_msgType=MSG_UI_TO_BACK;

       if(DEV_ON==m_devInfo->m_status)
       {
           snd_pack.reqSetDevDataByPlatE(m_va->m_mfInfo->m_id,m_devInfo->m_mac,(char)value);
           memcpy(snd_buf.m_msgData,(void*)&snd_pack,sizeof(snd_pack));

           cout<<"msgsnding..."<<endl;
           ret = msgsnd(m_va->m_msg_skfd, (void *)&snd_buf, SIZE, 0); //阻塞发送消息
           if(ret < 0) err_fun(__FILE__, __LINE__, "msgsnd", errno);
           cout<<"msgsnd finish"<<endl;

            m_devInfo->m_data=(char)value;
            m_dataValueLabel->setNum(value);
       }
       else if(DEV_OFF==m_devInfo->m_status)
       {
            m_dataSlider->setValue(0);
            m_dataValueLabel->setNum(0);
       }
}
