#ifndef TCPTHREAD_H
#define TCPTHREAD_H
#include <QThread>
#include <QTcpSocket>
#include <QtNetwork>
#include <QtNetwork/QAuthenticator>
#include <QtNetwork/QNetworkDiskCache>
#include <QtNetwork/QNetworkProxy>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QSslError>
//继承QThread的TCP传输线程
//主要是完成run()虚函数的定义
//还有一些辅助变量的声明
class QFile;
class QTcpSocket;

class TcpThread : public QThread
{
    Q_OBJECT
public:
//    TcpThread();
//    TcpThread(int socketDescriptor, QObject *parent);
    TcpThread(int socketDescriptor, QObject *parent);
    void run();
signals:
    void error(QTcpSocket::SocketError socketError);
    void bytesArrived(qint64 ,qint32 ,qintptr);
public slots:
//    void receiveFile();
    void readyRead();
    void disconnected();
private:
    qintptr  socketDescriptor;   // socket 描述符
    qint64 bytesReceived;     //收到的总字节
    qint64 byteToRead;      //准备读取的字节
    qint32 TotalBytes;       //总共传输的字节数
    QTcpSocket *tcpSocket;
    QHostAddress fileName;    //文件名
    QFile  *localFile;
    QByteArray  inBlock;      //读取缓存


};

#endif // TCPTHREAD_H
