#ifndef BOXMANAGER_H
#define BOXMANAGER_H

#include <QObject>
#include<QVector>
#include"common.h"

class BoxManager : public QObject
{
    Q_OBJECT
public:
    explicit BoxManager(QObject *parent = nullptr);
    void InitBoxs(QVector<Box_Warehouse> boxs);//each item open system ,need to init this
    float Get_LeftVolumeOfBox(int box_id);
    int Get_MostEmptyBox();
    void Update_Box_LeftVolume(int box_id,float left_volume);//used by takepage to update box left volume

    //for expansion
    int Get_MostFitBox(float volume);//used by single putIn
    int Get_MostFitBox_Except(float volume,int box_id);//used by single putIn's change box
private:
    void send_sig(Sig_BoxManagement_DBManagement sig);
signals:
    void sig_dbmanager(Sig_BoxManagement_DBManagement sig);
    void sig_jobmanager(Sig_BoxManagement_JobManagement sig);
public slots:
    void slot_dbmanager(Sig_DBManagement_BoxManagement sig);
    void slot_jobmanager(Sig_JobManagement_BoxManagement sig);
    void slot_mainwindow(Sig_Mainwindow_Boxmanagement sig);
private:
    QVector<Box_Warehouse> vec_box_;
};

#endif // BOXMANAGER_H
