#-------------------------------------------------
#
# Project created by QtCreator 2016-10-17T22:02:51
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SmartHomeService
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    ../cpp_src/protocal.cpp \
    ../cpp_src/pack.cpp \
    ../cpp_src/mf_info.cpp \
    ../cpp_src/file_opt.cpp \
    ../cpp_src/dev_info.cpp \
    ../cpp_src/dev.cpp \
    server.cpp

HEADERS  += widget.h \
    ../h_src/protocal.h \
    ../h_src/pack.h \
    ../h_src/mf_info.h \
    ../h_src/file_opt.h \
    ../h_src/dev_info.h \
    ../h_src/dev.h \
    ../h_src/common.h \
    server.h \
    sev_common.h

RESOURCES += \
    SmartHomeService.qrc
