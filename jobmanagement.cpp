#include "jobmanagement.h"
#include<QDebug>

JobManagement::JobManagement(QObject *parent) : QObject(parent)
{

}

void JobManagement::updateBoxVolume(int boxId,float volume)
{
    Sig_JobManagement_BoxManagement sig;
    sig.type = SIG_JOBMANAGEMENT_BOXMANAGEMENT_UPDATE_BOX_VOLUME;
    sig.value_update_boxvolume.box_id = boxId;
    sig.value_update_boxvolume.volume = volume;

    emit sig_boxmanager(sig);
}

void JobManagement::askItemsByBoxId(int boxId)
{
    Sig_JobManagement_ItemManagement sig;
    sig.type = SIG_JOBMANAGEMENT_ITEMMANAGEMENT_ASK_ITEMS_BYBOXID;
    sig.value_askItemByBoxId.box_id = boxId;

    emit sig_itemmanager(sig);
}

void JobManagement::getBoxIdByItems(QVector<Item_FamilyMart> items)
{
    Sig_JobManagement_ItemManagement sig;
    sig.type = SIG_JOBMANAGEMENT_ITEMMANAGEMENT_ASK_BOXID_BYITEMS;
    sig.value_askBoxIdByItems.items = items;

    emit sig_itemmanager(sig);
}

void JobManagement::get_emptyBoxSample()
{
    Sig_JobManagement_ConfigManagement sig;
    sig.type = SIG_JOBMANAGEMENT_CONFIGMANAGEMENT_GET_EMPTYBOXSAMPLE_ELEVATOR1;
    emit sig_configmanager(sig);
}

void JobManagement::askBoxDown_MostEmpty()
{

    Sig_JobManagement_BoxManagement sig;
    sig.type = SIG_JOBMANAGEMENT_BOXMANAGEMENT_ASK_BOXMOSTEMPTY;

    emit sig_boxmanager(sig);
}

int JobManagement::askBoxDown_MostEmpty_noVolume()
{
    int result = askBoxDown_MostEmpty_noVolume_boxId;
    askBoxDown_MostEmpty_noVolume_boxId++;
    if(25 == askBoxDown_MostEmpty_noVolume_boxId)
    {
        askBoxDown_MostEmpty_noVolume_boxId = 1;
    }
    return result;
}

void JobManagement::askBoxDownByItems(QVector<Item_FamilyMart> items)
{
    //get boxes by items
    QSet<int> boxs_needDown;
    for(int i=0;i<items.size();i++)
    {   
        if(!boxs_needDown.contains(items[i].box.box_id))
        {
            boxs_needDown.insert(items[i].box.box_id);
            qDebug()<<"box need to down:"<<items[i].box.box_id;
        }
    }

    //ask first box down
    if(boxs_needDown.size()>0)
    {
        int firstBoxId = *(boxs_needDown.begin());
        int elevatorId = -1;
        //choose elevator
        if(elevator1_position_ == MODBUS_ELEVATOR_STATE_POSITION_UP)
        {
            elevatorId = 1;
        }
        else if(elevator2_position_ == MODBUS_ELEVATOR_STATE_POSITION_UP)
        {
            elevatorId = 2;
        }
        else
        {
            qDebug()<<"no elevator up";
        }
//        if(elevator2_position_ == MODBUS_ELEVATOR_STATE_POSITION_UP)
//        {
//            elevatorId = 2;
//        }
//        else if(elevator1_position_ == MODBUS_ELEVATOR_STATE_POSITION_UP)
//        {
//            elevatorId = 1;
//        }
//        else
//        {
//            qDebug()<<"no elevator up";
//        }

        Sig_JobManagement_Modbusmanagement sig;
        sig.type = SIG_JOBMANAGEMENT_MODBUSMANAGEMENT_XY_OUT;
        sig.value_xy.boxId = firstBoxId;
        sig.value_xy.elevatorId = elevatorId;

        emit sig_modbusmanager(sig);
        boxs_needDown.remove(firstBoxId);
    }
    boxsNeedDown_ = boxs_needDown;
}

void JobManagement::askNextBoxDown()
{
    //ask first box down
    if(boxsNeedDown_.size()>0)
    {
        int firstBoxId = *(boxsNeedDown_.begin());
        int elevatorId = -1;
        //choose elevator
        if(elevator1_position_ == MODBUS_ELEVATOR_STATE_POSITION_UP)
        {
            elevatorId = 1;
        }
        else if(elevator2_position_ == MODBUS_ELEVATOR_STATE_POSITION_UP)
        {
            elevatorId = 2;
        }
        else
        {
            qDebug()<<"no elevator up";
        }

        Sig_JobManagement_Modbusmanagement sig;
        sig.type = SIG_JOBMANAGEMENT_MODBUSMANAGEMENT_XY_OUT;
        sig.value_xy.boxId = firstBoxId;
        sig.value_xy.elevatorId = elevatorId;

        emit sig_modbusmanager(sig);
        boxsNeedDown_.remove(firstBoxId);
    }
}

void JobManagement::cancelNextBox()
{
    boxsNeedDown_.clear();
    askBoxUp();
}

void JobManagement::askBoxUp()
{
    askBoxUp_detectLeftVolume();
}

void JobManagement::askBoxUp_detectLeftVolume()
{
//    Sig_JobManagement_RealsenceManagement sig;
//    sig.type = SIG_JOBMANAGEMENT_REALSENCEMANAGEMENT_TAKE_BOXLEFTVOLUME;

//    emit sig_realsencemanager(sig);
    Sig_JobManagement_RealsenceManagement sig;
    if(elevator1_position_ == MODBUS_ELEVATOR_STATE_POSITION_DOWN)
    {
        qDebug()<<"take elevator 1 volume";
        sig.type = SIG_JOBMANAGEMENT_REALSENCEMANAGEMENT_TAKE_BOXLEFTVOLUME_ELEVATOR1;
        emit sig_realsencemanager(sig);
    }
    else if(elevator2_position_ == MODBUS_ELEVATOR_STATE_POSITION_DOWN)
    {
        qDebug()<<"take elevator 2 volume";
        sig.type = SIG_JOBMANAGEMENT_REALSENCEMANAGEMENT_TAKE_BOXLEFTVOLUME_ELEVATOR2;
        emit sig_realsencemanager(sig);
    }
    else
    {
        qDebug()<<"no elevator down";
    }
}

void JobManagement::askBoxUp_sendOrderToPlc()
{
    Sig_JobManagement_Modbusmanagement sig;
    sig.type = SIG_JOBMANAGEMENT_MODBUSMANAGEMENT_ELEVATOR_UP;

    int elevatorId = -1;
    //choose elevator
    if(elevator1_position_ == MODBUS_ELEVATOR_STATE_POSITION_DOWN)
    {
        elevatorId = 1;
    }
    else if(elevator2_position_ == MODBUS_ELEVATOR_STATE_POSITION_DOWN)
    {
        elevatorId = 2;
    }
    else
    {
        qDebug()<<"no elevator down";
    }
    sig.value_elevator.elevatorId = elevatorId;

    qDebug()<<"ask elevator up :"<<elevatorId;
    emit sig_modbusmanager(sig);
}

void JobManagement::takeBoxVolume()
{
    Sig_JobManagement_RealsenceManagement sig;
    sig.type = SIG_JOBMANAGEMENT_REALSENCEMANAGEMENT_TAKE_BOXLEFTVOLUME;
    emit sig_realsencemanager(sig);
}

void JobManagement::slot_modbus(Sig_ModbusManagement_JobManagement sig)
{
    switch (sig.type) {
    case(SIG_MODBUSMANAGEMENT_JOBMANAGEMENT_ELEVATOR_DOWN):
    {
        qDebug()<<"elevator down is:"<<sig.value_elevatorDown.elevatorId;
        if(1 == sig.value_elevatorDown.elevatorId)
        {
            elevator1_position_ = MODBUS_ELEVATOR_STATE_POSITION_DOWN;
            elevator1_state_ = sig.value_elevatorDown.type;

            if(MODBUS_ELEVATOR_STATE_STATE_HASBOX == elevator2_state_)
            {
                Sig_JobManagement_Modbusmanagement sigToModbus;
                sigToModbus.type = SIG_JOBMANAGEMENT_MODBUSMANAGEMENT_XY_PUTIN;
                sigToModbus.value_xy.boxId = box_needToPutIn_;
                sigToModbus.value_xy.elevatorId = 2;
                emit sig_modbusmanager(sigToModbus);

//                break;
            }
            else
            {
//                qDebug()<<"box need to down:"<<boxsNeedDown_.size();
                if(boxsNeedDown_.size()>0)
                {
                    QTimer::singleShot(500,this,[&](){
                        qDebug()<<"ask next box to prepare";
                        askNextBoxDown();
                    });
                }
                else
                {
                    qDebug()<<"no next box : set prepared";
                    Sig_Jobmanagement_Mainwindow sigToWindow_Prepared;
                    sigToWindow_Prepared.type = SIG_JOBMANAGEMENT_MAINWINDOW_NEXT_BOX_PREPARED;
                    emit sig_mainwindow(sigToWindow_Prepared);
                }
            }
        }
        else
        {
            elevator2_position_ = MODBUS_ELEVATOR_STATE_POSITION_DOWN;
            elevator2_state_ = sig.value_elevatorDown.type;

            if(MODBUS_ELEVATOR_STATE_STATE_HASBOX == elevator1_state_)
            {
//                qDebug()<<"elevator 2 got a box";

                Sig_JobManagement_Modbusmanagement sigToModbus;
                sigToModbus.type = SIG_JOBMANAGEMENT_MODBUSMANAGEMENT_XY_PUTIN;
                sigToModbus.value_xy.boxId = box_needToPutIn_;
                sigToModbus.value_xy.elevatorId = 1;
                emit sig_modbusmanager(sigToModbus);
            }
            else
            {
                qDebug()<<"box need to down:"<<boxsNeedDown_.size();
                if(boxsNeedDown_.size()>0)
                {
                    QTimer::singleShot(500,this,[&](){
                        qDebug()<<"ask next box to prepare";
                        askNextBoxDown();
                    });
                }
                else
                {
                    qDebug()<<"no next box : set prepared";
                    Sig_Jobmanagement_Mainwindow sigToWindow_Prepared;
                    sigToWindow_Prepared.type = SIG_JOBMANAGEMENT_MAINWINDOW_NEXT_BOX_PREPARED;
                    emit sig_mainwindow(sigToWindow_Prepared);
                }
            }
        }

        if(MODBUS_ELEVATOR_STATE_STATE_HASBOX == sig.value_elevatorDown.type)
        {
//            qDebug()<<"boxId is"<<box_current_;
//            box_current_ = sig.value_elevatorDown.boxId;

            Sig_Jobmanagement_Mainwindow sigToWindow;
            sigToWindow.type = SIG_JOBMANAGEMENT_MAINWINDOW_BOXDOWN;
            sigToWindow.value_boxDown.boxId = box_current_;

            emit sig_mainwindow(sigToWindow);
        }

        break;
    }
    case(SIG_MODBUSMANAGEMENT_JOBMANAGEMENT_ELEVATOR_UP):
    {
        qDebug()<<"elevator up :" <<sig.value_elevatorUp.elevatorId;
        if(1 == sig.value_elevatorUp.elevatorId)
        {
            elevator1_position_ = MODBUS_ELEVATOR_STATE_POSITION_UP;
            elevator1_state_ = sig.value_elevatorUp.type;

            if(elevator2_state_ == MODBUS_ELEVATOR_STATE_STATE_HASBOX&&elevator2_position_ == MODBUS_ELEVATOR_STATE_POSITION_UP)
            {
                Sig_JobManagement_Modbusmanagement sigToModbus;
                sigToModbus.type = SIG_JOBMANAGEMENT_MODBUSMANAGEMENT_ELEVATOR_DOWN;
                sigToModbus.value_elevator.elevatorId = 2;

                emit sig_modbusmanager(sigToModbus);
                box_needToPutIn_ = box_current_;
                box_current_ = box_needToDown_;
                break;
            }
        }
        else
        {
            elevator2_position_ = MODBUS_ELEVATOR_STATE_POSITION_UP;
            elevator2_state_ = sig.value_elevatorUp.type;

            if(elevator1_state_ == MODBUS_ELEVATOR_STATE_STATE_HASBOX&&elevator1_position_ == MODBUS_ELEVATOR_STATE_POSITION_UP)
            {
                Sig_JobManagement_Modbusmanagement sigToModbus;
                sigToModbus.type = SIG_JOBMANAGEMENT_MODBUSMANAGEMENT_ELEVATOR_DOWN;
                sigToModbus.value_elevator.elevatorId = 1;

                emit sig_modbusmanager(sigToModbus);

                box_needToPutIn_ = box_current_;
                box_current_ = box_needToDown_;
                break;
            }
        }

        if(boxsNeedDown_.size()>0)
        {
            askNextBoxDown();
        }
        else
        {
            if(MODBUS_ELEVATOR_STATE_STATE_HASBOX == sig.value_elevatorUp.type)
            {
                Sig_JobManagement_Modbusmanagement sigToModbus;
                sigToModbus.type = SIG_JOBMANAGEMENT_MODBUSMANAGEMENT_XY_PUTIN;
                sigToModbus.value_xy.boxId = box_current_;
                sigToModbus.value_xy.elevatorId = sig.value_elevatorUp.elevatorId;
                qDebug()<<"sig to modubs:"<<"putIn:"<<"box is "<<box_current_<<",elevator:"<<sigToModbus.value_xy.elevatorId;

                emit sig_modbusmanager(sigToModbus);
            }
        }

        break;
    }
    case(SIG_MODBUSMANAGEMENT_JOBMANAGEMENT_XY_IN):
    {
        qDebug()<<"xy in from elevator :"<<sig.value_xyIn.elevatorId;
        if(1 == sig.value_xyIn.elevatorId)
        {
            elevator1_state_ = MODBUS_ELEVATOR_STATE_STATE_NOBOX;
            elevator1_position_ = MODBUS_ELEVATOR_STATE_POSITION_UP;
        }
        else if(2 == sig.value_xyIn.elevatorId)
        {
            elevator2_state_ = MODBUS_ELEVATOR_STATE_STATE_NOBOX;
            elevator2_position_ = MODBUS_ELEVATOR_STATE_POSITION_UP;
        }

        if(boxsNeedDown_.empty()&&elevator1_state_==MODBUS_ELEVATOR_STATE_STATE_NOBOX&&elevator2_state_==MODBUS_ELEVATOR_STATE_STATE_NOBOX)
        {
            Sig_Jobmanagement_Mainwindow sigToWindow;
            sigToWindow.type = SIG_JOBMANAGEMENT_MAINWINDOW_BOX_XYIN;

            emit sig_mainwindow(sigToWindow);
        }
        else
        {
            if(boxsNeedDown_.empty()&&(elevator1_state_==MODBUS_ELEVATOR_STATE_STATE_HASBOX||elevator2_state_==MODBUS_ELEVATOR_STATE_STATE_HASBOX))
            {
                qDebug()<<"last box put in success";
                Sig_Jobmanagement_Mainwindow sigToWindow;
                sigToWindow.type = SIG_JOBMANAGEMENT_MAINWINDOW_NEXT_BOX_PREPARED;
                emit sig_mainwindow(sigToWindow);
            }
            qDebug()<<"box need to down:"<<boxsNeedDown_.size();
            QTimer::singleShot(500,this,[&](){
                qDebug()<<"ask next box to prepare";
                askNextBoxDown();
            });
        }
        break;
    }
    case(SIG_MODBUSMANAGEMENT_JOBMANAGEMENT_XY_OUT):
    {

        qDebug()<<"xy out to elevator :"<<sig.value_xyOut.elevatorId;
        if(1 == sig.value_xyOut.elevatorId)
        {
            elevator1_state_ = MODBUS_ELEVATOR_STATE_STATE_HASBOX;
            elevator1_position_ = MODBUS_ELEVATOR_STATE_POSITION_UP;
            if(elevator2_position_ == MODBUS_ELEVATOR_STATE_POSITION_DOWN&&elevator2_state_==MODBUS_ELEVATOR_STATE_STATE_HASBOX)
            {
                box_needToDown_ = sig.value_xyOut.boxId;

                //for mainwindow
                Sig_Jobmanagement_Mainwindow sigToMainwindow;
                sigToMainwindow.type = SIG_JOBMANAGEMENT_MAINWINDOW_NEXT_BOX_PREPARED;

                emit sig_mainwindow(sigToMainwindow);
                break;
            }
        }
        else if(2 == sig.value_xyOut.elevatorId)
        {
            elevator2_state_ = MODBUS_ELEVATOR_STATE_STATE_HASBOX;
            elevator2_position_ = MODBUS_ELEVATOR_STATE_POSITION_UP;
            if(elevator1_position_ == MODBUS_ELEVATOR_STATE_POSITION_DOWN&&elevator2_state_==MODBUS_ELEVATOR_STATE_STATE_HASBOX)
            {
                box_needToDown_ = sig.value_xyOut.boxId;

                qDebug()<<"next box : prepared";
                //for mainwindow
                Sig_Jobmanagement_Mainwindow sigToMainwindow;
                sigToMainwindow.type = SIG_JOBMANAGEMENT_MAINWINDOW_NEXT_BOX_PREPARED;

                emit sig_mainwindow(sigToMainwindow);
                break;
            }
        }
        else
        {
            qDebug()<<"sig from modbus: error elevatorId";
        }

        Sig_JobManagement_Modbusmanagement sigToModbus;
        sigToModbus.type = SIG_JOBMANAGEMENT_MODBUSMANAGEMENT_ELEVATOR_DOWN;
//        sigToModbus.value_elevator.boxId = sig.value_xyOut.boxId;
        sigToModbus.value_elevator.elevatorId = sig.value_xyOut.elevatorId;
        box_current_ = sig.value_xyOut.boxId;

//        qDebug()<<"type :"<<sigToModbus.type;
        emit sig_modbusmanager(sigToModbus);
        break;
    }
    default:
    {
        qDebug()<<"sig from modbus : unknow signal";
        break;
    }
    }
}

void JobManagement::slot_config(Sig_ConfigManagement_JobManagement sig)
{
    switch (sig.type) {
    case SIG_CONFIGMANAGEMENT_JOBMANAGEMENT_ALLCONFIG:
    {
        realsence_box_sample_elevator1_ = sig.value_allConfig.realsence_box_sample_elevator1;
        realsence_empty_volume_elevator1_ = sig.value_allConfig.empty_box_volume_elevator1;
        realsence_box_sample_elevator2_ = sig.value_allConfig.realsence_box_sample_elevator2;
        realsence_empty_volume_elevator2_ = sig.value_allConfig.empty_box_volume_elevator2;
        break;
    }
    default:
    {
        qDebug()<<"sig from config : unknow signal";
        break;
    }
    }
}

void JobManagement::slot_boxmanager(Sig_BoxManagement_JobManagement sig)
{
    switch (sig.type) {
    case SIG_BOXMANAGEMENT_JOBMANAGEMENT_MOSTEMPTYBOX:
    {
        Sig_JobManagement_Modbusmanagement sigToModbus;
        sigToModbus.type = SIG_JOBMANAGEMENT_MODBUSMANAGEMENT_XY_OUT;
        sigToModbus.value_xy.boxId = sig.value_mostEmptyBox.box_id;
        int elevatorId=-1;
        if(MODBUS_ELEVATOR_STATE_STATE_NOBOX == elevator2_state_)
        {
            elevatorId = 2;
        }
        else
        {
            elevatorId = 1;
        }
        sigToModbus.value_xy.elevatorId = elevatorId;

        emit sig_modbusmanager(sigToModbus);
        break;
    }
    default:
    {
        qDebug()<<"JobManagement::slot_boxmanager : unknown signal";
        break;
    }
    }
}

void JobManagement::slot_itemmanager(Sig_ItemManagement_JobManagement sig)
{
    switch (sig.type) {
    case SIG_ITEMMANAGEMENT_JOBMANAGEMENT_ITEMS_BOXID:
    {
        //todo: no need
        break;
    }
    case SIG_ITEMMANAGEMENT_JOBMANAGEMENT_ADDNEWITEMS_SUCCESS:
    {
        askBoxUp();
        break;
    }
    default:
    {
        qDebug()<<"JobManagement::slot_itemmanager : known signal";
        break;
    }
    }
}

void JobManagement::slot_realsence(Sig_RealsenceManagement_JobManagement sig)
{
    switch (sig.type) {
    case SIG_REALSENCEMANAGEMENT_JOBMANAGEMENT_TAKE_BOXEMPTYVOLUME:
    {
        qDebug()<<"JobManagement::slot_realsence volume is "<<sig.value_takeBoxEmptyVolume.volume;

        if(elevator1_position_ == MODBUS_ELEVATOR_STATE_POSITION_DOWN)
        {
            realsence_box_sample_elevator1_ = sig.value_takeBoxEmptyVolume.realsence_box_sample;
            realsence_empty_volume_elevator1_ = sig.value_takeBoxEmptyVolume.volume;

            Sig_JobManagement_ConfigManagement sigToConfig;
            sigToConfig.type = SIG_JOBMANAGEMENT_CONFIGMANAGEMENT_SET_EMPTYBOXSAMPLE_ELEVATOR1;
            sigToConfig.value_setEmptyBoxSample.box_empty_volume = realsence_empty_volume_elevator1_;
            sigToConfig.value_setEmptyBoxSample.box_sample = realsence_box_sample_elevator1_;

            emit sig_configmanager(sigToConfig);
        }
        else if(elevator2_position_ == MODBUS_ELEVATOR_STATE_POSITION_DOWN)
        {
            realsence_box_sample_elevator2_ = sig.value_takeBoxEmptyVolume.realsence_box_sample;
            realsence_empty_volume_elevator2_ = sig.value_takeBoxEmptyVolume.volume;

            Sig_JobManagement_ConfigManagement sigToConfig;
            sigToConfig.type = SIG_JOBMANAGEMENT_CONFIGMANAGEMENT_SET_EMPTYBOXSAMPLE_ELEVATOR2;
            sigToConfig.value_setEmptyBoxSample.box_empty_volume = realsence_empty_volume_elevator2_;
            sigToConfig.value_setEmptyBoxSample.box_sample = realsence_box_sample_elevator2_;

            emit sig_configmanager(sigToConfig);
        }


        break;
    }
    case SIG_REALSENCEMANAGEMENT_JOBMANAGEMENT_TAKE_BOXLEFTVOLUME:
    {
        if(sig.value_takeBoxLeftVolume.volume < 0)
        {
            flag_realsenceState_ = false;
        }
        else
        {
            flag_realsenceState_ = true;
            updateBoxVolume(box_current_, REALSENSE_BOX_FULLVOLUME - sig.value_takeBoxLeftVolume.volume);
        }
        askBoxUp_sendOrderToPlc();
        break;
    }
    case SIG_REALSENCEMANAGEMENT_JOBMANAGEMENT_MODE_RANK:
    {
        flag_realsenceState_ = false;
        break;
    }
    case SIG_REALSENCEMANAGEMENT_JOBMANAGEMENT_MODE_NORMAL:
    {
        flag_realsenceState_ = true;
        break;
    }
    default:
    {
        qDebug()<<"JobManagement::slot_realsence : unknown signal";
        break;
    }
    }
}

void JobManagement::slot_mainwindow(Sig_Mainwindow_Jobmanagement sig)
{
    switch (sig.type) {
    case SIG_MAINWINDOWN_JOBMANAGEMENT_ASK_BOXDOWN_BYITEMS:
    {
        askBoxDownByItems(sig.value_askBoxDownByItems.items);
        break;
    }
    case SIG_MAINWINDOWN_JOBMANAGEMENT_ASK_BOXUP:
    {
        askBoxUp();
        break;
    }
    case SIG_MAINWINDOWN_JOBMANAGEMENT_ASK_BOXDOWN_MOSTEMPTY:
    {
        qDebug()<<"askBoxDown_MostEmpty ";
        if(flag_realsenceState_)
        {
            askBoxDown_MostEmpty();
        }
        else
        {
            int boxId = askBoxDown_MostEmpty_noVolume();
            Sig_JobManagement_Modbusmanagement sigToModbus;
            sigToModbus.type = SIG_JOBMANAGEMENT_MODBUSMANAGEMENT_XY_OUT;
            sigToModbus.value_xy.boxId = boxId;
            int elevatorId=-1;
            if(MODBUS_ELEVATOR_STATE_STATE_NOBOX == elevator1_state_)
            {
                elevatorId = 1;
            }
            else
            {
                elevatorId = 2;
            }
            sigToModbus.value_xy.elevatorId = elevatorId;

            emit sig_modbusmanager(sigToModbus);
        }
        //for test

//        Sig_Jobmanagement_Mainwindow sigToWindow;
//        sigToWindow.type = SIG_JOBMANAGEMENT_MAINWINDOW_BOXDOWN;
//        sigToWindow.value_boxDown.boxId = 1;
//        box_current_ = 1;

//        emit sig_mainwindow(sigToWindow);
        break;
    }
    case SIG_MAINWINDOWN_JOBMANAGEMENT_DETECT_EMPTYBOXVOLUME_VOLUME_ELEVATOR1:
    {
        Sig_JobManagement_RealsenceManagement sig;
        sig.type = SIG_JOBMANAGEMENT_REALSENCEMANAGEMENT_TAKE_EMPTYBOXVOLUME_ELEVATOR1;

        emit sig_realsencemanager(sig);
        break;
    }
    case SIG_MAINWINDOWN_JOBMANAGEMENT_DETECT_EMPTYBOXVOLUME_VOLUME_ELEVATOR2:
    {
        Sig_JobManagement_RealsenceManagement sig;
        sig.type = SIG_JOBMANAGEMENT_REALSENCEMANAGEMENT_TAKE_EMPTYBOXVOLUME_ELEVATOR2;

        emit sig_realsencemanager(sig);
        break;
    }
    case SIG_MAINWINDOWN_JOBMANAGEMENT_PLC_RECOVERY:
    {
        Sig_JobManagement_Modbusmanagement sig;
        sig.type = SIG_JOBMANAGEMENT_MODBUSMANAGEMENT_PLC_RECOVERY;

        emit sig_modbusmanager(sig);
        break;
    }
    case SIG_MAINWINDOWN_JOBMANAGEMENT_ADDNEWITEMS:
    {
        qDebug()<<"SIG_MAINWINDOWN_JOBMANAGEMENT_ADDNEWITEMS:"<<box_current_;
        Sig_JobManagement_ItemManagement sigToItem;
        sigToItem.type = SIG_JOBMANAGEMENT_ITEMMANAGEMENT_ADD_NEWITEMS;
        for(int i=0;i<sig.value_addNewItems.items.size();i++)
        {
            //maybe have bug
            sig.value_addNewItems.items[i].box.box_id = box_current_;
        }
        sigToItem.value_addNewItems.items = sig.value_addNewItems.items;

        emit sig_itemmanager(sigToItem);
        break;
    }
    case SIG_MAINWINDOWN_JOBMANAGEMENT_INIT_MODBUS_ELEVATOR_DOWN:
    {
        if(1 == sig.value_Init_Modbus_ElevatorDown.elevator_id)
        {
             elevator1_position_ = MODBUS_ELEVATOR_STATE_POSITION_DOWN;
        }
        else if(2 == sig.value_Init_Modbus_ElevatorDown.elevator_id)
        {
            elevator2_position_ = MODBUS_ELEVATOR_STATE_POSITION_DOWN;
        }
        break;
    }
    case SIG_MAINWINDOWN_JOBMANAGEMENT_INIT_MODBUS_ELEVATOR_UP:
    {
        if(1 == sig.value_Init_Modbus_ElevatorUp.elevator_id)
        {
             elevator1_position_ = MODBUS_ELEVATOR_STATE_POSITION_UP;
        }
        else if(2 == sig.value_Init_Modbus_ElevatorUp.elevator_id)
        {
            elevator2_position_ = MODBUS_ELEVATOR_STATE_POSITION_UP;
        }
        break;
    }
    case SIG_MAINWINDOWN_JOBMANAGEMENT_INIT_MODBUS_XYIN:
    {
        //no need
        break;
    }
    case SIG_MAINWINDOWN_JOBMANAGEMENT_INIT_MODBUS_XYOUT:
    {
        //update current box and elevator position
        box_current_ = sig.value_Init_Modbus_XyOut.box_id;
        if(1 == sig.value_Init_Modbus_XyOut.elevator_id)
        {
             elevator1_position_ = MODBUS_ELEVATOR_STATE_POSITION_UP;
        }
        else if(2 == sig.value_Init_Modbus_XyOut.elevator_id)
        {
            elevator2_position_ = MODBUS_ELEVATOR_STATE_POSITION_UP;
        }
        break;
    }
    default:
    {
        qDebug()<<"JobManagement::slot_mainwindow : unknow signal";
        break;
    }
    }
}
