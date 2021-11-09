#ifndef JOBMANAGEMENT_H
#define JOBMANAGEMENT_H

#include <QObject>
#include<QTimer>
#include "common.h"

class JobManagement : public QObject
{
    Q_OBJECT
public:
    explicit JobManagement(QObject *parent = nullptr);
private:
    //boxmanager:
    void updateBoxVolume(int boxId,float volume);
    //itemmanager:
    void askItemsByBoxId(int boxId);
    void getBoxIdByItems(QVector<Item_FamilyMart> items);
    //configmanager:
    void get_emptyBoxSample();
    //modbusmanager:
    void askBoxDown_MostEmpty();

    void askBoxDownByItems(QVector<Item_FamilyMart> items);
    void askNextBoxDown();
    void cancelNextBox();

    void askBoxUp();
    void askBoxUp_detectLeftVolume();
    void askBoxUp_sendOrderToPlc();
    //realsense:
    void takeBoxVolume();
    int askBoxDown_MostEmpty_noVolume();
signals:
    void sig_boxmanager(Sig_JobManagement_BoxManagement);
    void sig_itemmanager(Sig_JobManagement_ItemManagement);
    void sig_configmanager(Sig_JobManagement_ConfigManagement);
    void sig_modbusmanager(Sig_JobManagement_Modbusmanagement);
    void sig_realsencemanager(Sig_JobManagement_RealsenceManagement);
    void sig_mainwindow(Sig_Jobmanagement_Mainwindow);

public slots:
    void slot_mainwindow(Sig_Mainwindow_Jobmanagement sig);
    void slot_modbus(Sig_ModbusManagement_JobManagement sig);
    void slot_config(Sig_ConfigManagement_JobManagement);
    void slot_boxmanager(Sig_BoxManagement_JobManagement sig);
    void slot_itemmanager(Sig_ItemManagement_JobManagement sig);
    void slot_realsence(Sig_RealsenceManagement_JobManagement sig);
private:
    Array_TwoDimensional<float> realsence_box_sample_elevator1_;
    float realsence_empty_volume_elevator1_;

    Array_TwoDimensional<float> realsence_box_sample_elevator2_;
    float realsence_empty_volume_elevator2_;

    //for modbus
    MODBUS_ELEVATOR_STATE_POSITION elevator1_position_ = MODBUS_ELEVATOR_STATE_POSITION_UP;
    MODBUS_ELEVATOR_STATE_STATE elevator1_state_ = MODBUS_ELEVATOR_STATE_STATE_NOBOX;
    MODBUS_ELEVATOR_STATE_POSITION elevator2_position_= MODBUS_ELEVATOR_STATE_POSITION_UP;
    MODBUS_ELEVATOR_STATE_STATE elevator2_state_= MODBUS_ELEVATOR_STATE_STATE_NOBOX;
    QSet<int> boxsNeedDown_;

    int box_current_ = -1;
    int box_needToPutIn_ = -1;
    int box_needToDown_ = -1;

    //realsence state , true is ok, false is error, update by the slot_realsence func
    bool flag_realsenceState_ = true;
    int  askBoxDown_MostEmpty_noVolume_boxId = 1;
};

#endif // JOBMANAGEMENT_H
