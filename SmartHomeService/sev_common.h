#ifndef SEV_COMMON_H
#define SEV_COMMON_H
#include"../h_src/common.h"
#include"../h_src/dev_info.h"
#include"../h_src/mf_info.h"
#include"../h_src/file_opt.h"
#include<list>


class Widget;

class SEV_Glb:public Glb
{
    public:
    int m_skfd;
    int m_clt_skfd;
    file_opt m_mfFile;
    file_opt m_devFile;
    list<MF_Info*> m_mfInfoList;
};

#endif // SEV_COMMON_H
