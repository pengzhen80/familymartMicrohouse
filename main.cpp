#include "mainwindow.h"
#include"modbusmanagement.h"
#include"dataapimanagement.h"
#include"jobmanagement.h"
#include"configfilemanagement.h"
#include"boxmanager.h"
#include"itemmanager.h"
#include"dbmanagement.h"
#include"logmanagement.h"
#include"realsencemanager.h"
//#include"clipboardmanagement.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    QString log_filename = QString::number(QDateTime::currentDateTime().date().year())
            +QString::number(QDateTime::currentDateTime().date().month())
            +QString::number(QDateTime::currentDateTime().date().day())
            +".txt";
    logSysInit(LOG_DIR+log_filename);

    ModbusManagement modbusmanager;
    modbusmanager.Init();
//    QObject::connect(&w,&MainWindow::sig_modbusmanager_orders,&modbusmanager,&ModbusManagement::slot_mainwindow_order);


    MyTcpServer apiserver;
    apiserver.Init();

    JobManagement jobmanager;
    BoxManager boxmanager;
    ItemManager itemmanager;

    ConfigFileManagement configmanager;
    configmanager.Init();
//    std::thread t_config(&ConfigFileManagement::Init, &configmanager);
//    t_config.detach();

    DBManagement dbmanager;
    dbmanager.Init();
//    std::thread t_dbmanager(&DBManagement::Init, &dbmanager);
//    t_dbmanager.detach();

    RealsenceManager realsencemanager;
    std::thread t_realsence(&RealsenceManager::Init, &realsencemanager);
    t_realsence.detach();
    qDebug()<<"main thread is "<<QThread::currentThreadId();

//    ClipBoardManagement clipboardManagement;
//    clipboardManagement.Init(QApplication::clipboard());

    QObject::connect(&dbmanager,&DBManagement::sig_dbmanager_initBoxs,&boxmanager,&BoxManager::slot_dbmanager);
    QObject::connect(&dbmanager,&DBManagement::sig_itemmanager_initItems,&itemmanager,&ItemManager::slot_dbmanager);

    QObject::connect(&boxmanager,&BoxManager::sig_dbmanager,&dbmanager,&DBManagement::slot_boxmanager);
    QObject::connect(&boxmanager,&BoxManager::sig_jobmanager,&jobmanager,&JobManagement::slot_boxmanager);

    QObject::connect(&itemmanager,&ItemManager::sig_dbmanager,&dbmanager,&DBManagement::slot_itemmanager);
    QObject::connect(&itemmanager,&ItemManager::sig_jobmanager,&jobmanager,&JobManagement::slot_itemmanager);
    QObject::connect(&itemmanager,&ItemManager::sig_mainwindow,&w,&MainWindow::slot_itemmanager);

    QObject::connect(&configmanager,&ConfigFileManagement::sig_jobmanager,&jobmanager,&JobManagement::slot_config);
    QObject::connect(&configmanager,&ConfigFileManagement::sig_realsence,&realsencemanager,&RealsenceManager::slot_config);

    QObject::connect(&realsencemanager,&RealsenceManager::sig_jobmanager,&jobmanager,&JobManagement::slot_realsence);
    QObject::connect(&realsencemanager,&RealsenceManager::sig_configmanager,&configmanager,&ConfigFileManagement::slot_realsence);
    QObject::connect(&realsencemanager,&RealsenceManager::sig_mainwindow,&w,&MainWindow::slot_realsence);

    QObject::connect(&w,&MainWindow::sig_jobmanager,&jobmanager,&JobManagement::slot_mainwindow);
    QObject::connect(&w,&MainWindow::sig_itemmanager,&itemmanager,&ItemManager::slot_mainwindow);
//    QObject::connect(&w,&MainWindow::sig_clipboard,&clipboardManagement,&ClipBoardManagement::slot_mainwindow);
    QObject::connect(&w,&MainWindow::sig_realsencemanager_test_format,&realsencemanager,&RealsenceManager::slot_mainwindow_test_format);
    QObject::connect(&w,&MainWindow::sig_quitProgram,&a,&QApplication::quit);
    QObject::connect(&w,&MainWindow::sig_modbusmanager_orders,&modbusmanager,&ModbusManagement::slot_mainwindow_order);
    QObject::connect(&w,&MainWindow::sig_boxManager,&boxmanager,&BoxManager::slot_mainwindow);



    QObject::connect(&modbusmanager,&ModbusManagement::sig_jobmanager,&jobmanager,&JobManagement::slot_modbus);
    QObject::connect(&modbusmanager,&ModbusManagement::sig_mainwindow,&w,&MainWindow::slot_modbusmanager);

    QObject::connect(&jobmanager,&JobManagement::sig_itemmanager,&itemmanager,&ItemManager::slot_jobmanager);
    QObject::connect(&jobmanager,&JobManagement::sig_boxmanager,&boxmanager,&BoxManager::slot_jobmanager);
    QObject::connect(&jobmanager,&JobManagement::sig_configmanager,&configmanager,&ConfigFileManagement::slot_jobmanager);
    QObject::connect(&jobmanager,&JobManagement::sig_modbusmanager,&modbusmanager,&ModbusManagement::slot_jobmanager);
    QObject::connect(&jobmanager,&JobManagement::sig_realsencemanager,&realsencemanager,&RealsenceManager::slot_jobmanager);
    QObject::connect(&jobmanager,&JobManagement::sig_mainwindow,&w,&MainWindow::slot_jobmanager);

//    QObject::connect(&clipboardManagement,&ClipBoardManagement::sig_mainwindow,&w,&MainWindow::slot_clipboard);

    QObject::connect(&apiserver,&MyTcpServer::sig_itemManager,&itemmanager,&ItemManager::slot_apimanager);


    dbmanager.Init_Data();
    configmanager.GetConfigs();

    w.show();

    return a.exec();
}
