TEMPLATE = app
CONFIG += console
CONFIG -= qt

SOURCES += main.c \
    common/cstringUtil.c \
    common/curlCmdUtil.c

HEADERS += \
    common/cstringUtil.h \
    common/curlCmdUtil.h

