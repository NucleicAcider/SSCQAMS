#include "fullcommand.h"
#include "ui_fullcommand.h"

FullCommand::FullCommand(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FullCommand)
{
    ui->setupUi(this);
    openDatabase();
    iniUI();

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), SLOT(time_update()));
    timer->start(1000); //1s执行一次
    QString StrWidth,StrHeigth;
    QString filename="icon.png";
    QImage* img=new QImage,* scaledimg=new QImage;//分别保存原图和缩放之后的图片
    if(! ( img->load(filename) ) ) //加载图像
    {
        QMessageBox::information(this,"打开图像失败","打开图像失败!");
        delete img;
        return;
    }
    int Owidth=img->width(),Oheight=img->height();
    int Fwidth,Fheight;       //缩放后的图片大小
    ui->label->setGeometry(0,0,50,50);
    int Mul;            //记录图片与label大小的比例，用于缩放图片
    int size=168;
    if(Owidth/size>=Oheight/size)
        Mul=Owidth/size;
    else
        Mul=Oheight/size;
    Fwidth=Owidth/Mul;
    Fheight=Oheight/Mul;
    *scaledimg=img->scaled(Fwidth,Fheight,Qt::KeepAspectRatio);
    qDebug()<<QString("width: ")+StrWidth.setNum(Fwidth)<<QString("\nheight: ")+StrHeigth.setNum(Fheight);
    ui->label->setPixmap(QPixmap::fromImage(*scaledimg));
}

void FullCommand::time_update()
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

FullCommand::~FullCommand()
{
    delete ui;
}

void FullCommand::openDatabase()
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
    query.exec("select * from student");
    query.last();
    int userNum=query.value(0).toInt();
    qDebug()<<query.lastError()<<query.value(0).toInt();
    if(query.first())
    {
        qDebug()<<query.lastError()<<query.value(1);
        for (int a=0;a<userNum;a++)
        {
            studlist[a].id=query.value(0).toInt();
            studlist[a].numb=query.value(1).toInt();
            studlist[a].name=query.value(2).toString();
            studlist[a].mark=query.value(3).toInt();
            studlist[a].birth=query.value(4).toInt();
            qDebug()<<studlist[a].id<<studlist[a].numb<<studlist[a].name<<studlist[a].mark<<studlist[a].birth;
            query.next();
            Stotal++;
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
    //query.clear();
    query.finish();
}

void FullCommand::iniUI()
{
    ui->tableWidget->setAlternatingRowColors(true);
    QTableWidgetItem    *headerItem;
    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setRowCount(Stotal);
    QStringList headerText;
    headerText<<"学 号"<<"姓 名"<<"分 数";  //表头标题用QStringList来表示/
    ui->tableWidget->setColumnCount(headerText.count());//列数设置为与 headerText的行数相等
    for (int i=0;i<3;i++)//列编号从0开始
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
        for (int i=0;i<3;i++)
        {
            ui->tableWidget->item(index,i)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        }
    }
    ui->tableWidget->verticalHeader()->setHidden(true);
    ui->tableWidget->show();
}

void FullCommand::on_tableWidget_itemDoubleClicked(QTableWidgetItem *item)
{
    TimeOut=0;
    sendData.numb=ui->tableWidget->item(item->row(),0)->text().toInt();
    sendData.name=ui->tableWidget->item(item->row(),1)->text();
    sendData.mark=ui->tableWidget->item(item->row(),2)->text().toInt();
    sendData.birth=studlist[item->row()].birth;
    query.prepare("select * from record where name=:name");
    query.bindValue(":name",sendData.name);//
    qDebug()<<"record:"<<query.lastError();
    if(query.exec())//213
    {
        qryModel=new QSqlQueryModel;
        qryModel->setQuery(query);
        query.first();
        sendData.record="";
        for (int a=0;a<qryModel->rowCount();a++)
        {
            qDebug()<<"record:"<<query.lastError();
            sendData.record+=query.value(2).toString()+"<br/>"+query.value(1).toString()+"<br/>"+query.value(3).toString()+"<br/><br/>";
            qDebug()<<"record:"<<sendData.record;
            query.next();
        }
        query.finish();
    }
    else
    {
        qDebug()<<"cannot read record"<<query.lastError();
    }
    qDebug()<<sendData.numb<<sendData.name<<sendData.mark<<sendData.birth;
    RecordEdit *record=new RecordEdit(this);
    int ret=record->exec();// 以模态方式显示对话框
    if (ret==QDialog::Accepted)
    {
        getData.name=record->returnname();
        getData.mark=record->returnmark();
        getData.birth=record->returnbirth();
        getData.record=record->returnrecord();
        if(getData.mark!=sendData.mark||getData.birth!=sendData.birth||getData.name!=sendData.name)
        {
            if(getData.mark!=sendData.mark)
            {
                query.prepare("insert into record ( name, addmark, datetimeweek, reason) values ( :name, :addMark, :datetimeweek, :reason)");
                //query.bindValue(":id", qryModel->rowCount()+1);id,:id,
                query.bindValue(":name", getData.name);
                query.bindValue(":addMark", getData.mark-sendData.mark);
                query.bindValue(":datetimeweek", datetime+" "+week);
                query.bindValue(":reason", "老师加分 "+getData.record);
                if(!query.exec())
                {
                    QMessageBox::warning(this,"警告","无法保存record数据");
                }
                query.finish();
            }
            query.prepare("update student set name=:name, mark=:mark, birth=:birth where numb=:numb");
            query.bindValue(":name", getData.name);
            query.bindValue(":mark", getData.mark);
            query.bindValue(":birth", getData.birth);
            query.bindValue(":numb", sendData.numb);
            if(!query.exec())
            {
                QMessageBox::warning(this,"警告","无法保存student数据");
            }
            studlist[sendData.numb-1].name=getData.name;
            studlist[sendData.numb-1].mark=getData.mark;
            studlist[sendData.numb-1].birth=getData.birth;
            iniUI();
        }
    }
    delete record;
}


void FullCommand::on_commandLinkButton_input_score_clicked()
{
    TimeOut=0;
    QMessageBox::information(this,"提示","功能维护中，敬请期待！");
}

QString FullCommand::encrypt(const QString &str)
{ //字符串MD5算法加密
    QByteArray btArray;
    btArray.append(str);//加入原始字符串
    QCryptographicHash hash(QCryptographicHash::Md5);  //Md5加密算法
    hash.addData(btArray);  //添加数据到加密哈希值
    QByteArray resultArray =hash.result();  //返回最终的哈希值
    QString md5 =resultArray.toHex();//转换为16进制字符串
    return  md5;
}


void FullCommand::on_commandLinkButton_change_pswd_clicked()
{
    TimeOut=0;
    QString organization="CQAMS";//用于注册表，
    QString appName="Password"; //HKEY_CURRENT_USER/Software/

    QSettings   setting(organization,appName);//创建
    pswd=setting.value("PSWD").toString();//读取PSWD 键的值，
    qDebug()<<pswd<<comd;

    QLineEdit::EchoMode echoMode=QLineEdit::Password;
    bool ok=false;
    QString PSWD=QInputDialog::getText(this,"请输入原密码","输入密码:",echoMode,"",&ok,Qt::WindowFlags(Qt::WindowStaysOnTopHint));
    if(encrypt(PSWD)==pswd)
    {
        PSWD=QInputDialog::getText(this,"请输入新密码","输入密码:",echoMode,"",&ok,Qt::WindowFlags(Qt::WindowStaysOnTopHint));
        pswd=QInputDialog::getText(this,"请重复新密码","输入密码:",echoMode,"",&ok,Qt::WindowFlags(Qt::WindowStaysOnTopHint));
        if(pswd==PSWD)
        {
            QSettings settingP("CQAMS","Password"); //注册表键组
            settingP.setValue("PSWD",encrypt(PSWD));
            QMessageBox::information(this,"提示","修改成功！请牢记新密码");
        }
        else
        {
            QMessageBox::warning(this,"警告","两次输入不同！");
        }
    }
    else
    {
        QMessageBox::warning(this,"警告","密码错误！");
    }
}


void FullCommand::on_commandLinkButton_change_comd_clicked()
{
    TimeOut=0;
    QString organization="CQAMS";//用于注册表，
    QString appName="Command"; //HKEY_CURRENT_USER/Software/

    QSettings   setting(organization,appName);//创建
    comd=setting.value("Command").toString();//读取PSWD 键的值，
    QLineEdit::EchoMode echoMode=QLineEdit::Password;
    bool ok=false;
    QString Comd=QInputDialog::getText(this,"请输入原口令","输入口令:",echoMode,"",&ok,Qt::WindowFlags(Qt::WindowStaysOnTopHint));
    if(encrypt(Comd)==comd)
    {
        Comd=QInputDialog::getText(this,"请输入新口令","输入口令:",echoMode,"",&ok,Qt::WindowFlags(Qt::WindowStaysOnTopHint));
        comd=QInputDialog::getText(this,"请重复新口令","输入口令:",echoMode,"",&ok,Qt::WindowFlags(Qt::WindowStaysOnTopHint));
        if(comd==Comd)
        {
            QSettings settingP("CQAMS","Command"); //注册表键组
            settingP.setValue("Command",encrypt(Comd));
            QMessageBox::information(this,"提示","修改成功！请牢记新口令");
        }
        else
        {
            QMessageBox::warning(this,"警告","两次输入不同！");
        }
    }
    else
    {
        QMessageBox::warning(this,"警告","口令错误！");
    }
}


void FullCommand::on_commandLinkButton_reset_all_clicked()
{
    TimeOut=0;
    QMessageBox::StandardButton  defaultBtn=QMessageBox::Cancel; //缺省按钮
    QMessageBox::StandardButton result;//返回选择的按钮
    result=QMessageBox::question(this, "警告", "此行为会删除原有数据库文件，是否继续？",QMessageBox::Yes|QMessageBox::Cancel,defaultBtn);
    qDebug()<<result;
    if(result==QMessageBox::Yes)
    {
        removeDB();
        accept();
    }
    return;
}

bool FullCommand::removeDB()
{
    return true;
}

void FullCommand::on_commandLinkButton_output_main_info_clicked()
{
    TimeOut=0;
    QString FilePath=QFileDialog::getSaveFileName(this,"保存为表格文件", date+"分数", "表格文件(*.xls *.xlsx);;所有文件(*.*)");
     if(FilePath.isEmpty()||FilePath=="")
     {
         //QMessageBox::about(this, "提示", "未找到excel文件。\nERROR ID:00001");
         return;
     }
     QFile createFile("path");
     if(!createFile.open(QIODevice::WriteOnly|QIODevice::Text))
     {
         QMessageBox::warning(this, "错误", "数据库导出文件创建失败\nERROR ID:00002", QMessageBox::Ok, QMessageBox::NoButton);
         qDebug()<<"文件打开失败";
     }
     createFile.write(FilePath.toStdString().c_str());
     createFile.close();
     if(!QProcess::startDetached("save_excel.exe",QStringList()))
     {
         //QMessageBox::warning(this,"提示","无法打开配置文件\nERROR ID:0000");
         qDebug()<<"cannot open save excel.py";
         system("save_excel.exe");
     }
     Sleep(3000);
}


void FullCommand::on_commandLinkButton_output_all_info_clicked()
{
    TimeOut=0;
    QString FilePath=QFileDialog::getSaveFileName(this,"保存为表格文件", date+"全部", "表格文件(*.xls *.xlsx);;所有文件(*.*)");
     if(FilePath.isEmpty()||FilePath=="")
     {
         //QMessageBox::about(this, "提示", "未找到excel文件。\nERROR ID:00001");
         return;
     }
     QFile createFile("path");
     if(!createFile.open(QIODevice::WriteOnly|QIODevice::Text))
     {
         QMessageBox::warning(this, "错误", "数据库导出文件创建失败\nERROR ID:00002", QMessageBox::Ok, QMessageBox::NoButton);
         qDebug()<<"文件打开失败";
     }
     createFile.write(FilePath.toStdString().c_str());
     createFile.close();
     if(!QProcess::startDetached("save_excel_all.exe",QStringList()))
     {
         //QMessageBox::warning(this,"提示","无法打开配置文件\nERROR ID:0000");
         qDebug()<<"cannot open save excel.py";
         system("save_excel_all.exe");
     }
     Sleep(3000);
}


