#ifndef TCPSERVER_H
#define TCPSERVER_H
#include <QMainWindow>
#include <QTcpServer>

class TcpServer : public QTcpServer
{
     Q_OBJECT
public:
    explicit TcpServer(QObject *parent = 0);
     ~TcpServer();

 //此信号用来更新UI
signals:
void bytesArrived(qint64,qint32,qintptr);
//QTcpServer类自带的函数
public:
//qt5 以及之后的参数都是qintptr。qt5之前的都是int，
//void incomingConnection(int socketDescriptor);
void incomingConnection(qintptr socketDescriptor);




};

#endif // TCPSERVER_H
