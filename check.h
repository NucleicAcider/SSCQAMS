#ifndef CHECK_H
#define CHECK_H

#include <QDialog>
#include <QSqlQuery>
#include <mainwindow.h>

namespace Ui {
class Check;
}

class Check : public QDialog
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
        QString record;
    }studentinfo;

public:
    explicit Check(QWidget *parent = nullptr);
    ~Check();

private slots:
    void on_lineEdit_textEdited(const QString &arg1);

    void on_lineEdit_editingFinished();

private:
    Ui::Check *ui;
};

#endif // CHECK_H
