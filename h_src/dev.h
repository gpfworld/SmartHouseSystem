#ifndef DEV_H
#define DEV_H

#include <QWidget>
#include<QGridLayout>
#include<QHBoxLayout>
#include<QVBoxLayout>

#include<QPushButton>
#include<QLabel>
#include<QSlider>
#include<QPixmap>
#include"dev_info.h"
#include"common.h"
#include"mf_info.h"
#include"pack.h"

#include"../SmartHomeMF/clt_common.h"
#include"../SmartHomeService/sev_common.h"

#define AUTO 1
#define MANUL 2
#define SIZE 2048


/*
自定义的界面显示控件，显示某设备的信息
*/

class Dev : public QWidget
{
    Q_OBJECT
public:
    explicit Dev(DEV_Info * t_devInfo,QWidget *parent = 0);

	/*根据设备的状态刷新界面*/
    void flushDev();

	/**/
    void setGlb(Glb *t_va);

public:
    DEV_Info * m_devInfo; 		//记录该控件显示的设备的信息

    Glb * m_va; 				//全局变量

    QLabel * m_nameLabel;		//显示名字label
    QLabel * m_isOnlineLabel; 	//显示是否在线label

    QGridLayout * m_gLayout; 	
    QGridLayout * m_rLayout;
    QHBoxLayout * m_hLayout;
    QVBoxLayout * m_vLayout;

    QPushButton * m_switchBtn;	//开关btn
    QPushButton * m_modelBtn; 	//模式选择btn

    QLabel * m_iconLabel; 		//设备的图片
    QPixmap * m_iconPixmap;		//设备图片
    QLabel * m_lightLabel; 		//显示‘传感数据’的label
    QLabel * m_lightValueLabel; //显示传感数据值label
    QLabel * m_dataLabel;		//显示要展示的数据的label，根据设备类型不同而不同
    QLabel * m_dataValueLabel;	//显示设备的数据，比如空调的温度，灯的R值

    QLabel * m_sliderLabel;		//显示要控制的数据的名称
    QSlider * m_dataSlider;		//设置要控制的数据，slider，空调温度，灯R值

    char m_model;               

signals:

public slots:

    void switchBtn_slot(); 				//设备开关的槽函数
    void modelBtn_slot();				//模式选择的槽函数
    void dataSlider_slot(int value);	//数据设置的槽函数


};

#endif // DEV_H
