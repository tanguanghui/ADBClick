#include "tcpsession.h"

//TcpSession::TcpSession()
//{

//}

TcpSession::TcpSession()
{
    connect(this, &TcpSession::readyRead, this, &TcpSession::SlotStartRead);

}

TcpSession::~TcpSession()
{
    disconnect(this, &TcpSession::readyRead, this, &TcpSession::SlotStartRead);
}

void TcpSession::Disconnect()
{
    this->disconnectFromHost();
}

qint64 TcpSession::Write(const char *data, qint64 len)
{
    return this->write(data, len);
}

qint64 TcpSession::Write(const char *data)
{
    return this->write(data);
}

void TcpSession::SlotStartRead()
{
    QByteArray buffer;
    buffer = this->readAll();
    emit this->SignalRead(buffer.toStdString().c_str(), buffer.length());
}
