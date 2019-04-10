#include "../h_src/protocal.h"

using namespace std;

/*
*默认构造函数，命令类型-1，命令字-1， 数据长度-1，token长度0，
*所有数组数据都为0.
*/
Protocal::Protocal()
{
    m_headLen=HEAD_LEN;

    this->m_type=INIT_VALUE;
    this->m_typeCon=INIT_VALUE;
    this->m_dataLen=INIT_VALUE;
    memset(this->m_data,0,MAX_DATA_SIZE);
    memset(this->m_deviceID,0,DEV_ID_LEN);
    this->m_tokenLen=0;
    this->m_token=new char[m_tokenLen];
    memset(this->m_token,0,this->m_tokenLen);
}
/**
 *
 */
Protocal::Protocal(Pack &t_pack)
{
    m_headLen=HEAD_LEN;
    this->m_type=t_pack.getMsgType();
    this->m_typeCon=t_pack.getMsg();
    this->m_dataLen=t_pack.getDataLen();
    memcpy(this->m_data,t_pack.getMsgData(),this->m_dataLen);
    memcpy(this->m_deviceID,t_pack.getRevDeviceID(),DEV_ID_LEN);
    this->m_tokenLen=t_pack.getTokenLen();
    this->m_token=new char[m_tokenLen];
    memcpy(this->m_token,t_pack.getToken(),this->m_tokenLen);
}

void Protocal::transcodeE(char * t_pack,int & t_count)
{
    if(0x55==t_pack[t_count])
    {
        t_pack[t_count++]=0x54;
        t_pack[t_count]=0x01;
    }
    else if(0x54==t_pack[t_count])
    {
        t_pack[++t_count]=0x02;
    }
}


void Protocal::transcodeD(char * t_pack,int & t_count)
{
    if((0x54==t_pack[t_count])&&(0x01==t_pack[t_count+1]))
    {
        t_pack[++t_count]=0x55;
    }
    else if((0x54==t_pack[t_count])&&(0x02==t_pack[t_count+1]))
    {
        t_pack[++t_count]=0x54;
    }
}

/*
*装包
*/
void Protocal::encode(char * t_pack,int & t_count)
{

    m_Len=m_headLen+m_dataLen+m_tokenLen;

    t_pack[t_count++]=0x55;
    t_pack[t_count]=m_Len%256;
    transcodeE(t_pack,t_count);
    t_count++;
    t_pack[t_count]=m_Len/256;
    transcodeE(t_pack,t_count);
    t_count++;
    t_pack[t_count]=m_type;
    transcodeE(t_pack,t_count);
    t_count++;
    t_pack[t_count]=m_typeCon;
    transcodeE(t_pack,t_count);
    t_count++;
    t_pack[t_count]=m_typeNo%256;
    transcodeE(t_pack,t_count);
     t_count++;
    t_pack[t_count]=m_typeNo/256;
    transcodeE(t_pack,t_count);
    t_count++;
    t_pack[t_count]=m_isExtend;
    transcodeE(t_pack,t_count);
    t_count++;
    t_pack[t_count]=m_extend%256;
    transcodeE(t_pack,t_count);
    t_count++;
    t_pack[t_count]=m_extend/256;
    transcodeE(t_pack,t_count);
    t_count++;
    t_pack[t_count]=m_status;
    transcodeE(t_pack,t_count);
    t_count++;
    int t_i=0;
    for(;t_i<8;t_i++)
    {
        t_pack[t_count]=m_deviceID[t_i];
        transcodeE(t_pack,t_count);
        t_count++;
    }
    t_pack[t_count]=m_tokenLen;
    transcodeE(t_pack,t_count);
    t_count++;
    for(;t_i<m_tokenLen;t_i++)
    {
        t_pack[t_count]=m_token[t_i];
        transcodeE(t_pack,t_count);
        t_count++;
    }
    for(t_i=0;t_i<m_dataLen;t_i++)
    {
        t_pack[t_count]=m_data[t_i];
        transcodeE(t_pack,t_count);
        t_count++;
    }
    t_pack[t_count]=m_isHasCrc;
    transcodeE(t_pack,t_count);
    t_count++;
    t_pack[t_count]=m_crc%256;
    transcodeE(t_pack,t_count);
    t_count++;
    t_pack[t_count]=m_crc/256;
    transcodeE(t_pack,t_count);
    t_count++;
    t_pack[t_count++]=0x55;
}
/**
 *解包
 */
void Protocal::decode(char * t_pack,int & t_count)
{
    while(1)
    {
        while(0x55!=t_pack[t_count])
        {
            t_count++;
        }
        t_count++;

        transcodeD(t_pack,t_count);
        m_Len=t_pack[t_count++];
        transcodeD(t_pack,t_count);
        m_Len=t_pack[t_count++]*256+m_Len;

        m_dataLen=m_Len-m_headLen-m_tokenLen;

        transcodeD(t_pack,t_count);
        m_type=t_pack[t_count++];
        transcodeD(t_pack,t_count);
        m_typeCon=t_pack[t_count++];
        transcodeD(t_pack,t_count);
        m_typeNo=t_pack[t_count++];
        transcodeD(t_pack,t_count);
        m_typeNo=t_pack[t_count++]*256+m_typeNo;
        transcodeD(t_pack,t_count);
        m_isExtend=t_pack[t_count++];
        transcodeD(t_pack,t_count);
        m_extend=t_pack[t_count++];
        transcodeD(t_pack,t_count);
        m_extend=t_pack[t_count++]*256+m_extend;
        transcodeD(t_pack,t_count);
        m_status=t_pack[t_count++];
        int t_i=0;
        for(t_i=0;t_i<8;t_i++)
        {
            transcodeD(t_pack,t_count);
            m_deviceID[t_i]=t_pack[t_count++];
        }
        transcodeD(t_pack,t_count);
        m_tokenLen=t_pack[t_count++];
        for(t_i=0;t_i<m_tokenLen;t_i++)
        {
            transcodeD(t_pack,t_count);
            m_token[t_i]=t_pack[t_count++];
        }


        for(t_i=0;t_i<m_dataLen;t_i++)
        {
            transcodeD(t_pack,t_count);
            m_data[t_i]=t_pack[t_count++];
        }
        transcodeD(t_pack,t_count);
        m_isHasCrc=t_pack[t_count++];
        transcodeD(t_pack,t_count);
        m_crc=t_pack[t_count++];
        transcodeD(t_pack,t_count);
        m_crc=t_pack[t_count++]*256+m_crc;
        //cout<<"tail:"<<t_pack[t_count]<<endl;
        if(0x55==t_pack[t_count++])
            break;
    }
}



void Protocal::setPack(Pack& t_pack)
{
    this->m_type=t_pack.getMsgType();
    this->m_typeCon=t_pack.getMsg();
    this->m_dataLen=t_pack.getDataLen();
    memcpy(this->m_data,t_pack.getMsgData(),this->m_dataLen);
    memcpy(this->m_deviceID,t_pack.getRevDeviceID(),DEV_ID_LEN);
    this->m_tokenLen=t_pack.getTokenLen();
    memcpy(this->m_token,t_pack.getToken(),this->m_tokenLen);
    this->m_status=t_pack.getStatus();
}

Pack Protocal::getPack()
{
    Pack t_pack;
    t_pack.setMsgType(this->m_type);
    t_pack.setMsg(this->m_typeCon);
    t_pack.setMsgData(this->m_data,this->m_dataLen);
    t_pack.setRevDeviceID(this->m_deviceID);
    t_pack.setToken(this->m_token,this->m_tokenLen);
    t_pack.setStatus(this->m_status);
    return t_pack;
}

void Protocal::getCrc()
{
}
