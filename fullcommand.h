#ifndef FULLCOMMAND_H
#define FULLCOMMAND_H

#include <QDialog>
#include <QMainWindow>
#include <QtSql>
#include <QSqlQuery>
#include <QMessageBox>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QTableWidget>
#include <QCloseEvent>
#include <recordedit.h>
#include <QLineEdit>
#include <QInputDialog>
#include <setting.h>
#include <windows.h>
#include "xlsxdocument.h"

namespace Ui {
class FullCommand;
}

class FullCommand : public QDialog
{
    Q_OBJECT
    QSqlDatabase  db; //数据库
    QSqlQuery query;
    QSqlQueryModel  *qryModel; //数据模型
    void openDatabase();

    class  student :public QMainWindow
    {
    public:
        int id;
        int numb;
        QString name;
        int mark;
        int birth;
    }studlist[100];

    void iniUI();

    int TimeOut=0;

    int Stotal=0;
    int index=0;

    QString encrypt(const QString& str);//字符串加密
    QString pswd,comd;

    struct earlyAddTime
    {
        int h;
        int m;
        int addMark;
    }earlyAddTime[4];

    struct getData{
        int numb;
        QString name;
        int mark;
        int birth;
        QString record;
    }getData;

    QDateTime time;
    QString date,datetime,Time,week;

public:

    bool removeDB();

    explicit FullCommand(QWidget *parent = nullptr);

    struct sendData{
        int numb;
        QString name;
        int mark;
        int birth;
        QString record;
    }sendData;


    ~FullCommand();

private slots:
    void time_update(void);

    void on_tableWidget_itemDoubleClicked(QTableWidgetItem *item);

    void on_commandLinkButton_input_score_clicked();

    void on_commandLinkButton_change_pswd_clicked();

    void on_commandLinkButton_change_comd_clicked();

    void on_commandLinkButton_reset_all_clicked();

    void on_commandLinkButton_output_main_info_clicked();

    void on_commandLinkButton_output_all_info_clicked();

private:
    Ui::FullCommand *ui;
};

#endif // FULLCOMMAND_H
