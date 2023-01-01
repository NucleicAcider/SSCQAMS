#include "draw.h"
#include "ui_draw.h"
#include <QTime>

Draw::Draw(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Draw)
{
    MainWindow *getInfo=(MainWindow*) parentWidget();
    Stotal=getInfo->Stotal;
    for (int a=0;a<Stotal;a++)
    {
        studlist[a].numb=getInfo->studlist[a].numb;
        studlist[a].name=getInfo->studlist[a].name;
    }
    ui->setupUi(this);
}

Draw::~Draw()
{
    delete ui;
}

void Draw::on_pushButton_clicked()
{
    int num=ui->spinBox->value();
    QTime time;
    time= QTime::currentTime();
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    student drawStudent[num];
    for (int a=0;a<num;a++)
    {
        int b = qrand()%Stotal;
        qDebug()<< b;
        drawStudent[a].numb=b;
        drawStudent[a].name=studlist[b].name;
    }
    ui->tableWidget->setAlternatingRowColors(true);
    QTableWidgetItem    *headerItem;
    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setRowCount(num);
    QStringList headerText;
    headerText<<"学 号"<<"Lucky Dog";  //表头标题用QStringList来表示/
    ui->tableWidget->setColumnCount(headerText.count());//列数设置为与 headerText的行数相等
    for (int i=0;i<2;i++)//列编号从0开始
    {
       headerItem=new QTableWidgetItem(headerText.at(i)); //新建一个QTableWidgetItem， headerText.at(i)获取headerText的i行字符串
       QFont font=headerItem->font();//获取原有字体设置
       font.setBold(true);//设置为粗体
       font.setPointSize(14);//字体大小
       headerItem->setForeground(Qt::black);//字体颜色
       headerItem->setFont(font);//设置字体
       ui->tableWidget->setHorizontalHeaderItem(i,headerItem); //设置表头单元格的Item
    }
    for (int index=0;index<num;index++)
    {
        ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->tableWidget->setItem(index,0,new QTableWidgetItem(QString::number(drawStudent[index].numb+1)));
        ui->tableWidget->setItem(index,1,new QTableWidgetItem(drawStudent[index].name));
        QFont font1=ui->tableWidget->item(index,1)->font();
        font1.setBold(true);
        font1.setPointSize(40);
        ui->tableWidget->item(index,1)->setFont(QFont(font1));
        ui->tableWidget->item(index,1)->setForeground(Qt::red);
        for (int i=0;i<2;i++)
        {
            ui->tableWidget->item(index,i)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        }
    }
    //ui->tableWidget->verticalHeader()->setHidden(true);
    ui->tableWidget->resizeColumnsToContents();
    ui->tableWidget->resizeRowsToContents();
    ui->tableWidget->show();
}

