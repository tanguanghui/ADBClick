#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLineEdit>
#include <QMessageBox>
#include <QDate>
#include <QGridLayout>
#include <QCheckBox>
#include <synchapi.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
 ui->setupUi(this);
 //         //////////////////////////////////////////////////////////////////
#if 1
    static const GUID GUID_DEVINTERFACE_LIST[] =
    {
    // GUID_DEVINTERFACE_USB_DEVICE
    { 0xA5DCBF10, 0x6530, 0x11D2, { 0x90, 0x1F, 0x00, 0xC0, 0x4F, 0xB9, 0x51, 0xED } },
    // GUID_DEVINTERFACE_DISK
    { 0x53f56307, 0xb6bf, 0x11d0, { 0x94, 0xf2, 0x00, 0xa0, 0xc9, 0x1e, 0xfb, 0x8b } },
    // GUID_DEVINTERFACE_HID,
    { 0x4D1E55B2, 0xF16F, 0x11CF, { 0x88, 0xCB, 0x00, 0x11, 0x11, 0x00, 0x00, 0x30 } },
    // GUID_NDIS_LAN_CLASS
    { 0xad498944, 0x762f, 0x11d0, { 0x8d, 0xcb, 0x00, 0xc0, 0x4f, 0xc3, 0x35, 0x8c } },
    // GUID_DEVINTERFACE_COMPORT
    { 0x86e0d1e0, 0x8089, 0x11d0, { 0x9c, 0xe4, 0x08, 0x00, 0x3e, 0x30, 0x1f, 0x73 } }
    // GUID_DEVINTERFACE_SERENUM_BUS_ENUMERATOR
    //{ 0x4D36E978, 0xE325, 0x11CE, { 0xBF, 0xC1, 0x08, 0x00, 0x2B, 0xE1, 0x03, 0x18 } },
    // GUID_DEVINTERFACE_PARALLEL
    //{ 0x97F76EF0, 0xF883, 0x11D0, { 0xAF, 0x1F, 0x00, 0x00, 0xF8, 0x00, 0x84, 0x5C } },
    // GUID_DEVINTERFACE_PARCLASS
    //{ 0x811FC6A5, 0xF728, 0x11D0, { 0xA5, 0x37, 0x00, 0x00, 0xF8, 0x75, 0x3E, 0xD1 } }
    };

    //注册插拔事件
    HDEVNOTIFY hDevNotify;
    DEV_BROADCAST_DEVICEINTERFACE NotifacationFiler;
    ZeroMemory(&NotifacationFiler,sizeof(DEV_BROADCAST_DEVICEINTERFACE));
    NotifacationFiler.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
    NotifacationFiler.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;

    for(int i=0;i<sizeof(GUID_DEVINTERFACE_LIST)/sizeof(GUID);i++)
    {
        NotifacationFiler.dbcc_classguid = GUID_DEVINTERFACE_LIST[i];//GetCurrentUSBGUID();//m_usb->GetDriverGUID();

        hDevNotify = RegisterDeviceNotification((HANDLE)this->winId(),&NotifacationFiler,DEVICE_NOTIFY_WINDOW_HANDLE);
        if(!hDevNotify)
        {
            int Err = GetLastError();
            qDebug() << "注册失败" <<Err<<endl;
        }
        //else
    }
#endif





 //         //////////////////////////////////////////////////////////////////
 mNetworkAccessManager=new QNetworkAccessManager(this);

 while (!mTcpServer.isListening() && !mTcpServer.listen(QHostAddress::Any,12345))
 {
 QMessageBox::StandardButton ret = QMessageBox::critical(this,tr("回环"),tr("无法开始测试: %1.").arg(mTcpServer.errorString()),
 QMessageBox::Retry | QMessageBox::Cancel);

 if (ret == QMessageBox::Cancel)
 return;
 }

//    MyWindowStyle();
//      qDebug()<<"设备号列表"<<mDevList;
    QStringList listdev = mCmdCommand.AdbDeviceID("devices");


//    第一步创建一个QWidget实例，并将这个实例设置为centralWidget：
    QWidget *widget = new QWidget();

    this->setCentralWidget(widget);
//    然后创建一个主布局mainLayout，并把所需要的所有控件都往里面放（工具栏、菜单栏、状态栏除外）：
    QVBoxLayout *mainLayout= new QVBoxLayout;
    //    添加地点
    QHBoxLayout *mAddressLayout = new QHBoxLayout;
//        QLabel *mAddressLabel = new QLabel(QWidget::tr("&地点:"), widget);
        QLabel *mAddressLabel = new QLabel(QWidget::tr("&地点:"));
        QLineEdit *mlineEdt = new QLineEdit;
        mAddressLabel->setBuddy(mlineEdt);
        mAddressLayout->addWidget(mAddressLabel);
        mAddressLayout->addWidget(mlineEdt);

   QHBoxLayout *mMapLayout = new QHBoxLayout;
//        QPushButton *btn_open = new QPushButton(QWidget::tr("打开地图"), widget);
        QPushButton *btn_open = new QPushButton(QWidget::tr("打开地图"));
//        btn_open->setDefault(true);
//        QPushButton *btn_set = new QPushButton(QWidget::tr("模拟位置设定"), widget);
        QPushButton *btn_set = new QPushButton(QWidget::tr("模拟位置设定"));
        mMapLayout->addStretch();
        mMapLayout->addWidget(btn_open);
        mMapLayout->addStretch();
        mMapLayout->addWidget(btn_set);
        mMapLayout->addStretch();

//    添加打招呼
   QHBoxLayout *mGreetLayout = new QHBoxLayout;
//       QLabel *mSayLabel = new QLabel(QWidget::tr("&打招呼:"), widget);
       QLabel *mSayLabel = new QLabel(QWidget::tr("&打招呼:"));
       QLineEdit *mContentLineEdit = new QLineEdit;
       mSayLabel->setBuddy(mContentLineEdit);
       mGreetLayout->addWidget(mSayLabel);
       mGreetLayout->addWidget(mContentLineEdit);

//   设备信息展示
    QHBoxLayout *mDeviceInfoLayout = new QHBoxLayout;
          QLabel *mDeviceLabel = new QLabel(QWidget::tr("当前设备:"), widget);
          mBrowserDevice = new QTextBrowser;
          mDeviceInfoLayout->addWidget(mDeviceLabel);
          mDeviceInfoLayout->addWidget(mBrowserDevice);

   QHBoxLayout *bomLayout = new QHBoxLayout;
//       QPushButton *btn_greet = new QPushButton(QWidget::tr("打招呼设置"), widget);
   QPushButton *btn_greet = new QPushButton(QWidget::tr("打招呼设置"));
//       btn_greet->setDefault(true);
//       QPushButton *btn_save = new QPushButton(QWidget::tr("保存设置"), widget);
       QPushButton *btn_save = new QPushButton(QWidget::tr("保存设置"));
       bomLayout->addStretch();
       bomLayout->addWidget(btn_greet);
       bomLayout->addStretch();
       bomLayout->addWidget(btn_save);
       bomLayout->addStretch();
//       功能按钮模块   functional block
        
   QHBoxLayout *mFunctionalLayout = new QHBoxLayout;
//           btn_lighten = new QPushButton(QWidget::tr("亮屏"), widget);
//           btn_lighten->setDefault(true);
    btn_lighten = new QPushButton(QWidget::tr("亮屏"), widget);
    btn_quench = new QPushButton(QWidget::tr("黑屏"), widget);
    btn_start = new QPushButton(QWidget::tr("开始"), widget);
    btn_stop = new QPushButton(QWidget::tr("停止"), widget);
    btn_restart = new QPushButton(QWidget::tr("重启设备"), widget);
    btn_frist_run = new QPushButton(QWidget::tr("第一次运行"), widget);
    btn_update = new QPushButton(QWidget::tr("检查更新"), widget);

           mFunctionalLayout->addStretch();
           mFunctionalLayout->addWidget(btn_lighten);
           mFunctionalLayout->addStretch();
           mFunctionalLayout->addWidget(btn_quench);
           mFunctionalLayout->addStretch();
           mFunctionalLayout->addWidget(btn_start);
           mFunctionalLayout->addStretch();
           mFunctionalLayout->addWidget(btn_stop);
           mFunctionalLayout->addStretch();
           mFunctionalLayout->addWidget(btn_restart);
           mFunctionalLayout->addStretch();
           mFunctionalLayout->addWidget(btn_frist_run);
           mFunctionalLayout->addStretch();
           mFunctionalLayout->addWidget(btn_update);
           mFunctionalLayout->addStretch();
       
    QHBoxLayout *mProgressStateLayout = new QHBoxLayout;
//     QLabel *mProgressLabel = new QLabel(QWidget::tr("进度条:"), widget);
    QLabel *mProgressLabel = new QLabel(QWidget::tr("进度条:"));
     mStateLabel= new  QLabel;
    mProgressBar =  new QProgressBar;
    mProgressBar->alignment();
    mProgressStateLayout->addWidget(mProgressLabel);
    mProgressStateLayout->addWidget(mStateLabel);
    mProgressStateLayout->addWidget(mProgressBar);

//    QCheckBox *cbox = new QCheckBox(this);
//       cbox->setText("choose");
//       cbox->setChecked(false);
//   QPushButton * button = new QPushButton(this);
//    ("* daemon not running. starting it now on port 5037 *", "* daemon started successfully *", "List of devices attached ", "7e429b807d14\tdevice", "", "")
//      设备号模块 不能使用  QVBoxLayout *devStrLayout = new QVBoxLayout(this);
//    mdevStrLayout = new QVBoxLayout;
    devStrLayout = new QVBoxLayout;
    for( int i =0;i<listdev.size();i++){
         if(listdev.at(i).compare("")!=0){
            mDevList.append(listdev.at(i).split("\t").at(0));
            QPushButton *button = new QPushButton(listdev.at(i).split("\t").at(0));
            devStrLayout->addWidget(button);
         }
     }
//    devStrLayout->addLayout(mdevStrLayout);

     deviceLayout = new QHBoxLayout;
//      QLabel *deviceLabel = new QLabel(QWidget::tr("&地点:"), widget);
     QLabel *deviceLabel = new QLabel(QWidget::tr("设备号:"), widget);
     deviceLayout->addWidget(deviceLabel);
     deviceLayout->addLayout(devStrLayout);


     mainLayout->addLayout(mAddressLayout);
     mainLayout->addLayout(mMapLayout);
     mainLayout->addLayout(mGreetLayout);
     mainLayout->addLayout(bomLayout);
     mainLayout->addLayout(mFunctionalLayout);
     mainLayout->addLayout(mProgressStateLayout);
     mainLayout->addLayout(mDeviceInfoLayout);
//     mainLayout->addWidget(cbox);
     mainLayout->addLayout(deviceLayout);
//     mainLayout->setSizeConstraint(QLayout::SetFixedSize);
     widget->setLayout(mainLayout);

    setWindowTitle(tr("设备列表"));
//消息处理

connect(btn_lighten, SIGNAL(clicked()), this, SLOT(LightenScreen()));
connect(btn_quench, SIGNAL(clicked()), this, SLOT(QuenchScreen()));
connect(btn_start, SIGNAL(clicked()), this, SLOT(Start()));
connect(btn_stop, SIGNAL(clicked()), this, SLOT(StopApp()));
connect(btn_restart, SIGNAL(clicked()), this, SLOT(RestartDevice()));
connect(btn_frist_run, SIGNAL(clicked()), this, SLOT(FristRun()));
connect(btn_update, SIGNAL(clicked()), this, SLOT(CheckUpdate()));

connect(&mTcpServer,SIGNAL(bytesArrived(qint64,qint32,qintptr)),
 this,SLOT(updateProgress(qint64,qint32,qintptr)));


connect(mNetworkAccessManager, SIGNAL(finished(QNetworkReply*)),this, SLOT(finishedReplySlot(QNetworkReply*)));
connect(btn_save,SIGNAL(clicked()),this,SLOT(onClickedPost()));


}

void MainWindow::updateProgress(qint64 ReceivedBytes, qint32 TotalBytes, qintptr socketDescriptor)
 {
    mProgressBar->setMaximum(TotalBytes);
    mProgressBar->setValue(ReceivedBytes);
    mStateLabel->setText(tr("已接收 %1MB").arg(ReceivedBytes / (1024 * 1024)));
    mStateLabel->setText(tr("现在连接的socket描述符:%1").arg(socketDescriptor));
// ui->progressBar->setMaximum(TotalBytes);
// ui->progressBar->setValue(ReceivedBytes);
// ui->statuslabel->setText(tr("已接收 %1MB").arg(ReceivedBytes / (1024 * 1024)));
// ui->textBrowser->setText(tr("现在连接的socket描述符:%1").arg(socketDescriptor));

}


void MainWindow::LightenScreen() {
    //what you want to do
    // 亮屏
    for(int i = 0;i<mDevList.size();i++){
//        qDebug()<<"设备号列表"<<mDevList.at(i);
        mCmdCommand.Shell( mDevList.at(i),"input keyevent 224");
//        Sleep(50);
        mCmdCommand.Shell( mDevList.at(i) ,"input swipe 250 1000 250 300");
//        Sleep(200);
    }

   QMessageBox::information(this, "提示", "已点击亮屏");
//   btn_lighten->setText(tr("已点击亮屏"));
}

 //熄灭屏幕
void MainWindow::QuenchScreen() {
    //what you want to do
    for(int i = 0;i<mDevList.size();i++){
//        qDebug()<<"设备号列表"<<mDevList.at(i);
        mCmdCommand.Shell( mDevList.at(i),"input keyevent 223");
//        Sleep(100);
    }
   QMessageBox::information(this, "提示", "已点击熄屏");
//   btn_quench->setText(tr("已点击熄屏"));
}
//开始
void MainWindow::Start() {
    //what you want to do

   QMessageBox::information(this, "提示", "已点击开始");
//   btn_start->setText(tr("已点击开始"));
}
//停止APP
void MainWindow::StopApp() {
    //what you want to do
    for(int i = 0;i<mDevList.size();i++){
//       home键操作
        mCmdCommand.Shell( mDevList.at(i),"input keyevent 3");
//      静默杀掉后台
        mCmdCommand.Shell( mDevList.at(i) ,"am force-stop vicmob.earn");
        mCmdCommand.Shell( mDevList.at(i) ,"am force-stop com.tencent.mm");
        mCmdCommand.Shell( mDevList.at(i) ,"am force-stop com.tencent.mobileqq");
//        Sleep(200);
    }
   QMessageBox::information(this, "提示", "已点击停止");
//   btn_stop->setText(tr("已点击停止"));
}
//重启设备
void MainWindow::RestartDevice() {
    //what you want to do

   QMessageBox::information(this, "提示", "已点击重启");
//   btn_restart->setText(tr("已点击重启"));
}
//第一次运行
void MainWindow::FristRun() {
    //what you want to do

   QMessageBox::information(this, "提示", "已点击运行");
//   btn_frist_run->setText(tr("已点击运行"));
}
//检查更新
void MainWindow::CheckUpdate() {
    //what you want to do

   QMessageBox::information(this, "提示", "已点击更新");
//   btn_update->setText(tr("已点击更新"));
}

void MainWindow::MyWindowStyle()            //设置窗体属性
{
    //设置窗体为固定大小，高度190为不显示列表状态
    this->setFixedSize(this->width(),500);
    //设置图标
    QIcon icon = QIcon(":/images/ico.png");
    this->setWindowIcon(icon);

    //设置界面无边框并位于状态栏上
    this->setWindowFlags(Qt::FramelessWindowHint );
    this->setMouseTracking(true);


    //设置能接收拖动
    this->setAcceptDrops(true);

    //设置初始背景颜色，深蓝色
//    QPalette p;
//    p.setBrush(this->backgroundRole(),QBrush(QColor(0,0,127,255)));
//    this->setPalette(p);
//    this->setAutoFillBackground(true);



}

void MainWindow::finishedReplySlot(QNetworkReply *reply)
{
#if 1
     // Reading attributes of the reply
     // e.g. the HTTP status code
     QVariant statusCodeV = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
     // Or the target URL if it was a redirect:
     QVariant redirectionTargetUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
     // see CS001432 on how to handle this

     // no error received?
     if (reply->error() == QNetworkReply::NoError)
     {
         QByteArray bytes = reply->readAll();  // bytes
         //QString string(bytes); // string
         QString string = QString::fromUtf8(bytes);
        qDebug() << "---------return--------------" << string ;
         QJsonParseError jsonpe;
         QJsonDocument json = QJsonDocument::fromJson(bytes, &jsonpe);
             if (jsonpe.error == QJsonParseError::NoError)
             {
                 if (json.isObject())
                 {
                     QJsonObject obj = json.object();
                     if (obj.contains("error_code"))
                     {
                         if(obj["error_code"] == 0){//成功
                             if(obj.contains("qaPath")){

                                 qDebug() << "obj[qaPath] = " << obj["qaPath"].toString();
                                 QString qrimageName_path  = obj["qaPath"].toString();
                                 //截取文件名
                                 int first = qrimageName_path.lastIndexOf ("/"); //从后面查找"/"位置
                                 QString qr_imageName = qrimageName_path.right (qrimageName_path.length ()-first-1); //从右边截取
                                 qDebug() << "qr_imageName = " << qr_imageName;
                                 emit sigReplyMessage(0,obj["qaPath"].toString());
                             }
                         }
                         else{
                             if(obj.contains("msg")){
                                 qDebug() << "obj[msg] = " << obj["msg"];
                                 emit sigReplyMessage(1,obj["msg"].toString());
                             }
                         }
                     }
                 }
                 else
                 {
                     qDebug() << "error, shoud json object";
                 }
             }
             else
             {
                 qDebug() << "error:" << jsonpe.errorString();
             }

            mStateLabel->setText(string);
//         ui->textBrowser->setText(string);


     }
     else
     {
         // handle errors here
     }

     // We receive ownership of the reply object
     // and therefore need to handle deletion.
     reply->deleteLater();
#endif
}

void MainWindow::onClickedPost()
{
    qDebug() << "Date:" << QDate::currentDate() <<"post on click";

//使用 QHttpMultiPart  FormDataType
        QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

        QHttpPart textPart;
        textPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"username\""));
        textPart.setBody("my text");

        QHttpPart imagePart;
        QString imageName_path  = "F:/soft/a.jpg";
        //截取文件名
        int first = imageName_path.lastIndexOf ("/"); //从后面查找"/"位置
        QString imageName = imageName_path.right (imageName_path.length ()-first-1); //从右边截取
        qDebug()<<imageName;

        //QString imageName="a.jpg";
        imagePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("image/jpeg"));
        imagePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"file\"; filename=\""+ imageName + "\""));


        QFile *file = new QFile(imageName_path);
        file->open(QIODevice::ReadOnly);
        imagePart.setBodyDevice(file);
        file->setParent(multiPart); // we cannot delete the file now, so delete it with the multiPart

        multiPart->append(textPart);
        multiPart->append(imagePart);

       // QString service_Address  = "http://127.0.0.1/testphp/saveimg/singleimage.php";
        QString service_Address  = "http://shiyi2014.gotoip2.com/testimage/timage.php";
     //  QString service_Address  ="http://127.0.0.1/testphp/testimage.php";
        QUrl url(service_Address);
        QNetworkRequest request(url);
     qDebug() << "---------Uploaded--------------" << url<< "of" ;
      //  QNetworkAccessManager manager;
        //request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded;charset=utf-8");
        reply = mNetworkAccessManager->post(request, multiPart);
        multiPart->setParent(reply); // de



}
//              /////////////设备更新///////////////

char MainWindow::FirstDriveFromMask(ULONG unitmask)
{
    char i;
    for (i = 0; i < 26; ++i)
    {
        if (unitmask & 0x1)
            break;
        unitmask = unitmask >> 1;
    }
    return (i + 'A');
}

bool MainWindow::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    Q_UNUSED(eventType);
    Q_UNUSED(result);
    MSG* msg = reinterpret_cast<MSG*>(message);
    int msgType = msg->message;
    if(msgType==WM_DEVICECHANGE)
    {
        //qDebug() << "Event DEVICECHANGE Happend" << endl;
        PDEV_BROADCAST_HDR lpdb = (PDEV_BROADCAST_HDR)msg->lParam;
        switch (msg->wParam) {
        case DBT_DEVICETYPESPECIFIC:
         {
         qDebug() << "DBT_DEVICETYPESPECIFIC " ;
         break;
         }
        case DBT_DEVICEARRIVAL:
            if(lpdb->dbch_devicetype == DBT_DEVTYP_VOLUME)
            {
                PDEV_BROADCAST_VOLUME lpdbv = (PDEV_BROADCAST_VOLUME)lpdb;
                if(lpdbv->dbcv_flags ==0)
                {
                    //插入u盘   now we can do us want to  do  somethings
                    QString USBDisk = QString(this->FirstDriveFromMask(lpdbv ->dbcv_unitmask));
                    qDebug() << "USB_Arrived and The USBDisk is: "<<USBDisk ;
                    mBrowserDevice->append("USB_Arrived and The USBDisk is " + USBDisk);

                }
            }
            // #define DBT_DEVTYP_PORT 0x00000003
             qDebug() << "设备状态： " <<lpdb->dbch_devicetype;
            if(lpdb->dbch_devicetype == DBT_DEVTYP_DEVICEINTERFACE)
            {
//                PDEV_BROADCAST_DEVICEINTERFACE pDevInf  = (PDEV_BROADCAST_DEVICEINTERFACE)lpdb;

//                QString strname = QString::fromWCharArray(pDevInf->dbcc_name,pDevInf->dbcc_size);
//                mBrowserDevice->append("插入设备 ：" + strname);
               // ui->textBrowser->append("插入设备 ：" + strname);
            }
            //  插入手机后的设备状态是 3
            if(lpdb->dbch_devicetype == DBT_DEVTYP_PORT)
            {
                Sleep(3000);
//                PDEV_BROADCAST_DEVICEINTERFACE pDevInf  = (PDEV_BROADCAST_DEVICEINTERFACE)lpdb;

//                QString strname = QString::fromWCharArray(pDevInf->dbcc_name,pDevInf->dbcc_size);
//                mBrowserDevice->append("插入设备 ：" + strname);
               // ui->textBrowser->append("插入设备 ：" + strname);
                QStringList mDeviceIDin = mCmdCommand.AdbDeviceID("devices");
                QString mDeviceIDStrin = mDeviceIDin.join(",");
                qDebug()<< "设备in：" << mDeviceIDStrin;

                mBrowserDevice->append("当前设备号in ：" + mDeviceIDStrin);

//                QLayoutItem *child;
//                devStrLayout->setParent(NULL);
                while(!devStrLayout->isEmpty()) {
                    QLayoutItem *child = devStrLayout->takeAt(0);

                    devStrLayout->removeWidget(child->widget());

                    delete child->widget();
                }

                for( int i =0;i<mDeviceIDin.size();i++){
                     if(mDeviceIDin.at(i).compare("")!=0){
                        mDevList.append(mDeviceIDin.at(i).split("\t").at(0));
                        QPushButton *button = new QPushButton(mDeviceIDin.at(i).split("\t").at(0));
                        devStrLayout->addWidget(button);
                     }
                 }


            }

            break;
        case DBT_DEVICEREMOVECOMPLETE:
            qDebug() << "设备移除" <<endl;
            if(lpdb->dbch_devicetype == DBT_DEVTYP_VOLUME)
            {
                PDEV_BROADCAST_VOLUME lpdbv = (PDEV_BROADCAST_VOLUME)lpdb;
                if(lpdbv->dbcv_flags == 0)
                {
                    //qDebug() << "USB_Removed";
                   mBrowserDevice->append("USB_Remove");

                }
            }
            qDebug() << "设备状态： " <<lpdb->dbch_devicetype;
            //  拔掉手机后的设备状态是 3
            if(lpdb->dbch_devicetype == DBT_DEVTYP_PORT)
            {
//                PDEV_BROADCAST_DEVICEINTERFACE pDevInf  = (PDEV_BROADCAST_DEVICEINTERFACE)lpdb;
//                qDebug()<< "移除设备(name)：" << pDevInf->dbcc_name;
//                qDebug()<< "移除设备(guid)：" << pDevInf->dbcc_classguid;
//                qDebug()<< "移除设备(size)：" << pDevInf->dbcc_size;

//                QString strname = QString::fromWCharArray(pDevInf->dbcc_name,pDevInf->dbcc_size);
//                qDebug()<< "移除设备：" << strname;
//                mBrowserDevice->append("移除设备 ：" + strname);

               QStringList mDeviceID = mCmdCommand.AdbDeviceID("devices");
               QString mDeviceIDStr = mDeviceID.join(",");
               qDebug()<< "设备：" << mDeviceIDStr;
               mBrowserDevice->append("当前设备号 ：" + mDeviceIDStr);

//                devStrLayout->setParent(NULL);
               while(!devStrLayout->isEmpty()) {
                   QLayoutItem *child = devStrLayout->takeAt(0);

                   devStrLayout->removeWidget(child->widget());
                   delete child->widget();
               }


                 for( int i =0;i<mDeviceID.size();i++){
                      if(mDeviceID.at(i).compare("")!=0){
                         mDevList.append(mDeviceID.at(i).split("\t").at(0));
                         QPushButton *button = new QPushButton(mDeviceID.at(i).split("\t").at(0));
                         devStrLayout->addWidget(button);
                      }
                  }

            }
            if(lpdb->dbch_devicetype == DBT_DEVTYP_DEVICEINTERFACE)
            {
//                PDEV_BROADCAST_DEVICEINTERFACE pDevInf  = (PDEV_BROADCAST_DEVICEINTERFACE)lpdb;
//                qDebug()<< "移除设备(name)：" << pDevInf->dbcc_name;
//                qDebug()<< "移除设备(guid)：" << pDevInf->dbcc_classguid;
//                qDebug()<< "移除设备(size)：" << pDevInf->dbcc_size;

//                QString strname = QString::fromWCharArray(pDevInf->dbcc_name,pDevInf->dbcc_size);
//                qDebug()<< "移除设备：" << strname;
//                mBrowserDevice->append("移除设备 " + strname);
            }
            break;
        }
    }
    return false;

}
//             /////////设备更新////////////////////

MainWindow::~MainWindow()
{
    delete ui;
}
