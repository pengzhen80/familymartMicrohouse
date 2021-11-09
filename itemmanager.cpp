#include "itemmanager.h"
#include<QTime>
#include<QDebug>

ItemManager::ItemManager(QObject *parent) : QObject(parent)
{
    //DefaultAddSomeItems();
}

void ItemManager::InitItems(QVector<Item_FamilyMart> items)
{
    for(int i=0;i<items.size();i++)
    {
        map_itemId_item_.insert(items[i].id,items[i]);
    }
}
void ItemManager::initItemsNew(QVector<Item_FamilyMart> items)
{
    for(int i=0;i<items.size();i++)
    {
        map_barcode_Items_.insert(items[i].infos.barcode,items[i]);
    }
}
void ItemManager::DefaultAddSomeItems()
{
    Item_FamilyMart_Info item_infos_1;
    item_infos_1.name = tr("郭朋振");
    item_infos_1.phone_number = tr("123");
    item_infos_1.date_arrive = tr("6/22");

    Item_FamilyMart_Info item_infos_2;
    item_infos_2.name = tr("郭朋振");
    item_infos_2.phone_number = tr("456");
    item_infos_2.date_arrive = tr("6/22");

    Item_FamilyMart_Info item_infos_3;
    item_infos_3.name = tr("XXX");
    item_infos_3.phone_number = tr("456");
    item_infos_3.date_arrive = tr("6/22");

//    AddItem(item_infos_1);
//    AddItem(item_infos_2);
//    AddItem(item_infos_3);
}

void ItemManager::AddItem(Item_FamilyMart_Info item_infos,int boxId)
{
    Item_FamilyMart item;
    item.id = make_itemId();
    item.box.box_id = boxId;
    item.infos = item_infos;

//    item.infos.date_arrive = QString::number(QDateTime::currentDateTime().date().year())
//            +QString("/")
//            +QString::number(QDateTime::currentDateTime().date().month())
//            +QString("/")
//            +QString::number(QDateTime::currentDateTime().date().day());
    //for test
    item.infos.date_arrive = QString::number(QDateTime::currentDateTime().date().year())
            +QString("/")
            +QString::number(5)
            +QString("/")
            +QString::number(QDateTime::currentDateTime().date().day());
    item.infos.weekday = make_weekDay();
//    qDebug()<<item.infos.date_arrive;
    map_itemId_item_.insert(item.id,item);
    qDebug()<<"add item "<<item.id;

    Sig_ItemManagement_DBManagement sig;
    sig.type = SIG_ITEMMANAGEMENT_DBMANAGEMENT_ADDITEM;
    sig.value_addItem.items.push_back(item);

    send_sig(sig);
}

void ItemManager::AddItems(QVector<Item_FamilyMart> items)
{
    for(int i=0;i<items.size();i++)
    {
        items[i].id = make_itemId();
        items[i].infos.date_arrive = make_currentdate();
        map_itemId_item_.insert(items[i].id,items[i]);
        qDebug()<<"add item "<<items[i].id;
    }

    Sig_ItemManagement_DBManagement sig;
    sig.type = SIG_ITEMMANAGEMENT_DBMANAGEMENT_ADDITEM;
    sig.value_addItem.items.append(items);

    send_sig(sig);
}

void ItemManager::addNewItemsFromApi(QVector<Item_FamilyMart> items)
{
    QVector<Item_FamilyMart> itemToDb;
    for(int i=0;i<items.size();i++)
    {
        if(map_barcode_Items_.contains(items[i].infos.barcode))
        {
            qDebug()<<"already has this item";
            continue;
        }
        items[i].id = make_itemId();
        map_barcode_Items_.insert(items[i].infos.barcode,items[i]);
        qDebug()<<"ItemManager::addNewItemsFromApi : add item "<<items[i].id;
        itemToDb.append(items[i]);
    }
    if(itemToDb.size()>0)
    {
        qDebug()<<"send api items to DB:"<<itemToDb.size();
        Sig_ItemManagement_DBManagement sig;
        sig.type = SIG_ITEMMANAGEMENT_DBMANAGEMENT_ADDITEM_NEW;
        sig.value_addItemsNew.items.append(itemToDb);

        send_sig(sig);

    }


}

//void ItemManager::SetItemName(QString path,QString itemId)
//{
//    if(map_itemId_item_.contains(itemId))
//    {
//        map_itemId_item_[itemId].infos.name = path;
//    }
//    else
//    {
//        qDebug()<<"ItemManager::SetItemName "<<"cannot find item";
//    }
//}

void ItemManager::RemoveItem(QString item_id)
{
    map_itemId_item_.remove(item_id);
    qDebug()<<" ItemManager::RemoveItem : "<<item_id;

    Sig_ItemManagement_DBManagement sig;
    sig.type = SIG_ITEMMANAGEMENT_DBMANAGEMENT_REMOVEITEM;
    sig.value_removeItem.items.push_back(item_id);
    send_sig(sig);
}

void ItemManager::RemoveItems(QVector<QString> itemIds)
{
    for(int i=0;i<itemIds.size();i++)
    {
        map_itemId_item_.remove(itemIds[i]);
//        qDebug()<<" ItemManager::RemoveItem : "<<itemIds[i];
    }

    Sig_ItemManagement_DBManagement sig;
    sig.type = SIG_ITEMMANAGEMENT_DBMANAGEMENT_REMOVEITEM;
    sig.value_removeItem.items.append(itemIds);
    send_sig(sig);
}

Item_FamilyMart_Box ItemManager::GetBoxInfoByItemId(QString item_id)
{
    return map_itemId_item_[item_id].box;
}

Item_FamilyMart ItemManager::GetItemById(QString item_id)
{
    return map_itemId_item_[item_id];
}

QVector<Item_FamilyMart> ItemManager::GetAllItem()
{
    QVector<Item_FamilyMart> result;
    QMapIterator<QString, Item_FamilyMart> it(map_itemId_item_);
    while (it.hasNext()) {
        it.next();
        result.push_back(it.value());
    }
    return result;
}

Item_FamilyMart ItemManager::GetItemByBarcode(QString barcode)
{
//    QMapIterator<QString, Item_FamilyMart> it(map_barcode_Items_);
//    while (it.hasNext()) {
//        it.next();
//        if(barcode == it.value().infos.barcode)
//        {
//            return it.value();
//        }
//    }
    if(map_barcode_Items_.contains(barcode))
    {
        return map_barcode_Items_[barcode];
    }
    else
    {
        qDebug()<<"ItemManagement:cannot find barcode";
    }
    Item_FamilyMart item;
    item.infos.name = "";
    qDebug()<<item.infos.name.size();
    return item;
}

QVector<Item_FamilyMart> ItemManager::GetItemsByBoxId(int boxId)
{
    QVector<Item_FamilyMart> items;
    QMapIterator<QString, Item_FamilyMart> it(map_itemId_item_);
    while (it.hasNext()) {
        it.next();
        if(boxId == it.value().box.box_id)
        {
            items.push_back(it.value());
        }
    }

    return items;
}

QString ItemManager::make_itemId()
{
    //todo
    //return QTime::currentTime()
    //QString result = QTime::currentTime().toString()+QString::number(id_count_++);
    //QStringList timelist = QTime::currentTime().toString().split(":");
    QString result = QString::number(QDateTime::currentDateTime().date().year())
            +QString::number(QDateTime::currentDateTime().date().month())
            +QString::number(QDateTime::currentDateTime().date().day())
            +QString::number(QDateTime::currentDateTime().time().hour())
            +QString::number(QDateTime::currentDateTime().time().minute())
            +QString::number(QDateTime::currentDateTime().time().second())
            +QString::number(id_count_++);
//    qDebug()<<"id is "<<result;
    return result;
}

QString ItemManager::make_currentdate()
{
    QString result = QString::number(QDateTime::currentDateTime().date().year())
            +"/"
            +QString::number(QDateTime::currentDateTime().date().month())
            +"/"
            +QString::number(QDateTime::currentDateTime().date().day());
    return result;
}

QString ItemManager::make_weekDay()
{
    QString result;
    int day = QDateTime::currentDateTime().date().dayOfWeek();
    switch (day) {
    case 1:
    {
        result = QString(tr("星期一"));
        break;
    }
    case 2:
    {
        result = QString(tr("星期二"));
        break;
    }
    case 3:
    {
        result = QString(tr("星期三"));
        break;
    }
    case 4:
    {
        result = QString(tr("星期四"));
        break;
    }
    case 5:
    {
        result = QString(tr("星期五"));
        break;
    }
    case 6:
    {
        result = QString(tr("星期六"));
        break;
    }
    case 7:
    {
        result = QString(tr("星期日"));
        break;
    }
    }
    return result;
}

QSet<int> ItemManager::getBoxIdByItems(QVector<Item_FamilyMart> items)
{
    QSet<int> boxIds;
    for(int i=0;i<items.size();i++)
    {
        if(!boxIds.contains(items[i].box.box_id))
        {
            boxIds.insert(items[i].box.box_id);
        }
    }
    return boxIds;
}

QVector<Item_FamilyMart> ItemManager::GetItemsByPhoneNumber(QString phone_number)
{
    QVector<Item_FamilyMart> result;
    QMapIterator<QString, Item_FamilyMart> it(map_itemId_item_);
    while (it.hasNext()) {
        it.next();
        if(phone_number == it.value().infos.phone_number)
        {
//            Item_FamilyMart item;
//            item.infos.phone_number = phone_number;
//            item.infos.name = it.value().infos.name;
//            item.infos.date_arrive = it.value().infos.date_arrive;
//            item.id = it.value().id;
            //item.box = it.value().box;
            result.push_back(it.value());
        }
    }
    return result;
}

void ItemManager::BindItemToBox(QString item_id,Item_FamilyMart_Box box)
{
    if(map_itemId_item_.contains(item_id))
    {
        map_itemId_item_[item_id].box.box_id = box.box_id;
    }
    else
    {
        qDebug()<<"ItemManager::BindItemToBox : cannot find item";
    }
}

QVector<Item_FamilyMart> ItemManager::GetItemsBeyondDay(int days)
{
    QVector<Item_FamilyMart> result;

    QDate currentDate =  QDateTime::currentDateTime().date();

    QList<Item_FamilyMart> item_list = map_itemId_item_.values();
    for(int i=0;i<item_list.size();i++)
    {
        QStringList datalist = item_list[i].infos.date_arrive.split("/");
        if(datalist.size()<3)
        {
            qDebug()<<"ItemManager::GetItemsBeyondDay : item date error";
            continue;
        }
        QDate data_item(datalist[0].toInt(),datalist[1].toInt(),datalist[2].toInt());
        if(data_item.daysTo(currentDate)>=days)
        {
            result.push_back(item_list[i]);
        }
    }
    return result;
}

void ItemManager::slot_dbmanager(Sig_DBManagement_ItemManagement sig)
{
    switch (sig.type) {
    case SIG_DBMANAGEMENT_ITEMMANAGEMENT_INITITEM:
    {
        InitItems(sig.value_initItem.items);
        initItemsNew(sig.value_initItem.itemsNew);
        break;
    }
    default:
    {
        qDebug()<<"SIG_DBMANAGEMENT_ITEMMANAGEMENT : error signal";
        break;
    }
    }
}

void ItemManager::slot_mainwindow(Sig_Mainwindow_Itemmanagement sig)
{
    switch (sig.type) {
    case SIG_MAINWINDOWN_ITEMMANAGEMENT_ASKITEMS_BYNUMBER:
    {
        Sig_ItemManagement_Mainwindow sigToWindow;
        sigToWindow.type = SIG_ITEMMANAGEMENT_MAINWINDOW_ITEMS_BYNUMBER;
        sigToWindow.value_itemsByNumber.items = GetItemsByPhoneNumber(sig.value_askItemsByNumber.number);

        emit sig_mainwindow(sigToWindow);
        break;
    }
    case SIG_MAINWINDOWN_ITEMMANAGEMENT_ASKITEMS_BYBEYONDDAY:
    {
        Sig_ItemManagement_Mainwindow sigToWindow;
        sigToWindow.type = SIG_ITEMMANAGEMENT_MAINWINDOW_ITEMS_BYDAY;
        sigToWindow.value_itemsByDay.items = GetItemsBeyondDay(sig.value_askItemsByBeyondDay.day);
        qDebug()<<sigToWindow.value_itemsByDay.items.size();

        emit sig_mainwindow(sigToWindow);
        break;
    }
    case SIG_MAINWINDOWN_ITEMMANAGEMENT_ASKITEM_BYBARCODE:
    {
        Sig_ItemManagement_Mainwindow sigToWindow;
        sigToWindow.type = SIG_ITEMMANAGEMENT_MAINWINDOW_ITEM_BYBARCODE;
        sigToWindow.value_itemByBarcode.item = GetItemByBarcode(sig.value_askItemByBarcode.barcode);

        qDebug()<<sigToWindow.value_itemByBarcode.item.infos.name.size();
        if(sigToWindow.value_itemByBarcode.item.infos.name.size()>0)
        {
            emit sig_mainwindow(sigToWindow);
        }
        else
        {
            qDebug()<<"cannot find item";
        }
//        qDebug()<<"barcode item:"<<sigToWindow.value_itemByBarcode.item.infos.name;
//        qDebug()<<sigToWindow.value_itemByBarcode.item.infos.phone_number;
        break;
    }
    case SIG_MAINWINDOWN_ITEMMANAGEMENT_ADDITEMS:
    {
        AddItems(sig.value_addItems.items);
        break;
    }
    case SIG_MAINWINDOWN_ITEMMANAGEMENT_ASKITEMS_ALL:
    {
        Sig_ItemManagement_Mainwindow sigToWindow;
        sigToWindow.type = SIG_ITEMMANAGEMENT_MAINWINDOW_ITEMS_ALL;
        sigToWindow.value_itemsAll.items = GetAllItem();

        emit sig_mainwindow(sigToWindow);
        break;
    }
    case SIG_MAINWINDOWN_ITEMMANAGEMENT_REMOVEITEMS:
    {
        QVector<QString> itemIds;
        for(int i=0;i<sig.value_removeItems.items.size();i++)
        {
            itemIds.push_back(sig.value_removeItems.items[i].id);
        }
        RemoveItems(itemIds);

        //for test
//        Sig_ItemManagement_DBManagement sigToDB;
//        sigToDB.type = SIG_ITEMMANAGEMENT_DBMANAGEMENT_REMOVEITEM;
//        sigToDB.value_removeItem.items = itemIds;

//        emit sig_dbmanager(sigToDB);
        break;
    }
    case SIG_MAINWINDOWN_ITEMMANAGEMENT_ASKITEMS_BYBOXID:
    {
        Sig_ItemManagement_Mainwindow sigToMainwindow;
        sigToMainwindow.type = SIG_ITEMMANAGEMENT_MAINWINDOW_ITEMS_BYBOXID;
        sigToMainwindow.value_itemsByBoxId.items = GetItemsByBoxId(sig.value_askItemsByBoxId.boxId);

        emit sig_mainwindow(sigToMainwindow);
        break;
    }
    default:
    {
        qDebug()<<"ItemManager::slot_mainwindow : error signal";
        break;
    }
    }
}

void ItemManager::slot_jobmanager(Sig_JobManagement_ItemManagement sig)
{
    switch (sig.type) {
    case SIG_JOBMANAGEMENT_ITEMMANAGEMENT_ASK_BOXID_BYITEMS:
    {
        Sig_ItemManagement_JobManagement sigToJob;
        sigToJob.type = SIG_ITEMMANAGEMENT_JOBMANAGEMENT_ITEMS_BOXID;
        sigToJob.value_itemsBoxId.boxIds = getBoxIdByItems(sig.value_askBoxIdByItems.items);

        emit sig_jobmanager(sigToJob);
        break;
    }
    case SIG_JOBMANAGEMENT_ITEMMANAGEMENT_ADD_NEWITEMS:
    {
        AddItems(sig.value_addNewItems.items);
        Sig_ItemManagement_JobManagement sigToJob;
        sigToJob.type = SIG_ITEMMANAGEMENT_JOBMANAGEMENT_ADDNEWITEMS_SUCCESS;
        emit sig_jobmanager(sigToJob);

        break;
    }
    default:
    {
        qDebug()<<"ItemManager::slot_jobmanager : unknown signal";
        break;
    }
    }
}

void ItemManager::slot_apimanager(Sig_ApiManagement_ItemManagement sig)
{
    switch (sig.type) {
    case SIG_APIMANAGEMENT_ITEMMANAGEMENT_NEWITEMS:
    {
        //compare with new items' map if barcode is old then delete
        addNewItemsFromApi(sig.value_newItems.items);
        break;
    }
    default:
    {
        qDebug()<<"unknown signal from apimanager";
        break;
    }
    }
}

void ItemManager::slot_barcodeScaner(Sig_BarcodeScaner_ItemManagement sig)
{
    switch (sig.type) {
    case SIG_BARCODESCANER_ITEMMANAGEMENT_BARCODE:
    {
        Item_FamilyMart item = GetItemByBarcode(sig.value_barcode.barcode);

        Sig_ItemManagement_Mainwindow sigToWindow;
        sigToWindow.type = SIG_ITEMMANAGEMENT_MAINWINDOW_ITEM_BYBARCODE;
        sigToWindow.value_itemByBarcode.item = item;
        emit sig_mainwindow(sigToWindow);
        break;
    }
    default:
    {
        qDebug()<<"unknown signal from barcode scaner";
        break;
    }
    }
}

void ItemManager::send_sig(Sig_ItemManagement_DBManagement sig)
{
    qDebug()<<"item send a sig to db";
    emit sig_dbmanager(sig);
}
