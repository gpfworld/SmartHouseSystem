#include "widget.h"

Widget::Widget(SEV_Glb *t_va, Widget *parent)
    : QWidget(parent)
{
    m_va=t_va;
    m_va->m_widget=this;

    m_gLayout=new QGridLayout(this);
    Dev * t_dev;
    list<DEV_Info*>::iterator t_it;
    int num=0;
    for(t_it=m_va->m_devInfoList.begin();t_it!=m_va->m_devInfoList.end();t_it++)
    {
        t_dev=new Dev(*t_it);
        t_dev->setGlb(m_va);
        QString t_macStr((*t_it)->m_mac);
        m_devCntlMap[t_macStr]=t_dev;
        m_gLayout->addWidget(t_dev,num/3,num%3);
        num++;
    }
    this->resize(600,400);
}

Widget::~Widget()
{

}
