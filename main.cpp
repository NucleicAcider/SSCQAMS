#include "mainwindow.h"

#include <QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //////////////////////////////////
    //设置中文字体
    a.setFont(QFont("Microsoft Yahei", 10));
    //////////////////////////////////////////////
    QTextCodec *codec = QTextCodec::codecForName("utf-8");
    QTextCodec::setCodecForLocale(codec);
    MainWindow w;
    w.showMaximized();
    return a.exec();
}
