#ifndef CONMON_H
#define CONMON_H
#include<iostream>
using namespace std;
#include<list>

#include"mf_info.h"
#include"dev_info.h"


#define DEV     0x00        //设备类型      单片机
#define MF      0x01        //设备类型      手机
#define DEV_LIGHT   0x03    //设备     	灯
#define DEV_CURTAIN 0x04    //设备   	窗帘
#define DEV_AIRCONDITIONER  0x05 //设备 	空调
#define DEV_WATERHEATER     0x06 //设备 	热水器
#define DEV_SWITCH 0x0E 		//设备 		开关
#define DEV_FAN 0x0F 			//设备 		电扇

#define SENSOR_DATA 0x07     //传感数据
#define COMMON_DATA 0x08 	 //命令数据

#define DEV_ONLINE 0x09      //设备在线
#define DEV_OFFLINE 0x0B 		//设备不在线

#define MSG_UI_TO_BACK   0x0C 		//消息队列的类型，qt界面发送到后台服务程序
#define MSG_BACK_TO_UI   0x0D		//消息队列的类型，后台服务程序发送到qt


#define LINK_OPERATE  0xA0 			//链路检测

#define MF_REGISTER     0xA1    	//手机注册
#define MF_LOGIN        0xA2    	//手机登录
#define DEV_LOGIN       0xA3        //设备登录
#define DEV_REGISTER    0xA4    	//设备注册
#define ACK_LOGIN_SUCCESS       0xA5 	//登录成功
#define ACK_LOGIN_FAIL  0xA6 			//登录失败
#define MF_LOGOUT       0xA7 			//手机退出
#define DEV_LOGOUT      0xA8			//设备退出
#define ACK_SUCCESS  0xA9 				//回复包，命令执行成功
#define ACK_FAIL     0xA10 				//回复包，命令执行失败


#define DATA_TRANSFER   0xB0 			//数据传输

#define GET_DEVS_INFO   0xB2    //获取所有的设备列表
#define GET_DEV_DATA    0xB3    //获取传感数据
#define GET_DEV_STATUS    0xB4  //获取设备状态
#define SET_DEV_DATA        0xB5 	//设置设备状态的情况，比如空调温度
#define DEV_ON  0xB6    			//设备开
#define DEV_OFF 0xB7     			//设备关
#define DEV_SET 0xB8 				//设备设置

#define MAX_DATA_SIZE   1400 		//协议数据区的最大数据长度

#define SCKT_DATA_LEN   1600 		//对数据封包以后数据的最大长度

struct msg_buf 						//消息队列的结构体定义
{
    long m_msgType;
    char m_msgData[SCKT_DATA_LEN];
};

class Widget;
class DEV_Info;

class Glb                          //全局变量类型的组装
{
public:
    Widget * m_widget;
    list<DEV_Info*> m_devInfoList;
    MF_Info * m_mfInfo;
    int m_msg_skfd;
};
 
/*错误输出函数
*返回值：无返回值
*file_nam:出错的文件
*line：出错的行数
*fun_nam：出错的函数
*：err_no:出错的原因
*/
void err_fun(const char *file_nam, const int line,const char *fun_nam, const int err_no);


#endif // CONMON_H
