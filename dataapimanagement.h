#ifndef DATAAPIMANAGEMENT_H
#define DATAAPIMANAGEMENT_H

#include <QObject>
#include<QNetworkAccessManager>
#include<QNetworkReply>
#include<QWebSocketServer>
#include<QWebSocket>
#include<QUrl>
#include<QTcpServer>
#include<QSslCertificate>
#include<QWebSocketCorsAuthenticator>
#include<QUdpSocket>
#include<QNetworkDatagram>
#include<QFile>
#include<QSslKey>
#include<openssl/aes.h>
#include<openssl/evp.h>
#include <openssl/sha.h>
#include"common.h"

class DataApiManagement : public QObject
{
    Q_OBJECT
public:
    explicit DataApiManagement(QObject *parent = nullptr);
    void Init();
    void SendRequest();
    void SendHttpsRequest(QString url,QString api_publicKey,QString api_privacyKey,\
                          QString xmlData,QString timeStamp,QString signature);
    void ReadRequest();
private:
    void DecodeReply(QByteArray data);
    QString DataToXmlData(QString data);


signals:
private:
    QNetworkAccessManager* manager_;
};

class ApiWebServer: public QObject
{
    Q_OBJECT
public:
    explicit ApiWebServer(QObject *parent = nullptr);
    ~ApiWebServer();
    void Init();
public slots:
    void slot_newConnection();
    void slot_close();
private:
    void processTextMessage(const QString &message);
    void processBinaryMessage(const QByteArray &message);
    void socketDisconnected();
    QString makeQUrl();
private:
    QWebSocketServer* server_;
    QWebSocket *pSocket_;
};

class MyTcpServer: public QObject
{
    Q_OBJECT
public:
    explicit MyTcpServer(QObject *parent = nullptr);
    ~MyTcpServer();
    void Init();
public slots:
    void slot_tcp_newClient();
private:
    void ParsingBuf(QByteArray buff);
    bool checkHead(QByteArray buff);
    void replyData(QString data);
    QByteArray solveData(QByteArray data);
    void decryptFile(QFile srcFile);
    QByteArray decryptData(QByteArray data);
    int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,
                unsigned char *iv, unsigned char *plaintext);
signals:
    void sig_itemManager(Sig_ApiManagement_ItemManagement);
private:
    QTcpServer* tcpServer_;
    QTcpSocket* set_client_;
};

class MyUdpServer: public QObject
{
    Q_OBJECT
public:
    explicit MyUdpServer(QObject *parent = nullptr);
    ~MyUdpServer();
    void Init();
public slots:
private:
    void readPendingDatagrams();
    void processTheDatagram(QNetworkDatagram datagram);
signals:
    void sig_itemManager(Sig_ApiManagement_ItemManagement);
private:
      QUdpSocket*  udpSocket_;
};

class SslEchoServer : public QObject
{
    Q_OBJECT
public:
    explicit SslEchoServer(quint16 port, QObject *parent = nullptr);
    ~SslEchoServer() override;

private Q_SLOTS:
    void onNewConnection();
    void processTextMessage(QString message);
    void processBinaryMessage(QByteArray message);
    void socketDisconnected();
    void onSslErrors(const QList<QSslError> &errors);

private:
    QWebSocketServer *m_pWebSocketServer;
    QList<QWebSocket *> m_clients;
};

#endif // DATAAPIMANAGEMENT_H
