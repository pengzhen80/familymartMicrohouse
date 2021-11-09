#ifndef REALSENCEMANAGER_H
#define REALSENCEMANAGER_H

#include <QObject>
#include<QThread>
#include<librealsense2/rs.hpp>
#include<QMutex>
#include<QTimer>
#include<QTime>
#include"common.h"

struct MyPoint
{
    int x;
    int y;
    float z;
};

class RealsenceManager : public QObject
{
    Q_OBJECT
public:
    explicit RealsenceManager(QObject *parent = nullptr);
    void Init();
    void Test_realsense();
private:
    bool check_put(int width, int length, int height, Box_Space box);
    float take_emptybox_volume();
    float take_emptybox_volume_elevator1();
    float take_emptybox_volume_elevator2();
    float take_box_left_volume();
    float take_box_left_volume_elevator1();
    float take_box_left_volume_elevator2();
    float test_take_box_left_volume();
    float test_take_box_left_volume_beta();
    void  test_tmp_volume();
    Array_TwoDimensional<float> transData(int elevatorId);
    void decodeData(int elevatorId,Array_TwoDimensional<float> data);

    void  takeVolume_Forever();
    float get_volume_elevator1();
    float get_volume_elevator2();

    bool restart_pipeline();
public slots:
//    void slot_jobmanager_InitEmptyBox();
    void slot_jobmanager_takeBoxLeftVolume();
    //void slot_jobmanager_initSamBox(float sample_box[REALSENSE_TOP_PIXEL_WIDTH][REALSENSE_TOP_PIXEL_LENGTH]);
//    void slot_jobmanager_initSamBox(Array_TwoDimensional<float> boxSample);

    void slot_testtools_askData();
    //for test
    void slot_mainwindow_test();
    void slot_mainwindow_test_detectvolume();
    void slot_mainwindow_test_format(Sig_MainWindow_RealsenceManagement sig);
    //for jobmanager
    void slot_jobmanager(Sig_JobManagement_RealsenceManagement sig);
    void slot_config(Sig_ConfigManagement_RealsenceManagement sig);
signals:
    void sig_jobmanager_boxLeftVolume(float volume);
    //void sig_jobmanager_boxEmptyVolume(float volume,float sample_box[REALSENSE_TOP_PIXEL_WIDTH][REALSENSE_TOP_PIXEL_LENGTH]);
    void sig_jobmanager_boxEmptyVolume(float volume,Array_TwoDimensional<float> boxSample);
    //void sig_jobmanager_initSamBox(float sample_box[REALSENSE_TOP_PIXEL_WIDTH][REALSENSE_TOP_PIXEL_LENGTH]);
    void sig_test_dbmanager_data(QVector<int> count,QVector<float> volumes);

    void sig_jobmanager(Sig_RealsenceManagement_JobManagement);
    void sig_configmanager(Sig_RealsenceManagement_ConfigManagement sig);

    //for mainwindow
    void sig_mainwindow(Sig_RealsenceManagement_MainWindow);
private:
    float sample_box_elevator1_[REALSENSE_TOP_PIXEL_WIDTH][REALSENSE_TOP_PIXEL_LENGTH];
    float sample_box_elevator2_[REALSENSE_TOP_PIXEL_WIDTH][REALSENSE_TOP_PIXEL_LENGTH];
    rs2::pipeline p_;
    rs2::config config_;
    bool flag_ = false;
    float volume_left_ = 0;
    int count_volume_left_ = 0;

    QMutex mutex_p_flag;
    bool flag_Init_ = false;
    float volume_empty_ = 0;
    int count_emptyBox_ = 0;

    int count_testtools_ = 0;
    QVector<int> testtools_data_excel_count;
    QVector<float> testtools_data_excel_volumes;

    //for tmp volumes
    QVector<int> tmp_volumes_;

    QVector<float> pre_storeVolumes_elevator1_;
    QVector<float> pre_storeVolumes_elevator2_;

    //count realsense error,if error counts > 100, set realsence error
    bool flag_realsenceState_ = true;
    int count_waitToReStart_ = 1;
};

#endif // REALSENCEMANAGER_H
