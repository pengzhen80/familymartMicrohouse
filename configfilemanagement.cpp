#include "configfilemanagement.h"

ConfigFileManagement::ConfigFileManagement(QObject *parent) : QObject(parent)
{

}

void ConfigFileManagement::Init()
{
    QString fileName;
    fileName = QCoreApplication::applicationDirPath();
    fileName += "/config.ini";

    qDebug()<<"config file : "<<fileName;

    settingcfg_ = new QSettings(fileName, QSettings::IniFormat);

//    if(settingcfg_->value("BoxLeftVolume").toString().size() == 0)
//    {
//        settingcfg_->setValue("BoxLeftVolume", 0.0);
//    }
//    settingcfg_->setValue("inter", 6);
//    qDebug()<<"setting : inter :"<<settingcfg_->value("inter").toDouble();  // returns 6.55
}

void ConfigFileManagement::GetConfigs()
{
//    Sig_ConfigManagement_JobManagement sig;
//    sig.type = SIG_CONFIGMANAGEMENT_JOBMANAGEMENT_ALLCONFIG;
//    sig.value_allConfig.realsence_box_sample_elevator1 = getRealsenceBoxSample_elevator1();
//    sig.value_allConfig.empty_box_volume_elevator1 = getEmptyBoxVolume_elevator1();
//    sig.value_allConfig.realsence_box_sample_elevator2 = getRealsenceBoxSample_elevator2();
//    sig.value_allConfig.empty_box_volume_elevator2 = getEmptyBoxVolume_elevator2();

//    emit sig_jobmanager(sig);
    Sig_ConfigManagement_RealsenceManagement sigToRealsence;
    sigToRealsence.type = SIG_CONFIGMANAGEMENT_REALSENCEMANAGEMENT_ALLCONFIG;
    sigToRealsence.value_allConfig.realsence_box_sample_elevator1 = getRealsenceBoxSample_elevator1();
    sigToRealsence.value_allConfig.empty_box_volume_elevator1 = getEmptyBoxVolume_elevator1();
    sigToRealsence.value_allConfig.realsence_box_sample_elevator2 = getRealsenceBoxSample_elevator2();
    sigToRealsence.value_allConfig.empty_box_volume_elevator2 = getEmptyBoxVolume_elevator2();

    emit sig_realsence(sigToRealsence);
}


void ConfigFileManagement::WriteData(QString key,QString value)
{
    if(settingcfg_ == nullptr)
    {
        qDebug()<<"cfg not open";
        return;
    }
    settingcfg_->setValue(key,value);
}

QString ConfigFileManagement::ReadData(QString key)
{
    QString result;
    if(settingcfg_ != nullptr)
    {
        result = settingcfg_->value(key).toString();
    }
    return result;
}

void ConfigFileManagement::slot_jobmanager_askconfig(QString key)
{
    qDebug()<<"ConfigFileManagement::slot_jobmanager_askconfig : "<<key;
    emit sig_jobmanager_config(key,ReadData(key));
}
void ConfigFileManagement::slot_jobmanager_setconfig(QString key,QString value)
{
    WriteData(key,value);
}

void ConfigFileManagement::setBoxSample_elevator1(Array_TwoDimensional<float> sample_box)
{
    for(unsigned int i_x=0;i_x<sample_box.x;i_x++)
    {
        for(unsigned int i_y=0;i_y<sample_box.y;i_y++)
        {
//            qDebug()<<"key "<< QString("elevator1_")+QString::number(i_x*sample_box.y+i_y)<<",value: "<< QString::number(sample_box.data.value(i_x*sample_box.y+i_y));
            WriteData(QString("elevator1_")+QString::number(i_x*sample_box.y+i_y),QString::number(sample_box.data.value(i_x*sample_box.y+i_y)));
        }
    }
}

void ConfigFileManagement::setEmptyBoxVolume_elevator1(float volume)
{
    WriteData(QString("elevator1_")+QString("volume"),QString::number(volume));
}

void ConfigFileManagement::setBoxSample_elevator2(Array_TwoDimensional<float> sample_box)
{
    for(unsigned int i_x=0;i_x<sample_box.x;i_x++)
    {
        for(unsigned int i_y=0;i_y<sample_box.y;i_y++)
        {
            WriteData(QString("elevator2_")+QString::number(i_x*sample_box.y+i_y),QString::number(sample_box.data.value(i_x*sample_box.y+i_y)));
        }
    }
}
void ConfigFileManagement::setEmptyBoxVolume_elevator2(float volume)
{
        WriteData(QString("elevator2_")+QString("volume"),QString::number(volume));
}

void ConfigFileManagement::slot_jobmanager(Sig_JobManagement_ConfigManagement sig)
{
    qDebug()<<"get sig from job";
    switch (sig.type) {
    case SIG_JOBMANAGEMENT_CONFIGMANAGEMENT_SET_EMPTYBOXSAMPLE_ELEVATOR1:
    {

        setBoxSample_elevator1(sig.value_setEmptyBoxSample.box_sample);
        setEmptyBoxVolume_elevator1(sig.value_setEmptyBoxSample.box_empty_volume);
        break;
    }
    case SIG_JOBMANAGEMENT_CONFIGMANAGEMENT_SET_EMPTYBOXSAMPLE_ELEVATOR2:
    {
        setBoxSample_elevator2(sig.value_setEmptyBoxSample.box_sample);
        setEmptyBoxVolume_elevator2(sig.value_setEmptyBoxSample.box_empty_volume);
        break;
    }
    default:
    {
        qDebug()<<"ConfigFileManagement::slot_jobmanager : unknown signal";
        break;
    }
    }
}

void ConfigFileManagement::slot_realsence(Sig_RealsenceManagement_ConfigManagement sig)
{
    qDebug()<<"get sig from realsence";
    switch (sig.type) {
    case SIG_REALSENCEMANAGEMENT_CONFIGMANAGEMENT_BOXEMPTYVOLUME_ELEVATOR1:
    {
        qDebug()<<"update elevator1 sample";
        setBoxSample_elevator1(sig.value_BoxEmptyVolume_elevator1.realsence_box_sample);
        setEmptyBoxVolume_elevator1(sig.value_BoxEmptyVolume_elevator1.volume);
        break;
    }
    case SIG_REALSENCEMANAGEMENT_CONFIGMANAGEMENT_BOXEMPTYVOLUME_ELEVATOR2:
    {
        qDebug()<<"update elevator2 sample";
        setBoxSample_elevator2(sig.value_BoxEmptyVolume_elevator2.realsence_box_sample);
        setEmptyBoxVolume_elevator2(sig.value_BoxEmptyVolume_elevator2.volume);
        break;
    }
    default:
    {
        qDebug()<<"ConfigFileManagement::slot_jobmanager : unknown signal";
        break;
    }
    }
}

Array_TwoDimensional<float> ConfigFileManagement::getRealsenceBoxSample_elevator1()
{
    Array_TwoDimensional<float> sampleBox;
    sampleBox.x = REALSENSE_TOP_PIXEL_WIDTH;
    sampleBox.y = REALSENSE_TOP_PIXEL_LENGTH;
    //float sample_box[REALSENSE_TOP_PIXEL_WIDTH][REALSENSE_TOP_PIXEL_LENGTH];
    for(unsigned int i_x=0;i_x<sampleBox.x;i_x++)
    {
        for(unsigned int i_y=0;i_y<sampleBox.y;i_y++)
        {
            float data = ReadData(QString("elevator1_")+QString::number(i_x*sampleBox.y+i_y)).toFloat();
//            qDebug()<<"key "<< QString("elevator1_")+QString::number(i_x*sampleBox.y+i_y)<<",value: "<< data;
            sampleBox.data.push_back(data);
        }
    }

    return sampleBox;
}
Array_TwoDimensional<float> ConfigFileManagement::getRealsenceBoxSample_elevator2()
{
    Array_TwoDimensional<float> sampleBox;
    sampleBox.x = REALSENSE_TOP_PIXEL_WIDTH;
    sampleBox.y = REALSENSE_TOP_PIXEL_LENGTH;
    //float sample_box[REALSENSE_TOP_PIXEL_WIDTH][REALSENSE_TOP_PIXEL_LENGTH];
    for(unsigned int i_x=0;i_x<sampleBox.x;i_x++)
    {
        for(unsigned int i_y=0;i_y<sampleBox.y;i_y++)
        {
            float data = ReadData(QString("elevator2_")+QString::number(i_x*sampleBox.y+i_y)).toFloat();
            sampleBox.data.push_back(data);
        }
    }

    return sampleBox;
}

float ConfigFileManagement::getEmptyBoxVolume_elevator1()
{
    return ReadData(QString("elevator1_")+QString("volume")).toFloat();
}

float ConfigFileManagement::getEmptyBoxVolume_elevator2()
{
    return ReadData(QString("elevator2_")+QString("volume")).toFloat();
}
