#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QProgressBar>
#include <QLabel>
#include <QHBoxLayout>
#include "tcpthread.h"
#include "tcpserver.h"
#include "cmdcommand.h"
#include <objsafe.h>
#include <dbt.h>
#include <windows.h>
#include <QTextBrowser>

#include<devguid.h>
#include<SetupAPI.h>
#include<InitGuid.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
   void MyWindowStyle();            //设置窗体属性
//   void Shell(QString command, QString device);            // adb shell 命令操作
   char FirstDriveFromMask(ulong unitmask);
   QTextBrowser *mBrowserDevice;     //外接设备信息展示浏览框
   QPushButton *btn_lighten;         //亮屏
   QPushButton *btn_quench;          //黑屏
   QPushButton *btn_start;           //"开始"
   QPushButton *btn_stop ;          //停止
   QPushButton *btn_restart;      //重启设备
   QPushButton *btn_frist_run ;   //第一次运行
   QPushButton *btn_update ;    // 检查更新
   QProgressBar *mProgressBar;     //进度条
   QLabel *mStateLabel;            //进度条状态
   QStringList mDevList;          //设备号列表
   QHBoxLayout *deviceLayout;
   QVBoxLayout *devStrLayout;
   QVBoxLayout *mdevStrLayout;

signals:
    void sigReplyMessage(int retCode,QString msg);
public slots:
    void LightenScreen();           //亮屏
    void QuenchScreen();           //熄灭屏幕
    void Start();                   //开始
    void StopApp();                 //停止APP
    void RestartDevice();           //重启设备
    void FristRun();                //第一次运行
    void CheckUpdate();            //检查更新
    void updateProgress(qint64,qint32,qintptr);    //更新进度条
    void finishedReplySlot(QNetworkReply *reply);      //请求完成
    void onClickedPost();           //点击使用post 发出请求
protected:
    bool nativeEvent(const QByteArray & eventType, void * message, long*result);

private:
    Ui::MainWindow *ui;
    CmdCommand mCmdCommand;             //cmd 命令操作对象
    TcpServer mTcpServer;
    QNetworkAccessManager *mNetworkAccessManager;//定义网络访问管理的成员变量
    QNetworkReply* reply;                         //网络响应
};

#endif // MAINWINDOW_H
