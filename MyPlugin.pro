#-------------------------------------------------
#
# Project created by QtCreator 2016-09-27T20:57:24
#
#-------------------------------------------------

QT       += core gui widgets network

TARGET = Weather
TEMPLATE = lib
CONFIG += plugin c++11

<<<<<<< HEAD
SOURCES +=  myplugin.cpp\
            pluginwidget.cpp

HEADERS +=  myplugin.h\
            pluginwidget.h

=======
SOURCES += myplugin.cpp

HEADERS += myplugin.h
>>>>>>> ee69e7f2e5856481aa5f131a0220dbce2cb7f995
DISTFILES += MyPlugin.json

INCLUDEPATH += ../dde-dock/

unix {
    target.path = /usr/lib/dde-dock/plugins/
    INSTALLS += target
}
<<<<<<< HEAD

RESOURCES += \
    filename.qrc
=======
>>>>>>> ee69e7f2e5856481aa5f131a0220dbce2cb7f995
