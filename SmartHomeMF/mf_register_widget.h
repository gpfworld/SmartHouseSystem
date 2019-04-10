#ifndef MF_REGISTER_WIDGET_H
#define MF_REGISTER_WIDGET_H

#include <QWidget>
#include<QLabel>
#include<QLineEdit>
#include<QPushButton>
#include<QGridLayout>
#include<QMessageBox>

#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <errno.h>

#include"../h_src/mf_info.h"
#include"../h_src/file_opt.h"
#include"clt_common.h"
#include"../h_src/pack.h"

#include"widget.h"
#include"mf_client.h"


#define SIZE 2048 //消息正文的大小

class mf_register_widget : public QWidget
{
    Q_OBJECT
public:
    explicit mf_register_widget(MF_Glb *t_va, QWidget *parent = 0);
    
    bool isRegisterSuccess();

    void getDevInfoList();
    QLabel * m_idLabel;
    QLabel * m_nameLabel;
    QLabel * m_pwdLabel;
    QLabel * m_confirePwdLabel;
    QLabel * m_mailBoxLabel;


    QLineEdit * m_idEdit;
    QLineEdit * m_nameEdit;
    QLineEdit * m_pwdEdit;
    QLineEdit * m_confirePwdEdit;
    QLineEdit * m_mailBoxEdit;

    QPushButton * m_cancelBtn;
    QPushButton * m_registerBtn;

    QGridLayout * m_gLayout;

    MF_Glb * m_va;

    Widget * m_mainWidget;

signals:
    
public slots:
    void cancelBtn_slot();
    void registerBtn_slot();
    
};

#endif // MF_REGISTER_WIDGET_H
