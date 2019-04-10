#ifndef CLT_COMMON_H
#define CLT_COMMON_H

#include"../h_src/dev_info.h"
#include"../h_src/mf_info.h"
#include"../h_src/dev_info.h"

class Widget;


/*全局变量类型的定义*/

class MF_Glb:public Glb
{
public:
    DEV_Info * m_devInfo;
};

#endif // CLT_COMMON_H
