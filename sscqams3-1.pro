QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

QT += sql
#数据库

QT += charts
#图表

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

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

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/Qxlsx/bin64/ -lQXlsx
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/Qxlsx/bin64/ -lQXlsxd

INCLUDEPATH += $$PWD/Qxlsx/include
DEPENDPATH += $$PWD/Qxlsx/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/Qxlsx/bin64/libQXlsx.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/Qxlsx/bin64/libQXlsxd.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/Qxlsx/bin64/QXlsx.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/Qxlsx/bin64/QXlsxd.lib

RESOURCES += \
    icon.qrc
