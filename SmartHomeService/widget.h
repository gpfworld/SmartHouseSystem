#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<QGridLayout>
#include<QPushButton>
#include<QMap>
#include<QString>

#include"../h_src/dev.h"
#include"sev_common.h"

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(SEV_Glb * t_va,Widget *parent = 0);
    ~Widget();


public:
    QGridLayout * m_gLayout;


    QMap<QString,Dev *> m_devCntlMap;

    SEV_Glb * m_va;

};

#endif // WIDGET_H
