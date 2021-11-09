#ifndef DBMANAGEMENT_H
#define DBMANAGEMENT_H

#include <QObject>
#include <QSqlDatabase>
#include<QSqlQuery>
#include<QSqlRecord>
#include<QString>
#include<QThread>
#include<QTimer>
#include <QSqlError>
#include"common.h"

class DBManagement : public QObject
{
    Q_OBJECT
public:
    explicit DBManagement(QObject *parent = nullptr);
    ~DBManagement();
    void Init();
    void Init_Data();

private:
    void Release();
    void InsertItem(QVector<Item_FamilyMart> item);
    void RemoveItem(QVector<QString> itemId);
    void AddData_Test_RealsenseData(QVector<int> counts,QVector<float> volumes);//for realsence data analyze

    bool insertItemsToItemTable(const QVector<Item_FamilyMart> items);
    bool insertItemsToNewItemTable(const QVector<Item_FamilyMart> items);
    bool removeItemsFromItemTable(const QVector<QString> items);

    QVector<Item_FamilyMart> searchItemsFromTableItem();
    QVector<Item_FamilyMart> searchItemsFromTableItemNew();
    QVector<Box_Warehouse> getBoxs();
    bool updateBoxLeftVolume(float volume, int boxId);

    //if insert failed need to reconnect
    void reconnectToDB();
public slots:
    void slot_boxmanager(Sig_BoxManagement_DBManagement sig);
    void slot_itemmanager(Sig_ItemManagement_DBManagement sig);
    //for test
    void slot_realsensemanager_realsensedata(QVector<int> count,QVector<float> volumes);
signals:
    void sig_dbmanager_initBoxs(Sig_DBManagement_BoxManagement sig);
    void sig_itemmanager_initItems(Sig_DBManagement_ItemManagement sig);
private:
    QSqlDatabase db_;

    //if insert failed to store data
    QVector<Item_FamilyMart> vec_tmp_items_api_;
    bool flag_db_isconnect_ = true;;
};

#endif // DBMANAGEMENT_H
