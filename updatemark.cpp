#include "updatemark.h"
#include "ui_updatemark.h"

UpdateMark::UpdateMark(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UpdateMark)
{
    ui->setupUi(this);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), SLOT(time_update()));
    timer->start(1000); //1s执行一次

    reason<<"讲卫生"<<"讲文明"<<"守纪律"<<"重安全"<<"其他";
    ui->comboBox->addItems(reason);
}

UpdateMark::~UpdateMark()
{
    delete ui;
}

void UpdateMark::openDatabase()
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


void UpdateMark::on_comboBox_currentTextChanged(const QString &arg1)
{
    TimeOut=0;
    db.setDatabaseName("rules.db");
    QStringList place,civilization,clean,obey,safety;
    civilization<<"个人文明"<<"教室文明"<<"食堂文明"<<"寝室文明";
    clean<<"教室卫生"<<"寝室卫生";
    obey<<"公共纪律"<<"两操纪律"<<"寝室纪律"<<"违禁物品"<<"学习纪律";
    safety<<"防疫安全"<<"交通安全"<<"寝室安全"<<"食品安全"<<"用电安全"<<"重大安全";

    if(arg1=="讲卫生")
    {
        ui->comboBox_2->clear();
        ui->comboBox_2->addItems(clean);
        ui->comboBox_2->addItem("其他");
    }
    if(arg1=="讲文明")
    {
        ui->comboBox_2->clear();
        ui->comboBox_2->addItems(civilization);
        ui->comboBox_2->addItem("其他");
        /*
        db.setDatabaseName("rules.db");
        query.exec("select * from civilization");
        qryModel=new QSqlQueryModel(this);
        query.first();
        for(int a=0;a<qryModel->rowCount();a++)
        {
           //query.value(0)
        }
        */
    }
    if(arg1=="守纪律")
    {
        ui->comboBox_2->clear();
        ui->comboBox_2->addItems(obey);
        ui->comboBox_2->addItem("其他");
    }
    if(arg1=="重安全")
    {
        ui->comboBox_2->clear();
        ui->comboBox_2->addItems(safety);
        ui->comboBox_2->addItem("其他");
    }
    if(arg1=="其他")
    {
        ui->comboBox_2->clear();
        ui->comboBox_2->addItem("其他");
    }
}

void UpdateMark::time_update()
{
    QDateTime::currentDateTime();
    time = QDateTime::currentDateTime();
    datetime = time.toString("yyyy年MM月dd日 hh:mm:ss");
    Time=time.toString("hh:mm:ss");
    date=time.toString("yyyy-MM-dd");
    week=time.toString("ddd");
    TimeOut++;
    if(TimeOut==300)
    {
        QMessageBox::warning(this,"警告","过长时间未操作");
        accept();
    }
}

void UpdateMark::on_pushButton_accept_clicked()
{
    TimeOut=0;
    if(ui->spinBox_mark->value()==0)
    {
        return;
    }
    query.prepare("update student set mark=:mark where numb=:numb");
    query.bindValue(":mark",studentinfo.mark+ui->spinBox_mark->value());
    query.bindValue(":numb",ui->spinBox->value());
    if(query.exec())
    {
        query.prepare("insert into record ( name, addmark, datetimeweek, reason) values ( :name, :addMark, :datetimeweek, :reason)");
        //query.bindValue(":id", qryModel->rowCount()+1);id,:id,
        query.bindValue(":name", ui->lineEdit_name->text());
        query.bindValue(":addMark", ui->spinBox_mark->value());
        query.bindValue(":datetimeweek", datetime+" "+week);
        query.bindValue(":reason", "班干部："+ui->comboBox->currentText()+" "+ui->comboBox_2->currentText()+" "+ui->textEdit->toPlainText());
        if(!query.exec())
        {
            QMessageBox::warning(this,"警告","无法保存record数据");
        }
        query.finish();

    }
    else
    {
        qDebug()<<"student info save error"<<query.lastError();

    }
    QMessageBox::information(this,"提示","编辑成功");
    ui->textEdit->setText("");
    ui->spinBox->setValue(0);
    ui->spinBox_mark->setValue(0);
}


void UpdateMark::on_comboBox_2_activated(int index)
{
    TimeOut=0;
}


void UpdateMark::on_spinBox_valueChanged(int arg1)
{
    TimeOut=0;
    query.prepare("select * from student where numb=:numb");
    query.bindValue(":numb",arg1);
    if(query.exec())
    {
        query.first();
        studentinfo.id=query.value(0).toInt();
        studentinfo.numb=query.value(1).toInt();
        studentinfo.name=query.value(2).toString();
        studentinfo.mark=query.value(3).toInt();
        qDebug()<<studentinfo.id<<studentinfo.numb<<studentinfo.name<<studentinfo.mark;
    }
    else
    {
        qDebug()<<"cannot read record"<<query.lastError();
    }
    query.finish();
    ui->lineEdit_name->setText(studentinfo.name);
    ui->spinBox_2->setValue(studentinfo.mark);
    //ui->spinBox_mark->setValue(studentinfo.mark);
    query.clear();
}

