#ifndef DRAW_H
#define DRAW_H

#include <QDialog>
#include <mainwindow.h>

namespace Ui {
class Draw;
}

class Draw : public QDialog
{
    Q_OBJECT
    class  student :public QMainWindow
    {
    public:
        int numb;
        QString name;
    }studlist[100];

    int Stotal=0;

public:
    explicit Draw(QWidget *parent = nullptr);
    ~Draw();

private slots:
    void on_pushButton_clicked();

private:
    Ui::Draw *ui;
};

#endif // DRAW_H
