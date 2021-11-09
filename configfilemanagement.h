#ifndef CONFIGFILEMANAGEMENT_H
#define CONFIGFILEMANAGEMENT_H

#include <QObject>
#include<QSettings>
#include<QCoreApplication>
#include<QDebug>
#include"common.h"

class ConfigFileManagement : public QObject
{
    Q_OBJECT
public:
    explicit ConfigFileManagement(QObject *parent = nullptr);
    void Init();
    void GetConfigs();
private:
    void WriteData(QString key,QString value);
    QString ReadData(QString key);
    Array_TwoDimensional<float> getRealsenceBoxSample_elevator1();
    Array_TwoDimensional<float> getRealsenceBoxSample_elevator2();
    float getEmptyBoxVolume_elevator1();
    float getEmptyBoxVolume_elevator2();
    void setBoxSample_elevator1(Array_TwoDimensional<float> sample_box);
    void setEmptyBoxVolume_elevator1(float volume);
    void setBoxSample_elevator2(Array_TwoDimensional<float> sample_box);
    void setEmptyBoxVolume_elevator2(float volume);
public slots:
    void slot_jobmanager_askconfig(QString key);
    void slot_jobmanager_setconfig(QString key,QString value);

    void slot_jobmanager(Sig_JobManagement_ConfigManagement sig);
    void slot_realsence(Sig_RealsenceManagement_ConfigManagement sig);
signals:
    void sig_jobmanager_config(QString key,QString value);
    void sig_jobmanager_boxSample(Array_TwoDimensional<float> sample_box);
    void sig_jobmanager(Sig_ConfigManagement_JobManagement);
    void sig_realsence(Sig_ConfigManagement_RealsenceManagement );
private:
    QSettings* settingcfg_ = nullptr;
};

#endif // CONFIGFILEMANAGEMENT_H
