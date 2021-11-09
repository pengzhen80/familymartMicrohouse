#include "modbusmanagement.h"
#include<QDebug>

ModbusManagement::ModbusManagement(QObject *parent) : QObject(parent)
{
    timer_checkErrors_ = new QTimer();
    connect(timer_checkErrors_, &QTimer::timeout, this, [&]()
    {
        ModbusState state = ReadData();
        if(checkErrors(state.errors))
        {
            plc_state_ = false;
        }
        else
        {
            if(false == plc_state_)
            {
                qDebug()<<"plc fixed";
                plc_state_ = true;

                Sig_ModbusManagement_MainWindow sigToWindow;
                sigToWindow.type = SIG_MODBUSMANAGEMENT_MAINWINDOW_MODBUS_FIXED;

                emit sig_mainwindow(sigToWindow);
            }
        }

        //todo : check plc's heart beat
    });
    timer_checkErrors_->start(1000);
}

 ModbusManagement:: ~ModbusManagement()
{
     modbus_close(server_);
     modbus_free(server_);
}

void ModbusManagement::Init(QString ip,int port)
{
    QByteArray ba = ip.toLocal8Bit();
    const char *c_str = ba.data();
    server_ =  modbus_new_tcp(c_str, port);
    MODBUS_API int modbus_tcp_listen(modbus_t *ctx, int nb_connection);
    MODBUS_API int modbus_tcp_accept(modbus_t *ctx, int *s);
}

void ModbusManagement::Init()
{
    // create a modbus object
     server_ = modbus_new_tcp("192.168.5.10", 502);  //由於是tcp client連接，在同一個程序中相同的端口可以連接多次。
//    server_ = modbus_new_tcp("127.0.0.1", 502);
    // set slave id
    modbus_set_slave(server_, 1);  //從機地址

    // connect with the server
    modbus_connect(server_);

    struct timeval t;
    t.tv_sec=0;
    t.tv_usec=1000000;   //設置modbus超時時間爲1000毫秒，注意：經測試，如果沒有成功建立tcp連接，則該設置無效。
    modbus_set_response_timeout(server_, t.tv_sec,t.tv_usec);

    timer_checkErrors_->start(MODBUS_CHECK_HEART_AND_ERROR*1000);
}

void ModbusManagement::UnitTest()
{
//    QVector<int> box;
//    box.push_back(1);
//    SendData(MODBUS_ORDER_PUTIN,1,box);
    Modbus_Order order;
    order.boxIn_id = 1;
    order.boxOut_id = 0;
    order.elevatorIn_id = 1;
    order.elevatorOut_id = 0;
    order.order_type = MODBUS_ORDER_PUTIN;
    SendData(order);

    QTimer* timer = new QTimer();
    timer->singleShot(5*1000,this,[&]()
    {
        ReadData();
    });
}

void ModbusManagement::readErrors()
{
//    QTimer* timer_read = new QTimer();
//    connect(timer_read, &QTimer::timeout, this, [&]()
//    {

//    });
//    timer_read->start(1000);
}

bool ModbusManagement::checkErrors(uint16_t errors[6])
{
    QVector<QString> result;

    uint16_t byte_1 = errors[0];
    uint16_t byte_2 = errors[1];
    uint16_t byte_3 = errors[2];
    uint16_t byte_4 = errors[3];
    uint16_t byte_5 = errors[4];
    uint16_t byte_6 = errors[5];

    //check byte_1
    if(byte_1 & MODBUS_ERROR_BYTE1_ELEVATOR1_POSITION)
    {
        result.push_back(tr("1號梯位置異常"));
    }
    if(byte_1 & MODBUS_ERROR_BYTE1_ELEVATOR1_SERVER)
    {
        result.push_back(tr("1號梯伺服異常"));
    }
    if(byte_1 & MODBUS_ERROR_BYTE1_ELEVATOR2_POSITION)
    {
        result.push_back(tr("2號梯位置異常"));
    }
    if(byte_1 & MODBUS_ERROR_BYTE1_ELEVATOR2_SERVER)
    {
        result.push_back(tr("1號梯伺服異常"));
    }
    if(byte_1 & MODBUS_ERROR_BYTE1_EMERGENCY)
    {
        result.push_back(tr("緊急停止中"));
    }
    if(byte_1 & MODBUS_ERROR_BYTE1_SUBMECHINE_CONNECTION)
    {
        result.push_back(tr("副機通訊異常"));
    }
    if(byte_1 & MODBUS_ERROR_BYTE1_X1_POSITION)
    {
        result.push_back(tr("X1軸_位置異常"));
    }
    if(byte_1 & MODBUS_ERROR_BYTE1_X1_SERVER)
    {
        result.push_back(tr("X1軸_伺服異常"));
    }
    if(byte_1 & MODBUS_ERROR_BYTE1_X2_POSITION)
    {
        result.push_back(tr("X2軸_位置異常"));
    }
    if(byte_1 & MODBUS_ERROR_BYTE1_X2_SERVER)
    {
        result.push_back(tr("X2軸_伺服異常"));
    }
    if(byte_1 & MODBUS_ERROR_BYTE1_Y_POSITION)
    {
        result.push_back(tr("Y軸_位置異常"));
    }
    if(byte_1 & MODBUS_ERROR_BYTE1_Y_SERVER)
    {
        result.push_back(tr("Y軸_伺服異常"));
    }

    //check byte2
    if(byte_2 & MODBUS_ERROR_BYTE2_ELEVATOR1_BOTH_IN_OUT)
    {
        result.push_back(tr("電梯1_出入重複"));
    }
    if(byte_2 & MODBUS_ERROR_BYTE2_ELEVATOR1_CANNOT_OUT)
    {
        result.push_back(tr("電梯1_無法出庫"));
    }
    if(byte_2 & MODBUS_ERROR_BYTE2_ELEVATOR1_OPENGATE)
    {
        result.push_back(tr("電梯1_門打開"));
    }
    if(byte_2 & MODBUS_ERROR_BYTE2_ELEVATOR1_PUTIN)
    {
        result.push_back(tr("電梯1_放入異常"));
    }
    if(byte_2 & MODBUS_ERROR_BYTE2_ELEVATOR2_OPENGATE)
    {
        result.push_back(tr("電梯2_門打開"));
    }
    if(byte_2 & MODBUS_ERROR_BYTE2_ELEVATOR2_PUTIN)
    {
        result.push_back(tr("電梯2_放入異常"));
    }

    //check byte 3
    if(byte_3 & MODBUS_ERROR_BYTE3_ELEVATOR2_BOTH_IN_OUT)
    {
        result.push_back(tr("電梯2_出入重複"));
    }
    if(byte_3 & MODBUS_ERROR_BYTE3_ELEVATOR2_CANNOT_OUT)
    {
        result.push_back(tr("電梯2_無法出庫"));
    }
    if(byte_3 & MODBUS_ERROR_BYTE3_NONUMBER)
    {
        result.push_back(tr("未輸入編號"));
    }
    if(byte_3 & MODBUS_ERROR_BYTE3_NO_STORE_POSITION)
    {
        result.push_back(tr("儲位已無空位"));
    }
    if(byte_3 & MODBUS_ERROR_BYTE3_OUT)
    {
        result.push_back(tr("取出異常"));
    }
    if(byte_3 & MODBUS_ERROR_BYTE3_NOBOXTOTAKE)
    {
        result.push_back(tr("無箱子可取出"));
    }
    if(byte_3 & MODBUS_ERROR_BYTE3_BOXPUSHFAILED)
    {
        result.push_back(tr("箱子推入失敗"));
    }


    //check byte4
    if(byte_4 & MODBUS_ERROR_BYTE4_ELEVATOR1_DOWN_LIMITED)
    {
        result.push_back(tr("電梯1_下降極限"));
    }

    if(byte_4 & MODBUS_ERROR_BYTE4_ELEVATOR1_UP_LIMITED)
    {
        result.push_back(tr("電梯1_上升極限"));
    }
    if(byte_4 & MODBUS_ERROR_BYTE4_ELEVATOR2_DOWN_LIMITED)
    {
        result.push_back(tr("電梯2_下降極限"));
    }
    if(byte_4 & MODBUS_ERROR_BYTE4_ELEVATOR2_UP_LIMITED)
    {
        result.push_back(tr("電梯2_上升極限"));
    }
    if(byte_4 & MODBUS_ERROR_BYTE4_X_BACKWARD_LIMITED)
    {
        result.push_back(tr("X軸_後退極限"));
    }
    if(byte_4 & MODBUS_ERROR_BYTE4_X_FORWARD_LIMITED)
    {
        result.push_back(tr("X軸_前進極限"));
    }
    if(byte_4 & MODBUS_ERROR_BYTE4_Y_BACKWARD_LIMITED)
    {
        result.push_back(tr("Y軸_後退極限"));
    }
    if(byte_4 & MODBUS_ERROR_BYTE4_Y_FORWARD_LIMITED)
    {
        result.push_back(tr("Y軸_前進極限"));
    }

    if(!result.empty())
    {
//        emit sig_jobmanger_errors(result);
        Sig_ModbusManagement_MainWindow sigToWindow;
        sigToWindow.type = SIG_MODBUSMANAGEMENT_MAINWINDOW_MODBUS_ERRORS;
        sigToWindow.value_modbusErrors.errors.append(result);

        emit sig_mainwindow(sigToWindow);
        return true;
    }
    return false;
}


//    //send data
////    uint16_t send_data[4] = {123, 123, 123,123};
////    modbus_write_registers(server_,0,4,send_data);
//}

void ModbusManagement::checkResult(Modbus_Order order)
{
//    qDebug()<<"check PLC:";
//    qDebug()<<"order type is :"<<order.order_type;
    QTimer* timer = new QTimer();
    QTimer* timer_count_timerOut = new QTimer();

    connect(timer,&QTimer::timeout,this,[&,order,timer,timer_count_timerOut](){

        ModbusState state = ReadData();
//        if(checkErrors(state.errors))
//        {
//            timer.stop();
//        }
//        qDebug()<<"order type is :"<<order.order_type<<"box id is :"<<order.boxOut_id<<"elevator id is : "<<order.elevatorOut_id;
        switch (order.order_type) {
        case MODBUS_ORDER_PUTIN:
        {
//            qDebug()<<"in : elevator is "<<order.elevatorIn_id;
            if(state.state == MODBUS_STATE_IDLE)
//            if(state.state == MODBUS_STATE_AUTOFINISH)
            {
                emit sig_jobmanager_finish(order.order_type);
                timer->stop();
            }
            break;
        }
        case MODBUS_ORDER_OUT:
        {
//            qDebug()<<"out : elevator is "<<order.elevatorOut_id<<" state is "<<state.state;
            if(state.state == MODBUS_STATE_AUTOFINISH)
            {
                emit sig_jobmanager_finish(order.order_type);
                timer->stop();
            }
            break;
        }
        case MODBUS_ORDER_RESTORE:
        {
            if(state.state == MODBUS_STATE_IDLE)
            {
                Sig_ModbusManagement_JobManagement sigToJob;
                sigToJob.type = SIG_MODBUSMANAGEMENT_JOBMANAGEMENT_RESTORED;
                qDebug()<<"modbus to job : restored";
                emit sig_jobmanager(sigToJob);

                timer->stop();
            }
            break;
        }
        case MODBUS_ORDER_DIRECT_PUTIN:
        {
//            qDebug()<<"in : elevator is "<<order.elevatorIn_id<<" state is "<<state.state;
            if(state.state == MODBUS_STATE_AUTOFINISH)
            {
                emit sig_jobmanager_finish(order.order_type);
                timer->stop();
            }
            break;
        }
        case MODBUS_ORDER_DIRECT_OUT:
        {
//            qDebug()<<"out : elevator is "<<order.elevatorOut_id<<" state is "<<state.state;
            if(state.state == MODBUS_STATE_AUTOFINISH)
            {
                emit sig_jobmanager_finish(order.order_type);
                timer->stop();
            }
            break;
        }
        case MODBUS_ORDER_ELEVATOR_1_UP:
        {
//            qDebug()<<state.state;
            if(state.elevator_1_position == MODBUS_ELEVATOR_STATE_POSITION_UP)
            {
                Sig_ModbusManagement_JobManagement sigToJob;
                sigToJob.type = SIG_MODBUSMANAGEMENT_JOBMANAGEMENT_ELEVATOR_UP;
                sigToJob.value_elevatorUp.type = state.elevator_1_state;
                sigToJob.value_elevatorUp.elevatorId = 1;
                sigToJob.value_elevatorUp.boxId = state.boxId_putIn;

                qDebug()<<"modbus to job"<<"elevator up:"<<1;
                emit sig_jobmanager(sigToJob);
                timer->stop();
            }
            break;
        }
        case MODBUS_ORDER_ELEVATOR_2_UP:
        {
            if(state.elevator_2_position == MODBUS_ELEVATOR_STATE_POSITION_UP)
            {
                Sig_ModbusManagement_JobManagement sigToJob;
                sigToJob.type = SIG_MODBUSMANAGEMENT_JOBMANAGEMENT_ELEVATOR_UP;
                sigToJob.value_elevatorUp.type = state.elevator_2_state;
                sigToJob.value_elevatorUp.elevatorId = 2;
                sigToJob.value_elevatorUp.boxId = state.boxId_putIn;

                qDebug()<<"modbus to job"<<"elevator up:"<<2;
                emit sig_jobmanager(sigToJob);
                timer->stop();
            }
            break;
        }
        case MODBUS_ORDER_ELEVATOR_1_DOWN:
        {
            if(state.elevator_1_position == MODBUS_ELEVATOR_STATE_POSITION_DOWN)
            {
                qDebug()<<"elevator 1 state is "<<state.elevator_1_state;
                Sig_ModbusManagement_JobManagement sigToJob;
                sigToJob.type = SIG_MODBUSMANAGEMENT_JOBMANAGEMENT_ELEVATOR_DOWN;
                sigToJob.value_elevatorDown.type = state.elevator_1_state;
                sigToJob.value_elevatorDown.elevatorId = 1;
                sigToJob.value_elevatorDown.boxId = state.boxId_out;
                emit sig_jobmanager(sigToJob);
                timer->stop();
            }
            break;
        }
        case MODBUS_ORDER_ELEVATOR_2_DOWN:
        {
            if(state.elevator_2_position == MODBUS_ELEVATOR_STATE_POSITION_DOWN)
            {
                qDebug()<<"elevator 2 state is "<<state.elevator_2_state;
                Sig_ModbusManagement_JobManagement sigToJob;
                sigToJob.type = SIG_MODBUSMANAGEMENT_JOBMANAGEMENT_ELEVATOR_DOWN;
                sigToJob.value_elevatorDown.type = state.elevator_2_state;
                sigToJob.value_elevatorDown.elevatorId = 2;
                sigToJob.value_elevatorDown.boxId = state.boxId_out;

                emit sig_jobmanager(sigToJob);
                timer->stop();
            }
            break;
        }
        case MODBUS_ORDER_XY_OUT:
        {
            if(state.state == MODBUS_STATE_IDLE)
//            if(state.state == MODBUS_STATE_AUTOFINISH)
            {
                Sig_ModbusManagement_JobManagement sigToJob;
                sigToJob.type = SIG_MODBUSMANAGEMENT_JOBMANAGEMENT_XY_OUT;
                sigToJob.value_xyOut.boxId = order.boxOut_id;
                sigToJob.value_xyOut.elevatorId = order.elevatorOut_id;
//                sigToJob.value_xyOut.elevatorId = state.elevatorId;

//                qDebug()<<"state: "<<"elevator :"<<state.elevatorId<<"box:"<<state.boxId_out;

                emit sig_jobmanager(sigToJob);
                timer->stop();
            }
            break;
        }
        case MODBUS_ORDER_XY_PUTIN:
        {
            if(state.state == MODBUS_STATE_IDLE)
//            if(state.state == MODBUS_STATE_AUTOFINISH)
            {
                Sig_ModbusManagement_JobManagement sigToJob;
                sigToJob.type = SIG_MODBUSMANAGEMENT_JOBMANAGEMENT_XY_IN;
                sigToJob.value_xyIn.boxId = order.boxIn_id;
                sigToJob.value_xyIn.elevatorId = order.elevatorIn_id;

                emit sig_jobmanager(sigToJob);
                timer->stop();
            }
            break;
        }
        default:
        {
            qDebug()<<"ModbusManagement::checkResult : unknown order";
        }
        }

        if(!timer->isActive())
        {
            qDebug()<<"get modbus response";
            timer_count_timerOut->stop();
        }
    });

    connect(timer_count_timerOut,&QTimer::timeout,this,[&,timer,timer_count_timerOut](){
            qDebug()<<"modbus no response";
            Sig_ModbusManagement_MainWindow sigToWindow;
            sigToWindow.type = SIG_MODBUSMANAGEMENT_MAINWINDOW_ORDER_TIMEOUT;
            emit sig_mainwindow(sigToWindow);
            timer->stop();
            timer_count_timerOut->stop();
            return;
    });

    timer->start(500);
    timer_count_timerOut->start(60*1000);
}

void ModbusManagement::checkResult_mainwindow(Modbus_Order order)
{
        QTimer* timer = new QTimer();
        connect(timer,&QTimer::timeout,this,[&,order,timer](){
            ModbusState state = ReadData();
            switch (order.order_type) {
            case MODBUS_ORDER_ELEVATOR_1_UP:
            {
    //            qDebug()<<state.state;
                if(state.elevator_1_position == MODBUS_ELEVATOR_STATE_POSITION_UP)
                {
                    Sig_ModbusManagement_MainWindow sigToWindow;
                    sigToWindow.type = SIG_MODBUSMANAGEMENT_MAINWINDOW_ELEVATOR_UP;
                    sigToWindow.value_elevatorUp.type = state.elevator_1_state;
                    sigToWindow.value_elevatorUp.elevatorId = 1;
                    sigToWindow.value_elevatorUp.boxId = state.boxId_putIn;

                    qDebug()<<"modbus to mainwindow"<<"elevator up:"<<1;
                    emit sig_mainwindow(sigToWindow);
                    timer->stop();
                }
                break;
            }
            case MODBUS_ORDER_ELEVATOR_2_UP:
            {
                if(state.elevator_2_position == MODBUS_ELEVATOR_STATE_POSITION_UP)
                {
                    Sig_ModbusManagement_MainWindow sigToWindow;
                    sigToWindow.type = SIG_MODBUSMANAGEMENT_MAINWINDOW_ELEVATOR_UP;
                    sigToWindow.value_elevatorUp.type = state.elevator_2_state;
                    sigToWindow.value_elevatorUp.elevatorId = 2;
                    sigToWindow.value_elevatorUp.boxId = state.boxId_putIn;

                    qDebug()<<"modbus to mainwindow"<<"elevator up:"<<2;
                    emit sig_mainwindow(sigToWindow);
                    timer->stop();
                }
                break;
            }
            case MODBUS_ORDER_ELEVATOR_1_DOWN:
            {
                if(state.elevator_1_position == MODBUS_ELEVATOR_STATE_POSITION_DOWN)
                {
                    qDebug()<<"elevator state is "<<state.elevator_1_state;
                    Sig_ModbusManagement_MainWindow sigToWindow;
                    sigToWindow.type = SIG_MODBUSMANAGEMENT_MAINWINDOW_ELEVATOR_DOWN;
                    sigToWindow.value_elevatorDown.type = state.elevator_1_state;
                    sigToWindow.value_elevatorDown.elevatorId = 1;
                    sigToWindow.value_elevatorDown.boxId = state.boxId_out;
                    emit sig_mainwindow(sigToWindow);
                    timer->stop();
                }
                break;
            }
            case MODBUS_ORDER_ELEVATOR_2_DOWN:
            {
                if(state.elevator_2_position == MODBUS_ELEVATOR_STATE_POSITION_DOWN)
                {
                    Sig_ModbusManagement_MainWindow sigToWindow;
                    sigToWindow.type = SIG_MODBUSMANAGEMENT_MAINWINDOW_ELEVATOR_DOWN;
                    sigToWindow.value_elevatorDown.type = state.elevator_2_state;
                    sigToWindow.value_elevatorDown.elevatorId = 2;
                    sigToWindow.value_elevatorDown.boxId = state.boxId_out;

                    emit sig_mainwindow(sigToWindow);
                    timer->stop();
                }
                break;
            }
            case MODBUS_ORDER_XY_OUT:
            {
                if(state.state == MODBUS_STATE_IDLE)
    //            if(state.state == MODBUS_STATE_AUTOFINISH)
                {
                    Sig_ModbusManagement_MainWindow sigToWindow;
                    sigToWindow.type = SIG_MODBUSMANAGEMENT_MAINWINDOW_XY_OUT;
                    sigToWindow.value_xyOut.boxId = order.boxOut_id;
                    sigToWindow.value_xyOut.elevatorId = order.elevatorIn_id;
    //                sigToJob.value_xyOut.elevatorId = state.elevatorId;

    //                qDebug()<<"state: "<<"elevator :"<<state.elevatorId<<"box:"<<state.boxId_out;

                    emit sig_mainwindow(sigToWindow);
                    timer->stop();
                }
                break;
            }
            case MODBUS_ORDER_XY_PUTIN:
            {
                if(state.state == MODBUS_STATE_IDLE)
    //            if(state.state == MODBUS_STATE_AUTOFINISH)
                {
                     Sig_ModbusManagement_MainWindow sigToWindow;
                    sigToWindow.type = SIG_MODBUSMANAGEMENT_MAINWINDOW_XY_IN;
                    sigToWindow.value_xyIn.boxId = order.boxIn_id;
                    sigToWindow.value_xyIn.elevatorId = order.elevatorIn_id;

                    emit sig_mainwindow(sigToWindow);
                    timer->stop();
                }
                break;
            }
            default:
            {
                qDebug()<<"ModbusManagement::checkResult : unknown order";
            }
            }
        });
        timer->start(500);
}

void ModbusManagement::checkPLC()
{
    ModbusState state = ReadData();
    switch(state.state)
    {
    case MODBUS_STATE_NEED_RESTORE:
    {
        QVector<QString> errors;
        errors.push_back(QString(tr("機臺需要復歸")));
        emit sig_jobmanger_errors(errors);
        break;
    }
    }
}

void ModbusManagement::SendData(Modbus_Order order)
{
    int boxIn_id = order.boxIn_id;
    int boxOut_id = order.boxOut_id;
    int elevatorIn_id = order.elevatorIn_id;
    int elevatorOut_id = order.elevatorOut_id;

    qDebug()<<"send data :"<<"type is "<<order.order_type<<" box in: "<<boxIn_id<<" box out:"<<boxOut_id<<"elevator in:"<<elevatorIn_id<<" elevator out:"<<elevatorOut_id;
    switch (order.order_type) {
    case MODBUS_ORDER_PUTIN:
    {
        uint16_t send_data[10] = MODBUS_PCORDER_PUTIN((uint16_t)boxIn_id,(uint16_t)boxOut_id,(uint16_t)elevatorIn_id);
        modbus_write_registers(server_,MODBUS_WRITE_ADDRESS,10,send_data);
        break;
        }
    case MODBUS_ORDER_OUT:
    {
        uint16_t send_data[10] = MODBUS_PCORDER_OUT((uint16_t)boxIn_id,(uint16_t)boxOut_id,(uint16_t)elevatorOut_id);
        modbus_write_registers(server_,MODBUS_WRITE_ADDRESS,10,send_data);
        break;
    }
    case MODBUS_ORDER_PUTIN_AND_OUT:
    {
        uint16_t send_data[10] = MODBUS_PCORDER_PUTIN_AND_OUT((uint16_t)boxIn_id,(uint16_t)boxOut_id,(uint16_t)elevatorIn_id,(uint16_t)elevatorOut_id);
        modbus_write_registers(server_,MODBUS_WRITE_ADDRESS,10,send_data);
        break;
    }
    case MODBUS_ORDER_RESTORE:
    {
        uint16_t send_data[10] = MODBUS_PCORDER_RESTORE;
        modbus_write_registers(server_,MODBUS_WRITE_ADDRESS,10,send_data);
        break;
    }
    case MODBUS_ORDER_CLEANERRORS:
    {
        uint16_t send_data[10] = MODBUS_PCORDER_CLEANERRORS;
        modbus_write_registers(server_,MODBUS_WRITE_ADDRESS,10,send_data);
        break;
    }
    case MODBUS_ORDER_AUTOCANCEL:
    {
        uint16_t send_data[10] = MODBUS_PCORDER_AUTOCANCEL;
        modbus_write_registers(server_,MODBUS_WRITE_ADDRESS,10,send_data);
        break;
    }
    case MODBUS_ORDER_NOORDER:
    {
        uint16_t send_data[10] = MODBUS_PCORDER_NOORDER;
        modbus_write_registers(server_,MODBUS_WRITE_ADDRESS,10,send_data);
        break;
    }
    case MODBUS_ORDER_DIRECT_PUTIN:
    {
        uint16_t send_data[10] =  MODBUS_PCORDER_DIRECT_PUTIN((uint16_t)boxIn_id,(uint16_t)elevatorIn_id);
        modbus_write_registers(server_,MODBUS_WRITE_ADDRESS,10,send_data);
        break;
    }
    case MODBUS_ORDER_DIRECT_OUT:
    {
        uint16_t send_data[10] = MODBUS_PCORDER_DIRECT_OUT((uint16_t)elevatorOut_id);
        modbus_write_registers(server_,MODBUS_WRITE_ADDRESS,10,send_data);
        break;
    }
    case MODBUS_ORDER_XY_PUTIN:
    {
        uint16_t send_data[10] = MODBUS_PCORDER_XY_PUTIN((uint16_t)boxIn_id,(uint16_t)elevatorIn_id);
        modbus_write_registers(server_,MODBUS_WRITE_ADDRESS,10,send_data);
        break;
    }
    case MODBUS_ORDER_XY_OUT:
    {
        uint16_t send_data[10] = MODBUS_PCORDER_XY_OUT((uint16_t)boxOut_id,(uint16_t)elevatorOut_id);
//        qDebug()<<"send :";
//        for(int i=0;i<10;i++)
//        {
//            qDebug()<<send_data[i];
//        }
        modbus_write_registers(server_,MODBUS_WRITE_ADDRESS,10,send_data);
        break;
    }
    case MODBUS_ORDER_ELEVATOR_1_DOWN:
    {
        uint16_t send_data[10] = MODBUS_PCORDER_ELEVATOR_1_DOWN;
        modbus_write_registers(server_,MODBUS_WRITE_ADDRESS,10,send_data);
        break;
    }
    case MODBUS_ORDER_ELEVATOR_1_UP:
    {
        uint16_t send_data[10] = MODBUS_PCORDER_ELEVATOR_1_UP;
        modbus_write_registers(server_,MODBUS_WRITE_ADDRESS,10,send_data);
        break;
    }
    case MODBUS_ORDER_ELEVATOR_2_DOWN:
    {
        uint16_t send_data[10] = MODBUS_PCORDER_ELEVATOR_2_DOWN;
        modbus_write_registers(server_,MODBUS_WRITE_ADDRESS,10,send_data);
        break;
    }
    case MODBUS_ORDER_ELEVATOR_2_UP:
    {
        uint16_t send_data[10] = MODBUS_PCORDER_ELEVATOR_2_UP;
        modbus_write_registers(server_,MODBUS_WRITE_ADDRESS,10,send_data);
        break;
    }
    default:
    {
        qDebug()<<"unknown plc order type";
    }
    }
}
ModbusState ModbusManagement::ReadData()
{
    //read data
    uint16_t regist_read[20];
    modbus_read_registers(server_,MODBUS_READ_ADDRESS+11,20,regist_read);
//    qDebug()<<"modbus data:";
//    for(int i=0;i<20;i++)
//    {
//        qDebug()<<regist_read[i];
//    }

    ModbusState state;

    state.state = (MODBUS_STATE)regist_read[0];
    state.boxId_putIn = regist_read[1];
    state.boxId_out = regist_read[2];
    state.elevatorId = regist_read[3];
    state.xy_state.state = (MODBUS_XY_STATE)regist_read[4];
    state.xy_state.target_position = regist_read[5];
    state.elevator_1_position = (MODBUS_ELEVATOR_STATE_POSITION)regist_read[6];
    state.elevator_1_state = (MODBUS_ELEVATOR_STATE_STATE)regist_read[7];
    state.elevator_2_position = (MODBUS_ELEVATOR_STATE_POSITION)regist_read[8];
    state.elevator_2_state = (MODBUS_ELEVATOR_STATE_STATE)regist_read[9];
    state.flag = regist_read[12];
    for(int i=0;i<6;i++)
    {
        state.errors[i] = regist_read[13+i];
    }
//    checkErrors(state.errors);
    return state;
}

void ModbusManagement::slot_jobmanager_order(Modbus_Order order)
{
    SendData(order);

    qDebug()<<"ModbusManagement::slot_jobmanager_order:"<<"order type is :"<<order.order_type;
    checkResult(order);
}

void ModbusManagement::slot_mainwindow_order(Sig_Mainwindown_Modbusmanagement sig)
{
    Modbus_Order order;
    switch (sig.type) {
    case SIG_MAINWINDOWN_MODBUSMANAGEMENT_XY_PUTIN:
    {

        order.order_type = MODBUS_ORDER_XY_PUTIN;
        order.boxIn_id = sig.value_xy.boxId;
        order.elevatorIn_id = sig.value_xy.elevatorId;

        SendData(order);
        break;
    }
    case SIG_MAINWINDOWN_MODBUSMANAGEMENT_XY_OUT:
    {
        order.order_type = MODBUS_ORDER_XY_OUT;
        order.boxOut_id = sig.value_xy.boxId;
        order.elevatorOut_id = sig.value_xy.elevatorId;

        SendData(order);
        break;
    }
    case SIG_MAINWINDOWN_MODBUSMANAGEMENT_ELEVATOR_UP:
    {
        if(sig.value_elevator.elevatorId == 1)
        {
            order.order_type = MODBUS_ORDER_ELEVATOR_1_UP;
        }
        else if(2 == sig.value_elevator.elevatorId)
        {
            order.order_type = MODBUS_ORDER_ELEVATOR_2_UP;
        }
        else
        {
            qDebug()<<"elevator number error";
        }
        order.elevatorIn_id = sig.value_elevator.elevatorId;

        SendData(order);
        break;
    }
    case SIG_MAINWINDOWN_MODBUSMANAGEMENT_ELEVATOR_DOWN:
    {
        if(1 == sig.value_elevator.elevatorId)
        {
            order.order_type = MODBUS_ORDER_ELEVATOR_1_DOWN;
        }
        else if(2 == sig.value_elevator.elevatorId)
        {
            order.order_type = MODBUS_ORDER_ELEVATOR_2_DOWN;
        }
        else
        {
            qDebug()<<"elevator number error";
        }
        order.elevatorIn_id = sig.value_elevator.elevatorId;

        SendData(order);

        break;
    }
    default:
    {
        qDebug()<<"ModbusManagement::slot_mainwindow_order : unknown type";
    }
    }
    checkResult_mainwindow(order);
}

void ModbusManagement::slot_jobmanager(Sig_JobManagement_Modbusmanagement sig)
{
    Modbus_Order order;
    switch (sig.type) {
    case SIG_JOBMANAGEMENT_MODBUSMANAGEMENT_XY_PUTIN:
    {
        order.order_type = MODBUS_ORDER_XY_PUTIN;
        order.boxIn_id = sig.value_xy.boxId;
        order.elevatorIn_id = sig.value_xy.elevatorId;

        SendData(order);
        break;
    }
    case SIG_JOBMANAGEMENT_MODBUSMANAGEMENT_XY_OUT:
    {
        order.order_type = MODBUS_ORDER_XY_OUT;
        order.boxOut_id = sig.value_xy.boxId;
        order.elevatorOut_id = sig.value_xy.elevatorId;

        qDebug()<<order.order_type<<","<<order.boxOut_id<<","<<order.elevatorOut_id;
        SendData(order);
        break;
    }
    case SIG_JOBMANAGEMENT_MODBUSMANAGEMENT_ELEVATOR_UP:
    {
        if(sig.value_elevator.elevatorId == 1)
        {
            order.order_type = MODBUS_ORDER_ELEVATOR_1_UP;
        }
        else if(2 == sig.value_elevator.elevatorId)
        {
            order.order_type = MODBUS_ORDER_ELEVATOR_2_UP;
        }
        else
        {
            qDebug()<<"elevator number error";
            return;
        }
        order.elevatorIn_id = sig.value_elevator.elevatorId;

        SendData(order);
        break;
    }
    case SIG_JOBMANAGEMENT_MODBUSMANAGEMENT_ELEVATOR_DOWN:
    {
        if(1 == sig.value_elevator.elevatorId)
        {
            order.order_type = MODBUS_ORDER_ELEVATOR_1_DOWN;
        }
        else if(2 == sig.value_elevator.elevatorId)
        {
            order.order_type = MODBUS_ORDER_ELEVATOR_2_DOWN;
        }
        else
        {
            qDebug()<<"elevator number error";
        }
        order.elevatorIn_id = sig.value_elevator.elevatorId;

        SendData(order);

        break;
    }
    case SIG_JOBMANAGEMENT_MODBUSMANAGEMENT_PLC_RECOVERY:
    {
        order.order_type = MODBUS_ORDER_RESTORE;
        SendData(order);

        break;
    }
    default:
    {
        qDebug()<<"ModbusManagement::slot_jobmanger : unknown type";
    }
    }

    checkResult(order);
}
