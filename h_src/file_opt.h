#ifndef FILE_OPT_H
#define FILE_OPT_H
#include"mf_info.h"
#include"dev_info.h"
#include<fstream>
#include <semaphore.h>  
#include<string.h>

#define MAX_FILE_NAME_LEN 20
using namespace std;
class file_opt
{
    private:
        int m_count;             	//用于记录当前读者数量
        sem_t m_sem_mutex;          //互斥访问m_count
        sem_t m_sem_rw;             //用户保护读者写者互斥访问文件
        sem_t m_sem_w;              //用于实现写优先
        fstream m_fInOut;			//读写文件流
        char  m_file_name[MAX_FILE_NAME_LEN];	//文件名称
    private:
        void read_sync_lock();  	//读加锁
        void read_sync_unlock();	//读解锁
        void write_sync_lock();		//写加锁
        void write_sync_unlock();	//写解锁

    public:
		/*默认构造函数*/
        file_opt(); 
		
		/*重载构造函数
		*t_file_name:文件名称
		*/
        file_opt(const char * t_file_name);  //open file
		
        /*析构函数*/
		~file_opt();  //close file
		
        /*设置需要操作的文件
		*t_file_name：文件名称
		*/
		void setFile(const char * t_file_name);
		
        /*手机注册函数
		*返回值：注册成功为true，失败为false
		*t_mfInfo：手机的类实例
		*/
		bool is_MF_Registersuccess(MF_Info & t_mfInfo);
		
        /*设备注册函数
		*返回值：注册成功为true，失败为false
		*t_devInfo：设备的类实例
		*/
		bool is_DEV_Registersuccess(DEV_Info & t_devInfo);
		
        /*手机登录函数
		*返回值：登录成功返回为true，失败为false
		*t_mfInfo：手机的类实例
		*/
		bool is_MF_Loginsuccess(MF_Info & t_mfInfo);
		
        /*设备登录函数
		*返回值：登录成功返回为true，失败为false
		*t_devInfo：设备的类实例
		*/
		bool is_DEV_Loginsuccess(DEV_Info & t_devInfo);
        
};
#endif
