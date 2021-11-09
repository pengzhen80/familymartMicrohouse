#include "logmanagement.h"
#include<QDebug>
//LogManagement::LogManagement(QObject *parent) : QObject(parent)
//{

//}

//LogManagement::~LogManagement(){
//    delete log_;
//    log_ = nullptr;
//    delete file_;
//    file_ = nullptr;
//}

//void LogManagement::Init()
//{
//    QString filename = QString::number(QDateTime::currentDateTime().date().year())
//            +QString::number(QDateTime::currentDateTime().date().month())
//            +QString::number(QDateTime::currentDateTime().date().day());
//    QFile* file = new QFile(LOG_DIR+filename);
//    if(!file->open(QIODevice::ReadWrite))
//    {
//        qDebug()<<"cannot create file";
//    }

//    log_ = new QMessageLogger();
//}

//void LogManagement::slot_writeLog(LOGMANAGEMENT_SEVERITYLEVEL level,QString msg)
//{
//    switch (level) {
//    case LOGMANAGEMENT_SEVERITYLEVEL_CRITICAL:{
//        log_->critical(msg.toStdString().c_str());
//        break;
//    }
//    case LOGMANAGEMENT_SEVERITYLEVEL_DEBUG:{
//        break;
//    }
//    case LOGMANAGEMENT_SEVERITYLEVEL_FATAL:{
//        break;
//    }
//    case LOGMANAGEMENT_SEVERITYLEVEL_INFO:{
//        log_->info(msg.toStdString().c_str());
//        break;
//    }
//    case LOGMANAGEMENT_SEVERITYLEVEL_WARNING:{
//        break;
//    }
//    }
//}

QFile *gFileLog=NULL;
QMessageLogger *gMLog=NULL;

char *msgHead[]={
    "Debug   ",
    "Warning ",
    "Critical",
    "Fatal   ",
    "Info    "
};

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    QString current_date_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd");

    if(gFileLog){
        QTextStream tWrite(gFileLog);

        QString msgText="%1 | %6 | %2:%3, %4 | %5\n";
        msgText = msgText.arg(msgHead[type]).arg(context.file).arg(context.line).arg(context.function).arg(localMsg.constData()).arg(current_date_time);
        //gFileLog->write(msgText.toLocal8Bit(), msgText.length());
        tWrite << msgText;
    }else{
        fprintf(stderr, "%s | %s | %s:%u, %s | %s\n", msgHead[type], current_date_time.toLocal8Bit().constData(), context.file, context.line, context.function, localMsg.constData());
    }

}




void logSysInit(QString filePath)
{
    gFileLog = new QFile(filePath);
    if (!gFileLog->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)){
        return;
    }
//初始化自定义日志处理函数myMessageOutput
    qInstallMessageHandler(myMessageOutput);
//gMLog 这个类不要也可以，执行的时候只能看一下效果。
    gMLog = new QMessageLogger(__FILE__, __LINE__, Q_FUNC_INFO);
}

void logSysInit()
{
    qInstallMessageHandler(myMessageOutput);

    gMLog = new QMessageLogger(__FILE__, __LINE__, Q_FUNC_INFO);
}
