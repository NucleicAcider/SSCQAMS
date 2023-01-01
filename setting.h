#ifndef SETTING_H
#define SETTING_H

#include <QDialog>

namespace Ui {
class Setting;
}

class Setting : public QDialog
{
    Q_OBJECT

public:
    explicit Setting(QWidget *parent = nullptr);
    ~Setting();

    int h1();
    int h2();
    int h3();
    int h4();
    int m1();
    int m2();
    int m3();
    int m4();
    int addm1();
    int addm2();
    int addm3();
    int addm4();

private:
    Ui::Setting *ui;
};

#endif // SETTING_H
