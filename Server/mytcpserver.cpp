#include "mytcpserver.h"
#include <QDebug>
#include <QCoreApplication>
#include <QString>
#include "database.h"
/*! \brief Методы класса mytcpserver
    Реализуется возможность подключения к серверу, отсоединения, подключения нескольких клиентов, считывание приходящих сообщений
    Авторизация, регистрация и подключение к бд
    */

std::string auth(std::string log,std::string pass) //авторизация, подключение к бд
{
    DataBase db;
    db.open();
    //db.querry("");
    std::string result = db.querry("SELECT login, password FROM bd_kr_nl WHERE login='" + log + "' AND password='"+pass+"' ;");
    qDebug()<<QString::fromStdString(result);
    if (result == ""){
            result = "auth_false";
        }
        else {
            result = "auth_true";
        }
    return result;
}
std::string reg(std::string log,std::string pass)
{
    DataBase db;
    db.open();
    //db.querry("");
    std::string result = db.querry("SELECT login, password FROM bd_kr_nl WHERE login='" + log + "' AND password='"+pass+"' ;");
    qDebug()<<QString::fromStdString(result);
    if (result == "" || result == " "){
            db.querry("INSERT INTO bd_kr_nl VALUES ('"+log+"', '"+pass+"', 0, 0);");
            qDebug()<<QString::fromStdString(result);
            result = "reg_true";
        }
        else {
            result = "reg_false";
        }
    return result;
}
std::string send_stat(std::string log){ //отправить статус
        DataBase db;
        db.open();
        std::string result = db.querry("SELECT pobed, porog FROM bd_kr_nl WHERE login = '" + log + "';");
        qDebug()<<QString::fromStdString(result);
        return result;

    //std::string result;
    //std::cout << "Search status" << std::endl;
    //std::cout << "Your status is " << std::endl;

    //return result;
}

std::string new_param(std::string & temp)
{
    int pos = -1;
    pos = temp.find("&");
    std::string res = temp.substr(0,pos);
    temp.erase(0,pos+1);
    return res;
}

std::string parsing(std::string mess)
{
    std::string name_of_func;

    name_of_func = new_param(mess);

    if(name_of_func=="auth"){

        std::string log = "";
        std::string pass = "";

        log = new_param(mess);
        pass = new_param(mess);
        return auth(log,pass);
    }

    else if (name_of_func=="reg")
    {
        std::string log = "";
        std::string pass = "";

        log = new_param(mess);
        pass = new_param(mess);
        return reg(log,pass);
    }

    else if (name_of_func=="status"){
        std::string log = "";
        log = new_param(mess);
        return send_stat(log);
    }

//    else {
//        return "statistic";
//    }
}

MyTcpServer::~MyTcpServer()
{
    foreach(int i,SClients.keys()){
                QTextStream os(SClients[i]);
                SClients[i]->close();
                SClients.remove(i);
            }
    server_status=0;
    mTcpServer->close();
}
MyTcpServer::MyTcpServer(QObject *parent) : QObject(parent){
    mTcpServer = new QTcpServer(this);
    connect(mTcpServer, &QTcpServer::newConnection,
            this, &MyTcpServer::slotNewConnection);

    if(!mTcpServer->listen(QHostAddress::Any, 33333)){
        qDebug() << "server is not started";
    } else {
        server_status=1;
        qDebug() << "server is started";
    }
}

void MyTcpServer::slotNewConnection(){
    if(server_status==1){
        QTcpSocket* clientSocket=mTcpServer->nextPendingConnection();
        int indesersocs=(int)clientSocket->socketDescriptor();
        SClients[indesersocs]=clientSocket;
        SClients[indesersocs]->write( "Hello, user!!! I am echo server!\r\n");
        connect(SClients[indesersocs], &QTcpSocket::readyRead,this,&MyTcpServer::slotServerRead);
        connect(SClients[indesersocs],&QTcpSocket::disconnected,this,&MyTcpServer::slotClientDisconnected);
    }
}

void MyTcpServer::slotServerRead(){
    QTcpSocket* clientSocket=(QTcpSocket*)sender();
    int id = (int)clientSocket->socketDescriptor();
    QByteArray array;
    std::string message="";

    while(clientSocket->bytesAvailable()>0)
    {
        array =clientSocket->readAll();
        message.append(array);
    }
    //message="auth&user&1234";
    qDebug()<<QString::fromStdString(message);

    array.clear();
    array.append(parsing(message));

    clientSocket->write(array);
}

void MyTcpServer::slotClientDisconnected(){
    QTcpSocket* clientSocket=(QTcpSocket*)sender();
    int id = (int)clientSocket->socketDescriptor();
    clientSocket->close();
    SClients.remove(id);
    qDebug() << QString::fromUtf8("Client is disconnected\n");
}
