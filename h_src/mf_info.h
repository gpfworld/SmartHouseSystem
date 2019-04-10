#ifndef MF_INFO_H
#define MF_INFO_H
using namespace std;
#include<iostream>


#define ID_MAX_LEN 12
#define PWD_MAX_LEN 12
#define NAME_MAX_LEN 20
#define MAILBOX_MAX_LEN 20

class MF_Info
{
    public:
        char m_id [ID_MAX_LEN] ;			//用户账号（手机号）
        char m_pwd [PWD_MAX_LEN];			//用户登录密码
        char m_name[NAME_MAX_LEN];			//用户名称
        char m_mailBox[MAILBOX_MAX_LEN];	//用户邮箱
        char * m_token;						//用户口令
        char m_tokenLen;					//用户口令长度
        int m_skfd;							//用户socket文件描述符
    public:
		/*默认构造函数*/
        MF_Info();
		
		/*重载构造函数
		*t_id:用户账号（手机号）
		*t_pwd：用户登录密码
		*t_name：用户名称
		*t_mailBox：用户邮箱
		*/
        MF_Info(const char * t_id,const char* t_pwd,const char * t_name,const char *t_mailBox);
		
		/*复制构造函数*/
        MF_Info(MF_Info & t_mfInfo);
        
		/*等号重载*/
		MF_Info & operator=(MF_Info & t_mfInfo);
};


#endif // PACK_H
