#ifndef SIGNIN_H
#define SIGNIN_H

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

namespace Ui {
class Signin;
}

class Signin : public QDialog
{
    Q_OBJECT
private:
    QSqlDatabase  db; //数据库
    QSqlQuery query;
    QSqlQueryModel  *qryModel; //数据模型
    void openDatabase();


    //QString jobs[4]={"teacher","student","monitor","classrep"};

    class  student :public QMainWindow
    {
    public:
        friend class Signin;
        int id;
        int numb;
        QString name;
        int mark;
        bool Signed=false;
    }studlist[100];

    int Stotal=0;
    int Signed=0;
    int addMark=0;

    int earlyAdd[4];
    struct earlyAddTime
    {
        int h;
        int m;
        int addMark;
    }earlyAddTime[4];

    void iniUI();
    int index=0;
    void moveRow(QTableWidget *pTable, int currentRow, int toRow);
    QDateTime time;
    QString date,datetime,Time,week;

public:
    explicit Signin(QWidget *parent = nullptr);
    ~Signin();

private slots:
    void time_update(void);

    void on_pushButton_clicked();

    void on_tableWidget_cellDoubleClicked(int row, int column);

    //void on_comboBox_currentIndexChanged(const QString &arg1);

    void on_tableWidget_cellClicked(int row, int column);

private:
    Ui::Signin *ui;
};

#endif // SIGNIN_H
