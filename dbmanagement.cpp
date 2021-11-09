#include "dbmanagement.h"
#include<QDebug>


DBManagement::DBManagement(QObject *parent) : QObject(parent)
{

}
DBManagement::~DBManagement()
{
    Release();
}

void DBManagement::Init()
{
    qDebug()<<"dbmanager thread is "<<QThread::currentThreadId();
    db_ = QSqlDatabase::addDatabase("QMYSQL");
    db_.setHostName(DB_SERVER_ADDRESS);
    db_.setPort(DB_SERVER_PORT);
    db_.setDatabaseName(DB_SERVER_DBNAME);
    db_.setUserName(DB_SERVER_USERNAME);
    db_.setPassword(DB_SERVER_PASSWORD);
    bool ok = db_.open();
    while(!ok)
    {
        QThread::sleep(1);
        ok = db_.open();
//        QSqlQuery query(db_);
//        QString search_item("select * from table_item");
//        query.exec(search_item);
//        int fieldNo = query.record().indexOf("phone");
//        while (query.next()) {
//            QString phone = query.value(fieldNo).toString();
//            QString itemId = query.value(0).toString();
//            QString name = query.value(1).toString();
//            qDebug()<<itemId<<name<<phone;
//        }

        //db_.close();
    }
    qDebug()<<"database open success";
    flag_db_isconnect_ = true;
}

void DBManagement::reconnectToDB()
{
    if(db_.isOpen())
    {
        db_.close();
    }

    //todo : wait to flag_takepage_nextBoxPrepared_ = true
    QTimer* timer_reconnectToDB = new QTimer();
    connect(timer_reconnectToDB,&QTimer::timeout,this,[&,timer_reconnectToDB]()
    {
        db_ = QSqlDatabase::addDatabase("QMYSQL");
        db_.setHostName(DB_SERVER_ADDRESS);
        db_.setPort(DB_SERVER_PORT);
        db_.setDatabaseName(DB_SERVER_DBNAME);
        db_.setUserName(DB_SERVER_USERNAME);
        db_.setPassword(DB_SERVER_PASSWORD);
        bool ok = db_.open();
        if(ok)
        {
            timer_reconnectToDB->stop();

            qDebug()<<"db reconnect success";
            flag_db_isconnect_ = true;
            insertItemsToNewItemTable(vec_tmp_items_api_);
            vec_tmp_items_api_.clear();
        }
    });
    timer_reconnectToDB->start(1000);
}

void DBManagement::Init_Data()
{
    if(db_.isOpen())
    {
        Sig_DBManagement_BoxManagement sig_box;
        sig_box.type = SIG_DBMANAGEMENT_BOXMANAGEMENT_INITBOX;
        sig_box.value_initbox.boxs = getBoxs();

        emit sig_dbmanager_initBoxs(sig_box);

        Sig_DBManagement_ItemManagement sig_item;
        sig_item.type = SIG_DBMANAGEMENT_ITEMMANAGEMENT_INITITEM;
        sig_item.value_initItem.items = searchItemsFromTableItem();
        sig_item.value_initItem.itemsNew = searchItemsFromTableItemNew();

        emit sig_itemmanager_initItems(sig_item);
    }
    else
    {
        qDebug()<<"DBManagement::Init_Data: db not open";
    }
}

void DBManagement::Release()
{
    db_.close();
}

void DBManagement::InsertItem(QVector<Item_FamilyMart> items)
{
    insertItemsToItemTable(items);
}
void DBManagement::RemoveItem(QVector<QString> itemId)
{
    removeItemsFromItemTable(itemId);
}


void DBManagement::AddData_Test_RealsenseData(QVector<int> counts,QVector<float> volumes)
{
    QSqlQuery query(db_);

    QString tableName("table_test_realsensedata");
    qDebug()<<"DBManagement::AddData_Test_RealsenseData : "<<counts.size();
    for(int i=0;i<counts.size();i++)
    {
        query.prepare(QString("INSERT INTO %1(count,volume)"
                "VALUES(:count,:volume)").arg(tableName));
        query.bindValue(":count", counts[i]);
        query.bindValue(":volume", QString::number(volumes[i]));
        query.exec();
    }
}

void DBManagement::slot_realsensemanager_realsensedata(QVector<int> count,QVector<float> volumes)
{
    AddData_Test_RealsenseData(count,volumes);
}

bool DBManagement::updateBoxLeftVolume(float volume, int boxId)
{
    QSqlQuery query(db_);
    query.prepare("update table_box set leftVolume = :leftVolume WHERE boxId = :boxId");
    query.bindValue(":leftVolume", QString::number(volume));
    query.bindValue(":boxId", QString::number(boxId));
    query.exec();

    return true;
}

bool DBManagement::insertItemsToItemTable(const QVector<Item_FamilyMart> items)
{
    QSqlQuery query(db_);

    for(int i=0;i<items.size();i++)
    {
        query.prepare("INSERT INTO table_item(itemId,name,phone,date,boxId,barcode)"
                          "VALUES (:itemId, :name, :phone,:date,:boxId,:barcode)");
        query.bindValue(":itemId", items[i].id);
        query.bindValue(":name", items[i].infos.name);
        query.bindValue(":phone", items[i].infos.phone_number);
        query.bindValue(":date", items[i].infos.date_arrive);
        query.bindValue(":boxId", QString::number(items[i].box.box_id));
        query.bindValue(":barcode", items[i].infos.barcode);
        query.exec();
    }
    return true;
}

bool DBManagement::insertItemsToNewItemTable(const QVector<Item_FamilyMart> items)
{
    if(!flag_db_isconnect_)
    {
        vec_tmp_items_api_.append(items);
        return true;
    }
    if(db_.isOpen())
    {
        QSqlQuery query(db_);
        for(int i=0;i<items.size();i++)
        {
            query.prepare("INSERT INTO table_item_api(barcode,name,phone)"
                              "VALUES (:barcode, :name, :phone)");
            query.bindValue(":barcode", items[i].infos.barcode);
            query.bindValue(":name", items[i].infos.name);
            query.bindValue(":phone", items[i].infos.phone_number);
            if(!query.exec())
            {
                qDebug()<< db_.lastError().text();
                flag_db_isconnect_ = false;
                vec_tmp_items_api_.push_back(items[i]);
//                qDebug()<<"name :"<<items[i].infos.name<<", phone:"<<items[i].infos.phone_number<<", barcode:"<<items[i].infos.barcode;
            }
        }
        if(!flag_db_isconnect_)
        {
            reconnectToDB();
        }
    }
    else
    {
        qDebug()<<"db is not open";
    }
    return true;
}

bool DBManagement::removeItemsFromItemTable(const QVector<QString> items)
{

    QSqlQuery query(db_);

    for(int i=0;i<items.size();i++)
    {
        query.prepare("DELETE from table_item where itemId = :itemId ");
        query.bindValue(":itemId", items[i]);
        query.exec();
    }

    return true;
}

QVector<Item_FamilyMart> DBManagement::searchItemsFromTableItem()
{
    QVector<Item_FamilyMart> result;
    QSqlQuery query(db_);

    QString tableName("table_item");
    QString search_item("select * from "+tableName);
    query.exec(search_item);
    int itemIdIndex = query.record().indexOf("itemId");
    int nameIndex = query.record().indexOf("name");
    int phoneIndex = query.record().indexOf("phone");
    int dateIndex = query.record().indexOf("date");
    int boxIdIndex = query.record().indexOf("boxId");
    int barcodeIndex = query.record().indexOf("barcode");
    while (query.next()) {
        Item_FamilyMart item;

        QString itemId = query.value(itemIdIndex).toString();
        QString name = query.value(nameIndex).toString();
        QString phone = query.value(phoneIndex).toString();
        QString date = query.value(dateIndex).toString();
        QString boxId = query.value(boxIdIndex).toString();
        QString barcode = query.value(barcodeIndex).toString();

        item.id = itemId;
        QString str = "FF";
        item.box.box_id = boxId.toInt();
        item.infos.date_arrive = date;
        item.infos.name = name;
        item.infos.phone_number = phone;
        item.infos.barcode = barcode;

        result.push_back(item);
        //qDebug()<<itemId;
    }
    return result;
}

QVector<Item_FamilyMart> DBManagement::searchItemsFromTableItemNew()
{
    QVector<Item_FamilyMart> result;
    QSqlQuery query(db_);

    QString tableName("table_item_api");
    QString search_item("select * from "+tableName);
    query.exec(search_item);
    int barcodeIndex = query.record().indexOf("barcode");
    int nameIndex = query.record().indexOf("name");
    int phoneIndex = query.record().indexOf("phone");
    while (query.next()) {
        Item_FamilyMart item;

        QString name = query.value(nameIndex).toString();
        QString phone = query.value(phoneIndex).toString();
        QString barcode = query.value(barcodeIndex).toString();

        QString str = "FF";
        item.infos.name = name;
        item.infos.phone_number = phone;
        item.infos.barcode = barcode;

        result.push_back(item);
        //qDebug()<<itemId;
    }
    return result;
}

QVector<Box_Warehouse> DBManagement::getBoxs()
{
    qDebug()<<"DBManagement::searchBoxs";
    QVector<Box_Warehouse> result;
    if(db_.isOpen())
    {
        QSqlQuery query(db_);


        QString tableName = "table_box";
        QString search_box = "select * from "+tableName;
        query.prepare(search_box);
        query.exec();
        while(query.next()) {
            Box_Warehouse box;
            Item_FamilyMart item;
            QString boxId = query.value(0).toString();
            QString leftVolume = query.value(1).toString();

            box.id = boxId.toInt();
            box.left_volume = leftVolume.toFloat();

    //        qDebug()<<query.value(0).toString()<<" "<<query.value(1).toString();
            result.push_back(box);

        }
    }
    return result;
}

void DBManagement::slot_boxmanager(Sig_BoxManagement_DBManagement sig)
{
    switch (sig.type) {
    case SIG_BOXMANAGEMENT_DBMANAGEMENT_UPDATE_BOXVOLUME:
    {
        updateBoxLeftVolume(sig.value_update_boxvolume.volume,sig.value_update_boxvolume.box_id);
        break;
    }
    default:
    {
        qDebug()<<"SIG_BOXMANAGEMENT_DBMANAGEMENT : error signal";
    }
    }
}

void DBManagement::slot_itemmanager(Sig_ItemManagement_DBManagement sig)
{
    switch (sig.type) {
    case SIG_ITEMMANAGEMENT_DBMANAGEMENT_ADDITEM:
    {
        insertItemsToItemTable(sig.value_addItem.items);
        break;
    }
    case SIG_ITEMMANAGEMENT_DBMANAGEMENT_ADDITEM_NEW:
    {
        qDebug()<<"get item from api:"<<sig.value_addItemsNew.items.size();
        insertItemsToNewItemTable(sig.value_addItemsNew.items);
        break;
    }
    case SIG_ITEMMANAGEMENT_DBMANAGEMENT_REMOVEITEM:
    {
        removeItemsFromItemTable(sig.value_removeItem.items);
        break;
    }
    default:
    {
        qDebug()<<"SIG_BOXMANAGEMENT_DBMANAGEMENT : error signal";
    }
    }
}
