#include "clipboardmanagement.h"

ClipBoardManagement::ClipBoardManagement(QObject *parent) : QObject(parent)
{
//    std::thread t_takeimage = std::thread(&ClipBoardManagement::TakeImage,this);
//    t_takeimage.detach();

    std::thread t_takebarcode= std::thread(&ClipBoardManagement::takeBarcode_Two,this);
    t_takebarcode.detach();
}

void ClipBoardManagement::Init(QClipboard* clipboard)
{
    clipboard_ = clipboard;
}

void ClipBoardManagement::TakeImage()
{
    qDebug()<< QThread::currentThreadId();
    connect(clipboard_,&QClipboard::dataChanged,this,[&](){
        const QMimeData *mimeData = clipboard_->mimeData();
        QImage img = qvariant_cast<QImage>(mimeData->imageData());
        qDebug()<<"get a image";
        if(flag_take_nameimage_)
        {
            emit sig_nameImage(img);
            flag_take_nameimage_ = false;
        }
        else
        {
            if(flag_take_numberImage_)
            {
                emit sig_numberImage(img);
                flag_take_numberImage_ = false;
            }
        }
    });
//    while(true)
//    {
//        const QMimeData *mimeData = clipboard_->mimeData();
//        if(mimeData->hasImage())
//        {
//            //将图片数据转为QImage
//            QImage img = qvariant_cast<QImage>(mimeData->imageData());
//            //qDebug()<<"get a image";
//            emit sig_nameImage(img);
//            clipboard_->clear();
//            //            if(true == flag_take_nameimage_)
//            //            {
//            //                emit sig_nameImage(img);
//            //                flag_take_nameimage_ =false;
//            //            }
//            //            if(true == flag_take_numberImage_)
//            //            {
//            //                emit sig_numberImage(img);
//            //                flag_take_numberImage_ = false;
//            //            }
//        }

//        QThread::msleep(1000);
//    }

}

void ClipBoardManagement::takeBarcode_Two()
{
    qDebug()<< "ClipBoardManagement::takeBarcode : "<<QThread::currentThreadId();
    connect(clipboard_,&QClipboard::dataChanged,this,[&](){
        const QMimeData *mimeData = clipboard_->mimeData();
        QString barcode = qvariant_cast<QString>(mimeData->imageData());
        qDebug()<<"get a barcode:"<<barcode;
        if(flag_take_barcode_)
        {
            if(flag_take_barcode_first_)
            {
                barcode_.clear();
                barcode_ += barcode;
                flag_take_barcode_first_ = false;
            }
            else
            {
                flag_take_barcode_first_ = true;

                barcode_ += barcode;
                Sig_Clipmanagement_Mainwindow sigToWindow;
                sigToWindow.type = SIG_CLIPMANAGEMENT_MAINWINDOW_BARCODE;
                sigToWindow.value_barcode.barcode = barcode_;

                emit sig_mainwindow(sigToWindow);
                barcode_.clear();
            }

        }
    });
}

void ClipBoardManagement::takeBarcode()
{
    qDebug()<< "ClipBoardManagement::takeBarcode : "<<QThread::currentThreadId();
    connect(clipboard_,&QClipboard::dataChanged,this,[&](){
        const QMimeData *mimeData = clipboard_->mimeData();
        QString barcode = qvariant_cast<QString>(mimeData->imageData());
//        qDebug()<<"get a barcode";
        if(flag_take_barcode_)
        {
            Sig_Clipmanagement_Mainwindow sigToWindow;
            sigToWindow.type = SIG_CLIPMANAGEMENT_MAINWINDOW_BARCODE;
            sigToWindow.value_barcode.barcode = barcode;

            emit sig_mainwindow(sigToWindow);
        }
    });
}

void ClipBoardManagement::slot_mainwindow_takeNameImage()
{
    flag_take_nameimage_ = true;
}

void ClipBoardManagement::slot_mainwindow_takeNumberImage()
{
    flag_take_numberImage_ = true;
}

void ClipBoardManagement::slot_mainwindow(Sig_Mainwindow_Clipmanagement sig)
{
    switch (sig.type) {
    case SIG_MAINWINDOW_CLIPMANAGEMENT_LISTEN_BARCODE:
    {
        flag_take_barcode_ = true;
        break;
    }
    default:
    {
        qDebug()<<"ClipBoardManagement::slot_mainwindow : unknown signal";
        break;
    }
    }
}
