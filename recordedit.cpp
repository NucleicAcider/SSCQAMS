#include "recordedit.h"
#include "ui_recordedit.h"

RecordEdit::RecordEdit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RecordEdit)
{
    ui->setupUi(this);
    FullCommand * getNumb = (FullCommand*) parentWidget();
    getData.numb=getNumb->sendData.numb;
    getData.birth=getNumb->sendData.birth;
    getData.mark=getNumb->sendData.mark;
    getData.name=getNumb->sendData.name;
    getData.record=getNumb->sendData.record;
    //delete  getNumb;
    ui->lineEdit_numb->setText(QString::number(getData.numb));
    ui->lineEdit_name->setText(getData.name);
    ui->spinBox_mark->setValue(getData.mark);
    ui->spinBox_year->setValue(getData.birth/10000);
    ui->spinBox_month->setValue((getData.birth/100)%100);
    ui->spinBox_day->setValue(getData.birth%100);
    ui->textEdit->setHtml("<html><head/><body><p align=\"left\"><span style=\" font-size:12pt; font-weight:700;\">"+getData.record+"</span></p></body></html>");
    setWindowTitle(getData.name+"的个人信息");

    qDebug()<<getData.numb<<getData.name<<getData.mark<<getData.birth<<getData.record;
}

RecordEdit::~RecordEdit()
{
    delete ui;
}

void RecordEdit::on_commandLinkButton_score_clicked()
{
    QMessageBox::information(this,"提示","功能维护中，敬请期待！");
}

int RecordEdit::returnmark()
{
    return ui->spinBox_mark->value();
}

QString RecordEdit::returnname()
{
    return ui->lineEdit_name->text();
}

int RecordEdit::returnbirth()
{
    QString birth=ui->spinBox_year->text();
    if(ui->spinBox_month->value()<10)
    {
        birth+="0"+ui->spinBox_month->text();
    }
    else
    {
        birth+=ui->spinBox_month->text();
    }
    if(ui->spinBox_day->value()<10)
    {
        birth+="0"+ui->spinBox_day->text();
    }
    else
    {
        birth+=ui->spinBox_day->text();
    }
    return birth.toInt();
}

QString RecordEdit::returnrecord()
{
    return ui->lineEdit->text();
}

void RecordEdit::on_spinBox_mark_valueChanged(int arg1)
{
    ui->lineEdit->setReadOnly(false);
}

