#include "about.h"
#include "ui_about.h"

About::About(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::About)
{
    ui->setupUi(this);
}

About::~About()
{
    delete ui;
}

void About::on_pushButton_clicked()
{
    QFile   aFile("Update Record.txt");
    if (!aFile.exists()) //文件不存在
        return;
    if (!aFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    QString updateRecord=aFile.readAll();
    QMessageBox::information(this,"更新日志",updateRecord);
    aFile.close();
    return;
}


void About::on_pushButton_2_clicked()
{
    system("start GNUSoftwareProtocol.html");
}


void About::on_pushButton_3_clicked()
{
    QMessageBox::information(this,"联系作者","微信/QQ：2740900346\n邮箱：williamblackf@qq.com");
}

