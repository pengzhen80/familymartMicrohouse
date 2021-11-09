#ifndef COMMON_H
#define COMMON_H

#include<QByteArray>
#include<QString>
#include<QImage>
#include<QSet>
#include <QMetaType>

//for barcode scanner
#define BARCODE_SIZE 25

//for web api
#define WEB_API_URL "https://ecfmetest.fme.com.tw / B2COrderAdd/B2COrderAdd.ashx"
uchar const WEB_API_KEY_PUBLIC[] = "rPhC3chnke0l7oSDDYCA3iaunyj48g40l7oDDYCALVKqZF73VXAkxDkeFOapnwyCTv6BlOCAkxDQnwNTmeEHUEHUDDwk9uJCnlGVpJabJqf9eQwvdecJ1LVKVX7HbbJJ";
uchar const WEB_API_KEY_PRIVATE[] = "xnwyCTYyiAWIAkxDkRDYCAkxhC38p9apbwS3SZVdL2oSZVdL3IjwthCAkxDkxDkRwHZwvdeceRXjvUp9VVEnjvU9CnvW6n36nQnNvR9YpJabapbwS3TvgmO045ZGeHHe";


//for box
#define DEFAULT_BOX_NUMBER 24
#define BOX_EMPTY_LEFT_VOLUME (33*45*31)

#define TIME_BOX_MOVING_UP 5
#define TIME_BOX_MOVING_DOWN 5

#define BOX_EMPTY_VOLUME 1194

//for DB
#define DB_SERVER_ADDRESS "localhost"
#define DB_SERVER_PORT 3306
#define DB_SERVER_DBNAME "test"
#define DB_SERVER_USERNAME "root"
#define DB_SERVER_PASSWORD "12345678"

//for name image path
#define NAME_IMAGE_PATH "./nameImage/"
#define NAME_IMAGE_FILENAME_EXTENSION ".png"

//for serila port management
#define BAUD_TRANSSYSTEM 9600

//for realsense
#define DWS_SERIALNUMBER "f1120538"
#define REALSENSE_SERIALNUMBER "f1062548"
#define REALSENSE_TOP_POINT_X_ELEVATOR1 380
#define	REALSENSE_TOP_POINT_Y_ELEVATOR1 120
#define REALSENSE_TOP_POINT_X_ELEVATOR2 130
#define	REALSENSE_TOP_POINT_Y_ELEVATOR2 120

#define REALSENSE_TOP_PIXEL_WIDTH 170
#define REALSENSE_TOP_PIXEL_LENGTH 250

//distance is cm
#define REALSENSE_BOX_BOTTOM_DISTANCE 121
#define REALSENSE_BOX_LENTH 45
#define REALSENSE_BOX_WIGHT 33

#define DEVIDE_LENTH_PIXEL 5

#define REALSENSE_BOX_FULLVOLUME 40000 //cm3
//for test server
#define TEST_SERVER_PORT 50009

//for log management
#define LOG_DIR "./log/"

//for config management
#define CONFIG_BOX_EMPTY_VOLUME "BoxLeftVolume"

//for web api
#define WEB_API_REQUEST_ADDRESS_OFFICIAL "203.73.24.218"
#define WEB_API_SERVER_ADDRESS_OFFICIAL "210.68.110.6"
#define WEB_API_SERVER_PORT_OFFICIAL 80
#define WEB_API_REQUEST_ADDRESS_TEST1 "124.219.54.69"
#define WEB_API_REQUEST_ADDRESS_TEST2 "60.250.228.29"
#define WEB_API_TEST "211.20.149.130"

//for modbus
#define MODBUS_IPADDRESS "127.0.0.1"
#define MODBUS_PORT 502
#define MODBUS_READ_ADDRESS 999
#define MODBUS_WRITE_ADDRESS 999
#define MODBUS_ELEVATOR_ID_1 1
#define MODBUS_ELEVATOR_ID_2 2
#define MODBUS_CHECK_HEART_AND_ERROR 3 //s
#define MODBUS_DEFAULT_ELEVATOR_ID 2

#define MODBUS_ERROR_BYTE1_EMERGENCY              0b0000000000000001
#define MODBUS_ERROR_BYTE1_X1_SERVER              0b0000000000000010
#define MODBUS_ERROR_BYTE1_X2_SERVER              0b0000000000000100
#define MODBUS_ERROR_BYTE1_Y_SERVER               0b0000000000001000
#define MODBUS_ERROR_BYTE1_ELEVATOR1_SERVER       0b0000000000010000
#define MODBUS_ERROR_BYTE1_ELEVATOR2_SERVER       0b0000000000100000
#define MODBUS_ERROR_BYTE1_X1_POSITION            0b0000000001000000
#define MODBUS_ERROR_BYTE1_X2_POSITION            0b0000000010000000
#define MODBUS_ERROR_BYTE1_Y_POSITION             0b0000000100000000
#define MODBUS_ERROR_BYTE1_ELEVATOR1_POSITION     0b0000001000000000
#define MODBUS_ERROR_BYTE1_ELEVATOR2_POSITION     0b0000010000000000
#define MODBUS_ERROR_BYTE1_SUBMECHINE_CONNECTION  0b1000000000000000

#define MODBUS_ERROR_BYTE2_ELEVATOR1_PUTIN        0b0000000000010000
#define MODBUS_ERROR_BYTE2_ELEVATOR1_OPENGATE     0b0000000000100000
#define MODBUS_ERROR_BYTE2_ELEVATOR1_CANNOT_OUT   0b0000000010000000
#define MODBUS_ERROR_BYTE2_ELEVATOR1_BOTH_IN_OUT  0b0000000100000000
#define MODBUS_ERROR_BYTE2_ELEVATOR2_PUTIN        0b0100000000000000
#define MODBUS_ERROR_BYTE2_ELEVATOR2_OPENGATE     0b1000000000000000

#define MODBUS_ERROR_BYTE3_ELEVATOR2_CANNOT_OUT   0b0000000000000010
#define MODBUS_ERROR_BYTE3_ELEVATOR2_BOTH_IN_OUT  0b0000000000000100
#define MODBUS_ERROR_BYTE3_NO_STORE_POSITION      0b0000000100000000
#define MODBUS_ERROR_BYTE3_OUT                    0b0000001000000000
#define MODBUS_ERROR_BYTE3_NONUMBER               0b0000010000000000
#define MODBUS_ERROR_BYTE3_NOBOXTOTAKE            0b0000100000000000
#define MODBUS_ERROR_BYTE3_BOXPUSHFAILED          0b0001000000000000

#define MODBUS_ERROR_BYTE4_X_FORWARD_LIMITED        0b0000000000000100
#define MODBUS_ERROR_BYTE4_X_BACKWARD_LIMITED       0b0000000000001000
#define MODBUS_ERROR_BYTE4_Y_FORWARD_LIMITED        0b0000000000010000
#define MODBUS_ERROR_BYTE4_Y_BACKWARD_LIMITED       0b0000000000100000
#define MODBUS_ERROR_BYTE4_ELEVATOR1_UP_LIMITED     0b0000000001000000
#define MODBUS_ERROR_BYTE4_ELEVATOR1_DOWN_LIMITED   0b0000000010000000
#define MODBUS_ERROR_BYTE4_ELEVATOR2_UP_LIMITED     0b0000000100000000
#define MODBUS_ERROR_BYTE4_ELEVATOR2_DOWN_LIMITED   0b0000001000000000

struct WebApi_LocalClient_Msg
{
    char barcode[26];
    char phone[4];
    char name[40];
};

struct MyFile_Image
{
    int width;
    int height;
    QByteArray file_data;
};

enum DWS_DETECTSTATE
{
    DWS_DETECTSTATE_DETECT = 0,
    DWS_DETECTSTATE_STOP = 1,
};

struct Point_Space
{
    int x;
    int y;
    int z;
    bool occupyed = false;
};

//structs of box
//width is 33cm, length is 45cm, height is 31cm;
struct Box_Space
{
    Point_Space points[33][45][31];
    Box_Space() {
        for (int i_x = 0; i_x < 33; i_x++)
        {
            for (int i_y = 0; i_y < 45; i_y++)
            {
                for (int i_z = 0; i_z < 31; i_z++)
                {
                    points[i_x][i_y][i_z].x = i_x;
                    points[i_x][i_y][i_z].y = i_y;
                    points[i_x][i_y][i_z].z = i_z;
                    points[i_x][i_y][i_z].occupyed = false;
                }
            }
        }
    }
};

//size is cm
struct Box_Warehouse_Size
{
    int width = 33;
    int length = 45;
    int height = 31;
};

struct Box_Warehouse_Position
{
    int pos;
};
//volume is cm*cm*cm
struct Box_Warehouse_Infos_Left_Volume
{
    float volume = 33*45*31;
};

struct Box_Warehouse_Infos
{
    Box_Warehouse_Infos_Left_Volume left_volume;
};

//my box
struct Box_Warehouse
{
    int id;
    float left_volume = BOX_EMPTY_LEFT_VOLUME;
//    Box_Warehouse_Size size;
//    Box_Warehouse_Position position;
//    Box_Warehouse_Infos infos;
//    QSet<QString> set_item;
};

//struct of items
struct Item_FamilyMart_Size
{
    int width;
    int length;
    int height;
};

struct Item_FamilyMart_Info
{
    QString barcode;
    QString name;
    QString phone_number;
    QString date_arrive = "00/00";
    QString weekday="";
    QImage name_image;
};

struct Item_FamilyMart_Box
{
    int box_id;
};

struct Item_FamilyMart
{
    QString id;
    Item_FamilyMart_Size size;
    Item_FamilyMart_Info infos;
    Item_FamilyMart_Box box;
};

//for test: server and client buf
enum TestServer_BoxAction
{
    TestServer_BoxAction_UP = 0,
    TestServer_BoxAction_DOWN
};

struct TestServer_Buff
{
    int boxId = -1;
    TestServer_BoxAction action;
};

//for log management
//enum LOGMANAGEMENT_SEVERITYLEVEL
//{
//    LOGMANAGEMENT_SEVERITYLEVEL_DEBUG = 0,
//    LOGMANAGEMENT_SEVERITYLEVEL_INFO,
//    LOGMANAGEMENT_SEVERITYLEVEL_WARNING,
//    LOGMANAGEMENT_SEVERITYLEVEL_CRITICAL,
//    LOGMANAGEMENT_SEVERITYLEVEL_FATAL,
//};

template<typename T>
struct Array_TwoDimensional
{
    QVector<T> data;
    unsigned int x=0;
    unsigned int y=0;
};

Q_DECLARE_METATYPE(Array_TwoDimensional<float>);


//protocol for modbus tcp
enum MODBUS_ORDER
{
    MODBUS_ORDER_PUTIN = 1,
    MODBUS_ORDER_OUT,
    MODBUS_ORDER_PUTIN_AND_OUT,
    MODBUS_ORDER_RESTORE,
    MODBUS_ORDER_CLEANERRORS,
    MODBUS_ORDER_AUTOCANCEL,
    MODBUS_ORDER_NOORDER,
    MODBUS_ORDER_DIRECT_PUTIN,
    MODBUS_ORDER_DIRECT_OUT,
    MODBUS_ORDER_ELEVATOR_1_UP,
    MODBUS_ORDER_ELEVATOR_2_UP,
    MODBUS_ORDER_ELEVATOR_1_DOWN,
    MODBUS_ORDER_ELEVATOR_2_DOWN,
    MODBUS_ORDER_XY_PUTIN,
    MODBUS_ORDER_XY_OUT,
};

#define MODBUS_PCORDER_PUTIN(boxInId,boxOutId,elevatorId)           {0,1, boxInId,boxOutId,elevatorId,0,0,0,0,0}
#define MODBUS_PCORDER_OUT(boxInId,boxOutId,elevatorId)             {0,2, boxInId,boxOutId,elevatorId,0,0,0,0,0}
#define MODBUS_PCORDER_PUTIN_AND_OUT(boxInId,boxOutId,elevatorInId,elevatorOutId)   {0,3, boxInId,boxOutId,elevatorInId,elevatorOutId,0,0,0,0}
#define MODBUS_PCORDER_RESTORE {0,111, 0,0,0,0,0,0,0,0}
#define MODBUS_PCORDER_CLEANERRORS {0,444, 0,0,0,0,0,0,0,0}
#define MODBUS_PCORDER_AUTOCANCEL {0,999, 0,0,0,0,0,0,0,0}
#define MODBUS_PCORDER_NOORDER {0,0, 0,0,0,0,0,0,0,0}
#define MODBUS_PCORDER_DIRECT_PUTIN(boxInId,elevatorInId) {0,11, boxInId,0,elevatorInId,0,0,0,0,0}
#define MODBUS_PCORDER_DIRECT_OUT(elevatorId) {0,22, 0,0,0,elevatorId,0,0,0,0}
#define MODBUS_PCORDER_ELEVATOR_1_UP {0,11,0,0,0,0,0,0,0,0}
#define MODBUS_PCORDER_ELEVATOR_2_UP {0,12,0,0,0,0,0,0,0,0}
#define MODBUS_PCORDER_ELEVATOR_1_DOWN {0,21,0,0,0,0,0,0,0,0}
#define MODBUS_PCORDER_ELEVATOR_2_DOWN {0,22,0,0,0,0,0,0,0,0}
#define MODBUS_PCORDER_XY_PUTIN(boxId,elevatorInId) {0,31,0,0,0,boxId,0,elevatorInId,0,0}
#define MODBUS_PCORDER_XY_OUT(boxId,elevatorOutId)  {0,32,0,0,0,0,boxId,elevatorOutId,0,0}

enum MODBUS_STATE
{
    MODBUS_STATE_PUTIN = 1,
    MODBUS_STATE_OUT=2,
    MODBUS_STATE_PUTIN_AND_OUT=3,
    MODBUS_STATE_NEED_RESTORE=111,
    MODBUS_STATE_RESTORING=1111,
    MODBUS_STATE_ERROR=444,
    MODBUS_STATE_AUTOCANCEL=999,
    MODBUS_STATE_AUTOFINISH=9999,
    MODBUS_STATE_IDLE=0,
};
enum MODBUS_ELEVATOR_STATE_STATE
{
    MODBUS_ELEVATOR_STATE_STATE_NOBOX = 0,
    MODBUS_ELEVATOR_STATE_STATE_HASBOX = 1,
};

enum MODBUS_ELEVATOR_STATE_POSITION
{
    MODBUS_ELEVATOR_STATE_POSITION_UP = 0,
    MODBUS_ELEVATOR_STATE_POSITION_DOWN = 1,
    MODBUS_ELEVATOR_STATE_POSITION_MOVING = 2,
};

enum MODBUS_XY_STATE
{
    MODBUS_XY_STATE_IDLE = 0,
    MODBUS_XY_STATE_MOVING_NOBOX = 1,
    MODBUS_XY_STATE_MOVING_HASBOX = 2,
};

struct ModbusState_XyState
{
    MODBUS_XY_STATE state;
    int target_position;
};

struct ModbusState
{
    MODBUS_STATE state;
    int boxId_putIn;
    int boxId_out;
    int elevatorId;
    ModbusState_XyState xy_state;
    MODBUS_ELEVATOR_STATE_POSITION elevator_1_position;
    MODBUS_ELEVATOR_STATE_STATE elevator_1_state;
    MODBUS_ELEVATOR_STATE_POSITION elevator_2_position;
    MODBUS_ELEVATOR_STATE_STATE elevator_2_state;
    int flag;
    uint16_t errors[6];
};

struct Modbus_Order
{
    MODBUS_ORDER order_type;
    int boxIn_id;
    int boxOut_id;
    int elevatorIn_id;
    int elevatorOut_id;
};

//struct Modbus_Elevator
//{
//    MODBUS_ELEVATOR_STATE elevator_1 = MODBUS_ELEVATOR_STATE_UNKOWN;
//    MODBUS_ELEVATOR_STATE elevator_2 = MODBUS_ELEVATOR_STATE_UNKOWN;
//};

//struct JobManagement_Elevator
//{
//    int current_elevator_id = -1;
//    MODBUS_ELEVATOR_STATE state = MODBUS_ELEVATOR_STATE_DOWN;
//};

//信號和槽機制，如果信號沒有參數的時候，
//就可以只是用一個connect，而這個信號帶一個參數表示參數類型，不同的信號編成enum參數；
//以此類推：兩個類之間需要溝通的時候用一個connect，帶兩個參數，第一個參數是信號類型，第二個參數是信號的值；
//design signals from jobmanagement to mainwindow
enum SIG_JOBMANAGEMENT_MAINWINDOW
{
    SIG_JOBMANAGEMENT_MAINWINDOW_MODBUS_RECOVERY_SUCCESS = 0,
    SIG_JOBMANAGEMENT_MAINWINDOW_BOXDOWN,
    SIG_JOBMANAGEMENT_MAINWINDOW_BOX_XYIN,
//    SIG_JOBMANAGEMENT_MAINWINDOW_BOX_CAN_UP,
    SIG_JOBMANAGEMENT_MAINWINDOW_NEXT_BOX_PREPARED,
};

struct Sig_Jobmanagement_Mainwindow_BoxDown
{
    int boxId;
};

struct Sig_Jobmanagement_Mainwindow
{
    SIG_JOBMANAGEMENT_MAINWINDOW type;
    Sig_Jobmanagement_Mainwindow_BoxDown value_boxDown;
};



enum SIG_MAINWINDOWN_JOBMANAGEMENT
{
    SIG_MAINWINDOW_JOBMANAGEMENT_MODBUS_DIRECT_PUTIN = 0,
    SIG_MAINWINDOW_JOBMANAGEMENT_MODBUS_DIRECT_OUT,
    SIG_MAINWINDOW_JOBMANAGEMENT_MODBUS_PUTIN,
    SIG_MAINWINDOW_JOBMANAGEMENT_MODBUS_OUT,
    SIG_MAINWINDOWN_JOBMANAGEMENT_ASK_BOXDOWN_BYITEMS,
    SIG_MAINWINDOWN_JOBMANAGEMENT_ASK_BOXDOWN_MOSTEMPTY,
    SIG_MAINWINDOWN_JOBMANAGEMENT_ASK_BOXUP,
    SIG_MAINWINDOWN_JOBMANAGEMENT_INIT_MODBUS_XYOUT,
    SIG_MAINWINDOWN_JOBMANAGEMENT_INIT_MODBUS_XYIN,
    SIG_MAINWINDOWN_JOBMANAGEMENT_INIT_MODBUS_ELEVATOR_DOWN,
    SIG_MAINWINDOWN_JOBMANAGEMENT_INIT_MODBUS_ELEVATOR_UP,
    SIG_MAINWINDOWN_JOBMANAGEMENT_DETECT_EMPTYBOXVOLUME_VOLUME_ELEVATOR1,
    SIG_MAINWINDOWN_JOBMANAGEMENT_DETECT_EMPTYBOXVOLUME_VOLUME_ELEVATOR2,
    SIG_MAINWINDOWN_JOBMANAGEMENT_PLC_RECOVERY,
    SIG_MAINWINDOWN_JOBMANAGEMENT_ADDNEWITEMS,
//    SIG_MAINWINDOWN_JOBMANAGEMENT_ADDNEWITEMS_WITH_NEXT_EMPTYBOX,
};

struct Sig_Mainwindow_Jobmanagement_Direct_PutIn
{
    int box_id = -1;
    int elevator_id = -1;
};

struct Sig_Mainwindow_Jobmanagement_PutIn
{
    int box_id = -1;
    int elevator_id = -1;
};

struct Sig_Mainwindow_Jobmanagement_Direct_Out
{
    int elevator_id = -1;
};

struct Sig_Mainwindow_Jobmanagement_Out
{
    int box_id = -1;
    int elevator_id = -1;
};
struct Sig_Mainwindow_Jobmanagement_Init_Modbus_XY_Out
{
    int box_id = -1;
    int elevator_id = -1;
};
struct Sig_Mainwindow_Jobmanagement_Init_Modbus_XY_In
{
    int box_id = -1;
    int elevator_id = -1;
};
struct Sig_Mainwindow_Jobmanagement_Init_Modbus_ElevatorUp
{
    int elevator_id = -1;
};
struct Sig_Mainwindow_Jobmanagement_Init_Modbus_ElevatorDown
{
    int elevator_id = -1;
};
struct Sig_Mainwindow_Jobmanagement_Ask_BoxDownByItems
{
    QVector<Item_FamilyMart> items;
};
struct Sig_Mainwindow_Jobmanagement_AddNewItems
{
    QVector<Item_FamilyMart> items;
};
struct Sig_Mainwindow_Jobmanagement
{
    SIG_MAINWINDOWN_JOBMANAGEMENT type;
    Sig_Mainwindow_Jobmanagement_Direct_PutIn value_direct_putin;
    Sig_Mainwindow_Jobmanagement_Direct_Out value_direct_out;
    Sig_Mainwindow_Jobmanagement_PutIn value_putIn;
    Sig_Mainwindow_Jobmanagement_Out value_out;
    Sig_Mainwindow_Jobmanagement_Ask_BoxDownByItems value_askBoxDownByItems;
    Sig_Mainwindow_Jobmanagement_AddNewItems value_addNewItems;
    Sig_Mainwindow_Jobmanagement_Init_Modbus_XY_Out value_Init_Modbus_XyOut;
    Sig_Mainwindow_Jobmanagement_Init_Modbus_XY_In value_Init_Modbus_XyIn;
    Sig_Mainwindow_Jobmanagement_Init_Modbus_ElevatorDown value_Init_Modbus_ElevatorDown;
    Sig_Mainwindow_Jobmanagement_Init_Modbus_ElevatorUp value_Init_Modbus_ElevatorUp;
};

//test: sig from mainwindown to modbus
enum SIG_MAINWINDOWN_MODBUSMANAGEMENT
{
    SIG_MAINWINDOWN_MODBUSMANAGEMENT_XY_PUTIN = 0,
    SIG_MAINWINDOWN_MODBUSMANAGEMENT_XY_OUT,
    SIG_MAINWINDOWN_MODBUSMANAGEMENT_ELEVATOR_UP,
    SIG_MAINWINDOWN_MODBUSMANAGEMENT_ELEVATOR_DOWN,
};

struct Sig_Mainwindown_Modbusmanagement_XY
{
    int boxId = -1;
    int elevatorId = -1;
};

struct Sig_Mainwindown_Modbusmanagement_Elevator
{
//    int boxId = -1;
    int elevatorId = -1;
};

struct Sig_Mainwindown_Modbusmanagement
{
    SIG_MAINWINDOWN_MODBUSMANAGEMENT type;
    Sig_Mainwindown_Modbusmanagement_XY value_xy;
    Sig_Mainwindown_Modbusmanagement_Elevator value_elevator;
};

enum SIG_MAINWINDOWN_ITEMMANAGEMENT
{
    SIG_MAINWINDOWN_ITEMMANAGEMENT_ASKITEMS_BYNUMBER,
    SIG_MAINWINDOWN_ITEMMANAGEMENT_ASKITEMS_BYBEYONDDAY,
    SIG_MAINWINDOWN_ITEMMANAGEMENT_ASKITEM_BYBARCODE,
    SIG_MAINWINDOWN_ITEMMANAGEMENT_ASKITEMS_ALL,
    SIG_MAINWINDOWN_ITEMMANAGEMENT_ASKITEMS_BYBOXID,
    SIG_MAINWINDOWN_ITEMMANAGEMENT_ADDITEMS,
    SIG_MAINWINDOWN_ITEMMANAGEMENT_REMOVEITEMS,
};

struct Sig_Mainwindow_Itemmanagement_AskItems_ByNumber
{
    QString number;
};

struct Sig_Mainwindow_Itemmanagement_AskItems_ByBeyondDay
{
    int day;
};
struct Sig_Mainwindow_Itemmanagement_AskItems_ByBoxId
{
    int boxId;
};
struct Sig_Mainwindow_Itemmanagement_AskItem_ByBarcode
{
    QString barcode;
};
struct Sig_Mainwindow_Itemmanagement_AddItems
{
    QVector<Item_FamilyMart> items;
};
struct Sig_Mainwindow_Itemmanagement_RemoveItems
{
    QVector<Item_FamilyMart> items;
};

struct Sig_Mainwindow_Itemmanagement
{
    SIG_MAINWINDOWN_ITEMMANAGEMENT type;
    Sig_Mainwindow_Itemmanagement_AskItems_ByNumber value_askItemsByNumber;
    Sig_Mainwindow_Itemmanagement_AskItems_ByBeyondDay value_askItemsByBeyondDay;
    Sig_Mainwindow_Itemmanagement_AskItem_ByBarcode value_askItemByBarcode;
    Sig_Mainwindow_Itemmanagement_AddItems value_addItems;
    Sig_Mainwindow_Itemmanagement_RemoveItems value_removeItems;
    Sig_Mainwindow_Itemmanagement_AskItems_ByBoxId value_askItemsByBoxId;
};

enum SIG_DBMANAGEMENT_BOXMANAGEMENT
{
    SIG_DBMANAGEMENT_BOXMANAGEMENT_INITBOX=0,
};
struct Sig_DBManagement_BoxManagement_InitBox
{
    QVector<Box_Warehouse> boxs;
};
struct Sig_DBManagement_BoxManagement
{
    SIG_DBMANAGEMENT_BOXMANAGEMENT type;
    Sig_DBManagement_BoxManagement_InitBox value_initbox;
};

enum SIG_DBMANAGEMENT_ITEMMANAGEMENT
{
    SIG_DBMANAGEMENT_ITEMMANAGEMENT_INITITEM=0,
};
struct Sig_DBManagement_ItemManagement_InitItem
{
    QVector<Item_FamilyMart> items;
    QVector<Item_FamilyMart> itemsNew;
};
struct Sig_DBManagement_ItemManagement
{
    SIG_DBMANAGEMENT_ITEMMANAGEMENT type;
    Sig_DBManagement_ItemManagement_InitItem value_initItem;
};

enum SIG_BOXMANAGEMENT_DBMANAGEMENT
{
    SIG_BOXMANAGEMENT_DBMANAGEMENT_UPDATE_BOXVOLUME=0,
};
struct Sig_BoxManagement_DBManagement_Update_BoxVolume
{
    int box_id;
    float volume;
};
struct Sig_BoxManagement_DBManagement
{
    SIG_BOXMANAGEMENT_DBMANAGEMENT type;
    Sig_BoxManagement_DBManagement_Update_BoxVolume value_update_boxvolume;
};

enum SIG_BOXMANAGEMENT_JOBMANAGEMENT
{
    SIG_BOXMANAGEMENT_JOBMANAGEMENT_MOSTEMPTYBOX=0,
};
struct Sig_BoxManagement_JobManagement_MostEmptyBox
{
    int box_id;
};
struct Sig_BoxManagement_JobManagement
{
    SIG_BOXMANAGEMENT_JOBMANAGEMENT type;
    Sig_BoxManagement_JobManagement_MostEmptyBox value_mostEmptyBox;
};

enum SIG_ITEMMANAGEMENT_DBMANAGEMENT
{
    SIG_ITEMMANAGEMENT_DBMANAGEMENT_ADDITEM=0,
    SIG_ITEMMANAGEMENT_DBMANAGEMENT_ADDITEM_NEW,
    SIG_ITEMMANAGEMENT_DBMANAGEMENT_REMOVEITEM,
};
struct Sig_ItemManagement_DBManagement_AddItem
{
    QVector<Item_FamilyMart> items;
};

struct Sig_ItemManagement_DBManagement_AddItem_New
{
    QVector<Item_FamilyMart> items;
};

struct Sig_ItemManagement_DBManagement_RemoveItem
{
    QVector<QString> items;
};
struct Sig_ItemManagement_DBManagement
{
    SIG_ITEMMANAGEMENT_DBMANAGEMENT type;
    Sig_ItemManagement_DBManagement_AddItem value_addItem;
    Sig_ItemManagement_DBManagement_AddItem_New value_addItemsNew;
    Sig_ItemManagement_DBManagement_RemoveItem value_removeItem;
};

enum SIG_ITEMMANAGEMENT_MAINWINDOW
{
    SIG_ITEMMANAGEMENT_MAINWINDOW_ITEMS_BYNUMBER,
    SIG_ITEMMANAGEMENT_MAINWINDOW_ITEMS_BYDAY,
    SIG_ITEMMANAGEMENT_MAINWINDOW_ITEM_BYBARCODE,
    SIG_ITEMMANAGEMENT_MAINWINDOW_ITEMS_ALL,
    SIG_ITEMMANAGEMENT_MAINWINDOW_ITEMS_BYBOXID,
};
struct Sig_ItemManagement_Mainwindow_ItemsByNumber
{
    QVector<Item_FamilyMart> items;
};
struct Sig_ItemManagement_Mainwindow_ItemsByBoxId
{
    QVector<Item_FamilyMart> items;
};
struct Sig_ItemManagement_Mainwindow_ItemsByDay
{
    QVector<Item_FamilyMart> items;
};
struct Sig_ItemManagement_Mainwindow_ItemsAll
{
    QVector<Item_FamilyMart> items;
};
struct Sig_ItemManagement_Mainwindow_ItemByBarcode
{
    Item_FamilyMart item;
};
struct Sig_ItemManagement_Mainwindow
{
    SIG_ITEMMANAGEMENT_MAINWINDOW type;
    Sig_ItemManagement_Mainwindow_ItemsByNumber value_itemsByNumber;
    Sig_ItemManagement_Mainwindow_ItemsByDay value_itemsByDay;
    Sig_ItemManagement_Mainwindow_ItemByBarcode value_itemByBarcode;
    Sig_ItemManagement_Mainwindow_ItemsAll value_itemsAll;
    Sig_ItemManagement_Mainwindow_ItemsByBoxId value_itemsByBoxId;
};

enum SIG_ITEMMANAGEMENT_JOBMANAGEMENT
{
    SIG_ITEMMANAGEMENT_JOBMANAGEMENT_ITEMS_BOXID,
    SIG_ITEMMANAGEMENT_JOBMANAGEMENT_ADDNEWITEMS_SUCCESS,
};
struct Sig_ItemManagement_JobManagement_ItemsBoxId
{
    QSet<int> boxIds;
};
struct Sig_ItemManagement_JobManagement
{
    SIG_ITEMMANAGEMENT_JOBMANAGEMENT type;
    Sig_ItemManagement_JobManagement_ItemsBoxId value_itemsBoxId;
};

enum SIG_JOBMANAGEMENT_BOXMANAGEMENT
{
    SIG_JOBMANAGEMENT_BOXMANAGEMENT_UPDATE_BOX_VOLUME,
    SIG_JOBMANAGEMENT_BOXMANAGEMENT_ASK_BOXMOSTEMPTY,
};
struct Sig_JobManagement_BoxManagement_Update_BoxVolume
{
    int box_id;
    float volume;
};
struct Sig_JobManagement_BoxManagement
{
    SIG_JOBMANAGEMENT_BOXMANAGEMENT type;
    Sig_JobManagement_BoxManagement_Update_BoxVolume value_update_boxvolume;
};

enum SIG_JOBMANAGEMENT_ITEMMANAGEMENT
{
    SIG_JOBMANAGEMENT_ITEMMANAGEMENT_ASK_ITEMS_BYBOXID,
    SIG_JOBMANAGEMENT_ITEMMANAGEMENT_ASK_BOXID_BYITEMS,
    SIG_JOBMANAGEMENT_ITEMMANAGEMENT_ADD_NEWITEMS,
};
struct Sig_JobManagement_ItemManagement_AskItemsByBoxId
{
    int box_id;
};
struct Sig_JobManagement_ItemManagement_AskBoxIdByItems
{
    QVector<Item_FamilyMart> items;
};
struct Sig_JobManagement_ItemManagement_AddNewItems
{
    QVector<Item_FamilyMart> items;
};
struct Sig_JobManagement_ItemManagement
{
    SIG_JOBMANAGEMENT_ITEMMANAGEMENT type;
    Sig_JobManagement_ItemManagement_AskItemsByBoxId value_askItemByBoxId;
    Sig_JobManagement_ItemManagement_AskBoxIdByItems value_askBoxIdByItems;
    Sig_JobManagement_ItemManagement_AddNewItems value_addNewItems;
};

enum SIG_JOBMANAGEMENT_CONFIGMANAGEMENT
{
    SIG_JOBMANAGEMENT_CONFIGMANAGEMENT_GET_EMPTYBOXSAMPLE_ELEVATOR1,
    SIG_JOBMANAGEMENT_CONFIGMANAGEMENT_GET_EMPTYBOXSAMPLE_ELEVATOR2,
    SIG_JOBMANAGEMENT_CONFIGMANAGEMENT_SET_EMPTYBOXSAMPLE_ELEVATOR1,
    SIG_JOBMANAGEMENT_CONFIGMANAGEMENT_SET_EMPTYBOXSAMPLE_ELEVATOR2,
};

struct Sig_JobManagement_ConfigManagement_SetEmptyBoxSample
{
    Array_TwoDimensional<float> box_sample;
    float box_empty_volume;
};

struct Sig_JobManagement_ConfigManagement
{
    SIG_JOBMANAGEMENT_CONFIGMANAGEMENT type;
    Sig_JobManagement_ConfigManagement_SetEmptyBoxSample value_setEmptyBoxSample;
};

enum SIG_JOBMANAGEMENT_MODBUSMANAGEMENT
{
    SIG_JOBMANAGEMENT_MODBUSMANAGEMENT_XY_PUTIN = 0,
    SIG_JOBMANAGEMENT_MODBUSMANAGEMENT_XY_OUT,
    SIG_JOBMANAGEMENT_MODBUSMANAGEMENT_ELEVATOR_UP,
    SIG_JOBMANAGEMENT_MODBUSMANAGEMENT_ELEVATOR_DOWN,
    SIG_JOBMANAGEMENT_MODBUSMANAGEMENT_PLC_RECOVERY,
};

struct Sig_JobManagement_Modbusmanagement_XY
{
    int boxId = -1;
    int elevatorId = -1;
};

struct Sig_JobManagement_Modbusmanagement_Elevator
{
//    int boxId = -1;
    int elevatorId = -1;
};

struct Sig_JobManagement_Modbusmanagement
{
    SIG_JOBMANAGEMENT_MODBUSMANAGEMENT type;
    Sig_JobManagement_Modbusmanagement_XY value_xy;
    Sig_JobManagement_Modbusmanagement_Elevator value_elevator;
};

enum SIG_JOBMANAGEMENT_REALSENCEMANAGEMENT
{
    SIG_JOBMANAGEMENT_REALSENCEMANAGEMENT_TAKE_EMPTYBOXVOLUME_ELEVATOR1,
    SIG_JOBMANAGEMENT_REALSENCEMANAGEMENT_TAKE_EMPTYBOXVOLUME_ELEVATOR2,
    SIG_JOBMANAGEMENT_REALSENCEMANAGEMENT_TAKE_BOXLEFTVOLUME,
    SIG_JOBMANAGEMENT_REALSENCEMANAGEMENT_TAKE_BOXLEFTVOLUME_ELEVATOR1,
    SIG_JOBMANAGEMENT_REALSENCEMANAGEMENT_TAKE_BOXLEFTVOLUME_ELEVATOR2,
};

struct Sig_JobManagement_RealsenceManagement
{
    SIG_JOBMANAGEMENT_REALSENCEMANAGEMENT type;
};

enum SIG_MAINWINDOW_REALSENCEMANAGEMENT
{
    SIG_MAINWINDOW_REALSENCEMANAGEMENT_TAKE_BOXLEFTVOLUME_ELEVATOR1,
    SIG_MAINWINDOW_REALSENCEMANAGEMENT_TAKE_BOXLEFTVOLUME_ELEVATOR2,
};

struct Sig_MainWindow_RealsenceManagement
{
    SIG_MAINWINDOW_REALSENCEMANAGEMENT type;
};

enum SIG_MODBUSMANAGEMENT_JOBMANAGEMENT
{
    SIG_MODBUSMANAGEMENT_JOBMANAGEMENT_ELEVATOR_DOWN,
    SIG_MODBUSMANAGEMENT_JOBMANAGEMENT_ELEVATOR_UP,
    SIG_MODBUSMANAGEMENT_JOBMANAGEMENT_XY_OUT,
    SIG_MODBUSMANAGEMENT_JOBMANAGEMENT_XY_IN,
    SIG_MODBUSMANAGEMENT_JOBMANAGEMENT_RESTORED,
};

struct Sig_ModbusManagement_JobManagement_Elevator_Down
{
    MODBUS_ELEVATOR_STATE_STATE type;
    int elevatorId = -1;
    int boxId = -1;
};

struct Sig_ModbusManagement_JobManagement_Elevator_Up
{
    MODBUS_ELEVATOR_STATE_STATE type;
    int elevatorId = -1;
    int boxId = -1;
};

struct Sig_ModbusManagement_JobManagement_Xy_Out
{
    int elevatorId = -1;
    int boxId = -1;
};
struct Sig_ModbusManagement_JobManagement_Xy_In
{
    int elevatorId = -1;
    int boxId = -1;
};
struct Sig_ModbusManagement_JobManagement
{
    SIG_MODBUSMANAGEMENT_JOBMANAGEMENT type;
    Sig_ModbusManagement_JobManagement_Elevator_Down value_elevatorDown;
    Sig_ModbusManagement_JobManagement_Elevator_Up value_elevatorUp;
    Sig_ModbusManagement_JobManagement_Xy_Out value_xyOut;
    Sig_ModbusManagement_JobManagement_Xy_In value_xyIn;
};

enum SIG_MODBUSMANAGEMENT_MAINWINDOW
{
    SIG_MODBUSMANAGEMENT_MAINWINDOW_ELEVATOR_DOWN,
    SIG_MODBUSMANAGEMENT_MAINWINDOW_ELEVATOR_UP,
    SIG_MODBUSMANAGEMENT_MAINWINDOW_XY_OUT,
    SIG_MODBUSMANAGEMENT_MAINWINDOW_XY_IN,
    SIG_MODBUSMANAGEMENT_MAINWINDOW_MODBUS_ERRORS,
    SIG_MODBUSMANAGEMENT_MAINWINDOW_MODBUS_FIXED,
    SIG_MODBUSMANAGEMENT_MAINWINDOW_ORDER_TIMEOUT,
};

struct Sig_ModbusManagement_MainWindow_Elevator_Down
{
    MODBUS_ELEVATOR_STATE_STATE type;
    int elevatorId = -1;
    int boxId = -1;
};

struct Sig_ModbusManagement_MainWindow_Elevator_Up
{
    MODBUS_ELEVATOR_STATE_STATE type;
    int elevatorId = -1;
    int boxId = -1;
};

struct Sig_ModbusManagement_MainWindow_Xy_Out
{
    int elevatorId = -1;
    int boxId = -1;
};
struct Sig_ModbusManagement_MainWindow_Xy_In
{
    int elevatorId = -1;
    int boxId = -1;
};
struct Sig_ModbusManagement_MainWindow_Modbus_Errors
{
    QVector<QString> errors;
};
struct Sig_ModbusManagement_MainWindow
{
    SIG_MODBUSMANAGEMENT_MAINWINDOW type;
    Sig_ModbusManagement_MainWindow_Elevator_Down value_elevatorDown;
    Sig_ModbusManagement_MainWindow_Elevator_Up value_elevatorUp;
    Sig_ModbusManagement_MainWindow_Xy_Out value_xyOut;
    Sig_ModbusManagement_MainWindow_Xy_In value_xyIn;
    Sig_ModbusManagement_MainWindow_Modbus_Errors value_modbusErrors;
};

enum SIG_CONFIGMANAGEMENT_JOBMANAGEMENT
{
    SIG_CONFIGMANAGEMENT_JOBMANAGEMENT_ALLCONFIG,
};

struct Sig_ConfigManagement_JobManagement_AllConfig
{
    Array_TwoDimensional<float> realsence_box_sample_elevator1;
    Array_TwoDimensional<float> realsence_box_sample_elevator2;
    float empty_box_volume_elevator1;
    float empty_box_volume_elevator2;
};

struct Sig_ConfigManagement_JobManagement
{
    SIG_CONFIGMANAGEMENT_JOBMANAGEMENT type;
    Sig_ConfigManagement_JobManagement_AllConfig value_allConfig;
};

enum SIG_CONFIGMANAGEMENT_REALSENCEMANAGEMENT
{
    SIG_CONFIGMANAGEMENT_REALSENCEMANAGEMENT_ALLCONFIG,
};

struct Sig_ConfigManagement_RealsenceManagement_AllConfig
{
    Array_TwoDimensional<float> realsence_box_sample_elevator1;
    Array_TwoDimensional<float> realsence_box_sample_elevator2;
    float empty_box_volume_elevator1;
    float empty_box_volume_elevator2;
};

struct Sig_ConfigManagement_RealsenceManagement
{
    SIG_CONFIGMANAGEMENT_REALSENCEMANAGEMENT type;
    Sig_ConfigManagement_RealsenceManagement_AllConfig value_allConfig;
};

enum SIG_REALSENCEMANAGEMENT_JOBMANAGEMENT
{
    SIG_REALSENCEMANAGEMENT_JOBMANAGEMENT_TAKE_BOXEMPTYVOLUME,
    SIG_REALSENCEMANAGEMENT_JOBMANAGEMENT_TAKE_BOXLEFTVOLUME,
    SIG_REALSENCEMANAGEMENT_JOBMANAGEMENT_MODE_NORMAL,
    SIG_REALSENCEMANAGEMENT_JOBMANAGEMENT_MODE_RANK,
};
struct Sig_RealsenceManagement_JobManagement_TakeBoxEmptyVolume
{
    float volume = -1;
    Array_TwoDimensional<float> realsence_box_sample;
};
struct Sig_RealsenceManagement_JobManagement_TakeBoxLeftVolume
{
    float volume = -1;
};
struct Sig_RealsenceManagement_JobManagement
{
    SIG_REALSENCEMANAGEMENT_JOBMANAGEMENT type;
    Sig_RealsenceManagement_JobManagement_TakeBoxEmptyVolume value_takeBoxEmptyVolume;
    Sig_RealsenceManagement_JobManagement_TakeBoxLeftVolume value_takeBoxLeftVolume;
};

enum SIG_REALSENCEMANAGEMENT_CONFIGMANAGEMENT
{
    SIG_REALSENCEMANAGEMENT_CONFIGMANAGEMENT_BOXEMPTYVOLUME_ELEVATOR1,
    SIG_REALSENCEMANAGEMENT_CONFIGMANAGEMENT_BOXEMPTYVOLUME_ELEVATOR2,
};
struct Sig_RealsenceManagement_ConfigManagement_BoxLeftVolume_Elevator1
{
    float volume = -1;
    Array_TwoDimensional<float> realsence_box_sample;
};
struct Sig_RealsenceManagement_ConfigManagement_BoxLeftVolume_Elevator2
{
    float volume = -1;
    Array_TwoDimensional<float> realsence_box_sample;
};
struct Sig_RealsenceManagement_ConfigManagement
{
    SIG_REALSENCEMANAGEMENT_CONFIGMANAGEMENT type;
    Sig_RealsenceManagement_ConfigManagement_BoxLeftVolume_Elevator1 value_BoxEmptyVolume_elevator1;
    Sig_RealsenceManagement_ConfigManagement_BoxLeftVolume_Elevator2 value_BoxEmptyVolume_elevator2;
};

enum SIG_REALSENCEMANAGEMENT_MAINWINDOW
{
    SIG_REALSENCEMANAGEMENT_MAINWINDOW_BOXEMPTYVOLUME_ELEVATOR2,
};
struct Sig_RealsenceManagement_MainWindow_BoxLeftVolume_Elevator
{
    float volume = -1;
};

struct Sig_RealsenceManagement_MainWindow
{
    SIG_REALSENCEMANAGEMENT_MAINWINDOW type;
    Sig_RealsenceManagement_MainWindow_BoxLeftVolume_Elevator value_BoxEmptyVolume_elevator2;
};

enum SIG_APIMANAGEMENT_ITEMMANAGEMENT
{
    SIG_APIMANAGEMENT_ITEMMANAGEMENT_NEWITEMS,
};
struct Sig_ApiManagement_ItemManagement_NewItems
{
    QVector<Item_FamilyMart> items;
};
struct Sig_ApiManagement_ItemManagement
{
    SIG_APIMANAGEMENT_ITEMMANAGEMENT type;
    Sig_ApiManagement_ItemManagement_NewItems value_newItems;
};

enum SIG_BARCODESCANER_ITEMMANAGEMENT
{
    SIG_BARCODESCANER_ITEMMANAGEMENT_BARCODE,
};
struct Sig_BarcodeScaner_ItemManagement_Barcode
{
    QString barcode;
};
struct Sig_BarcodeScaner_ItemManagement
{
    SIG_BARCODESCANER_ITEMMANAGEMENT type;
    Sig_BarcodeScaner_ItemManagement_Barcode value_barcode;
};

enum SIG_MAINWINDOW_CLIPMANAGEMENT
{
    SIG_MAINWINDOW_CLIPMANAGEMENT_LISTEN_BARCODE,
};
struct Sig_Mainwindow_Clipmanagement
{
    SIG_MAINWINDOW_CLIPMANAGEMENT type;
};

enum SIG_MAINWINDOW_BOXMANAGEMENT
{
    SIG_MAINWINDOW_BOXMANAGEMENT_UPDATE_BOXVOLUME=0,
};
struct Sig_Mainwindow_Boxmanagement_Update_BoxVolume
{
    int boxId= -1;
    float volume = -1;
};
struct Sig_Mainwindow_Boxmanagement
{
    SIG_MAINWINDOW_BOXMANAGEMENT type;
    Sig_Mainwindow_Boxmanagement_Update_BoxVolume value_updateBoxVolume;
};

enum SIG_CLIPMANAGEMENT_MAINWINDOW
{
    SIG_CLIPMANAGEMENT_MAINWINDOW_BARCODE,
};
struct Sig_Clipmanagement_Mainwindow_Barcode
{
    QString barcode;
};
struct Sig_Clipmanagement_Mainwindow
{
    SIG_CLIPMANAGEMENT_MAINWINDOW type;
    Sig_Clipmanagement_Mainwindow_Barcode value_barcode;
};

#endif // COMMON_H
