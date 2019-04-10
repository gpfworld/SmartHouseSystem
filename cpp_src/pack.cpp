#include "../h_src/pack.h"
#include<stdio.h>
#include<string>
#include<list>
#define INIT_VALUE -1

Pack::Pack()
{
    memset(this->m_msgData,0,MAX_DATA_SIZE);
    memset(this->m_revDeviceID,0,DEV_ID_LEN);
    this->m_msgType=INIT_VALUE;
    this->m_msg=INIT_VALUE;
    this->m_dataLen=INIT_VALUE;
    this->m_status=INIT_VALUE;
    this->m_tokenLen=0;
    this->m_token=new char(this->m_tokenLen);
    memset(this->m_token,0,this->m_tokenLen);
}
Pack::Pack(Pack& t_pack)
{
    memcpy(this->m_msgData,t_pack.getMsgData(),MAX_DATA_SIZE);
    memcpy(this->m_revDeviceID,t_pack.getRevDeviceID(),DEV_ID_LEN);
    this->m_msgType=t_pack.getMsgType();
    this->m_msg=t_pack.getMsg();
    this->m_dataLen=t_pack.getDataLen();
    this->m_status=t_pack.getStatus();
    this->m_tokenLen=t_pack.getTokenLen();
    this->m_token=new char(this->m_tokenLen);
    memcpy(this->m_token,t_pack.getToken(),this->m_tokenLen);
}
Pack & Pack::operator = (const Pack & t_pack)
{
    if(this==&t_pack)
        return *this;
    memcpy(this->m_msgData,t_pack.getMsgData(),MAX_DATA_SIZE);
    memcpy(this->m_revDeviceID,t_pack.getRevDeviceID(),DEV_ID_LEN);
    this->m_msgType=t_pack.getMsgType();
    this->m_msg=t_pack.getMsg();
    this->m_dataLen=t_pack.getDataLen();
    this->m_status=t_pack.getStatus();
    this->m_tokenLen=t_pack.getTokenLen();
    this->m_token=new char(this->m_tokenLen);
    memcpy(this->m_token,t_pack.getToken(),this->m_tokenLen);

    return *this;
}
void Pack::setMsgType(unsigned char t_msgType)
{
    this->m_msgType=t_msgType;
}

const unsigned char Pack::getMsgType() const
{
    return this->m_msgType;
}

void Pack::setMsg(unsigned char t_msg)
{
    this->m_msg=t_msg;
}

const unsigned char Pack::getMsg() const
{
    return this->m_msg;
}

void Pack::setMsgData(char *t_data,short t_dataLen)
{
    memcpy(this->m_msgData,t_data,t_dataLen);
    this->m_dataLen=t_dataLen;
}

const char * Pack::getMsgData() const
{
    return this->m_msgData;
}

const int Pack::getDataLen() const
{
    return this->m_dataLen;
}

void Pack::setRevDeviceID(unsigned char * t_deviceID)
{
    memcpy(this->m_revDeviceID,t_deviceID,8);
}

const unsigned char * Pack::getRevDeviceID() const
{
    return this->m_revDeviceID;
}

void Pack::setToken(char *t_token, short t_tokenLen)
{
    this->m_token=new char(t_tokenLen);
    memcpy(this->m_token,t_token,t_tokenLen);
    this->m_tokenLen=t_tokenLen;
}

const char *Pack::getToken() const
{
    return this->m_token;
}

const char Pack::getTokenLen() const
{
    return this->m_tokenLen;
}

const unsigned char Pack::getStatus() const
{
    return this->m_status;
}

void Pack::setStatus(unsigned char t_status)
{
    this->m_status=t_status;
}


/**
 *字符串分割函数，t_src源字符串，t_subStr存放取出的参数，t_start记录开始的下标
 */
void Pack::spitString(char * t_src,char * t_subStr,int & t_start)
{
    int t_s=t_start;
    int t_len=strlen(t_src);
    for(;t_start<t_len;t_start++)
    {
        if(':'==t_src[t_start])
            break;
    }
    if(':'==t_src[t_start])
        memcpy(t_subStr,t_src+t_s,t_start-t_s);
    else
        t_subStr=NULL;
}

void Pack::mfRegisterE(MF_Info & t_mfInfo)
{
    this->m_msgType=LINK_OPERATE;
    this->m_msg=MF_REGISTER;

    this->m_tokenLen=0;
    int t_len=0;
    t_len = sprintf(this->m_msgData,"%s:",t_mfInfo.m_id);
    t_len += sprintf( this->m_msgData + t_len,"%s:", t_mfInfo.m_pwd);
    t_len += sprintf( this->m_msgData + t_len, "%s:",t_mfInfo.m_name);
    t_len += sprintf( this->m_msgData + t_len, "%s:", t_mfInfo.m_mailBox);

    this->m_dataLen=t_len;
    //test
    memset(this->m_revDeviceID,0,DEV_ID_LEN);
}

void Pack::mfRegisterD(MF_Info &t_mfInfo)
{
    int t_start=0;
    spitString(this->m_msgData,t_mfInfo.m_id,t_start);
    t_start++;
    spitString(this->m_msgData,t_mfInfo.m_pwd,t_start);
    t_start++;
    spitString(this->m_msgData,t_mfInfo.m_name,t_start);
    t_start++;
    spitString(this->m_msgData,t_mfInfo.m_mailBox,t_start);
}

void Pack::devRegisterE(DEV_Info & t_devInfo)
{
    this->m_msgType=LINK_OPERATE;
    this->m_msg=DEV_REGISTER;

    this->m_tokenLen=0;
    int t_len=0;
    t_len = sprintf(this->m_msgData,"%s:",t_devInfo.m_mac);
    t_len += sprintf( this->m_msgData + t_len,"%s:",t_devInfo.m_pwd);
    t_len += sprintf( this->m_msgData + t_len, "%s:", t_devInfo.m_name);
    t_len += sprintf( this->m_msgData + t_len, "%s:", t_devInfo.m_ip);
    char t_ch[3];
    t_ch[0]=t_devInfo.m_port%256;
    t_ch[1]=t_devInfo.m_port/256;
    t_ch[2]='\0';
    t_len += sprintf( this->m_msgData + t_len, "%s:", t_ch);
    this->m_msgData[t_len++]=t_devInfo.m_devType;
    this->m_msgData[t_len++]=':';
    this->m_dataLen=t_len;

    memset(this->m_revDeviceID,0,DEV_ID_LEN);
}

void Pack::devRegisterD(DEV_Info& t_devInfo)
{
    int t_start=0;
    spitString(this->m_msgData,t_devInfo.m_mac,t_start);
    t_start++;
    spitString(this->m_msgData,t_devInfo.m_pwd,t_start);
    t_start++;
    spitString(this->m_msgData,t_devInfo.m_name,t_start);
    t_start++;
    spitString(this->m_msgData,t_devInfo.m_ip,t_start);
    t_start++;
    char t_ch[3];
    spitString(this->m_msgData,t_ch,t_start);
    t_devInfo.m_port=t_ch[0]+t_ch[1]*256;
    t_start++;
    t_devInfo.m_devType=this->m_msgData[t_start];
}

void Pack::loginE(const char * t_devName, const char * t_devPwd, char t_devType)
{
    this->m_msgType=LINK_OPERATE;
    this->m_msg=t_devType;

    this->m_tokenLen=0;
    memset(this->m_revDeviceID,0,DEV_ID_LEN);

    int t_len=0;
    t_len = sprintf(this->m_msgData,"%s:",t_devName);
    t_len += sprintf( this->m_msgData + t_len,"%s:", t_devPwd);

    this->m_dataLen=t_len;
}

void Pack::loginD(char * t_devName, char * t_devPwd)
{
    int t_start=0;
    spitString(this->m_msgData,t_devName,t_start);
    t_start++;
    spitString(this->m_msgData,t_devPwd,t_start);
}
void Pack::ackLoginE(const char *t_token,char t_msg)
{
    this->m_msgType=LINK_OPERATE;
    this->m_msg=t_msg;

    this->m_tokenLen=0;
    memset(this->m_revDeviceID,0,DEV_ID_LEN);
    int t_len=0;
    t_len = sprintf(this->m_msgData,"%s:",t_token);
    this->m_dataLen=t_len;
}
void Pack::ackLoginD(char *t_token)
{
    t_token=new char(this->getDataLen());
    if(ACK_LOGIN_SUCCESS==this->m_msg)
    {
        int t_start=0;
        spitString(this->m_msgData,t_token,t_start);
    }
}
void Pack::logoutE(const char * t_devName, char t_devType)
{
    this->m_msgType=LINK_OPERATE;
    this->m_msg=t_devType;

    this->m_tokenLen=0;
    memset(this->m_revDeviceID,0,DEV_ID_LEN);

    int t_len=0;
    t_len = sprintf(this->m_msgData,"%s:",t_devName);

    this->m_dataLen=t_len;
}

void Pack::logoutD(char * t_devName)
{
    int t_start=0;
    spitString(this->m_msgData,t_devName,t_start);
}
void Pack::beatE()
{
    this->m_msgType=0xA0;
    this->m_msg=0x01;
    this->m_dataLen=0;
    memset(this->m_revDeviceID,0,8);
}

void Pack::beatD()
{

}

void Pack::reqDevToDevE()
{
    this->m_msgType=0xA0;
    this->m_msg=0x55;
    this->m_dataLen=8;
    memset(this->m_revDeviceID,0,8);
}

void Pack::reqDevToDevD()
{
}
void Pack::reqFromMfToPlatE(unsigned char t_msg)
{
    this->m_msgType=DATA_TRANSFER;
    this->m_msg=t_msg;
    this->m_dataLen=0;
}
void Pack::reqFromMfToPlatD()
{
    
}
void Pack::ackGetDevsFromPlatE(list<DEV_Info*> & t_devInfoList)
{
    cout<<"ackGetDevsFromPlatE dev num"<<t_devInfoList.size()<<endl;

    this->m_msgType=DATA_TRANSFER;
    this->m_msg=GET_DEVS_INFO;
    list<DEV_Info*>::iterator t_it;
    int t_len=0;
    for(t_it=t_devInfoList.begin();t_it!=t_devInfoList.end();t_it++)
    {
        t_len+=sprintf(this->m_msgData+t_len,"%s:",(*t_it)->m_mac);
        t_len+=sprintf(this->m_msgData+t_len,"%s:",(*t_it)->m_name);
        this->m_msgData[t_len++]=(*t_it)->m_devType;
        this->m_msgData[t_len++]=':';
        this->m_msgData[t_len++]=(*t_it)->m_isOnline;
        this->m_msgData[t_len++]=':';
    }
    this->m_dataLen=t_len;
}

void Pack::ackGetDevsFromPlatD( list<DEV_Info*> & t_devInfoList)
{

    t_devInfoList.clear();
    int t_start=0;
    DEV_Info * t_devInfo=new DEV_Info;
    spitString(this->m_msgData,t_devInfo->m_mac,t_start);
    t_start++;
    spitString(this->m_msgData,t_devInfo->m_name,t_start);
    t_start++;
    t_devInfo->m_devType=this->m_msgData[t_start];
    t_start++;
    t_start++;
    t_devInfo->m_isOnline=this->m_msgData[t_start];
    t_start++;
    t_start++;
    cout<<"pack mac ackGetDevsFromPlatD"<<t_devInfo->m_mac<<endl;
    cout<<"pack mac len  ackGetDevsFromPlatD"<<strlen(t_devInfo->m_mac)<<endl;
    if(0!=strlen(t_devInfo->m_mac))
        t_devInfoList.push_back(t_devInfo);
    while(0!=this->m_msgData[t_start])
    {
         t_devInfo=new DEV_Info;
         spitString(this->m_msgData,t_devInfo->m_mac,t_start);
         t_start++;
         spitString(this->m_msgData,t_devInfo->m_name,t_start);
         t_start++;
         t_devInfo->m_devType=this->m_msgData[t_start];
         t_start++;
         t_start++;
         t_devInfo->m_isOnline=this->m_msgData[t_start];
         t_start++;
         t_start++;
         if(0!=strlen(t_devInfo->m_mac))
             t_devInfoList.push_back(t_devInfo);
    }

    cout<<"ackGetDevsFromPlatD dev num"<<t_devInfoList.size()<<endl;
}
void Pack::reqFromMfToDevByPlatE(char * t_src,char * t_dest,unsigned char t_msg)
{
    this->m_msgType=DATA_TRANSFER;
    this->m_msg=t_msg;
    int t_len=0;
        
    t_len+=sprintf(this->m_msgData,"%s:",t_src);
    t_len+=sprintf(this->m_msgData+t_len,"%s:",t_dest);
    this->m_dataLen=t_len;
}
void Pack::reqFromMfToDevByPlatD(char * t_src,char * t_dest)
{
    int t_start=0;
    spitString(this->m_msgData,t_src,t_start);
    t_start++;
    spitString(this->m_msgData,t_dest,t_start);
}
void Pack::ackGetDataFromDevsByPlatE(char * t_src,char * t_dest,unsigned char t_msg,float t_data)
{
    this->m_msgType=DATA_TRANSFER;
    this->m_msg=t_msg;
    int t_len=0;
        
    t_len+=sprintf(this->m_msgData+t_len,"%s:",t_src);
    t_len+=sprintf(this->m_msgData+t_len,"%s:",t_dest);

    memcpy(this->m_msgData+t_len,&t_data,sizeof(t_data));

    t_len+=sprintf(this->m_msgData+t_len,":");
    this->m_dataLen=t_len+sizeof(t_data);
}
void Pack::ackGetDataFromDevsByPlatD(char* t_src,char* t_dest,float& t_data)
{
    int t_start=0;
    spitString(this->m_msgData,t_src,t_start);
    t_start++;
    spitString(this->m_msgData,t_dest,t_start);
    t_start++;
    memcpy(&t_data,this->m_msgData+t_start,sizeof(t_data));
}
void Pack::ackLightStatusByPlatE(char * t_mac,char * t_id,unsigned char t_light_status,char t_light_r)
{
    this->m_msgType=DATA_TRANSFER;
    this->m_msg=GET_DEV_STATUS;
    int t_len=0;
        
    t_len+=sprintf(this->m_msgData+t_len,"%s:",t_mac);
    t_len+=sprintf(this->m_msgData+t_len,"%s:",t_id);

    this->m_msgData[t_len++]=t_light_status;
    this->m_msgData[t_len++]=':';
    this->m_msgData[t_len++]=t_light_r;
    this->m_msgData[t_len]=':';

    this->m_dataLen=t_len;
}
void Pack::ackLightStatusByPlatD(char* t_mac,char* t_id,unsigned char & t_light_status,char & t_light_r)
{
    int t_start=0;
    spitString(this->m_msgData,t_mac,t_start);
    t_start++;
    spitString(this->m_msgData,t_id,t_start);
    t_start++;
    t_light_status=this->m_msgData[t_start];
    t_start++;
    t_start++;
    t_light_r=this->m_msgData[t_start];
}
void Pack::ackE(unsigned char t_msg)
{
    this->m_msgType=DATA_TRANSFER;
    this->m_msg=t_msg;
    this->m_dataLen=0; 
}
void Pack::ackD()
{
    
}
void Pack::ackE(char * t_dest,unsigned char t_msg)
{
    this->m_msgType=DATA_TRANSFER;
    this->m_msg=t_msg;
    int t_len=0;

    t_len+=sprintf(this->m_msgData+t_len,"%s:",t_dest);
    
    this->m_dataLen=t_len; 
    
}
void Pack::ackD(char * t_dest)
{
    int t_start=0;
    spitString(this->m_msgData,t_dest,t_start);
    
}
void Pack::reqSetDevDataByPlatE(char * t_id,char * t_mac,char t_data)
{
    this->m_msgType=DATA_TRANSFER;
    this->m_msg=DEV_SET;
    int t_len=0;
        
    t_len+=sprintf(this->m_msgData+t_len,"%s:",t_id);
    t_len+=sprintf(this->m_msgData+t_len,"%s:",t_mac);

    this->m_msgData[t_len++]=t_data;
    this->m_msgData[t_len++]=':';

    this->m_dataLen=t_len;
}
void Pack::reqSetDevDataByPlatD(char* t_id,char* t_mac,char & t_data)
{
    int t_start=0;
    spitString(this->m_msgData,t_id,t_start);
    t_start++;
    spitString(this->m_msgData,t_mac,t_start);
    t_start++;
    t_data=this->m_msgData[t_start];
}
