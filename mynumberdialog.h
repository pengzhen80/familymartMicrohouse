#ifndef MYNUMBERDIALOG_H
#define MYNUMBERDIALOG_H

#include<QObject>
#include<QKeyEvent>
#include<QDialog>
#include<QButtonGroup>
#include"ui_mynumberdialog.h"
class MyNumberDialog: public QDialog,public Ui::MyNumberDialog
{
    Q_OBJECT
public:
    MyNumberDialog();
    ~MyNumberDialog();
    void affirmString();
    void deleteString();
    void initGb();
public slots:
    //void buttonClickResponse(int gemfield);
signals:
    void sendMessage(QString command);
    void sendNumber(int number);
    void sendfinish();
private slots:
    void on_btn_1_clicked();
    void on_btn_2_clicked();
    void on_btn_3_clicked();
    void on_btn_4_clicked();
    void on_btn_5_clicked();
    void on_btn_6_clicked();
    void on_btn_7_clicked();
    void on_btn_8_clicked();
    void on_btn_9_clicked();
    void on_btn_0_clicked();
    void on_btn_back_clicked();

    void on_btn_clearNumber_clicked();

private:
    Ui::MyNumberDialog *ui;
    QKeyEvent* key_event_;
    QButtonGroup* btn_group_;
    QVector<QPushButton*> btn_vertor_;
};

#endif // MYNUMBERDIALOG_H
