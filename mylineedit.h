#ifndef MYLINEEDIT_H
#define MYLINEEDIT_H

#include <QObject>
#include<QWidget>
#include <QLineEdit>

class MyLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit MyLineEdit(QWidget *parent = nullptr);
private:
    void mousePressEvent(QMouseEvent* event);
signals:
    void sig_data(QString data);
    void sig_numberSizeIsThree();
public slots:
    void slot_number(int number);
    void slot_message(QString command);
};

#endif // MYLINEEDIT_H
