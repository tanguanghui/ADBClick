#ifndef TCPSESSION_H
#define TCPSESSION_H
#include <QTcpSocket>
#include <functional>

class TcpSession : public QTcpSocket
{
    Q_OBJECT
signals:
    void SignalRead(const char *data, int len);
public:
    TcpSession();
    ~TcpSession();

    void Disconnect();
    qint64 Write(const char *data, qint64 len);
    qint64 Write(const char *data);

protected:

private slots:
    void SlotStartRead();

};

#endif // TCPSESSION_H
