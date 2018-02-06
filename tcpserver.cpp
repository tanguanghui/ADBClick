#include "tcpserver.h"
#include "tcpthread.h"
#include <QDebug>
//TcpServer::TcpServer()
//{

//}
//构造函数
 TcpServer::TcpServer(QObject *parent) :
 QTcpServer(parent)
 {
     int port = 6666;
     QDateTime current_date_time =QDateTime::currentDateTime();
     QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz ddd");
   qDebug() << "TcpServer:" << QDateTime::currentDateTime()<<"  current_date:  "<<current_date;
   if(this->listen (QHostAddress::Any,port))
      {
          qDebug("invocation!!!");
          qDebug() << "port:" << port;

      }
      else
      {
          qDebug("disinvocation!!!");
      }

  }
TcpServer::~TcpServer()
{
//    this->Stop();
   qDebug() << "~TcpServer:" << QDate::currentDate();
}

//重新定义了incomingConnection这个虚函数，
//开辟一个新的tcpsocket线程，从TcpServer获得socketDescriptor，
//并完成相应的信号连接。
void TcpServer::incomingConnection(qintptr socketDescriptor)
{
     qDebug() << "incomingConnection   " <<socketDescriptor;
//  在incomingConnection()中，定义一个线程TcpThread，并将socketDescriptor传递给其构造函数，完成线程的创建，并且调用QThread的start函数，开始执行线程的虚函数run()。
TcpThread *thread = new TcpThread(socketDescriptor, this);
//将线程结束信号与线程的deleteLater槽关联
connect(thread, SIGNAL(finished()),thread, SLOT(deleteLater()));
//关联相应的UI更新槽
connect(thread, SIGNAL(bytesArrived(qint64,qint32,qintptr)), this,SIGNAL(bytesArrived(qint64,qint32,qintptr)));
//QT的线程都是从start开始，调用run()函数
thread->start();
}
