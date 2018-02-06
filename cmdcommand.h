#ifndef CMDCOMMAND_H
#define CMDCOMMAND_H
#include <QString>
#include <QDebug>
#include <QStringList>
class CmdCommand
{
public:
    CmdCommand();
     ~CmdCommand();
    void Shell(QString device , QString command);            // adb shell 命令操作
    void Adb(QString command);                              //直接使用adb命令
    QStringList AdbDeviceID(QString command);                  //直接使用adb命令
//    QStringList  mDeviceList;
};

#endif // CMDCOMMAND_H
