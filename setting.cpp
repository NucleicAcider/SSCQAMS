#include "setting.h"
#include "ui_setting.h"

Setting::Setting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Setting)
{
    ui->setupUi(this);
}

int Setting::h1()
{
    return ui->timeEdit_1->time().hour();
}

int Setting::h2()
{
    return ui->timeEdit_2->time().hour();
}

int Setting::h3()
{
    return ui->timeEdit_3->time().hour();
}

int Setting::h4()
{
    return ui->timeEdit_4->time().hour();
}

int Setting::m1()
{
    return ui->timeEdit_1->time().minute();
}

int Setting::m2()
{
    return ui->timeEdit_2->time().minute();
}

int Setting::m3()
{
    return ui->timeEdit_3->time().minute();
}

int Setting::m4()
{
    return ui->timeEdit_4->time().minute();
}

int Setting::addm1()
{
    return ui->spinBox_1->text().toInt();
}

int Setting::addm2()
{
    return ui->spinBox_2->text().toInt();
}

int Setting::addm3()
{
    return ui->spinBox_3->text().toInt();
}

int Setting::addm4()
{
    return ui->spinBox_4->text().toInt();
}

Setting::~Setting()
{
    delete ui;
}
