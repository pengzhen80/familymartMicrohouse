#ifndef MODBUSMANAGEMENT_H
#define MODBUSMANAGEMENT_H

#include <QObject>
#include<QTimer>
#include<modbus/modbus-tcp.h>
#include"common.h"

class ModbusManagement : public QObject
{
    Q_OBJECT
public:
    explicit ModbusManagement(QObject *parent = nullptr);
    ~ ModbusManagement();
    void Init(QString ip,int port);
    void Init();
//    void SendData(MODBUS_ORDER order,int elevatorID,QVector<int> vec_boxs);
    void SendData(Modbus_Order order);
    ModbusState ReadData();
    ModbusState ReadState();
    void UnitTest();
signals:
    void sig_jobmanager_state();
    void sig_jobmanager_finish(MODBUS_ORDER order_type);
    void sig_jobmanger_errors(QVector<QString> errors);
    void sig_jobmanager_fixed();

    void sig_jobmanager(Sig_ModbusManagement_JobManagement sig);
    void sig_mainwindow(Sig_ModbusManagement_MainWindow);
private:
    void readErrors();
    bool checkErrors(uint16_t errors[6]);
    void checkResult(Modbus_Order order);
    void checkResult_mainwindow(Modbus_Order order);
    void checkPLC();
public slots:
    void slot_jobmanager_order(Modbus_Order order);
    //for test
    void slot_mainwindow_order(Sig_Mainwindown_Modbusmanagement sig);

    //for jobmanager
    void slot_jobmanager(Sig_JobManagement_Modbusmanagement sig);
private:
    MODBUS_API modbus_t* server_;
    QTimer* timer_checkErrors_;
    bool plc_state_ = true; // plc state is error(false) or not(true);
};

#endif // MODBUSMANAGEMENT_H
