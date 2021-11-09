#ifndef LOGMANAGEMENT_H
#define LOGMANAGEMENT_H

#include <QObject>
#include<QMessageLogger>
#include <QFile>
#include<QDateTime>
#include"common.h"
//class LogManagement : public QObject
//{
//    Q_OBJECT
//public:
//    explicit LogManagement(QObject *parent = nullptr);
//    ~LogManagement();
//    void Init();
//public slots:
//    void slot_writeLog(LOGMANAGEMENT_SEVERITYLEVEL,QString msg);
//signals:
//private:
//    QMessageLogger* log_;
//    QFile* file_;
//};

extern QMessageLogger *gMLog;
void logSysInit(QString filePath);
void logSysInit();

#endif // LOGMANAGEMENT_H
