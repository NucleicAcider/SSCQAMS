#ifndef RECORDEDIT_H
#define RECORDEDIT_H

#include <QDialog>
#include <fullcommand.h>

namespace Ui {
class RecordEdit;
}

class RecordEdit : public QDialog
{
    Q_OBJECT


    struct getData{
        int numb;
        QString name;
        int mark;
        int birth;
        QString record;
    }getData;

public:
    explicit RecordEdit(QWidget *parent = nullptr);
    ~RecordEdit();

    QString returnname();
    int returnmark();
    int returnbirth();
    QString returnrecord();

private slots:
    void on_commandLinkButton_score_clicked();

    void on_spinBox_mark_valueChanged(int arg1);

private:
    Ui::RecordEdit *ui;
};

#endif // RECORDEDIT_H
