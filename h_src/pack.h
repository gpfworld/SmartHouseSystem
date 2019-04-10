#ifndef PACK_H
#define PACK_H

#include<cstring>
#include"common.h"
#include"dev_info.h"
#include"mf_info.h"
#include<list>
class Pack
{
public:
	/*默认构造函数*/
    Pack();
	
	/*等号重载*/
    Pack & operator=(const Pack & t_pack);
    
	/*复制构造函数*/
	Pack(Pack & t_pack);

	/*设置消息类型，链路检测，数据传输*/
    void setMsgType(unsigned char t_msgType);
    
	/*获取消息类型，链路检测，数据传输*/
	const unsigned char getMsgType() const ;
    
	/*设置具体的消息类型，如登录、注册*/
	void setMsg(unsigned char t_msg);
	
	/*获取具体的消息类型，如登录，注册*/
    const unsigned char getMsg() const;
	
	/*设置具体要传输的数据*/
    void setMsgData(char * t_data, short t_dataLen);
	
	/*获取具体要传输的数据*/
    const char * getMsgData() const;

	/*获取数据区数据的长度*/
    const int getDataLen() const;
	
	/*设置接收消息的设备的ID*/
    void setRevDeviceID(unsigned char *t_deviceID);
	
	/*获取接收消息的设备的ID*/
    const unsigned char * getRevDeviceID() const;

	/*设置请求设备的token*/
    void setToken(char * t_token,short t_tokenLen);
	
	/*获取请求设备的token*/
    const char * getToken() const;
	
	/*获取tokne的长度*/
    const char getTokenLen() const;

	/*获取状态*/
    const unsigned char getStatus() const;
	
	/*设置状态*/
    void setStatus(unsigned char t_status);
	
	/*字符串的拆分函数*/
    void spitString(char * t_src,char * t_subStr,int & t_start);

	
	
	/*功能的设置一般是成对，函数结尾是E是封包，函数结尾是D是解包*/

    //链路操作
    void mfRegisterE(MF_Info & t_mfInfo);           //手机注册
    void mfRegisterD(MF_Info & t_mfInfo);
    void devRegisterE(DEV_Info & t_devInfo);        //设备注册
    void devRegisterD(DEV_Info & t_devInfo);
    
	
	/*手机和设备登录*/
    void loginE(const char * t_devName,const char * t_devPwd,char t_devType);      //登录装包
    void loginD(char *t_devName, char * t_devPwd);      //登录解包
    
	//登录以后的发送的回复的包，t_token 口令，t_msg指明消息类型，登录成功或者失败
    void ackLoginE(const char* t_token,char t_msg);                                    
    void ackLoginD(char* t_token);
     
	 //手机和设备退出
    void logoutE(const char * t_devName,char t_devType);      //退出装包
    void logoutD(char *t_devName);      //退出解包

	//心跳检测
    void beatE();      
    void beatD();
	
	/*函数开头是req都是请求命令，开头是ack的函数，是对命令的回复包*/
	
    //数据传输

	/*设备向平台获取设备列表*/
    void ackGetDevsFromPlatE(list<DEV_Info*> & t_devInfoList);    
    void ackGetDevsFromPlatD(list<DEV_Info*> & t_devInfoList);

	/*手机向中控服务器的请求包*/
    void reqFromMfToPlatE(unsigned char t_msg);         
    void reqFromMfToPlatD();

    /*回复通过平台回复指定类型的设备传感数据*/
    void ackGetDataFromDevsByPlatE(char * t_src,char * t_dest,unsigned char t_msg,float t_data);    
    void ackGetDataFromDevsByPlatD(char * t_src,char * t_dest,float & t_data);

	/*回复通过平台回复指定类型的设备传感数据*/
    void ackLightStatusByPlatE(char * t_mac,char * t_id,unsigned char t_light_status,char t_light_r);       
    void ackLightStatusByPlatD(char * t_mac,char * t_id,unsigned char & t_light_status,char & t_light_r);

	/*手机到设备的请求命令*/
    void reqFromMfToDevByPlatE(char * t_id,char * t_mac,unsigned char t_msg);   //from mf to dev ctl 
    void reqFromMfToDevByPlatD(char * t_id,char * t_mac);
	
	/*回复的确认的包，参数指明成功还是失败*/
    void ackE(unsigned char t_msg);
    void ackD();
	
	/*回复的确认的包，参数为接收者，还有命令执行成功还是失败*/
    void ackE(char * t_dest,unsigned char t_msg);
    void ackD(char * t_dest);
    
	/*请求设置设备具体状态的包*/
    void reqSetDevDataByPlatE(char * t_id,char * t_mac,char  t_data);
    void reqSetDevDataByPlatD(char * t_id,char * t_mac,char & t_data);


public:
    unsigned char m_msgType;    //命令类型
    unsigned char m_msg;        //命令字
    char m_msgData[MAX_DATA_SIZE];       //命令数据
    int m_dataLen;              //命令数据长度
    unsigned char m_revDeviceID[DEV_ID_LEN];//接受命令设备ID
    char * m_token;             //token
    char  m_tokenLen;           //token长度
    unsigned char m_status;     //状态
};






#endif // PACK_H
