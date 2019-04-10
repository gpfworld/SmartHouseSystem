#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include<QLabel>
#include<QLineEdit>
#include<QPushButton>
#include<QGridLayout>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <errno.h>
#include<QMessageBox>

#include"../h_src/mf_info.h"
#include"../h_src/file_opt.h"
#include"clt_common.h"
#include"../h_src/pack.h"
#include"mf_register_widget.h"
#include"widget.h"
#include"mf_client.h"

using namespace std;
#include<string>


#define SIZE 2048 //消息正文的大小

class login : public QWidget
{
    Q_OBJECT
public:
    explicit login(MF_Glb * t_va,QWidget *parent = 0);
    

    bool isLoginSuccess();          //判断是否注册成功
    void getDevInfoList();          //获取设备列表

    QLabel * m_idLabel;             //提示id label
    QLabel * m_pwdLabel;            //提示pwd label

    QLineEdit * m_idEdit;           //输入id edit

    QLineEdit * m_pwdEdit;          //输入pwd edit

    QPushButton * m_loginBtn;       //登录  btn
    QPushButton * m_registerBtn;    //注册  btn

    QGridLayout * m_gLayout;

    MF_Glb * m_va;                  //全局变量

    mf_register_widget * m_registerWidget;//注册窗体指针

    Widget * m_mainWidget;          //主窗体指针


signals:
    
public slots:
    void loginBtn_slot();           //登录槽函数
    void registerBtn_slot();        //注册槽函数
};

#endif // LOGIN_H
