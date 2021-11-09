#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QImage>
#include<QLabel>
#include<QStandardItemModel>
#include<QPixmap>

#include<mylineedit.h>
#include<QPainter>
#include<QDate>
#include<QThread>
#include<QScrollBar>

#include"mynumberdialog.h"
#include<QKeyEvent>
//#include"dws_management.h"
#include"common.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

#define STACK_WIDGET_PAGE_HOME_INDEX 0
#define STACK_WIDGET_PAGE_STORE_INDEX 1
#define STACK_WIDGET_PAGE_STORE_MULTI_INDEX 2
#define STACK_WIDGET_PAGE_ITEMINFO_INDEX 3
#define STACK_WIDGET_PAGE_TAKE_CUSTOMER_INDEX 4
#define STACK_WIDGET_PAGE_STORE_SINGLE_INDEX 5
#define STACK_WIDGET_PAGE_TAKEHOME_INDEX 6
#define STACK_WIDGET_PAGE_SEVENDAY_INDEX 7
#define STACK_WIDGET_PAGE_STORE_SINGLE_STOREPAGE_INDEX 8
#define STACK_WIDGET_PAGE_TAKE_CUSTOMER_TAKEPAGE_INDEX 9
#define STACK_WIDGET_PAGE_BACKGROUD_INDEX 10
#define STACK_WIDGET_PAGE_HOMENEW_INDEX 11
#define STACK_WIDGET_PAGE_PLC_ERROR_INDEX 12
#define STACK_WIDGET_PAGE_BACKGROUND_PLC_INDEX 13
#define STACK_WIDGET_PAGE_ASKBOXDOWNBYBUTTON 14
#define STACK_WIDGET_PAGE_BACKGROUD_SHOWITEMSBYBOXID_INDEX 15

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void show_numberkeyboard_ui(QString data);
    void slot_dialog_finish();
private slots:
    void on_pushButton_clicked();

    void on_btn_home_store_clicked();

    void on_btn_home_take_clicked();

    void on_btn_store_back_clicked();

    void on_btn_take_back_clicked();

    void on_btn_store_takeimage_clicked();

    void on_btn_itemInfo_back_clicked();

    void on_btn_store_store_clicked();

    void on_btn_store_finish_clicked();

    void on_btn_store_singlestore_clicked();

    void on_btn_store_multistore_clicked();

    void on_btn_store_single_takePicAgain_clicked();

    void on_btn_store_single_finish_clicked();

    void on_btn_takehome_customer_clicked();

    void on_btn_takehome_sevenday_clicked();

    void on_pushButton_2_clicked();

    void on_btn_store_multi_back_clicked();

    void on_btn_store_single_back_clicked();

    void on_btn_store_single_size_comfirm_clicked();

    void on_btn_store_single_info_comfirm_clicked();

    void on_btn_store_single_storepage_back_clicked();

    void on_btn_store_multi_store_clicked();

    void on_btn_take_customer_take_clicked();

    void on_btn_take_takepage_finish_clicked();

    void on_btn_take_takepage_cancel_clicked();

    void on_btn_take_sevenday_take_clicked();

    void on_btn_take_sevenday_back_clicked();

    void on_btn_store_single_storepage_store_clicked();

    void on_btn_store_single_storepage_changebox_clicked();

    void on_btn_take_customer_back_clicked();

    void on_btn_store_single_size_takePicAgain_clicked();
    void on_btn_store_multi_InfoComfirm_clicked();

    void on_tableView_store_multi_clicked(const QModelIndex &index);
    void on_tablewidget_take_customer_cellClicked(int row, int column);

    void on_table_servenday_list_clicked(const QModelIndex &index);


    void on_btn_store_single_name_takePicAgain_clicked();

    void on_btn_store_multi_takeNamePicAgain_clicked();

    void on_btn_background_detectEmptyBox_clicked();

    void on_btn_home_title_clicked();

    void on_btn_background_back_clicked();

    void on_btn_homeNew_customerTake_clicked();

    void on_btn_homeNew_multiStore_clicked();

    void on_btn_homeNew_sevendayTake_clicked();

    void on_btn_homeNew_logo_clicked();

    void on_btn_background_plc_recovery_clicked();

    void on_btn_background_plc_recovery_2_clicked();

    void on_btn_background_plc_putIn_clicked();

    void on_btn_background_plc_out_clicked();

    void on_btn_background_plc_back_clicked();

    void on_btn_background_test_clicked();

    void on_btn_background_detect_volume_clicked();

//    void on_btn_store_multi_takeNumberPicAgain_clicked();

    void on_btn_background_plc_XY_putIn_clicked();

    void on_btn_background_plc_XY_out_clicked();

    void on_btn_background_plc_elevator_up_clicked();

    void on_btn_background_plc_elevator_down_clicked();


    void on_btn_background_takeE1Volume_clicked();

    void on_btn_background_takeE2Volume_clicked();

    void on_btn_background_quitProgram_clicked();

    void on_btn_store_multi_changeBox_clicked();


    void on_tableView_store_multi_doubleClicked(const QModelIndex &index);

    void on_btn_background_searchItem_clicked();

    void on_btn_background_askBoxDownByBtn_back_clicked();

    void on_btn_background_askBoxDownByBtn_box1_clicked();

    void on_btn_background_askBoxDownByBtn_store_clicked();

    void on_btn_background_askBoxDownByBtn_box2_clicked();

    void on_btn_background_askBoxDownByBtn_box3_clicked();

    void on_btn_background_askBoxDownByBtn_box4_clicked();

    void on_btn_background_askBoxDownByBtn_box5_clicked();

    void on_btn_background_askBoxDownByBtn_box6_clicked();

    void on_btn_background_askBoxDownByBtn_box7_clicked();

    void on_btn_background_askBoxDownByBtn_box8_clicked();

    void on_btn_background_askBoxDownByBtn_box9_clicked();

    void on_btn_background_askBoxDownByBtn_box10_clicked();

    void on_btn_background_askBoxDownByBtn_box11_clicked();

    void on_btn_background_askBoxDownByBtn_box12_clicked();

    void on_btn_background_askBoxDownByBtn_box13_clicked();

    void on_btn_background_askBoxDownByBtn_box14_clicked();

    void on_btn_background_askBoxDownByBtn_box15_clicked();

    void on_btn_background_askBoxDownByBtn_box16_clicked();

    void on_btn_background_askBoxDownByBtn_box17_clicked();

    void on_btn_background_askBoxDownByBtn_box18_clicked();

    void on_btn_background_askBoxDownByBtn_box19_clicked();

    void on_btn_background_askBoxDownByBtn_box20_clicked();

    void on_btn_background_askBoxDownByBtn_box21_clicked();

    void on_btn_background_askBoxDownByBtn_box22_clicked();

    void on_btn_background_askBoxDownByBtn_box23_clicked();

    void on_btn_background_askBoxDownByBtn_box24_clicked();

    void on_btn_background_searchItemsByBoxId_clicked();

    void on_btn_background_showItemsByBoxId_back_clicked();


public slots:


    //for clipboard
    void slot_clipboardmanager_nameImage(QImage img);
    //for jobmanager
    void slot_jobmanager_boxUpArrive();
    void slot_jobmanager_boxDownArrive();
    void slot_jobmanager_modbus_errors(QVector<QString> errors);
    void slot_jobmanager_modbux_fixed();
    void slot_jobmanager(Sig_Jobmanagement_Mainwindow signal);


    //background table

    void slot_jobmanager_emptyBoxDetected();

    //for box size detect
//    void slot_dwsmanager_boxsizeupdate(DWS_Box buff);

    //for scan number
    void slot_jobmanager_phoneNumber(QString number);
    //for itemmanager
    void slot_itemmanager(Sig_ItemManagement_Mainwindow sig);
    //for take barcode
    void slot_clipboard(Sig_Clipmanagement_Mainwindow sig);
    //for modbus
    void slot_modbusmanager(Sig_ModbusManagement_MainWindow sig);
    //for realsence
    void slot_realsence(Sig_RealsenceManagement_MainWindow sig);

signals:
    //page store single
    void sig_jobmanager_askFitBoxDown(Item_FamilyMart_Size);
    void sig_jobmanager_ItemInBox(QVector<Item_FamilyMart>);
    void sig_jobmanager_ChangeBoxMoreFit(Item_FamilyMart);
    void sig_jobmanager_JustUpBox();

    //page store multi
    void sig_jobmanager_askMostEmptyBoxDown();
    //for dws
//    void sig_dwsmanager_start();
//    void sig_dwsmanager_stop();

    //page take customer
    void sig_jobmanager_searchItemInfos(QString phonenumber);
    void sig_jobmanager_removeItems(QVector<QString>);
    void sig_jobmanager_askBoxDownByItems(QVector<Item_FamilyMart>);

    //page take sevenday
    void sig_jobmanager_searchItemsBeyondDays(int days);

    //for scan pen
    void sig_clipboardmanager_askNameImage();
    void sig_clipboardmanager_askNumberImage();

    //detect empty box
    void sig_jobmanager_detectEmptyBox();

    //background table
    void sig_jobmanager_allItems();

    //detect box size
    void sig_dwsmanager_start();
    void sig_dwsmanager_stop();

    //for modbus
    void sig_jobmanager_modbus_plc_recovery();

    void sig_jobmanager(Sig_Mainwindow_Jobmanagement);

    //for test
    void sig_realsencemanager_test();
    void sig_realsencemanager_detectvolume();
    void sig_realsencemanager_test_format(Sig_MainWindow_RealsenceManagement);
    void sig_modbusmanager_orders(Sig_Mainwindown_Modbusmanagement);

    //for item
    void sig_itemmanager(Sig_Mainwindow_Itemmanagement sig);


    //for multistore take barcode
    void sig_clipboard(Sig_Mainwindow_Clipmanagement sig);

    //for quit program
    void sig_quitProgram();

    //for boxmanager
    void sig_boxManager(Sig_Mainwindow_Boxmanagement sig);
private:
    void askItemsByNumber(QString number);
    void askItemsByBeyongdDay(int day);
    void askItemByBarcode(QString barcode);
    void askAllItems();
    void askBoxDownByItems(QVector<Item_FamilyMart> items);
    void askBoxUpByItems(QVector<Item_FamilyMart> items);
    void askBoxDownMostEmpty();
    void boxDown(int boxId);
    void boxInFinished();
    void show_ItemsInfoSearchedByPhoneNumber(QVector<Item_FamilyMart> vec_item);
    void show_ItemsInfoSearchedByDay(QVector<Item_FamilyMart> vec_item);
    //for take_customer page
    void show_ItemsByBoxId(QVector<Item_FamilyMart> vec_item);
    void show_ItemsInfo_takePage(QVector<Item_FamilyMart> vec_item);

    void show_allItems(QVector<Item_FamilyMart> items);
    void show_ItemByBarcode(Item_FamilyMart item);
    void detect_emptyBoxVolume();
    void ask_plc_recovery();

    void multiStore_getBarcode(QString barcode);
    void keyPressEvent(QKeyEvent  *event);
    //emit sig to modbus xy out
    void emitSigToModbusXyOut(int boxId,int elevatorId);
    //to sort items
    QVector<Item_FamilyMart> sortItemsByNameAndPhone(QVector<Item_FamilyMart> items);
    QVector<Item_FamilyMart> sortItemsByBoxId(QVector<Item_FamilyMart> items);
    //for takepage select row by barcode
    void takepage_selectRowByBarcode(QString barcode);
private:
    Ui::MainWindow *ui;
    MyNumberDialog* numberkeyboard_take_customer;
    MyNumberDialog* numberboard_store_multi;
    MyNumberDialog* numberboard_store_single;

    QLabel* label_box_moving_;
    QImage* label_box_moving_img_moving_;
    QImage* label_box_moving_img_arrive_;

    //tmp items info
    Item_FamilyMart store_single_temp_item_;
    QVector<Item_FamilyMart> vec_tmp_item_;

    //for page store multi's table view
    QStandardItemModel * table_store_multi_model_;
    int count_item_selected_ = 0;

    //for page take customer's table widget
    int table_take_customer_count_selected_ = 0;
    QMap<int,Item_FamilyMart> table_take_customer_map_row_item_;
    //for page take customer takepage's table widget
    QVector<QString> table_take_customer_tapepagem_itemId_;
    bool take_takepage_hasnextbox_ = false;

    //for page take sevenday's table view
    QStandardItemModel * table_take_servenday_model_;
    int count_table_take_servenday_model_selected_ = 0;
    QMap<int,Item_FamilyMart> table_take_servenday_map_row_item_;

    //for name image
    QImage nameImage_;

    //for plc error
    int recover_page_index_ = -1;

    //for takepage
    QVector<Item_FamilyMart> items_takepage_;
    int pageIndex_prepage_ = -1;
    //for tables
    QImage* img_tables_selected_;
    QImage* img_tables_unselected_;
    //for barcode
    QString barcode_;
    QString tmp_barcode_;
    QString barcode_nine_;
    QString barcode_sixteen_;
    //for multi page : change box
    bool flag_multi_store_changeBox = false;
    //for takepage : multi box take
    bool flag_takepage_nextBoxPrepared_ = false;
    //background : for ask box down by id
    int background_askBoxDownByBtn_boxDownId_ = 0;

};
#endif // MAINWINDOW_H
