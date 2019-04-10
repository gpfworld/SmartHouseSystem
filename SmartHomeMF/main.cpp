#include "widget.h"
#include <QApplication>
#include<list>

#include"mf_client.h"
#include"../h_src/common.h"
#include"login.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MF_Glb g_va;

    mk_get_msg(0664,'b',g_va.m_msg_skfd);

    login login(&g_va);

    login.show();

    return a.exec();
}
