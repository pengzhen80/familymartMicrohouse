#ifndef ITEMMANAGER_H
#define ITEMMANAGER_H

#include <QObject>
#include<QMap>
#include"common.h"

class ItemManager : public QObject
{
    Q_OBJECT
public:
    explicit ItemManager(QObject *parent = nullptr);
    void AddItem(Item_FamilyMart_Info item,int boxId);
    void AddItems(QVector<Item_FamilyMart> items);
//    void SetItemName(QString path,QString itemId);
    void InitItems(QVector<Item_FamilyMart> items);
    void RemoveItem(QString item_id);
    void RemoveItems(QVector<QString> itemIds);
    Item_FamilyMart_Box GetBoxInfoByItemId(QString item_id);
    QVector<Item_FamilyMart> GetItemsByPhoneNumber(QString phone_number);
    void BindItemToBox(QString item_id,Item_FamilyMart_Box box);
    QVector<Item_FamilyMart> GetItemsBeyondDay(int days);
    Item_FamilyMart GetItemById(QString item_id);
    QVector<Item_FamilyMart> GetAllItem();
    Item_FamilyMart GetItemByBarcode(QString barcode);
    QVector<Item_FamilyMart> GetItemsByBoxId(int boxId);
    //for test
    void DefaultAddSomeItems();

private:
    QString make_itemId();
    QString make_weekDay();
    QString make_currentdate();
    void send_sig(Sig_ItemManagement_DBManagement sig);
    QSet<int> getBoxIdByItems(QVector<Item_FamilyMart> items);
    void addNewItemsFromApi(QVector<Item_FamilyMart> items);
    void initItemsNew(QVector<Item_FamilyMart> items);
signals:
    void sig_dbmanager(Sig_ItemManagement_DBManagement sig);
    void sig_mainwindow(Sig_ItemManagement_Mainwindow);
    void sig_jobmanager(Sig_ItemManagement_JobManagement sig);

public slots:
    void slot_dbmanager(Sig_DBManagement_ItemManagement sig);
    void slot_mainwindow(Sig_Mainwindow_Itemmanagement sig);
    void slot_jobmanager(Sig_JobManagement_ItemManagement sig);
    void slot_apimanager(Sig_ApiManagement_ItemManagement sig);
    void slot_barcodeScaner(Sig_BarcodeScaner_ItemManagement sig);
private:
    QMap<QString,Item_FamilyMart> map_itemId_item_;
    QMap<QString,Item_FamilyMart> map_barcode_Items_;
    int id_count_ = 0;
};

#endif // ITEMMANAGER_H
