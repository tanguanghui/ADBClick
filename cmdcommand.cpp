#include "cmdcommand.h"
#include <QProcess>

CmdCommand::CmdCommand()
{

}
       // adb shell 命令操作
void CmdCommand::Shell( QString device , QString command){
    QProcess p(0);
    p.start("cmd", QStringList()<<"/c"<<"adb -s " + device + " shell su -root " + command);
    p.waitForStarted();
    p.waitForFinished();
}
       //直接使用adb命令
void CmdCommand::Adb(QString command){
    QProcess p(0);
    p.start("cmd", QStringList()<<"/c"<<"adb "+command);
    p.waitForStarted();
    p.waitForFinished();
}
QStringList CmdCommand::AdbDeviceID(QString command){
    QProcess p(0);
    p.start("cmd", QStringList()<<"/c"<<"adb "+command);
    p.waitForStarted();
    p.waitForFinished();
    QString strTemp=QString::fromLocal8Bit(p.readAllStandardOutput());
    qDebug()<<"%s"<<strTemp;
   //  "List of devices attached \r\n685e97037d34\tdevice\r\n7YRBBDB751101241\tdevice\r\n\r\n"
//     先分割一次，是避免出现daemon not running
    QStringList mDevicesStr = strTemp.split("attached \r\n");
     qDebug()<<"设备信息："<<mDevicesStr.at(1);
   QStringList listdev = mDevicesStr.at(1).split("\r\n");
    qDebug()<<"%s"<<listdev;
    qDebug()<<"%s"<<listdev.size();
    qDebug()<<"%s"<<listdev.at(0)<<"  "<<listdev.at(1);
   QStringList  mDeviceList;
   int i ;
   for( i =0;i<listdev.size();i++){
        if(listdev.at(i).compare("")!=0){
           mDeviceList.append(listdev.at(i).split("\t").at(0));

        }
    }
    qDebug()<<"设备号列表"<<mDeviceList;
    return mDeviceList;
}


CmdCommand::~CmdCommand()
{

}
