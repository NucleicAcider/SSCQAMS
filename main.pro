QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


QT += gui
QT+=sql

RC_ICONS = icon.ico

SOURCES += \
    about.cpp \
    check.cpp \
    draw.cpp \
    fullcommand.cpp \
    main.cpp \
    mainwindow.cpp \
    recordedit.cpp \
    setting.cpp \
    signin.cpp \
    updatemark.cpp

HEADERS += \
    about.h \
    check.h \
    draw.h \
    fullcommand.h \
    mainwindow.h \
    recordedit.h \
    setting.h \
    signin.h \
    updatemark.h

FORMS += \
    about.ui \
    check.ui \
    draw.ui \
    fullcommand.ui \
    mainwindow.ui \
    recordedit.ui \
    setting.ui \
    signin.ui \
    updatemark.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

#程序版本
VERSION = 3.0.2
#公司名称
QMAKE_TARGET_COMPANY ="丽水学院附属高级中学"
#程序说明
QMAKE_TARGET_DESCRIPTION = "丽水学院附属高级中学学生综合素质考核管理系统"
#版权信息
QMAKE_TARGET_COPYRIGHT = "Copyright(C) 2022 傅博韬\n本程序为自由软件，在自由软件联盟发布的GNU通用公共许可协议的约束下，你可以对其进行再发布及修改。协议版本为第三版或（随你）更新的版本。\n我们希望发布的这款程序有用，但不保证，甚至不保证它有经济价值和适合特定用途。详情参见GNU通用公共许可协议。\n你理当已收到一份GNU通用公共许可协议的副本，如果没有，请查阅<http://www.gnu.org/licenses/>"
#程序名称
QMAKE_TARGET_PRODUCT = "丽水学院附属高级中学学生综合素质考核管理系统"

#程序语言
#0x0800代表和系统当前语言一致
RC_LANG = 0x0800
