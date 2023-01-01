#ifndef UPDATEMARK_H
#define UPDATEMARK_H

#include <QDialog>
#include <mainwindow.h>

namespace Ui {
class UpdateMark;
}

class UpdateMark : public QDialog
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
        QString record;
    }studentinfo;

    QStringList reason;

    QDateTime time;
    QString date,datetime,Time,week;
    int TimeOut=0;

    void time_update();

public:
    explicit UpdateMark(QWidget *parent = nullptr);
    ~UpdateMark();

private slots:
    void on_comboBox_currentTextChanged(const QString &arg1);

    void on_pushButton_accept_clicked();

    void on_comboBox_2_activated(int index);

    void on_spinBox_valueChanged(int arg1);

private:
    Ui::UpdateMark *ui;
};

#endif // UPDATEMARK_H
