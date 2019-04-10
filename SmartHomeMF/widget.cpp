#include "widget.h"
#include<typeinfo>

Widget::Widget(MF_Glb * t_va,QWidget *parent)
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

void Widget::clearWidget(QLayout *layout)
{
      QLayoutItem *item = NULL;
      int itemCount = layout->layout()->count();

      /* 整个递归返回的条件是，当整个布局中的部件删除完毕，
       * for循环借结束后，会自动返回上一级 */
      for(int i=itemCount-1; i>=0; i--)
      {
          item = layout->layout()->takeAt(i);
          QLayout *lay = item->layout();
          QWidget *widget = item->widget();
          /* 如果部件是layout的hua，递归删除layout中的所有部件 */
          if((typeid(QLayout *)==typeid(lay)) && (lay!=NULL))
          {
              clearWidget(item->layout());
          }
          /* 部件不是layout的话，直接删除 */
          else if((typeid(QWidget *)==typeid(widget) && (widget!=NULL)))
          {
              delete item->widget(); //如果是子控件不是QWidget，这里会出错
          }
      }
}

void Widget::flushWidget()
{
        clearWidget(this->layout());

        delete this->layout();

        m_gLayout=new QGridLayout(this);
        m_devCntlMap.clear();

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
}
