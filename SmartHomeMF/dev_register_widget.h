#ifndef DEV_REGISTER_WIDGET_H
#define DEV_REGISTER_WIDGET_H

#include <QWidget>
#include<QLabel>
#include<QLineEdit>
#include<QPushButton>
#include<QGridLayout>


#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <errno.h>

#include"../h_src/mf_info.h"
#include"../h_src/file_opt.h"
#include"clt_common.h"
#include"../h_src/pack.h"


#define SIZE 2048 //消息正文的大小

class dev_register_widget : public QWidget
{
    Q_OBJECT
public:
    explicit dev_register_widget(MF_Glb * t_va,QWidget *parent = 0);


    bool isRegisterSuccess();           //判断注册是否成功
    QLabel * m_macLabel;                //提示mac label
    QLabel * m_nameLabel;               //提示name label
    QLabel * m_pwdLabel;                //提示密码 label
    QLabel * m_confirePwdLabel;         //提示确认密码 label
    QLabel * m_ipLabel;                 //提示ip label
    QLabel * m_portLabel;               //提示port label

    QLineEdit * m_macEdit;              //输入mac  edit
    QLineEdit * m_nameEdit;             //输入name edit
    QLineEdit * m_pwdEdit;              //输入pwd edit
    QLineEdit * m_confirePwdEdit;       //输入确认pwd edit
    QLineEdit * m_ipEdit;               //输入ip edit
    QLineEdit * m_portEdit;             //输入port edit

    QPushButton * m_cancelBtn;          //取消 btn
    QPushButton * m_registerBtn;        //注册 btn

    QGridLayout * m_gLayout;

    MF_Glb * m_va;                      //全局变量
signals:
    
public slots:
    void cancelBtn_slot();              //取消槽函数
    void registerBtn_slot();            //注册槽函数
    
};

#endif // DEV_REGISTER_WIDGET_H
