#include "signin.h"
#include "ui_signin.h"

QXLSX_USE_NAMESPACE            // 添加Xlsx命名空间

Signin::Signin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Signin)
{
    ui->setupUi(this);
    openDatabase();
    iniUI();

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), SLOT(time_update()));
    timer->start(1000); //1s执行一次
}

void Signin::time_update() //显示系统时间的功能
{
    time = QDateTime::currentDateTime();
    datetime = time.toString("yyyy年MM月dd日 hh:mm:ss");
    Time=time.toString("hh:mm:ss");
    date=time.toString("yyyy-MM-dd");
    week=time.toString("ddd");
    ui->label->setText("<html><head/><body><p align=\"center\"><span style=\" font-size:12pt; font-weight:600;\">"+datetime+"</span></p></body></html>");

    int h=time.toString("h").toInt();
    int m=time.toString("m").toInt();
    if(h<earlyAddTime[0].h)
    {
        addMark=earlyAddTime[0].addMark;
    }
    else
    {
        if((h==earlyAddTime[0].h&&m<=earlyAddTime[0].m))
        {
            addMark=earlyAddTime[0].addMark;
        }
        else
        {
            if(h==earlyAddTime[1].h&&m<=earlyAddTime[1].m)
            {
                addMark=earlyAddTime[1].addMark;
            }
            else
            {
                if(h==earlyAddTime[2].h&&m<=earlyAddTime[2].m)
                {
                    addMark=earlyAddTime[2].addMark;
                }
                else
                {
                    if(h==earlyAddTime[3].h&&m<=earlyAddTime[3].m)
                    {
                        addMark=earlyAddTime[3].addMark;
                    }
                    else
                    {
                        addMark=0;
                    }
                }
            }
        }
    }
    qDebug()<<h<<":"<<m<<addMark;
    ui->label_2->setText("<html><head/><body><p align=\"center\"><span style=\" font-size:12pt; font-weight:600;\">现在可加</span><span style=\" font-size:12pt; font-weight:600; color:#aa0000;\">"+QString::number(addMark)+"分</span></p></body></html>");
}


void Signin::openDatabase()
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
    query.exec("SELECT COUNT(*) AS rowid FROM student");
    query.first();
    int userNum=query.value(0).toInt();
    Stotal=userNum;
    qDebug()<<"userNum:"<<userNum;
    query.exec("select * from student");
    query.last();
    qDebug()<<query.lastError();
    if(query.first())
    {
        qDebug()<<query.lastError()<<query.value(1);
        for (int a=0;a<userNum;a++)
        {
            studlist[a].id=query.value(0).toInt();
            studlist[a].numb=query.value(1).toInt();
            studlist[a].name=query.value(2).toString();
            studlist[a].mark=query.value(3).toInt();
            qDebug()<<studlist[a].id<<"\t"<<studlist[a].numb<<"\t"<<studlist[a].name<<"\t"<<studlist[a].mark;
            query.next();
        }
    }
    else
    {
        qDebug()<<"cannot position";
        QMessageBox::warning(this, "错误", "数据库定位失败\nERROR ID:00004", QMessageBox::Ok, QMessageBox::NoButton);
    }
    //query.finish();
    qDebug()<<"student数据读取完成";
    //ui->label->setText(studentlist[0]);
    query.clear();
    if(query.exec("select * from setting"))
    {
        query.first();
        for (int a=0;a<4;a++)
        {
            earlyAddTime[a].h=query.value(0).toInt();
            earlyAddTime[a].m=query.value(1).toInt();
            earlyAddTime[a].addMark=query.value(2).toInt();
            qDebug()<<a<<query.lastError()<<query.value(0).toInt();
            qDebug()<<a<<query.lastError()<<query.value(1).toInt();
            qDebug()<<a<<query.lastError()<<query.value(2).toInt();
            query.next();
        }
    }
    else
    {
        qDebug()<<"cannot position";
        QMessageBox::warning(this, "错误", "数据库定位失败\nERROR ID:00004", QMessageBox::Ok, QMessageBox::NoButton);
    }
    query.finish();
    qDebug()<<"setting数据读取完成";
    if(query.exec("select * from today"))
    {
        query.first();
        time_update();
        qDebug()<<"today读取完成"<<query.lastError()<<query.value(3).toString()<<date;
        QString is_today=query.value(3).toString();
        if(is_today!=date)
        {
            query.exec("drop table today");
            qDebug()<<"today删除完成"<<query.lastError();
            query.finish();
        }
        else
        {
            query.exec("SELECT COUNT(*) AS rowid FROM today");
            query.first();
            int userNum=query.value(0).toInt();
            Stotal=userNum;
            qDebug()<<"userNum:"<<userNum;
            Signed=userNum;
            query.exec("select * from today");
            query.first();
            for(int a=0;a<Signed;a++)
            {
                studlist[query.value(0).toInt()-1].Signed=true;
                qDebug()<<query.value(0)<<studlist[query.value(0).toInt()-1].Signed;
                query.next();
            }
        }
        query.finish();
    }
}

void Signin::iniUI()
{
    //QStringList selectmode;
    //selectmode<<"全部"<<"未签到"<<"已签到";
    //ui->comboBox->addItems(selectmode);
    //ui->tableWidget->horizontalHeader()->setVisible(true);
    ui->tableWidget->setAlternatingRowColors(true);
    QTableWidgetItem    *headerItem;
    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->setRowCount(Stotal);
    QStringList headerText;
    headerText<<"学 号"<<"姓 名"<<"分 数"<<"签到状态";  //表头标题用QStringList来表示/
    ui->tableWidget->setColumnCount(headerText.count());//列数设置为与 headerText的行数相等
    for (int i=0;i<4;i++)//列编号从0开始
    {
       headerItem=new QTableWidgetItem(headerText.at(i)); //新建一个QTableWidgetItem， headerText.at(i)获取headerText的i行字符串
       QFont font=headerItem->font();//获取原有字体设置
       font.setBold(true);//设置为粗体
       font.setPointSize(14);//字体大小
       headerItem->setForeground(Qt::black);//字体颜色
       headerItem->setFont(font);//设置字体
       ui->tableWidget->setHorizontalHeaderItem(i,headerItem); //设置表头单元格的Item
    }
    for (index=0;index<Stotal;index++)
    {
        ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->tableWidget->setItem(index,0,new QTableWidgetItem(QString::number(studlist[index].numb)));
        ui->tableWidget->setItem(index,1,new QTableWidgetItem(studlist[index].name));
        ui->tableWidget->setItem(index,2,new QTableWidgetItem(QString::number(studlist[index].mark)));
        ui->tableWidget->setItem(index,3,new QTableWidgetItem("未签到"));
        QFont font1=ui->tableWidget->item(index,3)->font();
        font1.setBold(true);
        ui->tableWidget->item(index,3)->setFont(QFont(font1));
        ui->tableWidget->item(index,3)->setForeground(Qt::red);
        for (int i=0;i<4;i++)
        {
            ui->tableWidget->item(index,i)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        }
    }
    ui->tableWidget->verticalHeader()->setHidden(true);
    ui->tableWidget->show();    
    for (index=0;index<Stotal;index++)
    {
        if(studlist[index].Signed)
        {
            ui->tableWidget->hideRow(index);
        }
    }
    if(Signed==Stotal)
    {
        QMessageBox::information(this,"提示","已全部签到！");
        QDialog::accept();
    }
}

Signin::~Signin()
{
    delete ui;
}

void Signin::on_pushButton_clicked()
{
    QString FilePath=QFileDialog::getSaveFileName(this,"保存为表格文件", date, "表格文件(*.xlsx *.xls);;所有文件(*.*)");
    if(FilePath.isEmpty()||FilePath=="")
    {
         //QMessageBox::about(this, "提示", "未找到excel文件。\nERROR ID:00001");
         return;
    }

    if (QSqlDatabase::contains("qt_sql_default_connection"))
    {
        db = QSqlDatabase::database("qt_sql_default_connection");
    }
    else
    {
        db = QSqlDatabase::addDatabase("QSQLITE");
    }
    db.setDatabaseName("info.db");
    if (!db.open())   //打开数据库
    {
        QMessageBox::warning(this, "错误", "打开数据库失败\nERROR ID:00003", QMessageBox::Ok, QMessageBox::NoButton);
        qDebug() << "Database Error!";
        return;
    }
    query=QSqlQuery(db);
    query.exec("SELECT COUNT(*) AS rowid FROM today");
    query.first();
    int userNum=query.value(0).toInt();
    qDebug()<<"userNum:"<<userNum;
    query.clear();
    query.exec("select * from today");
    QXlsx::Document document(FilePath);
    document.write(1,1,"学号");
    document.write(1,2,"姓名");
    document.write(1,3,"加分");
    document.write(1,4,"时间");
    if(query.first())
    {
        qDebug()<<query.lastError()<<query.value(1);
        for (int a=0;a<userNum;a++)
        {
            document.write(a+2,1,query.value(0).toInt());
            document.write(a+2,2,query.value(1).toString());
            document.write(a+2,3,query.value(2).toInt());
            document.write(a+2,4,query.value(4).toString());
            query.next();
        }
    }
    document.saveAs(FilePath);
}

void Signin::on_tableWidget_cellDoubleClicked(int row, int column)
{
    //if(ui->tableWidget->item(row,3)->text()=="已签到")
    //    return;
    time_update();
    //ui->label->setText(studentlist[0]);
    query.clear();
    QTableWidgetItem *item1 = new QTableWidgetItem;
    item1 = ui->tableWidget->item(row, 0);
    //ui->tableWidget->setItem(row,3,new QTableWidgetItem("已签到"));

    query.prepare("create table today (id int(10)  primary key, name varchar(20), addmark int(100), date varchar(20), time varchar(20))");
    //query.bindValue(":date",date);
    if(!query.exec())
        qDebug()<<"creat table error"<<query.lastError();
    query.finish();

    query.prepare("insert into today (id, name, addmark, date,time) values (:id, :name, :addMark, :date, :time)");
    //query.bindValue(":date",date);
    query.bindValue(":id",ui->tableWidget->item(row,0)->text());
    query.bindValue(":name", ui->tableWidget->item(row,1)->text());
    query.bindValue(":addMark", QString::number(addMark));
    query.bindValue(":date", date);
    query.bindValue(":time", Time);
    if(!query.exec())
        qDebug()<<"add record error:"<<query.lastError();
    query.finish();

    int update=studlist[ui->tableWidget->item(row,0)->text().toInt()-1].mark+addMark;
    qDebug()<<update<<ui->tableWidget->item(row,0)->text();

    query.prepare("update student set mark = :change where numb = :numb");
    query.bindValue(":change",QString::number(update));
    query.bindValue(":numb",ui->tableWidget->item(row,0)->text().toInt());
    qDebug()<<"change numb:"<<ui->tableWidget->item(row,0)->text().toInt();
    if(!query.exec())
        qDebug()<<"update error"<<query.lastError();
    query.finish();
    query.clear();
    qryModel=new QSqlQueryModel;
    qryModel->setQuery(query);
    //query.exec("create table record ( name varchar(20), addmark int(100), datetimeweek varchar(20), reason varchar(200))");
    query.prepare("insert into record (name, addmark, datetimeweek, reason) values (:name, :addMark, :datetimeweek, :reason)");
    query.bindValue(":name", ui->tableWidget->item(row,1)->text());
    query.bindValue(":addMark", QString::number(addMark));
    query.bindValue(":datetimeweek", datetime+" "+week);
    query.bindValue(":reason", "签到加分");
    if(!query.exec())
    {
        qDebug()<<"cannot save record:"<<query.lastError();
    }

    //移动到最后并隐藏
    moveRow(ui->tableWidget,ui->tableWidget->currentRow(),ui->tableWidget->rowCount());
    ui->tableWidget->hideRow(Stotal-1);
    ui->tableWidget->scrollToTop();
}

void Signin::moveRow(QTableWidget *pTable, int currentRow, int toRow)
{
    if(pTable == NULL)
        return;

    int nRowCount = pTable->rowCount();
    int nColumnCount = pTable->columnCount();

    pTable->setFocus();

    if(currentRow == toRow)
        return;

    if(currentRow < 0 || toRow <0 ||currentRow > nRowCount || toRow > nRowCount)
        return;

    if(toRow < currentRow)
        currentRow++;

    pTable->insertRow(toRow);

    for(int i = 0; i <nColumnCount;i++)
    {
        pTable->setItem(toRow,i,pTable->takeItem(currentRow,i));
    }
    if(currentRow < toRow)
        toRow--;

    pTable->removeRow(currentRow);
    pTable->selectRow(toRow);

}
/*
void Signin::on_comboBox_currentIndexChanged(const QString &arg1)
{
    qDebug()<<arg1;
    bool hide[100]={0};
    if(arg1=="全部")
    {
        for (int i=0;i<=Stotal;i++)
        {
            if(hide[i])
            {
                hide[i]=false;
                ui->tableWidget->setRowHidden(i,hide[i]);
            }

        }
    }
    if(arg1=="已签到")
    {
        for (int i=0;i<=Stotal;i++)
        {
            if(ui->tableWidget->item(i,3)->text().toStdString()!="已签到"&&hide[i]==false)
            {
                hide[i]=true;
                ui->tableWidget->setRowHidden(i,hide[i]);
            }
            else
            {
                if(hide[i])
                {
                    hide[i]=false;
                    ui->tableWidget->setRowHidden(i,hide[i]);
                }
            }
        }
    }
    if(arg1=="未签到")
    {
        for (int i=0;i<=Stotal;i++)
        {
            if(ui->tableWidget->item(i,3)->text()=="已签到"&&hide[i]==false)
            {
                hide[i]=true;
                ui->tableWidget->setRowHidden(i,hide[i]);
            }
            else
            {
                if(hide[i])
                {
                    hide[i]=false;
                    ui->tableWidget->setRowHidden(i,hide[i]);
                }
            }
        }
    }
}*/


void Signin::on_tableWidget_cellClicked(int row, int column)
{

}
