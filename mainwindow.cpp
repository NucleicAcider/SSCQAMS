#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "signin.h"
#include "setting.h"
#include <windows.h>
#include <QSettings>
#include <QByteArray>
#include <QCryptographicHash>
#include <QDesktopWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    openDatabase();
    QDesktopWidget *deskdop = QApplication::desktop();
    move((deskdop->width() - this->width())/2, (deskdop->height() - this->height())/2-50);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), SLOT(time_update()));
    timer->start(100); //0.1s执行一次
}

void MainWindow::time_update()
{
    if(ui->label_1->size().width()!=window_width)
    {
        iniUI();
        QFont font;
        if(ui->label_1->size().width()>window_width)
        {
            fo=26;
        }
        else
        {
            fo=16;
        }
        font.setPointSize(fo);
        ui->label_1->setFont(font);
        window_width=ui->label_1->size().width();
    }
}

void MainWindow::iniUI()
{
    this->setAutoFillBackground(true); // 这句要加上, 否则可能显示不出背景图.
    QPalette palette = this->palette();
    palette.setBrush(QPalette::Window,
            QBrush(QPixmap("school door.png").scaled(// 缩放背景图.
                this->size(),
                Qt::IgnoreAspectRatio,
                Qt::SmoothTransformation)));             // 使用平滑的缩放方式
    this->setPalette(palette);
    window_width=this->size().width();

    /*QString StrWidth,StrHeigth;
    QString filename="school door.png";
    QImage* img=new QImage,* scaledimg=new QImage;//分别保存原图和缩放之后的图片
    if(! ( img->load(filename) ) ) //加载图像
    {
        QMessageBox::information(this,"打开图像失败","打开图像失败!");
        delete img;
        return;
    }
    int Owidth=img->width(),Oheight=img->height();
    int Fwidth,Fheight;       //缩放后的图片大小
    ui->label->setGeometry(0,0,600,400);
    int Mul;            //记录图片与label大小的比例，用于缩放图片
    if(Owidth/600>=Oheight/400)
        Mul=Owidth/600;
    else
        Mul=Oheight/400;
    Fwidth=Owidth/Mul;
    Fheight=Oheight/Mul;
    *scaledimg=img->scaled(Fwidth,Fheight,Qt::KeepAspectRatio);
    qDebug()<<QString("width: ")+StrWidth.setNum(Fwidth)<<QString("\nheight: ")+StrHeigth.setNum(Fheight);
    ui->label->setPixmap(QPixmap::fromImage(*scaledimg));*/
}

void MainWindow::closeEvent(QCloseEvent *event)
{ //窗口关闭时询问是否退出
    QMessageBox::StandardButton result=QMessageBox::question(this, "确认", "是否退出？",QMessageBox::Yes|QMessageBox::No, QMessageBox::No);
    if (result==QMessageBox::Yes)
    {
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

bool MainWindow::checkDB()
{
    QFile file("info.db");
    if(!file.exists())
    {
        QMessageBox::about(this, "提示", "未找到数据库文件，请以下面格式创建excel表格或打开已有表格并重新启动软件\n|学号|姓名|分数|\nERROR ID:00000");
        QString FilePath=QFileDialog::getOpenFileName(this,"选择表格文件", "", "表格文件(*.xls *.xlsx);;所有文件(*.*)");
        if(FilePath.isEmpty()||FilePath=="")
        {
            QMessageBox::about(this, "提示", "未找到excel文件。\nERROR ID:00001");
            exit(EXIT_FAILURE);
        }
        QFile createFile("path");
        if(!createFile.open(QIODevice::WriteOnly|QIODevice::Text))
        {
            QMessageBox::warning(this, "错误", "数据库生成文件创建失败\nERROR ID:00002", QMessageBox::Ok, QMessageBox::NoButton);
            qDebug()<<"文件打开失败";
        }
        createFile.write(FilePath.toStdString().c_str());
        createFile.close();
        if(!QProcess::startDetached("read_excel.exe",QStringList()))
        {
            //QMessageBox::warning(this,"提示","无法打开配置文件\nERROR ID:0000");
            qDebug()<<"cannot open save excel.py";
            system("read_excel.exe");
        }
        Sleep(3000);

        QLineEdit::EchoMode echoMode=QLineEdit::Password;
        bool ok=false,OK=false;
        while(1)
        {
            QString PSWD=QInputDialog::getText(this,"请输入密码","输入密码:",echoMode,"",&ok,Qt::WindowFlags(Qt::WindowStaysOnTopHint));
            if(PSWD=="")
            {
                exit(EXIT_FAILURE);
            }
            QString PSWDchk=QInputDialog::getText(this,"请再次输入密码","确认密码:",echoMode,"",&OK,Qt::WindowFlags(Qt::WindowStaysOnTopHint));

            if(ok&&OK&&PSWD==PSWDchk&&PSWD!="")
            {
                while(1)
                {
                    QSettings settingP("CQAMS","Password"); //注册表键组
                    settingP.setValue("PSWD",encrypt(PSWD));   //密码，经过加密的
                    QMessageBox::information(this,"提示","请牢记密码\n创建的口令不得与密码相同！");
                    QString Command=QInputDialog::getText(this,"请输入口令","输入口令:",echoMode,"",&ok,Qt::WindowFlags(Qt::WindowStaysOnTopHint));
                    QString Commandchk=QInputDialog::getText(this,"请再次输入口令","确认口令:",echoMode,"",&ok,Qt::WindowFlags(Qt::WindowStaysOnTopHint));
                    if(ok&&OK&&Command==Commandchk&&Command!=""&&Command!=PSWD)
                    {
                        QSettings settingC("CQAMS","Command"); //注册表键组
                        settingC.setValue("Command",encrypt(Command));   //密码，经过加密的
                        QMessageBox::information(this,"提示","请牢记口令");
                        break;
                    }
                    else
                    {
                        if(Command==Commandchk&&Command=="")
                        {
                            return false;
                        }
                        if(Command==Commandchk&&Command==PSWD)
                        {
                            QMessageBox::warning(this,"警告","口令不得与密码相同！");
                        }
                        QMessageBox::warning(this,"警告","两次输入不同！");
                        continue;
                    }
                }
                break;
            }
            else
            {
                if(PSWD==PSWDchk&&PSWD=="")
                {
                    return false;
                }
                QMessageBox::warning(this,"警告","两次输入不同！");
                continue;
            }
        }

        if (QSqlDatabase::contains("qt_sql_default_connection"))
        {
            db = QSqlDatabase::database("qt_sql_default_connection");
        }
        else
        {
            db = QSqlDatabase::addDatabase("QSQLITE");
        }
        db.setDatabaseName("./info.db");
        //query=QSqlQuery::QSqlQuery(db);
        if (!db.open())   //打开数据库
        {
            QMessageBox::warning(this, "错误", "打开数据库失败\nERROR ID:00003", QMessageBox::Ok, QMessageBox::NoButton);
            qDebug() << "Database Error!";
            return true;
        }
        query.clear();
        query=QSqlQuery(db);

        Setting *setting=new Setting(this);
        int ret=setting->exec();// 以模态方式显示对话框
        if (ret==QDialog::Accepted)
        {
            earlyAddTime[0].h=setting->h1();
            earlyAddTime[1].h=setting->h2();
            earlyAddTime[2].h=setting->h3();
            earlyAddTime[3].h=setting->h4();
            earlyAddTime[0].m=setting->h1();
            earlyAddTime[1].m=setting->h2();
            earlyAddTime[2].m=setting->h3();
            earlyAddTime[3].m=setting->h4();
            earlyAddTime[0].addMark=setting->addm1();
            earlyAddTime[1].addMark=setting->addm2();
            earlyAddTime[2].addMark=setting->addm3();
            earlyAddTime[3].addMark=setting->addm4();
        }
        if(ret==QDialog::Rejected)
        {
            earlyAddTime[0].h=6;
            earlyAddTime[1].h=6;
            earlyAddTime[2].h=6;
            earlyAddTime[3].h=6;
            earlyAddTime[0].m=0;
            earlyAddTime[1].m=10;
            earlyAddTime[2].m=20;
            earlyAddTime[3].m=30;
            earlyAddTime[0].addMark=3;
            earlyAddTime[1].addMark=2;
            earlyAddTime[2].addMark=1;
            earlyAddTime[3].addMark=0;
        }
        delete setting;

        query.exec("create table setting (hour int(10), minute int(10), addmark int(10))");
        query.finish();
        for (int a=0;a<4;a++)
        {
            query.prepare("insert into setting (hour,minute,addmark)values(:hour,:minute,:addmark)");
            query.bindValue(":hour",earlyAddTime[a].h);
            query.bindValue(":minute",earlyAddTime[a].m);
            query.bindValue(":addmark",earlyAddTime[a].addMark);
        }
        query.finish();
        QMessageBox::information(this,"提示","请重启本软件");
        exit(EXIT_SUCCESS);
    }
    file.close();
    return true;
}

QString MainWindow::encrypt(const QString &str)
{ //字符串MD5算法加密
    QByteArray btArray;
    btArray.append(str);//加入原始字符串
    QCryptographicHash hash(QCryptographicHash::Md5);  //Md5加密算法
    hash.addData(btArray);  //添加数据到加密哈希值
    QByteArray resultArray =hash.result();  //返回最终的哈希值
    QString md5 =resultArray.toHex();//转换为16进制字符串
    return  md5;
}
void MainWindow::openDatabase()
{
    bool flag=checkDB();
    if (QSqlDatabase::contains("qt_sql_default_connection"))
    {
        db = QSqlDatabase::database("qt_sql_default_connection");
    }
    else
    {
        db = QSqlDatabase::addDatabase("QSQLITE");
    }
    db.setDatabaseName("./info.db");
    //query=QSqlQuery::QSqlQuery(db);
    if (!db.open())   //打开数据库
    {
        QMessageBox::warning(this, "错误", "打开数据库失败\nERROR ID:00003", QMessageBox::Ok, QMessageBox::NoButton);
        qDebug() << "Database Error!";
        return;
    }
    query.clear();
    query=QSqlQuery(db);
    if(!query.exec("select * from student"))
    {
        qDebug()<<"cannot select"<<query.lastError()<<QDir::currentPath();
    }
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
            query.next();
            Stotal++;
        }
    }
    else
    {
        qDebug()<<"cannot position";
        QMessageBox::warning(this, "错误", "数据库定位失败\nERROR ID:00004", QMessageBox::Ok, QMessageBox::NoButton);
    }
    query.finish();
    qDebug()<<"数据读取完成";
    //ui->label->setText(studentlist[0]);
    query.clear();
    //db.close();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_commandLinkButton_signin_clicked()
{
    Signin *signin=new Signin(this);
    int ret=signin->exec();// 以模态方式显示对话框
    if (ret==QDialog::Accepted)
    {

    }
    delete signin;
}

void MainWindow::save_set()
{
    query.exec("create table setting (hour int(10), minute int(10), addmark int(10))");

}

void MainWindow::on_commandLinkButton_command_clicked()
{//读取存储的用户名和密码, 密码是经过加密的
    QString organization="CQAMS";//用于注册表，HKEY_CURRENT_USER/Software/

    QSettings   settingp(organization,"Password");//创建
    pswd=settingp.value("PSWD").toString();//读取PSWD 键的值，
    QSettings   settingc(organization,"Command");//创建
    comd=settingc.value("Command").toString();//读取PSWD 键的值，
    qDebug()<<pswd<<comd;

    QLineEdit::EchoMode echoMode=QLineEdit::Password;
    bool ok=false;
    QString PSWD=QInputDialog::getText(this,"请输入密码","输入密码:",echoMode,"",&ok,Qt::WindowFlags(Qt::WindowStaysOnTopHint));
    if(encrypt(PSWD)==pswd&&ok)
    {
        FullCommand *command=new FullCommand(this);
        int ret=command->exec();// 以模态方式显示对话框
        if (ret==QDialog::Accepted)
        {
            if(command->removeDB())
            {
                delete command;
                db.close();
                QFile::remove("info.db");
                QMessageBox::information(this,"提示","请重启软件");
                exit(EXIT_SUCCESS);
            }
        }
        delete command;
        return;
    }
    else
    {
        if(encrypt(PSWD)==comd&&ok)
        {//登记扣分
            UpdateMark *updatemark=new UpdateMark(this);
            int ret=updatemark->exec();// 以模态方式显示对话框
            if (ret==QDialog::Accepted)
            {

            }
            delete updatemark;
            return;
        }
        else
        {
            QMessageBox::warning(this,"警告","密码错误！");
        }
    }
}


void MainWindow::on_commandLinkButton_more_clicked()
{
    system("start https://jnshengjie.cn/zhejiang_lishuixueyuanfuzhong/");
}


void MainWindow::on_commandLinkButton_about_clicked()
{
    About *about=new About(this);
    int ret=about->exec();// 以模态方式显示对话框
    if (ret==QDialog::Accepted)
    {

    }
    delete about;
    return;
}


void MainWindow::on_commandLinkButton_tools_clicked()
{
    Draw *draw=new Draw(this);
    draw->show();
}


void MainWindow::on_commandLinkButton_check_clicked()
{
    Check *check=new Check(this);
    check->show();
}

