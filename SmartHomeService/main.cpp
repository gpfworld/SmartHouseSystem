#include "widget.h"
#include <QApplication>
#include"server.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    int ret=-1;

    srand(unsigned(time(0)));

    SEV_Glb g_va;

    g_va.m_mfFile.setFile(mf_file);
    g_va.m_devFile.setFile(dev_file);

    g_va.m_mfInfo=new MF_Info;

    memcpy(g_va.m_mfInfo->m_id,"96632558",strlen("96632558"));

    memcpy(g_va.m_mfInfo->m_name,"server",strlen("server"));

    pthread_t pth_sev;
    pthread_t pth_msg;
    pthread_t pth_getUpdateData;

    mk_get_msg(0664, 'a',&g_va);


    g_va.m_devFile.init_DEV_InfoList(g_va.m_devInfoList);


    Widget w(&g_va);
    w.show();

    ret=pthread_create(&pth_msg,NULL,pth_msg_fun,(void*)&g_va);
    if(ret<0) err_fun(__FILE__,__LINE__,"pthread ",ret);

    ret=pthread_create(&pth_getUpdateData,NULL,pth_getUpdateData_fun,(void*)&g_va);
    if(ret<0) err_fun(__FILE__,__LINE__,"pthread ",ret);

    return a.exec();

}
