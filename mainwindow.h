#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include <QSqlQuery>
#include <QMessageBox>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <signin.h>
#include <setting.h>
#include <fullcommand.h>
#include <QCloseEvent>
#include <QLineEdit>
#include <QInputDialog>
#include <check.h>
#include <draw.h>
#include <updatemark.h>
#include <about.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QSqlDatabase  db; //数据库
    QSqlQuery query;
    QSqlQueryModel  *qryModel; //数据模型
    void openDatabase();
    void iniUI();
    bool checkDB();

    QString pswd,comd;
    int window_width;
    int fo;

    int tryCount=0; //试错次数

    QString encrypt(const QString& str);//字符串加密

    //QString jobs[4]={"teacher","student","monitor","classrep"};

    //学生名单

    struct earlyAddTime
    {
        int h;
        int m;
        int addMark;
    }earlyAddTime[4];

    //管理名单
    class teacher :public QMainWindow
    {
    public:
        friend class Signin;
        int id;
        QString name;
    }teaclist[10];

    void save_set();

public:
    //friend class Signin;
    MainWindow(QWidget *parent = nullptr);
    void closeEvent(QCloseEvent *event);
    ~MainWindow();

    class  student :public QMainWindow
    {
    public:
        friend class Signin;
        int id;
        int numb;
        QString name;
        int mark;
    }studlist[100];

    int Stotal=0;

private slots:
    void time_update(void);

    void on_commandLinkButton_signin_clicked();

    void on_commandLinkButton_command_clicked();

    void on_commandLinkButton_more_clicked();

    void on_commandLinkButton_about_clicked();

    void on_commandLinkButton_tools_clicked();

    void on_commandLinkButton_check_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
