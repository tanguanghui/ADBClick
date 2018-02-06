#include "tcpthread.h"
#include <QtGui>
#include <QtNetwork>
 //构造函数完成简单的赋值/
//TcpThread::TcpThread()
//{

//}
TcpThread::TcpThread(qintptr socketDescriptor, QObject *parent):
QThread(parent),socketDescriptor(socketDescriptor)
 {
    this->socketDescriptor = socketDescriptor;
    bytesReceived = 0;
 }
//因为QT的线程的执行都是从run()开始，
//所以在此函数里完成tcpsocket的创建，相关信号的绑定
void TcpThread::run()
{
    tcpSocket = new QTcpSocket;
//将Server传来的socketDescriptor与刚创建的tcpSocket关联
if (!tcpSocket->setSocketDescriptor(this->socketDescriptor)) {
    emit error(tcpSocket->error());
    return;
}
    qDebug()<<socketDescriptor;
 //这是重中之重，必须加Qt::BlockingQueuedConnection！
 //这里困扰了我好几天，原因就在与开始没加，默认用的Qt::AutoConnection。
 //简单介绍一下QT信号与槽的连接方式：
//Qt::AutoConnection表示系统自动选择相应的连接方式，如果信号与槽在同一线程，就采用Qt::DirectConnection，
//                                                 如果信号与槽不在同一线程，将采用Qt::QueuedConnection的连接方式。
 //Qt::DirectConnection表示一旦信号产生，立即执行槽函数。
 //Qt::QueuedConnection表示信号产生后，将发送Event给你的receiver所在的线程，postEvent(QEvent::MetaCall,...)，slot函数会在receiver所在的线程的event loop中进行处理。
 //Qt::BlockingQueuedConnection表示信号产生后调用sendEvent(QEvent::MetaCall,...)
//在receiver所在的线程处理完成后才会返回;只能当sender,receiver不在同一线程时才可以。
 //Qt::UniqueConnection表示只有它不是一个重复连接，连接才会成功。如果之前已经有了一个链接（相同的信号连接到同一对象的同一个槽上），那么连接将会失败并将返回false。
 //Qt::AutoCompatConnection与QT3保持兼容性
 //说明一下，对于任何的QThread来说，其线程只存在于run()函数内，其它的函数都不在线程内，所以此处要采用Qt::BlockingQueuedConnection，
 //因为当SOCKET有数据到达时就会发出readyRead()信号，但是此时可能之前的receiveFile()还未执行完毕，之前使用的Qt::AutoConnection，
 //结果传输大文件的时候就会出错，原因就在于只要有数据到达的时候，就会连接信号，但是数据接收还没处理完毕，而Qt::BlockingQueuedConnection会阻塞
 //此连接，直到receiveFile()处理完毕并返回后才发送信号。

//    connect(tcpSocket, SIGNAL(readyRead()),this, SLOT(receiveFile()),Qt::BlockingQueuedConnection);
    connect(tcpSocket, SIGNAL(readyRead()),this,  SLOT(readyRead()), Qt::DirectConnection);
    connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));

    exec();

 }

void TcpThread::readyRead()
{
    // get the information
    QByteArray Data = tcpSocket->readAll();
    qDebug() << socketDescriptor << " Data in: " << Data ;
    QString ReceivedStr =Data;

    QStringList mDevicesStr = ReceivedStr.split("*");
     qDebug()<<"解析："<<mDevicesStr.at(0)<<"  "<<mDevicesStr.at(1);
    // will write on server side window
//    qDebug() << socketDescriptor << " Data in: " << Data <<" 收到的数据：  "<<ReceivedStr;

    tcpSocket->write(Data);
}

void TcpThread::disconnected()
{
    qDebug() << socketDescriptor << " Disconnected";


    tcpSocket->deleteLater();
    exit(0);
}






