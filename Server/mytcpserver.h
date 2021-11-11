#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

/*!
     Класс mytcpserver реализует подлючение к серверу и прием сообщений от клиента через сокет

 */

#include <QObject>
#include <QTcpServer> //класс с сигналами tcp
#include <QTcpSocket> //класс для работы с сокетами, передача инфы между клиентом и сервером

#include <QtNetwork>
#include <QByteArray>
#include <QDebug>

class MyTcpServer : public QObject
{
    Q_OBJECT
public:
    explicit MyTcpServer(QObject *parent = nullptr);
    ~MyTcpServer();
public slots:
    void slotNewConnection();
    void slotClientDisconnected();
    void slotServerRead();
    //void slotReadClient();

private:
    QTcpServer * mTcpServer;
    QTcpSocket * mTcpSocket;
    int server_status;
    QMap<int, QTcpSocket *> SClients;
};
#endif // MYTCPSERVER_H







