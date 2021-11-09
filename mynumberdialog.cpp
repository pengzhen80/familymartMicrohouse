#include "mynumberdialog.h"

//QString number[12] = {"0","1","2","3","4","5","6","7","8","9","."};

MyNumberDialog::MyNumberDialog():
    QDialog(0,Qt::Tool|Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint),btn_group_(new QButtonGroup(this))
{
    setupUi(this);
    initGb();
}

MyNumberDialog::~MyNumberDialog()
{
    delete ui;
}

void MyNumberDialog::initGb()
{
    //QPushButton* btn = new QPushButton(this);
}

void MyNumberDialog::on_btn_1_clicked()
{
    emit sendNumber(1);
}

void MyNumberDialog::on_btn_2_clicked()
{
    emit sendNumber(2);
}

void MyNumberDialog::on_btn_3_clicked()
{
    emit sendNumber(3);
}

void MyNumberDialog::on_btn_4_clicked()
{
    emit sendNumber(4);
}

void MyNumberDialog::on_btn_5_clicked()
{
    emit sendNumber(5);
}

void MyNumberDialog::on_btn_6_clicked()
{
    emit sendNumber(6);
}

void MyNumberDialog::on_btn_7_clicked()
{
    emit sendNumber(7);
}

void MyNumberDialog::on_btn_8_clicked()
{
    emit sendNumber(8);
}

void MyNumberDialog::on_btn_9_clicked()
{
    emit sendNumber(9);
}

void MyNumberDialog::on_btn_0_clicked()
{
    emit sendNumber(0);
}

void MyNumberDialog::on_btn_back_clicked()
{
    //todo
    emit sendMessage(QString("clean"));
}

void MyNumberDialog::on_btn_clearNumber_clicked()
{
    emit sendMessage(QString("clearNumber"));
}
