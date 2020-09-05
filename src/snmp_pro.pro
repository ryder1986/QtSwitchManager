#-------------------------------------------------
#
# Project created by QtCreator 2018-09-01T20:03:53
#
#-------------------------------------------------

QT       += core gui
QMAKE_CXXFLAGS+=-DSTDCXX_98_HEADERS
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = snmp_pro
TEMPLATE = app


SOURCES += main.cpp \
    common/Widget.cpp \
    DevInfoFrame/DevInfoFrame.cpp \
    IpFrame/IpFrame.cpp \
    PortFrame/BasicGraph.cpp \
    PortFrame/PortFrame.cpp \
    TopologyFrame/TopologyFrame.cpp \
    WarnFrame/WarnFrame.cpp \
    MainWidget.cpp \
    snmp/Snmp_lsm.cpp \
    TopologyFrame/NetworkGraph.cpp \
    TopologyFrame/portconfig.cpp

HEADERS  += snmp/Snmp_lsm.h \
    common/Widget.h \
    DevInfoFrame/DevInfoFrame.h \
    IpFrame/IpFrame.h \
    PortFrame/BasicGraph.h \
    PortFrame/PortFrame.h \
    TopologyFrame/TopologyFrame.h \
    WarnFrame/WarnFrame.h \
    MainWidget.h \
    common/basicinfo.h \
    TopologyFrame/NetworkGraph.h \
    TopologyFrame/portconfig.h
INCLUDEPATH += common TopologyFrame PortFrame IpFrame DevInfoFrame WarnFrame snmp
QT += network
#LIBS += -lsnmp++
include(snmp/snmp_pp/snmp_pp.pri)
win32{
LIBS+=-lws2_32
}

unix:{
	DEFINES+=HAVE_SYS_SOCKET_H HAVE_SYS_TIME_H HAVE_LOCALTIME_R
}
PICS.path=$$OUT_PWD/
PICS.files=pic
CONFIGS.path=$$OUT_PWD/
CONFIGS.files=../bin/config.ini
INSTALLS+=PICS CONFIGS
