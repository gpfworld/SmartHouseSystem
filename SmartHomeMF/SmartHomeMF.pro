#-------------------------------------------------
#
# Project created by QtCreator 2016-10-17T21:53:08
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SmartHomeMF
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    ../cpp_src/protocal.cpp \
    ../cpp_src/pack.cpp \
    ../cpp_src/mf_info.cpp \
    ../cpp_src/file_opt.cpp \
    ../cpp_src/dev_info.cpp \
    ../cpp_src/dev.cpp \
    mf_client.cpp \
    login.cpp \
    dev_register_widget.cpp \
    mf_register_widget.cpp

HEADERS  += widget.h \
    ../h_src/protocal.h \
    ../h_src/pack.h \
    ../h_src/mf_info.h \
    ../h_src/file_opt.h \
    ../h_src/dev_info.h \
    ../h_src/dev.h \
    ../h_src/common.h \
    mf_client.h \
    clt_common.h \
    login.h \
    dev_register_widget.h \
    mf_register_widget.h

RESOURCES += \
    SmartHomeMF.qrc
