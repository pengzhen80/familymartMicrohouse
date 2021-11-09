#ifndef BARCODESCANER_H
#define BARCODESCANER_H

#include <QObject>
#include"common.h"

class BarcodeScaner : public QObject
{
    Q_OBJECT
public:
    explicit BarcodeScaner(QObject *parent = nullptr);

signals:
    void sig_ItemManager(Sig_BarcodeScaner_ItemManagement);
};

#endif // BARCODESCANER_H
