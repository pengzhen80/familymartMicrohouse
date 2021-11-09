#ifndef CLIPBOARDMANAGEMENT_H
#define CLIPBOARDMANAGEMENT_H

#include <QObject>
#include<QMimeData>
#include<QClipboard>
#include<QDebug>
#include<QImage>
#include<QThread>
#include<thread>
#include"common.h"

class ClipBoardManagement : public QObject
{
    Q_OBJECT
public:
    explicit ClipBoardManagement(QObject *parent = nullptr);
    void Init(QClipboard* clipboard);
    void TakeImage();
public slots:
    void slot_mainwindow_takeNameImage();
    void slot_mainwindow_takeNumberImage();

    void slot_mainwindow(Sig_Mainwindow_Clipmanagement sig);
signals:
    void sig_nameImage(QImage);
    void sig_numberImage(QImage);

    void sig_mainwindow(Sig_Clipmanagement_Mainwindow);
private:
    void takeBarcode();
    void takeBarcode_Two();
private:
    QClipboard* clipboard_;
    QImage image_;
    bool flag_take_nameimage_ = false;
    bool flag_take_numberImage_ = false;

    bool flag_take_barcode_ = false;
    bool flag_take_barcode_first_ = true;
    QString barcode_;
};

#endif // CLIPBOARDMANAGEMENT_H
