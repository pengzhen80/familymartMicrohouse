#include "dataapimanagement.h"

DataApiManagement::DataApiManagement(QObject *parent) : QObject(parent)
{

}

void DataApiManagement::Init()
{
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished,this, [&](QNetworkReply* reply){
        QByteArray data =  reply->readAll();
        DecodeReply(data);
    });
    manager->get(QNetworkRequest(QUrl("http://qt-project.org")));
}

void DataApiManagement::SendRequest()
{
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished,this, [&](QNetworkReply* reply){
        QByteArray data =  reply->readAll();
        DecodeReply(data);
    });
    manager->post(QNetworkRequest(QUrl("http://qt-project.org")),QString("hello").toUtf8());
}

void DataApiManagement::DecodeReply(QByteArray data)
{
    qDebug()<<QString::fromStdString(data.toStdString());
}

QString DataApiManagement::DataToXmlData(QString data)
{
    QString xmldata = "Data=<?xml version='1.0' encoding='UTF-8'?><Doc>"+data+"</Doc>";
    return xmldata;
}

void DataApiManagement::SendHttpsRequest(QString url,QString api_publicKey,QString api_privacyKey,\
                                         QString xmlData,QString timeStamp,QString signature)
{
    QNetworkAccessManager* pManager = new QNetworkAccessManager(this);
    QNetworkRequest request;

    // 发送https请求前准备工作;
    QSslConfiguration conf = request.sslConfiguration();
//    conf.setPrivateKey(QSslKey(QString("").toUtf8(),));
    conf.setPeerVerifyMode(QSslSocket::VerifyNone);
    conf.setProtocol(QSsl::TlsV1SslV3);
    request.setSslConfiguration(conf);

    request.setUrl(QUrl(url));       // "https://www.temp.com"

    QString data_post = api_publicKey+"&"+xmlData+"&"+timeStamp+"&"+signature;

    QNetworkReply *reply = pManager->post(request, data_post.toUtf8());

    connect(reply, &QIODevice::readyRead,this,[&](){
        QByteArray data =  reply->readAll();
        DecodeReply(data);
    });
    connect(reply, &QNetworkReply::errorOccurred,
            this, [&](QNetworkReply::NetworkError code){
        qDebug()<<code;
    });
    connect(reply, &QNetworkReply::sslErrors,
            this, [&](const QList<QSslError> &errors){
        qDebug()<<errors;
    });
}

ApiWebServer::ApiWebServer(QObject *parent) : QObject(parent)
{

}
ApiWebServer::~ApiWebServer()
{
    server_->close();
}

void ApiWebServer::Init()
{
    server_ = new QWebSocketServer(QStringLiteral("Echo Server"),QWebSocketServer::SslMode::NonSecureMode);
    server_->setHandshakeTimeout(60*1000);
    qDebug()<<"web server init";
    if(server_->listen(QHostAddress("192.168.8.2"), 443))
    {
        qDebug() << "server url:"<<server_->serverUrl().url();
        qDebug() << "server name:"<<server_->serverName();
        qDebug() << "server address:"<<server_->serverAddress();
        qDebug() << "server port:"<<server_->serverPort();
    }
    else
    {
        qDebug()<<"web server open failed";
        return;
    }
    QWebSocketServer::
    connect(server_, &QWebSocketServer::newConnection,this,[&]()
    {
        qDebug()<<"new connection";
    });
    connect(server_, &QWebSocketServer::acceptError,this, [&](QAbstractSocket::SocketError socketError)
    {
        qDebug()<<"got en error";
    });
    connect(server_, &QWebSocketServer::originAuthenticationRequired,this, [&](QWebSocketCorsAuthenticator *authenticator)
    {
        qDebug()<<"originAuthenticationRequired";
    });
    connect(server_, &QWebSocketServer::peerVerifyError,this, [&](const QSslError &error)
    {
        qDebug()<<"peerVerifyError";
    });
    connect(server_, &QWebSocketServer::preSharedKeyAuthenticationRequired,this, [&](QSslPreSharedKeyAuthenticator *authenticator)
    {
        qDebug()<<"preSharedKeyAuthenticationRequired";
    });
    connect(server_, &QWebSocketServer::serverError,this, [&](QWebSocketProtocol::CloseCode closeCode)
    {
        qDebug()<<"serverError";
    });
    connect(server_, &QWebSocketServer::sslErrors,this, [&](const QList<QSslError> &errors)
    {
        qDebug()<<"sslErrors";
    });
    connect(server_, &QWebSocketServer::closed, this, [&]()
    {
         qDebug()<<"closed";
    });

}

void ApiWebServer::slot_newConnection()
{
    qDebug()<<"new connection";
    pSocket_ = server_->nextPendingConnection();

    connect(pSocket_, &QWebSocket::textMessageReceived, this, &ApiWebServer::processTextMessage);
    connect(pSocket_, &QWebSocket::binaryMessageReceived, this, &ApiWebServer::processBinaryMessage);
    connect(pSocket_, &QWebSocket::disconnected, this, [&](){
        pSocket_->close();
        pSocket_ = nullptr;
    });

}

void ApiWebServer::slot_close()
{

}

void ApiWebServer::processTextMessage(const QString &message)
{
    qDebug()<<message;
}

void ApiWebServer::processBinaryMessage(const QByteArray &message)
{
    qDebug()<<QString::fromStdString(message.toStdString());
}
void ApiWebServer::socketDisconnected()
{
    qDebug()<<"socket disconnect";
}

QString ApiWebServer::makeQUrl()
{
    QUrl url;
    url.setHost("127.0.0.1");
    url.setPort(8000);
    qDebug()<<url.url();
    return url.url();
}

MyTcpServer::MyTcpServer(QObject *parent):QObject(parent)
{

}
MyTcpServer::~MyTcpServer()
{
    tcpServer_->close();
}

void MyTcpServer::Init()
{
    tcpServer_ = new QTcpServer();
    if(!tcpServer_->listen(QHostAddress("127.0.0.1"),4431))
    {
        qDebug()<<"Server_Test::Init failed";
        return;
    }
//    qDebug()<<tcpServer_->
    connect(tcpServer_,&QTcpServer::newConnection,this,&MyTcpServer::slot_tcp_newClient);
//    connect(tcpServer_, &QTcpServer::newConnection,this,[&]()
//    {
//        qDebug()<<"new connection";
//    });
}

void MyTcpServer::ParsingBuf(QByteArray buff)
{
    qDebug()<<QString::fromStdString(buff.data());
    //get size size<10000 take 4 byte to make size
    QVector<Item_FamilyMart> vec_items;
    unsigned char size_1000 = buff.data()[0];
    unsigned char size_100 = buff.data()[1];
    unsigned char size_10 = buff.data()[2];
    unsigned char size_1 = buff.data()[3];
//    int data_size = (size_1000-'0')*1000 + (size_100-'0')*100+(size_10-'0')*10+(size_1-'0');
    int start_index = 4;
    qDebug()<<"buff size is"<<buff.size();
    while(start_index<buff.size())
    {
        Item_FamilyMart item;

//        unsigned char cell_size_10 = buff.data()[start_index];
//        unsigned char cell_size_1 = buff.data()[start_index+1];
//        int cell_size = (cell_size_10-'0')*10+(cell_size_1-'0');
//        qDebug()<<"cell size : "<<cell_size;
//        start_index += 2;
        WebApi_LocalClient_Msg data;
        //assign barcode
        for(int i=0;i<25;i++)
        {
            data.barcode[i] = buff.data()[start_index+ i];
        }
        data.barcode[25] = '\0';
        qDebug()<<"barcode:"<<data.barcode;
        //assign phone
        for(int i=25;i<28;i++)
        {
            data.phone[i-25] = buff.data()[start_index+i];
        }
        data.phone[3] = '\0';
        qDebug()<<"phone:"<<data.phone;
        //assign name
        int count_name = 0;
        for(int i=28;i<buff.size()-start_index;i++)
        {
            unsigned char tmp_c = buff.data()[start_index+i];
            if(',' == tmp_c)
            {
                break;
            }
            else
            {
                data.name[i-28] = tmp_c;
                count_name ++;
            }
        }
        data.name[count_name] ='\0';
        qDebug()<<"name:"<<data.name;
        start_index += 28+count_name+1;
        qDebug()<<"start_index : "<<start_index;

        QString barcode(data.barcode);
//        qDebug()<<barcode;
        QString phone(data.phone);
//        qDebug()<<phone;
        QString name(data.name);
//        qDebug()<<name;

        item.infos.barcode = barcode;
        item.infos.name = name;
        item.infos.phone_number = phone;
        vec_items.push_back(item);
    }

    Sig_ApiManagement_ItemManagement sigToItem;
    sigToItem.type = SIG_APIMANAGEMENT_ITEMMANAGEMENT_NEWITEMS;
    sigToItem.value_newItems.items =vec_items;
    emit sig_itemManager(sigToItem);
    qDebug()<<"add items from api : "<<vec_items.size();
}

int MyTcpServer::decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,
            unsigned char *iv, unsigned char *plaintext)
{
    EVP_CIPHER_CTX *ctx;

    int len;

    int plaintext_len;

    /* Create and initialise the context */
    if(!(ctx = EVP_CIPHER_CTX_new()))
    {

    }

    /*
     * Initialise the decryption operation. IMPORTANT - ensure you use a key
     * and IV size appropriate for your cipher
     * In this example we are using 256 bit AES (i.e. a 256 bit key). The
     * IV size for *most* modes is the same as the block size. For AES this
     * is 128 bits
     */
    if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
    {

    }

    /*
     * Provide the message to be decrypted, and obtain the plaintext output.
     * EVP_DecryptUpdate can be called multiple times if necessary.
     */
    if(1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
    {

    }
    plaintext_len = len;

    /*
     * Finalise the decryption. Further plaintext bytes may be written at
     * this stage.
     */
    if(1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len))
    {

    }
    plaintext_len += len;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    return plaintext_len;
}

bool MyTcpServer::checkHead(QByteArray buff)
{
    qDebug()<<QString::fromStdString(buff.toStdString());
    return true;
}

void MyTcpServer::replyData(QString data)
{
//    if(set_client_->isOpen())
//    {
        //socket send buf
//        char * p = (char*)(&data);
//        QByteArray sendbuf(p,sizeof(data));
        QByteArray sendbuf = data.toUtf8();

//        if(set_client_ == nullptr)
//        {
//            qDebug()<<"no connect";
//            return;
//        }
//        if(set_client_->state() == QAbstractSocket::ConnectedState)
//        {
//            set_client_->write(sendbuf); //write the data itself
//        }
        set_client_->write(sendbuf);
//    }
}

QByteArray MyTcpServer::solveData(QByteArray data)
{

}



void MyTcpServer::decryptFile(QFile srcFile)
{
        QFile destFile;

//        srcFile.setFileName(m_encryptedFile);
//        destFile.setFileName(m_destinationFile);

//        if(!srcFile.open(QIODevice::ReadOnly))
//        {
//            return false;
//        }

//        if(!destFile.open(QIODevice::WriteOnly))
//        {
//            return false;
//        }

        const int MAX_BUFFER_SIZE = 1024;
        int out_len;
        EVP_CIPHER_CTX * ctx = EVP_CIPHER_CTX_new();

        EVP_CipherInit(ctx, EVP_aes_256_cbc_hmac_sha256(), WEB_API_KEY_PUBLIC, WEB_API_KEY_PRIVATE , false);
        int blocksize = EVP_CIPHER_CTX_block_size(ctx);
        QByteArray cipher_buf;
        cipher_buf.resize(MAX_BUFFER_SIZE + blocksize);

        qint64 size = 0;

        while(!srcFile.atEnd())
        {
            // Read in data in blocks until EOF. Update the ciphering with each read.
            QByteArray inRead = srcFile.read(MAX_BUFFER_SIZE);
            EVP_CipherUpdate(ctx, reinterpret_cast<uchar *>(cipher_buf.data()), &out_len, reinterpret_cast<uchar *>(inRead.data()), inRead.count());
            destFile.write(cipher_buf.data(), out_len);
            size += out_len;
        }

        // Now cipher the final block and write it out.
        EVP_CipherFinal(ctx, reinterpret_cast<uchar *>(cipher_buf.data()), &out_len);
        destFile.write(cipher_buf.data(), out_len);
        size += out_len;
        destFile.close();
        srcFile.close();

        EVP_CIPHER_CTX_free(ctx);
}

QByteArray MyTcpServer::decryptData(QByteArray data)
{
    /* Buffer for the decrypted text */
       unsigned char decryptedtext[128];

       int EVP_DecryptInit(EVP_CIPHER_CTX *ctx, const EVP_CIPHER *type,
               const unsigned char *key, const unsigned char *iv);
       int EVP_DecryptFinal(EVP_CIPHER_CTX *ctx, unsigned char *outm,
               int *outl);
}

void MyTcpServer::slot_tcp_newClient()
{
    set_client_ = tcpServer_->nextPendingConnection();
    qDebug()<<"Server_Test::slot_tcp_newClient : new connection : "<<set_client_->peerAddress().toString();
//    tcpServer_->close();
    connect(set_client_,&QTcpSocket::readyRead,this,[&](){
        QByteArray data = set_client_->readAll();
        ParsingBuf(data);
//        while(data.size()>0)
//        {
//            qDebug()<<"get data :";
//            ParsingBuf(data);
//            data.clear();
//            data = set_client_->readAll();
//        }
//        set_client_->write(QString("hello,I am server").toUtf8());
    });
    connect(set_client_, &QAbstractSocket::disconnected,this, [&](){
        qDebug()<<"closed";
        set_client_->close();
//        if(!tcpServer_->listen(QHostAddress("223.22.240.67"),443))
//        {
//            qDebug()<<"Server_Test::Init failed";
//        }
    });
}

MyUdpServer::MyUdpServer(QObject *parent):QObject(parent)
{

}
MyUdpServer::~MyUdpServer()
{

}

void MyUdpServer::Init()
{
    udpSocket_ = new QUdpSocket(this);
    udpSocket_->bind(QHostAddress("192.168.8.2"),443);

    connect(udpSocket_, &QUdpSocket::readyRead,this, &MyUdpServer::readPendingDatagrams);
}

void MyUdpServer::readPendingDatagrams()
{
    while (udpSocket_->hasPendingDatagrams()) {
            QNetworkDatagram datagram = udpSocket_->receiveDatagram();
            processTheDatagram(datagram);
        }
}

void MyUdpServer::processTheDatagram(QNetworkDatagram datagram)
{
    qDebug()<<datagram.senderAddress();
    qDebug()<<QString::fromStdString(datagram.data().toStdString());
}

SslEchoServer::SslEchoServer(quint16 port, QObject *parent) :
    QObject(parent),
    m_pWebSocketServer(nullptr)
{
    m_pWebSocketServer = new QWebSocketServer(QStringLiteral("SSL Echo Server"),
                                              QWebSocketServer::SecureMode,
                                              this);
    QSslConfiguration sslConfiguration;
    QFile certFile("keys/csr.pem");
    QFile keyFile("keys/key.pem");
    certFile.open(QIODevice::ReadOnly);
    keyFile.open(QIODevice::ReadOnly);
    QSslCertificate certificate(&certFile, QSsl::Pem);
    QSslKey sslKey(&keyFile, QSsl::Rsa, QSsl::Pem);
    certFile.close();
    keyFile.close();
//    sslConfiguration.setPeerVerifyMode(QSslSocket::VerifyNone);
    sslConfiguration.setPeerVerifyMode(QSslSocket::AutoVerifyPeer);
    sslConfiguration.setLocalCertificate(certificate);
    sslConfiguration.setPrivateKey(sslKey);
    m_pWebSocketServer->setSslConfiguration(sslConfiguration);

    if (m_pWebSocketServer->listen(QHostAddress("192.168.8.2"), port))
    {
        qDebug() << "SSL Echo Server listening on port" << port;
        connect(m_pWebSocketServer, &QWebSocketServer::newConnection,
                this, &SslEchoServer::onNewConnection);
        connect(m_pWebSocketServer, &QWebSocketServer::sslErrors,
                this, &SslEchoServer::onSslErrors);
    }
}
//! [constructor]

SslEchoServer::~SslEchoServer()
{
    m_pWebSocketServer->close();
    qDeleteAll(m_clients.begin(), m_clients.end());
}

//! [onNewConnection]
void SslEchoServer::onNewConnection()
{
    QWebSocket *pSocket = m_pWebSocketServer->nextPendingConnection();

    qDebug() << "Client connected:" << pSocket->peerName() << pSocket->origin();

    connect(pSocket, &QWebSocket::textMessageReceived, this, &SslEchoServer::processTextMessage);
    connect(pSocket, &QWebSocket::binaryMessageReceived,
            this, &SslEchoServer::processBinaryMessage);
    connect(pSocket, &QWebSocket::disconnected, this, &SslEchoServer::socketDisconnected);

    m_clients << pSocket;
}
//! [onNewConnection]

//! [processTextMessage]
void SslEchoServer::processTextMessage(QString message)
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (pClient)
    {
        pClient->sendTextMessage(message);
    }
}
//! [processTextMessage]

//! [processBinaryMessage]
void SslEchoServer::processBinaryMessage(QByteArray message)
{

    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (pClient)
    {
        pClient->sendBinaryMessage(message);
    }
}
//! [processBinaryMessage]

//! [socketDisconnected]
void SslEchoServer::socketDisconnected()
{
    qDebug() << "Client disconnected";
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (pClient)
    {
        m_clients.removeAll(pClient);
        pClient->deleteLater();
    }
}

void SslEchoServer::onSslErrors(const QList<QSslError> &)
{
    qDebug() << "Ssl errors occurred";
}
//! [socketDisconnected]
