#include "boxmanager.h"
#include<QDebug>

BoxManager::BoxManager(QObject *parent) : QObject(parent)
{

}

void BoxManager::InitBoxs(QVector<Box_Warehouse> boxs)
{
    //qDebug()<<"BoxManager::InitBoxs get box number: "<<boxs.size();
    for(int i=0;i<boxs.size();i++)
    {
        //qDebug()<<"BoxManager::InitBoxs get box number: "<<boxs[i].id;
        if(boxs[i].id>0)
        {
            vec_box_.push_back(boxs[i]);
        }
    }
}
//int Default_Set_NumberOfBox();
float BoxManager::Get_LeftVolumeOfBox(int box_id)
{
    for(auto box: vec_box_)
    {
        if(box.id == box_id)
        {
            return box.left_volume;
        }
    }
    qDebug()<<"BoxManager::Get_LeftVolumeOfBox :box not found";
    return 0;
}
int BoxManager::Get_MostEmptyBox()
{
    if(vec_box_.empty())
    {
        return -1;
    }
    int most_empty_box_index = 0;
    for(int i=0;i<vec_box_.size();i++)
    {
        qDebug()<< vec_box_[i].left_volume<<",most empty volume:"<<vec_box_[most_empty_box_index].left_volume;
        if(vec_box_[i].left_volume > vec_box_[most_empty_box_index].left_volume)
        {
            most_empty_box_index = i;
            qDebug()<<"most empty box is "<<vec_box_[most_empty_box_index].id;
        }
    }
    return vec_box_[most_empty_box_index].id;
}

int BoxManager::Get_MostFitBox(float volume)
{
    if(vec_box_.empty())
    {
        qDebug()<<"BoxManager::Get_MostFitBox : no box";
        return -1;
    }
    int most_fit_box_index = 0;
    for(int i=0;i<vec_box_.size();i++)
    {
        if(vec_box_[i].left_volume > volume)
        {
            most_fit_box_index = i;
            break;
        }
    }

    for(int i=0;i<vec_box_.size();i++)
    {
        if(vec_box_[i].left_volume  > volume &&vec_box_[i].left_volume < vec_box_[most_fit_box_index].left_volume)
        {
            most_fit_box_index = i;
        }
    }
    return vec_box_[most_fit_box_index].id;
}

int BoxManager::Get_MostFitBox_Except(float volume,int box_id)
{
    if(vec_box_.empty())
    {
        qDebug()<<"BoxManager::Get_MostFitBox : no box";
        return -1;
    }
    int most_fit_box_index = 0;
    for(int i=0;i<vec_box_.size();i++)
    {
        if(vec_box_[i].left_volume > volume&&vec_box_[i].id != box_id)
        {
            most_fit_box_index = i;
            break;
        }
    }

    for(int i=0;i<vec_box_.size();i++)
    {
        if(vec_box_[i].id != box_id && vec_box_[i].left_volume  > volume &&vec_box_[i].left_volume < vec_box_[most_fit_box_index].left_volume)
        {
            most_fit_box_index = i;
        }
    }
    return vec_box_[most_fit_box_index].id;
}

void BoxManager::Update_Box_LeftVolume(int box_id,float left_volume)
{
    for(int i=0;i<vec_box_.size();i++)
    {
        if(vec_box_[i].id == box_id)
        {
            qDebug()<<"update box "<<box_id<<" volume : "<<left_volume;
            vec_box_[i].left_volume = left_volume;

            Sig_BoxManagement_DBManagement sig;
            sig.type = SIG_BOXMANAGEMENT_DBMANAGEMENT_UPDATE_BOXVOLUME;
            sig.value_update_boxvolume.box_id = box_id;
            sig.value_update_boxvolume.volume = left_volume;
            send_sig(sig);
            return;
        }
    }
    qDebug()<<"BoxManager::Update_Box_LeftVolume :box not found";
    return;
}


void BoxManager::slot_dbmanager(Sig_DBManagement_BoxManagement sig)
{
    switch (sig.type) {
    case SIG_DBMANAGEMENT_BOXMANAGEMENT_INITBOX:
    {
        InitBoxs(sig.value_initbox.boxs);
        break;
    }
    default:
    {
        qDebug()<<"SIG_BOXMANAGEMENT_DBMANAGEMENT : error signal";
    }
    }
}

void BoxManager::slot_jobmanager(Sig_JobManagement_BoxManagement sig)
{
    switch (sig.type) {
    case SIG_JOBMANAGEMENT_BOXMANAGEMENT_ASK_BOXMOSTEMPTY:
    {
        Sig_BoxManagement_JobManagement sigToJob;
        sigToJob.type = SIG_BOXMANAGEMENT_JOBMANAGEMENT_MOSTEMPTYBOX;
        sigToJob.value_mostEmptyBox.box_id = Get_MostEmptyBox();

        emit sig_jobmanager(sigToJob);
        break;
    }
    case SIG_JOBMANAGEMENT_BOXMANAGEMENT_UPDATE_BOX_VOLUME:
    {
        Update_Box_LeftVolume(sig.value_update_boxvolume.box_id,sig.value_update_boxvolume.volume);
        break;
    }
    default:
    {
        qDebug()<<"SIG_BOXMANAGEMENT_DBMANAGEMENT : error signal";
        break;
    }
    }
}

void BoxManager::slot_mainwindow(Sig_Mainwindow_Boxmanagement sig)
{
    switch (sig.type) {
    case SIG_MAINWINDOW_BOXMANAGEMENT_UPDATE_BOXVOLUME:
    {
        int boxId = sig.value_updateBoxVolume.boxId;
        float volume = sig.value_updateBoxVolume.volume;
        if(boxId>0&&volume>=0)
        {
            Update_Box_LeftVolume(sig.value_updateBoxVolume.boxId,sig.value_updateBoxVolume.volume);
        }
        break;
    }
    default:
    {
        qDebug()<<"unknow sig from window";
        break;
    }
    }
}

void BoxManager::send_sig(Sig_BoxManagement_DBManagement sig)
{
    qDebug()<<"box send sig to db";
    emit sig_dbmanager(sig);
}
