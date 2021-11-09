#include "realsencemanager.h"
#include<QDebug>
#include<windows.h>

RealsenceManager::RealsenceManager(QObject *parent) : QObject(parent)
{
    //QThread t_detect;
}

void RealsenceManager::Test_realsense()
{
      rs2::pipeline p;

      // Configure and start the pipeline
      p.start();

      while (true)
      {
          // Block program until frames arrive
          rs2::frameset frames = p.wait_for_frames();

          // Try to get a frame of a depth image
          rs2::depth_frame depth = frames.get_depth_frame();

          // Get the depth frame's dimensions
          auto width = depth.get_width();
          auto height = depth.get_height();

          // Query the distance from the camera to the object in the center of the image
          float dist_to_center = depth.get_distance(width / 2, height / 2);

          // Print the distance
          qDebug() << "The camera is facing an object " << dist_to_center << " meters away \r";
      }
}

void RealsenceManager::Init()
{
    qDebug()<< "RealsenceManager::Init thread id is "<<QThread::currentThreadId();
//    config_.enable_device(REALSENSE_SERIALNUMBER);
//    p_.start(config_);
    try {
        p_.start();
    }
    catch (...) {
        qDebug()<<"cannot find realsence";
        flag_realsenceState_ = false;
        return;
    }
//    p_.start();
//    qDebug()<<"start success";
    qDebug()<<"RealsenceManager::Init : "<<p_.get_active_profile().get_device().get_info( RS2_CAMERA_INFO_SERIAL_NUMBER);
    // Block program until frames arrive
//    rs2::frameset frames = p_.wait_for_frames();

    // Try to get a frame of a depth image
//    rs2::depth_frame depth = frames.get_depth_frame();

    // Get the depth frame's dimensions
//    auto width = depth.get_width();
//    auto height = depth.get_height();

    // Query the distance from the camera to the object in the center of the image
//    float dist_to_center = depth.get_distance(width / 2, height / 2);

    // Print the distance
//    qDebug() << "The camera is facing an object " << dist_to_center << " meters away \r";

//    p_.stop();

//    p_.start();
//    qDebug()<<"RealsenceManager::Init : "<<p_.get_active_profile().get_device().get_info( RS2_CAMERA_INFO_SERIAL_NUMBER);
//    // Block program until frames arrive
//    rs2::frameset frames_test = p_.wait_for_frames();

//    // Try to get a frame of a depth image
//    rs2::depth_frame depth_test = frames_test.get_depth_frame();

//    // Get the depth frame's dimensions
//    auto width_test = depth_test.get_width();
//    auto height_test = depth_test.get_height();

//    // Query the distance from the camera to the object in the center of the image
//    float dist_to_center_test = depth.get_distance(width_test / 2, height_test / 2);

//    // Print the distance
//    qDebug() << "The camera is facing an object " << dist_to_center_test << " meters away \r";
////    p_.stop();
////    test_tmp_volume();

    takeVolume_Forever();
}

bool RealsenceManager::restart_pipeline()
{
//    qDebug()<<"restart realsence thread is "<<QThread::currentThreadId();
    p_.stop();
    QThread::sleep(count_waitToReStart_);
    try {
        p_.start();
    }
    catch (...) {
        qDebug()<<"cannot find realsence";
        return false;
    }

    rs2::frameset frames;
    if(p_.try_wait_for_frames(&frames))
    {
        flag_realsenceState_ = true;
        qDebug()<<"realsence recovery";
        count_waitToReStart_ = 1;
        return true;
    }
    else
    {
        qDebug()<<"realsence restart fail";
        if(count_waitToReStart_<10)
        {
            count_waitToReStart_ ++;
        }
    }
    return false;
}

//return empty box volume and set sample_box_
float RealsenceManager::take_emptybox_volume()
{
    qDebug()<<"take emptybox volume";
//    p_.start();
    qDebug()<<"RealsenceManager::Init : "<<p_.get_active_profile().get_device().get_info( RS2_CAMERA_INFO_SERIAL_NUMBER);
    // Block program until frames arrive
    rs2::frameset frames_test = p_.wait_for_frames();

    // Try to get a frame of a depth image
    rs2::depth_frame depth_test = frames_test.get_depth_frame();

    // Get the depth frame's dimensions
    auto width_test = depth_test.get_width();
    auto height_test = depth_test.get_height();

    // Query the distance from the camera to the object in the center of the image
    float dist_to_center_test = depth_test.get_distance(width_test / 2, height_test / 2);

    // Print the distance
    qDebug() << "The camera is facing an object " << dist_to_center_test << " meters away \r";

    int detect_count = 0;
    float volume_empty = 0;
    while(detect_count<10)
    {
        qDebug()<<"detect count "<<detect_count;
        // Block program until frames arrive
        rs2::frameset frames = p_.wait_for_frames();

        // Try to get a frame of a depth image
        rs2::depth_frame depth = frames.get_depth_frame();

        if(detect_count == 0)
        {
            qDebug()<<"first time take a image frame start";
            for (int i_x = 0; i_x < REALSENSE_TOP_PIXEL_WIDTH; i_x++)
            {
                for (int i_y = 0; i_y < REALSENSE_TOP_PIXEL_LENGTH; i_y++)
                {
                    MyPoint point_elevator1;
                    point_elevator1.x = REALSENSE_TOP_POINT_X_ELEVATOR1 + i_x;
                    point_elevator1.y = REALSENSE_TOP_POINT_Y_ELEVATOR1 + i_y;
                    point_elevator1.z = depth.get_distance(point_elevator1.x, point_elevator1.y);
                    sample_box_elevator1_[i_x][i_y] = point_elevator1.z;

                    MyPoint point_elevator2;
                    point_elevator2.x = REALSENSE_TOP_POINT_X_ELEVATOR2 + i_x;
                    point_elevator2.y = REALSENSE_TOP_POINT_Y_ELEVATOR2 + i_y;
                    point_elevator2.z = depth.get_distance(point_elevator2.x, point_elevator2.y);
                    sample_box_elevator2_[i_x][i_y] = point_elevator2.z;
                }
            }
            qDebug()<<"first time take a image frame sample";
        }
        else if(detect_count > 0 && detect_count < 5)
        {
            qDebug() << "taking empty volume sample: " << detect_count;
            //box_sample.clear();
            for (int i_x = 0; i_x < REALSENSE_TOP_PIXEL_WIDTH; i_x++)
            {
                for (int i_y = 0; i_y < REALSENSE_TOP_PIXEL_LENGTH; i_y++)
                {
                    MyPoint point_elevator1;
                    point_elevator1.x = REALSENSE_TOP_POINT_X_ELEVATOR1 + i_x;
                    point_elevator1.y = REALSENSE_TOP_POINT_Y_ELEVATOR1 + i_y;
                    point_elevator1.z = depth.get_distance(point_elevator1.x, point_elevator1.y);
                    if (point_elevator1.z < sample_box_elevator1_[i_x][i_y])
                    {
                        sample_box_elevator1_[i_x][i_y] = point_elevator1.z;
                    }

                    MyPoint point_elevator2;
                    point_elevator2.x = REALSENSE_TOP_POINT_X_ELEVATOR2 + i_x;
                    point_elevator2.y = REALSENSE_TOP_POINT_Y_ELEVATOR2 + i_y;
                    point_elevator2.z = depth.get_distance(point_elevator2.x, point_elevator2.y);
                    if (point_elevator2.z < sample_box_elevator2_[i_x][i_y])
                    {
                        sample_box_elevator2_[i_x][i_y] = point_elevator2.z;
                    }
                }
            }
        }
        else if(detect_count>=5 && detect_count<10)
        {
            //take empty box volume
            //qDebug()<<"start: "<<QTime::currentTime();
            float sum_height = 0;
            for (int i_x = 0; i_x < REALSENSE_TOP_PIXEL_WIDTH / DEVIDE_LENTH_PIXEL; i_x++)
            {
                //qDebug()<<"x:"<<i_x;
                for (int i_y = 0; i_y < REALSENSE_TOP_PIXEL_LENGTH / DEVIDE_LENTH_PIXEL; i_y++)
                {
                    //qDebug()<<"y:"<<i_y;
                    int tmp_x = REALSENSE_TOP_POINT_X_ELEVATOR1 + DEVIDE_LENTH_PIXEL / 2 + i_x * DEVIDE_LENTH_PIXEL;
                    int tmp_y = REALSENSE_TOP_POINT_Y_ELEVATOR1 + DEVIDE_LENTH_PIXEL / 2 + i_y * DEVIDE_LENTH_PIXEL;
                    float z = depth.get_distance(tmp_x, tmp_y);
                    //qDebug()<<"depth:"<<z;
                    if (z * 100 < REALSENSE_BOX_BOTTOM_DISTANCE)
                    {
                        if (z+0.02  < sample_box_elevator1_[tmp_x- REALSENSE_TOP_POINT_X_ELEVATOR1][tmp_y- REALSENSE_TOP_POINT_Y_ELEVATOR1])
                            //if (z < sample_box[tmp_x - TOP_POINT_X][tmp_y - TOP_POINT_Y])
                        {
                            float distance = z * 100;
                            float height = REALSENSE_BOX_BOTTOM_DISTANCE - distance;
                            //qDebug() << "height is " << height << " cm";
                            sum_height += height;
                        }
                    }

                }
            }
            //qDebug()<<"end time"<<QTime::currentTime();
            float area_buttom = REALSENSE_BOX_LENTH * REALSENSE_BOX_WIGHT;
            float average_height = sum_height / ((REALSENSE_TOP_PIXEL_WIDTH / DEVIDE_LENTH_PIXEL) * (REALSENSE_TOP_PIXEL_LENGTH / DEVIDE_LENTH_PIXEL));
            //qDebug() << "average_height is " << average_height;
            float volume = average_height * area_buttom;
            //qDebug() << "empty box volume is " << volume;
            //qDebug() << "volume tolerance is " << 2 * area_buttom;
            //                if(volume > volume_empty_)
            //                {
            //                    volume_empty_ = volume;
            //                }
            volume_empty += volume;
        }
        detect_count++;
    }
//    p_.stop();
    qDebug()<<"empty box volume is "<<volume_empty/5;
    return volume_empty/5;
}

float RealsenceManager::take_emptybox_volume_elevator1()
{
    qDebug()<<"take emptybox volume";
//    p_.start();
    qDebug()<<"RealsenceManager::Init : "<<p_.get_active_profile().get_device().get_info( RS2_CAMERA_INFO_SERIAL_NUMBER);
    // Block program until frames arrive
    rs2::frameset frames_test = p_.wait_for_frames();

    // Try to get a frame of a depth image
    rs2::depth_frame depth_test = frames_test.get_depth_frame();

    // Get the depth frame's dimensions
    auto width_test = depth_test.get_width();
    auto height_test = depth_test.get_height();

    // Query the distance from the camera to the object in the center of the image
    float dist_to_center_test = depth_test.get_distance(width_test / 2, height_test / 2);

    // Print the distance
    qDebug() << "The camera is facing an object " << dist_to_center_test << " meters away \r";

    int detect_count = 0;
    float volume_empty = 0;
    while(detect_count<10)
    {
        qDebug()<<"detect count "<<detect_count;
        // Block program until frames arrive
        rs2::frameset frames = p_.wait_for_frames();

        // Try to get a frame of a depth image
        rs2::depth_frame depth = frames.get_depth_frame();

        if(detect_count == 0)
        {
            qDebug()<<"first time take a image frame start";
            for (int i_x = 0; i_x < REALSENSE_TOP_PIXEL_WIDTH; i_x++)
            {
                for (int i_y = 0; i_y < REALSENSE_TOP_PIXEL_LENGTH; i_y++)
                {
                    MyPoint point_elevator1;
                    point_elevator1.x = REALSENSE_TOP_POINT_X_ELEVATOR1 + i_x;
                    point_elevator1.y = REALSENSE_TOP_POINT_Y_ELEVATOR1 + i_y;
                    point_elevator1.z = depth.get_distance(point_elevator1.x, point_elevator1.y);
                    sample_box_elevator1_[i_x][i_y] = point_elevator1.z;

//                    qDebug()<<i_x<<","<<i_y<<":"<<point_elevator1.z;
                }
            }
//            qDebug()<<"first time take a image frame sample";
        }
        else if(detect_count > 0 && detect_count < 5)
        {
            qDebug() << "taking empty volume sample: " << detect_count;
            //box_sample.clear();
            for (int i_x = 0; i_x < REALSENSE_TOP_PIXEL_WIDTH; i_x++)
            {
                for (int i_y = 0; i_y < REALSENSE_TOP_PIXEL_LENGTH; i_y++)
                {
                    MyPoint point_elevator1;
                    point_elevator1.x = REALSENSE_TOP_POINT_X_ELEVATOR1 + i_x;
                    point_elevator1.y = REALSENSE_TOP_POINT_Y_ELEVATOR1 + i_y;
                    point_elevator1.z = depth.get_distance(point_elevator1.x, point_elevator1.y);
                    if (point_elevator1.z < sample_box_elevator1_[i_x][i_y])
                    {
                        sample_box_elevator1_[i_x][i_y] = point_elevator1.z;
                    }
                }
            }
        }
        else if(detect_count>=5 && detect_count<10)
        {
            //take empty box volume
            //qDebug()<<"start: "<<QTime::currentTime();
            float sum_height = 0;
            for (int i_x = 0; i_x < REALSENSE_TOP_PIXEL_WIDTH / DEVIDE_LENTH_PIXEL; i_x++)
            {
                //qDebug()<<"x:"<<i_x;
                for (int i_y = 0; i_y < REALSENSE_TOP_PIXEL_LENGTH / DEVIDE_LENTH_PIXEL; i_y++)
                {
                    //qDebug()<<"y:"<<i_y;
                    int tmp_x = REALSENSE_TOP_POINT_X_ELEVATOR1 + DEVIDE_LENTH_PIXEL / 2 + i_x * DEVIDE_LENTH_PIXEL;
                    int tmp_y = REALSENSE_TOP_POINT_Y_ELEVATOR1 + DEVIDE_LENTH_PIXEL / 2 + i_y * DEVIDE_LENTH_PIXEL;
                    float z = depth.get_distance(tmp_x, tmp_y);
                    //qDebug()<<"depth:"<<z;
                    if (z * 100 < REALSENSE_BOX_BOTTOM_DISTANCE)
                    {
                        if (z+0.02  < sample_box_elevator1_[tmp_x- REALSENSE_TOP_POINT_X_ELEVATOR1][tmp_y- REALSENSE_TOP_POINT_Y_ELEVATOR1])
                            //if (z < sample_box[tmp_x - TOP_POINT_X][tmp_y - TOP_POINT_Y])
                        {
                            float distance = z * 100;
                            float height = REALSENSE_BOX_BOTTOM_DISTANCE - distance;
                            //qDebug() << "height is " << height << " cm";
                            sum_height += height;
                        }
                    }

                }
            }
            //qDebug()<<"end time"<<QTime::currentTime();
            float area_buttom = REALSENSE_BOX_LENTH * REALSENSE_BOX_WIGHT;
            float average_height = sum_height / ((REALSENSE_TOP_PIXEL_WIDTH / DEVIDE_LENTH_PIXEL) * (REALSENSE_TOP_PIXEL_LENGTH / DEVIDE_LENTH_PIXEL));
            //qDebug() << "average_height is " << average_height;
            float volume = average_height * area_buttom;
            //qDebug() << "empty box volume is " << volume;
            //qDebug() << "volume tolerance is " << 2 * area_buttom;
            //                if(volume > volume_empty_)
            //                {
            //                    volume_empty_ = volume;
            //                }
            volume_empty += volume;
        }
        detect_count++;
    }
//    p_.stop();
    qDebug()<<"empty box volume is "<<volume_empty/5;
    return volume_empty/5;
}
float RealsenceManager::take_emptybox_volume_elevator2()
{
    qDebug()<<"take emptybox volume";
//    p_.start();
    qDebug()<<"RealsenceManager::Init : "<<p_.get_active_profile().get_device().get_info( RS2_CAMERA_INFO_SERIAL_NUMBER);
    // Block program until frames arrive
    rs2::frameset frames_test = p_.wait_for_frames();

    // Try to get a frame of a depth image
    rs2::depth_frame depth_test = frames_test.get_depth_frame();

    // Get the depth frame's dimensions
    auto width_test = depth_test.get_width();
    auto height_test = depth_test.get_height();

    // Query the distance from the camera to the object in the center of the image
    float dist_to_center_test = depth_test.get_distance(width_test / 2, height_test / 2);

    // Print the distance
    qDebug() << "The camera is facing an object " << dist_to_center_test << " meters away \r";

    int detect_count = 0;
    float volume_empty = 0;
    while(detect_count<10)
    {
        qDebug()<<"detect count "<<detect_count;
        // Block program until frames arrive
        rs2::frameset frames = p_.wait_for_frames();

        // Try to get a frame of a depth image
        rs2::depth_frame depth = frames.get_depth_frame();

        if(detect_count == 0)
        {
            qDebug()<<"first time take a image frame start";
            for (int i_x = 0; i_x < REALSENSE_TOP_PIXEL_WIDTH; i_x++)
            {
                for (int i_y = 0; i_y < REALSENSE_TOP_PIXEL_LENGTH; i_y++)
                {
                    MyPoint point_elevator2;
                    point_elevator2.x = REALSENSE_TOP_POINT_X_ELEVATOR2 + i_x;
                    point_elevator2.y = REALSENSE_TOP_POINT_Y_ELEVATOR2 + i_y;
                    point_elevator2.z = depth.get_distance(point_elevator2.x, point_elevator2.y);
                    sample_box_elevator2_[i_x][i_y] = point_elevator2.z;
                }
            }
            qDebug()<<"first time take a image frame sample";
        }
        else if(detect_count > 0 && detect_count < 5)
        {
            qDebug() << "taking empty volume sample: " << detect_count;
            //box_sample.clear();
            for (int i_x = 0; i_x < REALSENSE_TOP_PIXEL_WIDTH; i_x++)
            {
                for (int i_y = 0; i_y < REALSENSE_TOP_PIXEL_LENGTH; i_y++)
                {
                    MyPoint point_elevator2;
                    point_elevator2.x = REALSENSE_TOP_POINT_X_ELEVATOR2 + i_x;
                    point_elevator2.y = REALSENSE_TOP_POINT_Y_ELEVATOR2 + i_y;
                    point_elevator2.z = depth.get_distance(point_elevator2.x, point_elevator2.y);
                    if (point_elevator2.z < sample_box_elevator2_[i_x][i_y])
                    {
                        sample_box_elevator2_[i_x][i_y] = point_elevator2.z;
                    }
                }
            }
        }
        else if(detect_count>=5 && detect_count<10)
        {
            //take empty box volume
            //qDebug()<<"start: "<<QTime::currentTime();
            float sum_height = 0;
            for (int i_x = 0; i_x < REALSENSE_TOP_PIXEL_WIDTH / DEVIDE_LENTH_PIXEL; i_x++)
            {
                //qDebug()<<"x:"<<i_x;
                for (int i_y = 0; i_y < REALSENSE_TOP_PIXEL_LENGTH / DEVIDE_LENTH_PIXEL; i_y++)
                {
                    //qDebug()<<"y:"<<i_y;
                    int tmp_x = REALSENSE_TOP_POINT_X_ELEVATOR2 + DEVIDE_LENTH_PIXEL / 2 + i_x * DEVIDE_LENTH_PIXEL;
                    int tmp_y = REALSENSE_TOP_POINT_Y_ELEVATOR2 + DEVIDE_LENTH_PIXEL / 2 + i_y * DEVIDE_LENTH_PIXEL;
                    float z = depth.get_distance(tmp_x, tmp_y);
                    //qDebug()<<"depth:"<<z;
                    if (z * 100 < REALSENSE_BOX_BOTTOM_DISTANCE)
                    {
                        if (z+0.02  < sample_box_elevator2_[tmp_x- REALSENSE_TOP_POINT_X_ELEVATOR2][tmp_y- REALSENSE_TOP_POINT_Y_ELEVATOR2])
                            //if (z < sample_box[tmp_x - TOP_POINT_X][tmp_y - TOP_POINT_Y])
                        {
                            float distance = z * 100;
                            float height = REALSENSE_BOX_BOTTOM_DISTANCE - distance;
                            //qDebug() << "height is " << height << " cm";
                            sum_height += height;
                        }
                    }

                }
            }
            //qDebug()<<"end time"<<QTime::currentTime();
            float area_buttom = REALSENSE_BOX_LENTH * REALSENSE_BOX_WIGHT;
            float average_height = sum_height / ((REALSENSE_TOP_PIXEL_WIDTH / DEVIDE_LENTH_PIXEL) * (REALSENSE_TOP_PIXEL_LENGTH / DEVIDE_LENTH_PIXEL));
            //qDebug() << "average_height is " << average_height;
            float volume = average_height * area_buttom;
            //qDebug() << "empty box volume is " << volume;
            //qDebug() << "volume tolerance is " << 2 * area_buttom;
            //                if(volume > volume_empty_)
            //                {
            //                    volume_empty_ = volume;
            //                }
            volume_empty += volume;
        }
        detect_count++;
    }
//    p_.stop();
    qDebug()<<"empty box volume is "<<volume_empty/5;
    return volume_empty/5;
}
float RealsenceManager::take_box_left_volume()
{
//    p_.start();
    int detect_count = 10;
    float volume_box_left = 0;
    rs2::frameset frames;
    if(p_.try_wait_for_frames(&frames,300))
    {
        rs2::depth_frame depth = frames.get_depth_frame();

        //qDebug() << "taking volume: " << count;
        while(detect_count)
        {
            float sum_height = 0;
            for (int i_x = 0; i_x < REALSENSE_TOP_PIXEL_WIDTH / DEVIDE_LENTH_PIXEL; i_x++)
            {
                for (int i_y = 0; i_y < REALSENSE_TOP_PIXEL_LENGTH / DEVIDE_LENTH_PIXEL; i_y++)
                {
                    int tmp_x = REALSENSE_TOP_POINT_X_ELEVATOR1 + DEVIDE_LENTH_PIXEL / 2 + i_x * DEVIDE_LENTH_PIXEL;
                    int tmp_y = REALSENSE_TOP_POINT_Y_ELEVATOR1 + DEVIDE_LENTH_PIXEL / 2 + i_y * DEVIDE_LENTH_PIXEL;
                    float z = depth.get_distance(tmp_x, tmp_y);
                    if (z * 100 < REALSENSE_BOX_BOTTOM_DISTANCE)
                    {
                        if (z+0.02  < sample_box_elevator1_[tmp_x- REALSENSE_TOP_POINT_X_ELEVATOR1][tmp_y- REALSENSE_TOP_POINT_Y_ELEVATOR1])
                            //if (z < sample_box[tmp_x - TOP_POINT_X][tmp_y - TOP_POINT_Y])
                        {
                            float distance = z * 100;
                            float height = REALSENSE_BOX_BOTTOM_DISTANCE - distance;
                            //qDebug() << "height is " << height << " cm\n";
                            sum_height += height;

                            //                        for (int i_height = 31 - (int)height; i_height < 31; i_height++)
                            //                        {
                            //                            box_space.points[i_x][tmp_y][i_height].occupyed = true;
                            //                        }
                        }
                    }

                }
            }
            float area_buttom = REALSENSE_BOX_LENTH * REALSENSE_BOX_WIGHT;
            float average_height = sum_height / ((REALSENSE_TOP_PIXEL_WIDTH / DEVIDE_LENTH_PIXEL) * (REALSENSE_TOP_PIXEL_LENGTH / DEVIDE_LENTH_PIXEL));
            //qDebug() << "average_height is " << average_height;
            float volume = average_height * area_buttom;
            volume_box_left += volume;
//            qDebug() << "volume each time is " << volume;
            detect_count--;
        }
    //    p_.stop();
        volume_box_left = volume_box_left/detect_count;
//        qDebug() << "volume is " << volume_box_left;
        return volume_box_left;
    }
    qDebug()<<"realsense error";
    return volume_box_left;
}

// update : 10-13 if realsence is not linking, then close this multi-thread func
void  RealsenceManager::takeVolume_Forever()
{
    //take volume and store into pre_storeVolumes_elevator1_ pre_storeVolumes_elevator2_
//    int detect_count = 10;
//    float volume_box_left = 0;
    qDebug()<<"start takevolume forever";
    rs2::frameset frames;
    while(true)
    {
//        if(!flag_realsenceState_)
//        {
//            continue;;
//        }
        if(p_.try_wait_for_frames(&frames))
        {
//            qDebug()<<"get a frame";
            flag_realsenceState_ = true;
            rs2::depth_frame depth = frames.get_depth_frame();

//            qDebug()<<"get a depth";
            float sum_height_elevator1 = 0;
//            qDebug()<<REALSENSE_TOP_PIXEL_WIDTH;
            for (int i_x = 0; i_x < REALSENSE_TOP_PIXEL_WIDTH / DEVIDE_LENTH_PIXEL; i_x++)
            {
                for (int i_y = 0; i_y < REALSENSE_TOP_PIXEL_LENGTH / DEVIDE_LENTH_PIXEL; i_y++)
                {
                    int tmp_x = REALSENSE_TOP_POINT_X_ELEVATOR1 + DEVIDE_LENTH_PIXEL / 2 + i_x * DEVIDE_LENTH_PIXEL;
                    int tmp_y = REALSENSE_TOP_POINT_Y_ELEVATOR1 + DEVIDE_LENTH_PIXEL / 2 + i_y * DEVIDE_LENTH_PIXEL;
//                    qDebug()<<"get a distance";
                    float z = depth.get_distance(tmp_x, tmp_y);
//                    qDebug() << "depth is " << z;
                    if (z * 100 < REALSENSE_BOX_BOTTOM_DISTANCE)
                    {
//                                                qDebug()<<sample_box_elevator1_[tmp_x- REALSENSE_TOP_POINT_X_ELEVATOR1][tmp_y- REALSENSE_TOP_POINT_Y_ELEVATOR1];
                        if (z+0.02  < sample_box_elevator1_[tmp_x- REALSENSE_TOP_POINT_X_ELEVATOR1][tmp_y- REALSENSE_TOP_POINT_Y_ELEVATOR1])
                            //if (z < sample_box[tmp_x - TOP_POINT_X][tmp_y - TOP_POINT_Y])
                        {
                            float distance = z * 100;
                            float height = REALSENSE_BOX_BOTTOM_DISTANCE - distance;
                            if(height<0)
                            {
                                height = 0;
                            }
//                                                        qDebug() << "height is " << height << " cm\n";
                            sum_height_elevator1 += height;
                        }
                    }

                }
            }
            float area_buttom = REALSENSE_BOX_LENTH * REALSENSE_BOX_WIGHT;
            float average_height_elevator1 = sum_height_elevator1 / ((REALSENSE_TOP_PIXEL_WIDTH / DEVIDE_LENTH_PIXEL) * (REALSENSE_TOP_PIXEL_LENGTH / DEVIDE_LENTH_PIXEL));
//                        qDebug() << "average_height is " << average_height;
            float volume_elevator1 = average_height_elevator1 * area_buttom;
//            qDebug()<<"elevator1 volume is "<<volume_elevator1;
            if(pre_storeVolumes_elevator1_.size()<600)
            {
                pre_storeVolumes_elevator1_.push_back(volume_elevator1);
            }
            else
            {
                pre_storeVolumes_elevator1_.pop_front();
                pre_storeVolumes_elevator1_.push_back(volume_elevator1);
            }

            float sum_height_elevator2 = 0;
            for (int i_x = 0; i_x < REALSENSE_TOP_PIXEL_WIDTH / DEVIDE_LENTH_PIXEL; i_x++)
            {
                for (int i_y = 0; i_y < REALSENSE_TOP_PIXEL_LENGTH / DEVIDE_LENTH_PIXEL; i_y++)
                {
                    int tmp_x = REALSENSE_TOP_POINT_X_ELEVATOR2 + DEVIDE_LENTH_PIXEL / 2 + i_x * DEVIDE_LENTH_PIXEL;
                    int tmp_y = REALSENSE_TOP_POINT_Y_ELEVATOR2 + DEVIDE_LENTH_PIXEL / 2 + i_y * DEVIDE_LENTH_PIXEL;
                    float z = depth.get_distance(tmp_x, tmp_y);
                    if (z * 100 < REALSENSE_BOX_BOTTOM_DISTANCE)
                    {
                        if (z+0.02  < sample_box_elevator2_[tmp_x- REALSENSE_TOP_POINT_X_ELEVATOR2][tmp_y- REALSENSE_TOP_POINT_Y_ELEVATOR2])
                            //if (z < sample_box[tmp_x - TOP_POINT_X][tmp_y - TOP_POINT_Y])
                        {
                            float distance = z * 100;
                            float height = REALSENSE_BOX_BOTTOM_DISTANCE - distance;
//                          qDebug() << "height is " << height << " cm\n";
                            sum_height_elevator2 += height;
                        }
                    }

                }
            }
            float average_height_elevator2 = sum_height_elevator2 / ((REALSENSE_TOP_PIXEL_WIDTH / DEVIDE_LENTH_PIXEL) * (REALSENSE_TOP_PIXEL_LENGTH / DEVIDE_LENTH_PIXEL));
//            qDebug() << "average_height is " << average_height;
            float volume_elevator2 = average_height_elevator2 * area_buttom;
//            qDebug()<<"elevator2 volume is "<<volume_elevator2;
            if(pre_storeVolumes_elevator2_.size()<600)
            {
                pre_storeVolumes_elevator2_.push_back(volume_elevator2);
            }
            else
            {
                pre_storeVolumes_elevator2_.pop_front();
                pre_storeVolumes_elevator2_.push_back(volume_elevator2);
            }
        }
        else
        {
            qDebug()<<"realsense error";
//            QThread::sleep(3);
            flag_realsenceState_ = restart_pipeline();
            if(!flag_realsenceState_)
            {
                qDebug()<<"realsence is error, change to queue mode";
                Sig_RealsenceManagement_JobManagement sigToJob;
                sigToJob.type = SIG_REALSENCEMANAGEMENT_JOBMANAGEMENT_MODE_RANK;
                emit sig_jobmanager(sigToJob);
            }
            else
            {
                qDebug()<<"realsence is recovery, change to normal mode";
                Sig_RealsenceManagement_JobManagement sigToJob;
                sigToJob.type = SIG_REALSENCEMANAGEMENT_JOBMANAGEMENT_MODE_NORMAL;
                emit sig_jobmanager(sigToJob);
            }
        }
    }

}



float RealsenceManager::take_box_left_volume_elevator1()
{
    int detect_count = 10;
    float volume_box_left = 0;
    rs2::frameset frames;
    if(p_.try_wait_for_frames(&frames,300))
    {
        rs2::depth_frame depth = frames.get_depth_frame();

        //qDebug() << "taking volume: " << count;
        while(detect_count)
        {
            float sum_height = 0;
            for (int i_x = 0; i_x < REALSENSE_TOP_PIXEL_WIDTH / DEVIDE_LENTH_PIXEL; i_x++)
            {
                for (int i_y = 0; i_y < REALSENSE_TOP_PIXEL_LENGTH / DEVIDE_LENTH_PIXEL; i_y++)
                {
                    int tmp_x = REALSENSE_TOP_POINT_X_ELEVATOR1 + DEVIDE_LENTH_PIXEL / 2 + i_x * DEVIDE_LENTH_PIXEL;
                    int tmp_y = REALSENSE_TOP_POINT_Y_ELEVATOR1 + DEVIDE_LENTH_PIXEL / 2 + i_y * DEVIDE_LENTH_PIXEL;
                    float z = depth.get_distance(tmp_x, tmp_y);
                    if (z * 100 < REALSENSE_BOX_BOTTOM_DISTANCE)
                    {
                        if (z+0.02  < sample_box_elevator1_[tmp_x- REALSENSE_TOP_POINT_X_ELEVATOR1][tmp_y- REALSENSE_TOP_POINT_Y_ELEVATOR1])
                            //if (z < sample_box[tmp_x - TOP_POINT_X][tmp_y - TOP_POINT_Y])
                        {
                            float distance = z * 100;
                            float height = REALSENSE_BOX_BOTTOM_DISTANCE - distance;
                            //qDebug() << "height is " << height << " cm\n";
                            sum_height += height;

                            //                        for (int i_height = 31 - (int)height; i_height < 31; i_height++)
                            //                        {
                            //                            box_space.points[i_x][tmp_y][i_height].occupyed = true;
                            //                        }
                        }
                    }

                }
            }
            float area_buttom = REALSENSE_BOX_LENTH * REALSENSE_BOX_WIGHT;
            float average_height = sum_height / ((REALSENSE_TOP_PIXEL_WIDTH / DEVIDE_LENTH_PIXEL) * (REALSENSE_TOP_PIXEL_LENGTH / DEVIDE_LENTH_PIXEL));
            //qDebug() << "average_height is " << average_height;
            float volume = average_height * area_buttom;
            volume_box_left += volume;
//            qDebug() << "volume each time is " << volume;
            detect_count--;
        }
    //    p_.stop();
        volume_box_left = volume_box_left/detect_count;
        qDebug() << "volume is " << volume_box_left;
        return volume_box_left;
    }
    qDebug()<<"realsense error";
//    return volume_box_left;
    return 46035;
}
float RealsenceManager::take_box_left_volume_elevator2()
{
    int detect_count = 10;
    float volume_box_left = 0;
    rs2::frameset frames;
    if(p_.try_wait_for_frames(&frames,300))
    {
        rs2::depth_frame depth = frames.get_depth_frame();

        //qDebug() << "taking volume: " << count;
        while(detect_count)
        {
            float sum_height = 0;
            for (int i_x = 0; i_x < REALSENSE_TOP_PIXEL_WIDTH / DEVIDE_LENTH_PIXEL; i_x++)
            {
                for (int i_y = 0; i_y < REALSENSE_TOP_PIXEL_LENGTH / DEVIDE_LENTH_PIXEL; i_y++)
                {
                    int tmp_x = REALSENSE_TOP_POINT_X_ELEVATOR2 + DEVIDE_LENTH_PIXEL / 2 + i_x * DEVIDE_LENTH_PIXEL;
                    int tmp_y = REALSENSE_TOP_POINT_Y_ELEVATOR2 + DEVIDE_LENTH_PIXEL / 2 + i_y * DEVIDE_LENTH_PIXEL;
                    float z = depth.get_distance(tmp_x, tmp_y);
                    if (z * 100 < REALSENSE_BOX_BOTTOM_DISTANCE)
                    {
                        if (z+0.02  < sample_box_elevator2_[tmp_x- REALSENSE_TOP_POINT_X_ELEVATOR2][tmp_y- REALSENSE_TOP_POINT_Y_ELEVATOR2])
                            //if (z < sample_box[tmp_x - TOP_POINT_X][tmp_y - TOP_POINT_Y])
                        {
                            float distance = z * 100;
                            float height = REALSENSE_BOX_BOTTOM_DISTANCE - distance;
                            //qDebug() << "height is " << height << " cm\n";
                            sum_height += height;

                            //                        for (int i_height = 31 - (int)height; i_height < 31; i_height++)
                            //                        {
                            //                            box_space.points[i_x][tmp_y][i_height].occupyed = true;
                            //                        }
                        }
                    }

                }
            }
            float area_buttom = REALSENSE_BOX_LENTH * REALSENSE_BOX_WIGHT;
            float average_height = sum_height / ((REALSENSE_TOP_PIXEL_WIDTH / DEVIDE_LENTH_PIXEL) * (REALSENSE_TOP_PIXEL_LENGTH / DEVIDE_LENTH_PIXEL));
            //qDebug() << "average_height is " << average_height;
            float volume = average_height * area_buttom;
            volume_box_left += volume;
//            qDebug() << "volume each time is " << volume;
            detect_count--;
        }
    //    p_.stop();
        volume_box_left = volume_box_left/detect_count;
//        qDebug() << "volume is " << volume_box_left;
        return volume_box_left;
    }
    qDebug()<<"realsense error";
//    return volume_box_left;
    //for test
    return 46035;
}


float RealsenceManager::test_take_box_left_volume_beta()
{
    //full box volume is REALSENSE_BOX_FULLVOLUME, 60是一秒拍的frame
    QVector<int> counts;
    QVector<float> volumes;
    int count = 0;
    for(int i=tmp_volumes_.size()-900;i<tmp_volumes_.size();i++)
    {
        count ++;
        counts.push_back(count);
        volumes.push_back(tmp_volumes_[i]);
    }
    emit sig_test_dbmanager_data(counts,volumes);

    for(int i = tmp_volumes_.size()-60;i>=0;i--)
    {
        qDebug()<<tmp_volumes_[i];
        if(tmp_volumes_[i]<REALSENSE_BOX_FULLVOLUME)
        {
            float tmp_volumes_left = tmp_volumes_[i-5]-tmp_volumes_[i];
            tmp_volumes_left = tmp_volumes_left>0?tmp_volumes_left:-tmp_volumes_left;
            float tmp_volumes_right = tmp_volumes_[i+5]-tmp_volumes_[i];
            tmp_volumes_right = tmp_volumes_right>0?tmp_volumes_right:-tmp_volumes_right;
            if(tmp_volumes_left<1000&&tmp_volumes_right<1000)
            {
                qDebug()<<tmp_volumes_left;
                qDebug()<<tmp_volumes_right;
                return tmp_volumes_[i];
            }
        }
    }
    qDebug()<<"take volume error";
    return 0;
}

float RealsenceManager::get_volume_elevator1()
{
    //multi thread : store to a tmp
    QVector<float> tmp_volumes = pre_storeVolumes_elevator1_;
    qDebug()<<"volume size is "<<tmp_volumes.size();
//    for(int i=0;i<tmp_volumes.size();i++)
//    {
//        qDebug()<<tmp_volumes[i];
//    }
    for(int i = tmp_volumes.size()-60;i>=5;i--)
    {
//        qDebug()<<tmp_volumes[i];
        if(tmp_volumes[i]<REALSENSE_BOX_FULLVOLUME)
        {
            float tmp_volumes_left = tmp_volumes[i-5]-tmp_volumes[i];
            tmp_volumes_left = tmp_volumes_left>0?tmp_volumes_left:-tmp_volumes_left;
            float tmp_volumes_right = tmp_volumes[i+5]-tmp_volumes[i];
            tmp_volumes_right = tmp_volumes_right>0?tmp_volumes_right:-tmp_volumes_right;
            if(tmp_volumes_left<1000&&tmp_volumes_right<1000)
            {
//                qDebug()<<tmp_volumes_left;
//                qDebug()<<tmp_volumes_right;
                return tmp_volumes[i];
            }
        }
    }
}
float RealsenceManager::get_volume_elevator2()
{
    QVector<float> tmp_volumes = pre_storeVolumes_elevator2_;

    qDebug()<<tmp_volumes_.size();
    for(int i = tmp_volumes.size()-60;i>=5;i--)
    {
        if(tmp_volumes[i]<REALSENSE_BOX_FULLVOLUME)
        {
            float tmp_volumes_left = tmp_volumes[i-5]-tmp_volumes[i];
            tmp_volumes_left = tmp_volumes_left>0?tmp_volumes_left:-tmp_volumes_left;
            float tmp_volumes_right = tmp_volumes[i+5]-tmp_volumes[i];
            tmp_volumes_right = tmp_volumes_right>0?tmp_volumes_right:-tmp_volumes_right;
            if(tmp_volumes_left<1000&&tmp_volumes_right<1000)
            {
//                qDebug()<<tmp_volumes_left;
//                qDebug()<<tmp_volumes_right;
                return tmp_volumes[i];
            }
        }
    }
}

float RealsenceManager::test_take_box_left_volume()
{
    float volume_box_left = 0;
    rs2::frameset frames;
    if(p_.try_wait_for_frames(&frames,300))
    {
        rs2::depth_frame depth = frames.get_depth_frame();

        //qDebug() << "taking volume: " << count;
        float sum_height = 0;
        for (int i_x = 0; i_x < REALSENSE_TOP_PIXEL_WIDTH / DEVIDE_LENTH_PIXEL; i_x++)
        {
            for (int i_y = 0; i_y < REALSENSE_TOP_PIXEL_LENGTH / DEVIDE_LENTH_PIXEL; i_y++)
            {
                int tmp_x = REALSENSE_TOP_POINT_X_ELEVATOR1 + DEVIDE_LENTH_PIXEL / 2 + i_x * DEVIDE_LENTH_PIXEL;
                int tmp_y = REALSENSE_TOP_POINT_Y_ELEVATOR1 + DEVIDE_LENTH_PIXEL / 2 + i_y * DEVIDE_LENTH_PIXEL;
                float z = depth.get_distance(tmp_x, tmp_y);
                if (z * 100 < REALSENSE_BOX_BOTTOM_DISTANCE)
                {
                    if (z+0.02  < sample_box_elevator1_[tmp_x- REALSENSE_TOP_POINT_X_ELEVATOR1][tmp_y- REALSENSE_TOP_POINT_Y_ELEVATOR1])
                        //if (z < sample_box[tmp_x - TOP_POINT_X][tmp_y - TOP_POINT_Y])
                    {
                        float distance = z * 100;
                        float height = REALSENSE_BOX_BOTTOM_DISTANCE - distance;
                        //qDebug() << "height is " << height << " cm\n";
                        sum_height += height;

                        //                        for (int i_height = 31 - (int)height; i_height < 31; i_height++)
                        //                        {
                        //                            box_space.points[i_x][tmp_y][i_height].occupyed = true;
                        //                        }
                    }
                }

            }
        }
        float area_buttom = REALSENSE_BOX_LENTH * REALSENSE_BOX_WIGHT;
        float average_height = sum_height / ((REALSENSE_TOP_PIXEL_WIDTH / DEVIDE_LENTH_PIXEL) * (REALSENSE_TOP_PIXEL_LENGTH / DEVIDE_LENTH_PIXEL));
        //qDebug() << "average_height is " << average_height;
        float volume = average_height * area_buttom;
        volume_box_left += volume;
//        qDebug() << "volume each time is " << volume;

        //    p_.stop();
//        qDebug() << "volume is " << volume_box_left;
        return volume_box_left;
    }
    qDebug()<<"realsense error";
    return volume_box_left;
}

//void RealsenceManager::test_tmp_volume()
//{
//    while(true)
//    {
//        float volume = test_take_box_left_volume();
//        if(tmp_volumes_.size()<1000)
//        {
//            tmp_volumes_.push_back(volume);
//        }
//        else
//        {
//            tmp_volumes_.pop_front();
//            tmp_volumes_.push_back(volume);
//        }
//    }
//}

bool RealsenceManager::check_put(int width, int length, int height, Box_Space box)
{
    if (width >= 33 || length >= 45 || height >= 31)
    {
        return false;
    }
    int space_size = width * length * height;
    for (int i_x = 0; i_x < 33 - width; i_x++)
    {
        for (int i_y = 0; i_y < 45 - length; i_y++)
        {
            int space_sum = 0;
            for (int i_sum_x = i_x; i_sum_x < i_x + width; i_sum_x++)
            {
                for (int i_sum_y = i_y; i_sum_y < i_y + length; i_sum_y++)
                {
                    for (int i_sum_z = 0; i_sum_z < height; i_sum_z++)
                    {
                        space_sum += box.points[i_sum_x][i_sum_y][i_sum_z].occupyed;
                    }
                }
            }
            if (space_sum == space_size)
            {
                return true;
            }
        }
    }
    return false;
}


//void RealsenceManager::slot_jobmanager_InitEmptyBox()
//{
//    float volume_empty = take_emptybox_volume();

//    Array_TwoDimensional<float> tmp_sample_box;
//    tmp_sample_box.x = REALSENSE_TOP_PIXEL_WIDTH;
//    tmp_sample_box.y = REALSENSE_TOP_PIXEL_LENGTH;
//    for(unsigned int i_x=0;i_x<tmp_sample_box.x;i_x++)
//    {
//        for(unsigned int i_y =0;i_y<tmp_sample_box.y;i_y++)
//        {
//           tmp_sample_box.data.push_back(sample_box_[i_x][i_y]);
//        }
//    }
////    qDebug()<<"RealsenceManager::slot_jobmanager_InitEmptyBox :emit sig";
//    emit sig_jobmanager_boxEmptyVolume(volume_empty,tmp_sample_box);
//}

void RealsenceManager::slot_jobmanager_takeBoxLeftVolume()
{
//    float volume_left = take_box_left_volume();
    float volume_left = test_take_box_left_volume_beta();
    emit sig_jobmanager_boxLeftVolume(volume_left);
}

//void RealsenceManager::slot_jobmanager_initSamBox(Array_TwoDimensional<float> boxSample)
//{
//    for(int i_x=0;i_x<REALSENSE_TOP_PIXEL_WIDTH;i_x++)
//    {
//        for(int i_y =0;i_y<REALSENSE_TOP_PIXEL_LENGTH;i_y++)
//        {
//            sample_box_[i_x][i_y] = boxSample.data[i_x*boxSample.y+i_y];
//        }
//    }
//}

void RealsenceManager::slot_testtools_askData()
{
//    emit sig_testtools_data(testtools_data_excel_count,testtools_data_excel_volumes);
}

//void RealsenceManager::slot_jobmanager_initSamBox(Array_TwoDimensional<float> boxSample)
//{
//    for(unsigned int i_x=0;i_x<boxSample.x;i_x++)
//    {
//        for(unsigned int i_y =0;i_y<boxSample.y;i_y++)
//        {
//            sample_box_[i_x][i_y] = boxSample.data.value(i_x*boxSample.y+i_y);
//        }
//    }
//}

void RealsenceManager::slot_mainwindow_test()
{
    int count = 1;
    QVector<int> counts;
    QVector<float> volumes;
    while(count<=1000)
    {
        float volume = test_take_box_left_volume();
        counts.push_back(count);
        volumes.push_back(volume);
        count++;
    }
    emit sig_test_dbmanager_data(counts,volumes);
}

void RealsenceManager::slot_mainwindow_test_detectvolume()
{
//    qDebug()<<"volume is"<<();
}

void RealsenceManager::slot_jobmanager(Sig_JobManagement_RealsenceManagement sig)
{
//    if(!flag_realsenceState_)
//    {
//        qDebug()<<"realsence is error, change to queue mode";
//        Sig_RealsenceManagement_JobManagement sigToJob;
//        sigToJob.type = SIG_REALSENCEMANAGEMENT_JOBMANAGEMENT_RANKMODE;
//        emit sig_jobmanager(sigToJob);
////        return;
//    }

    switch (sig.type) {
    case SIG_JOBMANAGEMENT_REALSENCEMANAGEMENT_TAKE_EMPTYBOXVOLUME_ELEVATOR1:
    {
//        Sig_RealsenceManagement_JobManagement sigToJob;
//        sigToJob.type = SIG_REALSENCEMANAGEMENT_JOBMANAGEMENT_TAKE_BOXEMPTYVOLUME;
//        sigToJob.value_takeBoxEmptyVolume.volume = take_emptybox_volume_elevator1();

//        emit sig_jobmanager(sigToJob);

        Sig_RealsenceManagement_ConfigManagement sigToConfig;
        sigToConfig.type = SIG_REALSENCEMANAGEMENT_CONFIGMANAGEMENT_BOXEMPTYVOLUME_ELEVATOR1;
        sigToConfig.value_BoxEmptyVolume_elevator1.volume = take_emptybox_volume_elevator1();
        sigToConfig.value_BoxEmptyVolume_elevator1.realsence_box_sample = transData(1);

        emit sig_configmanager(sigToConfig);
        break;
    }
    case SIG_JOBMANAGEMENT_REALSENCEMANAGEMENT_TAKE_EMPTYBOXVOLUME_ELEVATOR2:
    {
//        Sig_RealsenceManagement_JobManagement sigToJob;
//        sigToJob.type = SIG_REALSENCEMANAGEMENT_JOBMANAGEMENT_TAKE_BOXEMPTYVOLUME;
//        sigToJob.value_takeBoxEmptyVolume.volume = take_emptybox_volume_elevator2();
//        emit sig_jobmanager(sigToJob);

        Sig_RealsenceManagement_ConfigManagement sigToConfig;
        sigToConfig.type = SIG_REALSENCEMANAGEMENT_CONFIGMANAGEMENT_BOXEMPTYVOLUME_ELEVATOR2;
        sigToConfig.value_BoxEmptyVolume_elevator2.volume = take_emptybox_volume_elevator2();
        sigToConfig.value_BoxEmptyVolume_elevator2.realsence_box_sample = transData(2);

        emit sig_configmanager(sigToConfig);

        break;
    }
    case SIG_JOBMANAGEMENT_REALSENCEMANAGEMENT_TAKE_BOXLEFTVOLUME:
    {
        float volume = take_box_left_volume();
        Sig_RealsenceManagement_JobManagement sigToJob;
        sigToJob.type = SIG_REALSENCEMANAGEMENT_JOBMANAGEMENT_TAKE_BOXLEFTVOLUME;
        sigToJob.value_takeBoxLeftVolume.volume = volume;

        emit sig_jobmanager(sigToJob);
        break;
    }
    case SIG_JOBMANAGEMENT_REALSENCEMANAGEMENT_TAKE_BOXLEFTVOLUME_ELEVATOR1:
    {
        float volume;
        if(!flag_realsenceState_)
        {
            volume = -1;
        }
        else
        {
            volume = get_volume_elevator1();
        }
        qDebug()<<"elevator 1 volume :" << volume;

        Sig_RealsenceManagement_JobManagement sigToJob;
        sigToJob.type = SIG_REALSENCEMANAGEMENT_JOBMANAGEMENT_TAKE_BOXLEFTVOLUME;
        sigToJob.value_takeBoxLeftVolume.volume = volume;

        emit sig_jobmanager(sigToJob);
        break;
    }
    case SIG_JOBMANAGEMENT_REALSENCEMANAGEMENT_TAKE_BOXLEFTVOLUME_ELEVATOR2:
    {
        float volume;
        if(!flag_realsenceState_)
        {
            volume = -1;
        }
        else
        {
            volume = get_volume_elevator2();
        }
        qDebug()<<"elevator 2 volume :" << volume;

        Sig_RealsenceManagement_JobManagement sigToJob;
        sigToJob.type = SIG_REALSENCEMANAGEMENT_JOBMANAGEMENT_TAKE_BOXLEFTVOLUME;
        sigToJob.value_takeBoxLeftVolume.volume = volume;

        emit sig_jobmanager(sigToJob);
        break;
    }
    default:
    {
        qDebug()<<"sig from jobmanager : unknow sig";
    }
    }
}

void RealsenceManager::slot_config(Sig_ConfigManagement_RealsenceManagement sig)
{
    switch (sig.type) {
    case SIG_CONFIGMANAGEMENT_REALSENCEMANAGEMENT_ALLCONFIG:
    {
        decodeData(1, sig.value_allConfig.realsence_box_sample_elevator1);
        decodeData(2, sig.value_allConfig.realsence_box_sample_elevator2);
        break;
    }
    default:
    {
        qDebug()<<"RealsenceManager::slot_config unknown signal";
        break;
    }
    }
}

 void RealsenceManager::slot_mainwindow_test_format(Sig_MainWindow_RealsenceManagement sig)
 {
     switch (sig.type) {
     case SIG_MAINWINDOW_REALSENCEMANAGEMENT_TAKE_BOXLEFTVOLUME_ELEVATOR1:
     {
         qDebug()<<"test elevator1 volume:";
         qDebug()<<get_volume_elevator1();

         break;
     }
     case SIG_MAINWINDOW_REALSENCEMANAGEMENT_TAKE_BOXLEFTVOLUME_ELEVATOR2:
     {
         qDebug()<<"test elevator2 volume:";
         float volume = get_volume_elevator2();
         qDebug()<<volume;

         Sig_RealsenceManagement_MainWindow sigToWindow;
         sigToWindow.type = SIG_REALSENCEMANAGEMENT_MAINWINDOW_BOXEMPTYVOLUME_ELEVATOR2;
         sigToWindow.value_BoxEmptyVolume_elevator2.volume = volume;

         emit sig_mainwindow(sigToWindow);
         break;
     }
     default:
     {
         qDebug()<<"RealsenceManager::slot_mainwindow_test_format unknown signal";
         break;
     }
     }
 }

 Array_TwoDimensional<float> RealsenceManager::transData(int elevatorId)
 {
     Array_TwoDimensional<float> result;
     result.x = REALSENSE_TOP_PIXEL_WIDTH;
     result.y = REALSENSE_TOP_PIXEL_LENGTH;
     if(1 == elevatorId)
     {
         for(int i_x=0;i_x<REALSENSE_TOP_PIXEL_WIDTH;i_x++)
         {
             for(int i_y=0;i_y<REALSENSE_TOP_PIXEL_LENGTH;i_y++)
             {
                 result.data.push_back(sample_box_elevator1_[i_x][i_y]);
             }
         }
     }
     else if (2 == elevatorId)
     {
         for(int i_x=0;i_x<REALSENSE_TOP_PIXEL_WIDTH;i_x++)
         {
             for(int i_y=0;i_y<REALSENSE_TOP_PIXEL_LENGTH;i_y++)
             {
                 result.data.push_back(sample_box_elevator2_[i_x][i_y]);
             }
         }
     }
     else
     {
         qDebug()<<"RealsenceManager::transData unknow paramiter";
     }
     return result;
 }

 void RealsenceManager::decodeData(int elevatorId,Array_TwoDimensional<float> data)
 {
     if(1 == elevatorId)
     {
         for(int i_x = 0;i_x <REALSENSE_TOP_PIXEL_WIDTH;i_x++)
         {
             for(int i_y = 0;i_y <REALSENSE_TOP_PIXEL_LENGTH;i_y++)
             {
                sample_box_elevator1_[i_x][i_y] = data.data.value(i_x*data.y+ data.y);
//                qDebug()<<i_x<<","<<i_y<<":"<<sample_box_elevator1_[i_x][i_y];
                if(0 == sample_box_elevator1_[i_x][i_y])
                {
                    sample_box_elevator1_[i_x][i_y] = 0.9;
                }
             }
         }
     }
     else if( 2 == elevatorId)
     {
         for(int i_x = 0;i_x <data.x;i_x++)
         {
             for(int i_y = 0;i_y <data.y;i_y++)
             {
                sample_box_elevator2_[i_x][i_y] = data.data.value(i_x*data.y+ data.y);
//                qDebug()<<i_x<<","<<i_y<<":"<<sample_box_elevator2_[i_x][i_y];
                if(0 == sample_box_elevator2_[i_x][i_y])
                {
                    sample_box_elevator2_[i_x][i_y] = 0.9;
                }
             }
         }
     }
     else
     {
         qDebug()<<"RealsenceManager::decodeData unknow elevatorId";
     }

 }
