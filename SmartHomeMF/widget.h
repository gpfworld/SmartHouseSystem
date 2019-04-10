#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<QGridLayout>
#include<QPushButton>
#include<QMap>
#include<QString>

#include"../h_src/dev.h"
#include"../h_src/dev_info.h"
//#include"mf_client.h"



class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(MF_Glb * t_va,QWidget *parent = 0);
    ~Widget();

    void clearWidget(QLayout *layout);
    void flushWidget();

public:
    QGridLayout * m_gLayout;

    QMap<QString,Dev *> m_devCntlMap;

    MF_Glb * m_va;

   // DEV_Info * m_devInfo;
   // Dev * m_dev;
   // Dev * m_dev1;
    //QPushButton * m_openBtn;
};

#endif // WIDGET_H
