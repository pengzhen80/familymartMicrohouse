#include "mylineedit.h"

MyLineEdit::MyLineEdit(QWidget *parent) : QLineEdit(parent)
{
    this->setText("input number here");
}

void MyLineEdit::mousePressEvent(QMouseEvent *event)
{
    emit sig_data(this->text());
    QLineEdit::mousePressEvent(event);
}

void MyLineEdit::slot_number(int number)
{
    this->setText(this->text()+QString::number(number));
    if(this->text().size() == 3)
    {
        emit sig_numberSizeIsThree();
    }
}

void MyLineEdit::slot_message(QString command)
{
    if(command == "clean")
    {
        this->clear();
    }
    if("clearNumber" == command)
    {
         this->clear();
    }
}
