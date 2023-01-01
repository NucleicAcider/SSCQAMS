#include "check.h"
#include "ui_check.h"
#include <QRegExpValidator>
#include <mainwindow.h>

Check::Check(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Check)
{
    ui->setupUi(this);
    QRegExp regx("[1-9][0-9][0-9]");
    QValidator *validator = new QRegExpValidator(regx, ui->lineEdit );
    ui->lineEdit->setValidator(validator);
    openDatabase();
}

Check::~Check()
{
    delete ui;
}

void Check::openDatabase()
{
    if (QSqlDatabase::contains("qt_sql_default_connection"))
    {
        db = QSqlDatabase::database("qt_sql_default_connection");
    }
    else
    {
        db = QSqlDatabase::addDatabase("QSQLITE");
    }
    db.setDatabaseName("info.db");
    //query=QSqlQuery::QSqlQuery(db);
    if (!db.open())   //打开数据库
    {
        QMessageBox::warning(this, "错误", "打开数据库失败\nERROR ID:00003", QMessageBox::Ok, QMessageBox::NoButton);
        qDebug() << "Database Error!";
        return;
    }
    query=QSqlQuery(db);
}

void Check::on_lineEdit_textEdited(const QString &arg1)
{
    query.prepare("select * from student where numb=:numb");
    query.bindValue(":numb",arg1);
    if(query.exec())
    {
        query.first();
        studentinfo.id=query.value(0).toInt();
        studentinfo.numb=query.value(1).toInt();
        studentinfo.name=query.value(2).toString();
        studentinfo.mark=query.value(3).toInt();
        studentinfo.birth=query.value(4).toInt();
        qDebug()<<studentinfo.id<<studentinfo.numb<<studentinfo.name<<studentinfo.mark<<studentinfo.birth;
    }
    else
    {
        qDebug()<<"cannot read record"<<query.lastError();
    }
    query.finish();
    query.prepare("select * from record where name=:name");
    query.bindValue(":name",studentinfo.name);
    if(query.exec())
    {
        qryModel=new QSqlQueryModel;
        qryModel->setQuery(query);
        query.first();
        studentinfo.record="";
        for (int a=0;a<qryModel->rowCount();a++)
        {
            qDebug()<<"record:"<<query.lastError();
            studentinfo.record+=query.value(2).toString()+"<br/>"+query.value(1).toString()+"<br/>"+query.value(3).toString()+"<br/><br/>";
            qDebug()<<"record:"<<studentinfo.record;
            query.next();
        }
        query.finish();
    }
    ui->lineEdit_name->setText(studentinfo.name);
    ui->lineEdit_mark->setText(QString::number(studentinfo.mark));
    QString birth=QString::number(studentinfo.birth/10000)+"年"+QString::number((studentinfo.birth/100)%100)+"月"+QString::number(studentinfo.birth%100)+"日";
    ui->lineEdit_birth->setText(birth);
    ui->textEdit->setText("<html><head/><body><p align=\"left\"><span style=\" font-size:12pt; font-weight:700;\">"+studentinfo.record+"</span></p></body></html>");
    query.clear();
}

void Check::on_lineEdit_editingFinished()
{
    on_lineEdit_textEdited(ui->lineEdit->text());
}

