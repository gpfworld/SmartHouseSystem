#ifndef PROTOCAL_H
#define PROTOCAL_H

#include"pack.h"
#include<cstring>
#include"common.h"
#include<iostream>
using namespace std;

#define HEAD_LEN    24
#define INIT_VALUE  -1

class Protocal
{
public:
	/*默认构造函数*/
    Protocal();
	
	/*重载构造函数
	*t_pack:Pack类的实例
	*/
    Protocal(Pack & t_pack);
    
	/*编码时转义
	*t_pack：被转义的字符数组
	*t_count：被转义在字符的位置
	*/
	inline void transcodeE(char * t_pack,int & t_count);
    
	/*解码时转义
	*t_pack：被转义的字符数组
	*t_count：被转义在字符的位置
	*/
	inline void transcodeD(char * t_pack,int & t_count);
    
	/*封包
	*t_pack:存储封装以后的数据的数组
	*t_count：从t_count开始的位置存放
	*/
	void encode(char * t_pack,int & t_count);
	
	/*解包
	*t_pack:存储被解包数据的数组
	*t_count：从t_count开始的位置解包
	*/
    void decode(char * t_pack,int & t_count);
    
	/*设备传输的的数据包数据
	*t_pack:设置的数据包
	*/
	void setPack(Pack & t_pack);
    
	/*获取解包以后的数据包*/
	Pack getPack();
	
	/*生产crc校验码*/
    void getCrc();
private:
    short m_headLen;            //包头的长度
    short m_Len;                //包的长度
    unsigned char m_type;       //命令类别
    unsigned char m_typeCon;    //命令字
    short m_typeNo;             //命令序号
    unsigned char m_isExtend;   //是否有扩展位
    short m_extend;             //扩展信息
    unsigned char m_status;     //状态
    unsigned char m_deviceID[DEV_ID_LEN];   //设备ID
    unsigned char m_tokenLen;   //token的长度
    char * m_token;
    char m_data[MAX_DATA_SIZE];          //数据
    short m_dataLen;            //数据的长度
    unsigned char m_isHasCrc;   //是否有CRC校验
    short m_crc;                //校验数据
};

#endif // PROTOCAL_H
