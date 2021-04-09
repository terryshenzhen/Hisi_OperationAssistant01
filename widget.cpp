#include "widget.h"
#include "ui_widget.h"

#include<QVector>

// QVector

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

     initUI();


}

Widget::~Widget()
{
    delete ui;
}



QString Widget::getIP()
{
    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    foreach (QHostAddress address, list)
    {
       if(address.protocol() == QAbstractSocket::IPv4Protocol)

            return address.toString();
    }
       return 0;
}

void Widget::initUI()
{

    m_bConnectState = false;
    //resize(700,350);

    ui->comboBox_2IP->addItem("192.168.1.147");
    ui->comboBox_2IP->addItem("192.168.1.232");
    ui->comboBox_2IP->addItem("192.168.154.129");
    ui->comboBox_2IP->addItem("192.168.1.222");


    ui->lineEdit_3username->setText("root");
    ui->lineEdit_3username->setVisible(false);
    ui->label_3username->setVisible(false);

    //ui->lineEdit_4password->setEchoMode(QLineEdit::Password);
    //ui->lineEdit_4password->setText("qq554869603");
    ui->lineEdit_4password->setText("zz");
    ui->lineEdit_4password->setVisible(false);
    ui->label_4password->setVisible(false);

    ui->lineEdit_2port->setText("22");
    ui->lineEdit_2port->setVisible(false);
    ui->label_2Port->setVisible(false);

    setConnectState(m_bConnectState);


    ui->lineEdit_5command->setText("scp 8615899879019@192.168.21.30:/ccc /opt");


#if 0
    // for get something
    QString tempIP= this->getIP();
    tempIP.prepend("wget -P/tmp/ http://");
    tempIP.append(":80/aa");

    QString tempIPping= this->getIP();
    tempIPping.prepend("ping ");
    tempIPping.append(" -c3");
#endif


    ui->label_6ConnectedState->resize(20,10);


    iPlistQListwidgetInit();
    cmdlistQListviewInit();


}

void Widget::createConnection()
{
    QString strIp = ui->comboBox_2IP->currentText();
    QString strUser = ui->lineEdit_3username->text();
    QString strPwd = ui->lineEdit_4password->text();

    //=======================111
    CConnectionForSshClient *m_sshSocket;

    m_sshSocket = new CConnectionForSshClient(strIp,ui->lineEdit_2port->text().toInt(),strPwd,strUser);
    m_sshSocket->init();

    //  ==========test
//    m_sshSocket->m_shell.isNull();
//    m_sshSocket->m_pSshSocket->connectionInfo();


// wget -P/tmp/ http://%s/recover/product_002/board_initres

    connect(m_sshSocket,SIGNAL(sigConnectStateChanged(bool,QString,int)),this,SLOT(slotConnectStateChanged(bool,QString,int)));
    connect(m_sshSocket,SIGNAL(sigDataArrived(QString ,QString , int )),this,SLOT(slotDataArrived(QString ,QString , int )));

    connect(this,SIGNAL(sendCMDsignal(QString)),m_sshSocket,SLOT(slotSend(QString)));
    connect(this,SIGNAL(sigDisconnected()),m_sshSocket,SLOT(slotDisconnected()));

}


void Widget::createConnection(QString inputIP)
{
    if(inputIP.isEmpty())
    {
        return;
    }

    QString strIp = inputIP;
    QString strUser = ui->lineEdit_3username->text();
    QString strPwd = ui->lineEdit_4password->text();

    //=======================111
    CConnectionForSshClient *m_sshSocket;

    m_sshSocket = new CConnectionForSshClient(strIp,ui->lineEdit_2port->text().toInt(),strPwd,strUser);
    m_sshSocket->init();

    //  ==========test
//    m_sshSocket->m_shell.isNull();
//    m_sshSocket->m_pSshSocket->connectionInfo();

//8615899879019
// wget -P/tmp/ http://%s/recover/product_002/board_initres

    connect(m_sshSocket,SIGNAL(sigConnectStateChanged(bool,QString,int)),this,SLOT(slotConnectStateChanged(bool,QString,int)));
    connect(m_sshSocket,SIGNAL(sigDataArrived(QString ,QString , int )),this,SLOT(slotDataArrived(QString ,QString , int )));

    connect(this,SIGNAL(sendCMDsignal(QString)),m_sshSocket,SLOT(slotSend(QString)));
    connect(this,SIGNAL(sigDisconnected()),m_sshSocket,SLOT(slotDisconnected()));

}




void Widget::disConnection()
{
    emit sigDisconnected();
}

const QString m_red_SheetStyle = "min-width: 16px; min-height: 16px;max-width:16px; max-height: 16px;border-radius: 8px;  border:1px solid black;background:red";

const QString m_green_SheetStyle = "min-width: 16px; min-height: 16px;max-width:16px; max-height: 16px;border-radius: 8px;  border:1px solid black;background:green";

const QString m_grey_SheetStyle = "min-width: 16px; min-height: 16px;max-width:16px; max-height: 16px;border-radius: 8px;  border:1px solid black;background:grey";

const QString m_yellow_SheetStyle = "min-width: 16px; min-height: 16px;max-width:16px; max-height: 16px;border-radius: 8px;  border:1px solid black;background:yellow";


//ui.lab_LED->setStyleSheet(m_red_SheetStyle);//改成 红色圆形
//ui.lab_LED->setStyleSheet(m_green_SheetStyle);
//ui.lab_LED->setStyleSheet(m_grey_SheetStyle);
//ui.lab_LED->setStyleSheet(m_yellow_SheetStyle);

void Widget::setConnectState(bool bState)
{
    if(!bState)
        //ui->label_6ConnectedState->setStyleSheet("QLabel{background-color:#ff0000;border-radius:5px;}");
        ui->label_6ConnectedState->setStyleSheet(m_grey_SheetStyle);
    else
        ui->label_6ConnectedState->setStyleSheet(m_yellow_SheetStyle);
        //ui->label_6ConnectedState->setStyleSheet("QLabel{background-color:#00ff00;border-radius:5px;}");

}



void Widget::slotDataArrived(QString strMsg, QString strIp, int nPort)
{
    Q_UNUSED(strIp)
    Q_UNUSED(nPort)

    qDebug()<< "\n slotDataArrived "<< strIp<< strMsg;

    QString receivedQstring =strMsg.replace(";0m","       ", Qt::CaseInsensitive);
    receivedQstring= receivedQstring.replace(";32m","       ", Qt::CaseInsensitive);
    receivedQstring= receivedQstring.replace(";34m","       ", Qt::CaseInsensitive);

    ui->textEditInfoOutput->append(receivedQstring);
    ui->textEditInfoOutput->moveCursor(QTextCursor::End);
}


void Widget::slotConnectStateChanged(bool bState, QString strIp, int nPort)
{
    Q_UNUSED(strIp)
    Q_UNUSED(nPort)

    qDebug()<< "\n slotConnectStateChanged "<< strIp;
    m_bConnectState = bState;
    setConnectState(m_bConnectState);
    if(m_bConnectState)
        //ui->pushButtonConnect->setText(QStringLiteral("断开"));
    ui->pushButtonConnect->setText(QString::fromLocal8Bit("断开"));
    else
        ui->pushButtonConnect->setText(QString::fromLocal8Bit("连接"));

}

//========================IPlist-- qlistwidget table

void Widget::iPlistQListwidgetInit()
{
    pIPlistStandarditemModel= new QStandardItemModel(10,2,this);
    pIPlistStandarditemModel->setHeaderData(0, Qt::Horizontal, "IP");
    pIPlistStandarditemModel->setHeaderData(1, Qt::Horizontal, "SW ver");


//    pIPlistStandarditemModel->setHorizontalHeaderItem(0, QStandardItem("IP"));
//    pIPlistStandarditemModel->setHorizontalHeaderItem(1, QStandardItem("SW ver"));


    ui->tableViewIP->setModel(pIPlistStandarditemModel);
    ui->tableViewIP->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableViewIP->setEditTriggers(QAbstractItemView::DoubleClicked);
    ui->tableViewIP->setAlternatingRowColors(true);
    ui->tableViewIP->setColumnWidth(0, 300);
    ui->tableViewIP->setColumnWidth(0, 110);


}



void Widget::cmdlistQListviewInit()
{
    cmdlistStandarditemModel=  new QStandardItemModel(0,0,this);


//    cmdlistStandarditemModel->setHeaderData(0,Qt::Horizontal, "cmd");
//     cmdlistStandarditemModel->setHeaderData(0,Qt::Horizontal, "eee");
//      cmdlistStandarditemModel->setHeaderData(0,Qt::Horizontal, "ddd");

    cmdlistStandarditemModel->appendRow(new QStandardItem("ls -l"));
    cmdlistStandarditemModel->appendRow(new QStandardItem("ps"));



    ui->listViewcmdlist->setModel(cmdlistStandarditemModel);
    ui->listViewcmdlist->setAlternatingRowColors(true);
    ui->listViewcmdlist->setSelectionBehavior(QAbstractItemView::SelectRows);
ui->listViewcmdlist->setContextMenuPolicy(Qt::ActionsContextMenu);

    RemoveCmditemAction= new QAction(tr("Delete"),this);
   // connect(RemoveCmditemAction, SIGNAL(triggered()),this, SLOT(removeActiontriggered()));
    ui->listViewcmdlist->addAction(RemoveCmditemAction);


//    QStandardItem bbb("bbb");
//    cmdlistStandarditemModel->appendRow(&bbb);

//connect(ui->listViewcmdlist, SIGNAL(), this, SLOT();

    qDebug()<<" cmdlistQListviewInit in "<< cmdlistStandarditemModel->rowCount()<< cmdlistStandarditemModel->columnCount();
}

//========================



void Widget::on_pushButtonConnect_clicked()
{
    if(!m_bConnectState){
        ui->pushButtonConnect->setText(QString::fromLocal8Bit("连接中..."));

        createConnection();  //发起连接
    }else{
        ui->pushButtonConnect->setText(QString::fromLocal8Bit("连接"));
        m_bConnectState = false;
        emit sigDisconnected();//断开连接
        setConnectState(m_bConnectState);
    }
}

void Widget::on_pushButton_2send_clicked()
{



    if(m_bConnectState){
        QString strCmd = ui->lineEdit_5command->text();
                //ui->comboBoxcmd->currentText();

        if(strCmd.isEmpty())
        {
            strCmd ="ls -l";
        }
        strCmd += " --color=never";
        strCmd += "\n"; //添加回车
        qDebug()<<"send "<< strCmd;
        emit sendCMDsignal(strCmd);
    }
}

void Widget::on_pushButton_2CleanOutput_clicked()
{
    ui->textEditInfoOutput->clear();
}





void Widget::on_pushButton_SearchIP_clicked()
{



}

void Widget::on_pushButton_allexe_clicked()
{
    qDebug()<< " totalrow:  "<< cmdlistStandarditemModel->rowCount();

    for(int32_t ii=0; ii< cmdlistStandarditemModel->rowCount();ii++)
    {
        qDebug()<<cmdlistStandarditemModel->item(ii,0)->text();

        QString strCmd =cmdlistStandarditemModel->item(ii,0)->text();
        if(strCmd.isEmpty())
        {
            strCmd ="ls -l";
        }
        strCmd += "\n"; //添加回车
        qDebug()<<"send "<< strCmd;
        emit sendCMDsignal(strCmd);

    }


//createConnection(IPfromlist);



//    QString strCmd = ui->lineEdit_5command->text();
//            //ui->comboBoxcmd->currentText();

//    if(strCmd.isEmpty())
//    {
//        strCmd ="ls -l";
//    }
//    strCmd += "\n"; //添加回车
//    qDebug()<<"send "<< strCmd;
//    emit sendCMDsignal(strCmd);


}

qint32 NowIPIndex;

qint32 ii;
QString IPString2[IPMaxNum]; //="192.168.1.232";

void Widget::on_Sendcmd2IP()
{
    for(ii=IPMinNum; ii< IPMaxNum; ii++)
    {
        qDebug()<<ii;

        if(MychildThread[ii]!=Q_NULLPTR)
        {
            qDebug("\n MychildThread[ii] is not Q_NULLPTR ");

            delete(MychildThread[ii]);
            MychildThread[ii]=Q_NULLPTR;


        }
        QString tempip("192.168.1.");
        tempip += QString::number(ii);
        IPString2[ii]=tempip;

        MychildThread[ii] = new QThread;
        myObjectThread[ii] = new MyThreadcmdHost;

        myObjectThread[ii]->moveToThread(MychildThread[ii]);                                      //将创建的对象移到线程容器中

        connect(MychildThread[ii],SIGNAL(finished()),myObjectThread[ii],SLOT(deleteLater()));        //终止线程时要调用deleteLater槽函数
        connect(MychildThread[ii],SIGNAL(finished()),MychildThread[ii],SLOT(deleteLater()));
        connect(MychildThread[ii],SIGNAL(finished()),this,SLOT(searchHost_finishedThreadSlot()));

        connect(MychildThread[ii],SIGNAL(started()),myObjectThread[ii],SLOT(startThreadSlot()));  //开启线程槽函数

        connect(myObjectThread[ii],SIGNAL(on_threadFinishedSignal(QString, qint32)),this,SLOT(on_threadFinishedSignalSlot(QString, qint32)) );

        MychildThread[ii]->start();

        qDebug()<<" MychildThread[ii]->start now for IP "<<IPString2[ii];

    }


}

//=====================multithread search host

MyThreadcmdHost::MyThreadcmdHost(QObject *parent) : QObject(parent)

{

    ;//isStop = false;

}


void MyThreadcmdHost::startThreadSlot()

{

        qDebug()<<"MyThread::startThreadSlot QThread::currentThreadId()=="<<QThread::currentThreadId();

        void* temp_sessions= Q_NULLPTR;


        int version = 0;
      //  int ret111 = board_control_get_software_version(temp_sessions, version);
        int ret111 =1;

        qDebug()<<"\n ===> "<<" -- "<< IPString2[NowIPIndex]<<" NowIPIndex "<<NowIPIndex;

        QString tempip2= IPString2[NowIPIndex];
        NowIPIndex++;



        if(ret111!=0)
        {
            qDebug()<<"\n Disconnected, we must reconnect again \n";


            //"192.168.1.232";

            int returnaa=0;
            int returnbb =0;
           // qDebug()<<"\N KK "<<IPString2;

//            int returnaa =board_control_try_connect(&temp_sessions, tempip2.toStdString().c_str(), 2000, 2000);

//            int returnbb =board_control_wait_connect(temp_sessions);

            qDebug()<<"\n reconnect result: "<<returnaa<<" \n returnbb:"<<returnbb;

            if(returnbb ==0)
            {
                qDebug()<<"\n connected OK!";
                //==== todo
            }
            else
            {
                qDebug("\n  no this IP \n");
                //==== skip and go
                //return;
                tempip2.clear();

            }

        }
        else if(ret111==0)
        {
            qDebug()<<"\n It was already connected!";
            //==== todo

        }
        else
        {
            qDebug()<<"\n you should never enter here!";
            //return;
            tempip2.clear();

        }

    if(!tempip2.isEmpty())
    {
    // get all you need

        int version = 0;
//        int ret111 = board_control_get_software_version(temp_sessions, version);

    }

    qDebug()<<"\n on_threadFinishedSignal"<< NowIPIndex;

    emit on_threadFinishedSignal(tempip2,NowIPIndex);
}

//====================



//void  Widget::startSearchIPtimer_timeoutSlot()
//{
//    searchHost_closeThreadSlot();
//    startSearchIPtimer->stop();
//    return;
//}

void Widget::on_threadFinishedSignalSlot(QString IPqstring, qint32 IPindex)
{

    qDebug()<<"\n signal received--IP:  "; //<<IPqstring<<" tableViewTotalLineNum "<<tableViewTotalLineNum<<"IPindex"<<IPindex;

//    if(IPindex>=IPMaxNum)
//    {

//        qDebug()<<"\n  NowIPIndex is full "<<IPindex;
//    }


    if(IPqstring.isEmpty())
    {
        qDebug(" IPqstring is empty ");
        return;
    }

//    pModel->setItem(tableViewTotalLineNum, 0,new QStandardItem(IPqstring) );
//    tableViewTotalLineNum++;

}



int32_t IPnum_temp;

void Widget::searchHost_finishedThreadSlot()

{
    if(MychildThread[IPnum_temp]!=Q_NULLPTR)
    {
        delete(MychildThread[IPnum_temp]);
        MychildThread[IPnum_temp]=Q_NULLPTR;
    }
//    if(myObjectThread[IPnum_temp]!=Q_NULLPTR)
//    {
//        delete(myObjectThread[IPnum_temp]);
//        myObjectThread[IPnum_temp]=Q_NULLPTR;
//    }



    qDebug()<<tr("finished triggled")<< IPnum_temp;
    IPnum_temp++;

}



void Widget::searchHost_closeThreadSlot()

{

    IPnum_temp=0;


    qint32 tempii;

    for(tempii=IPMinNum; tempii< IPMaxNum; tempii++)
    {
        qDebug()<<tr("\n close..")<< tempii;

        if(MychildThread[tempii]==Q_NULLPTR)
        {

            qDebug("  MychildThread    Q_NULLPTR ");
            continue;
        }
        else
        {

            qDebug(" MychildThread  not    Q_NULLPTR ");
        }

        if(myObjectThread[tempii]==Q_NULLPTR)
        {

            qDebug("  myObjectThread    Q_NULLPTR ");
            continue;
        }
        else
        {

            qDebug(" myObjectThread  not    Q_NULLPTR ");
        }




        //    if(MychildThread[tempii]->isRunning())
        if(MychildThread[tempii])
        {
            //myObjectThread[tempii]->closeThread();  //关闭线程槽函数

            MychildThread[tempii]->quit();            //退出事件循环

            MychildThread[tempii]->wait();            //释放线程槽函数资源



        }

    }



}






void Widget::on_pushButton_addCmd_clicked()
{
      cmdlistStandarditemModel->appendRow(new QStandardItem(""));
}




void Widget::on_pushButtontest111_clicked()
{

    qDebug()<<tr("test 111 in");

    sshFileCreated("192.168.1.232");


}

QProcess *myQprocess;


#include<QKeyEvent>

/*
return: true success;  false: failed
*/
bool Widget::sshFileCreated(QString destIP)
{

    qDebug()<<tr("sshFileCreated 111 in");

    if(destIP.isEmpty())
    {
        return false;
    }


    QString id_rsa_path= QDir::homePath() +"/.ssh/id_rsa";
    QString id_rsa_pub_path= QDir::homePath() +"/.ssh/id_rsa.pub";

    qDebug()<<tr("id_rsa_path")<< id_rsa_path;
    qDebug()<<tr("id_rsa_pub_path")<< id_rsa_pub_path;

    // http connect
    //ssh gen
    // file teansfer to board
        // run cmd to change auther
    // change files
      //  ok.

    //QDir::tempPath();

    QFile id_rsaFile(id_rsa_path);

    if(id_rsaFile.exists())
    {
        id_rsaFile.remove();
    }

    QFile id_rsa_pubFile(id_rsa_pub_path);

    if(id_rsa_pubFile.exists())
    {
        id_rsa_pubFile.remove();
    }



    myQprocess = new QProcess;
    QString cmdtostart= QString("sss.bat\n");
    myQprocess->start(cmdtostart);

    myQprocess->waitForStarted();

    connect(myQprocess, &QProcess::readyReadStandardOutput, [](){

        qDebug()<< " output normal " << myQprocess->readAllStandardOutput();
    });

    connect(myQprocess, &QProcess::readyReadStandardError, [](){

        qDebug()<< " output error " << myQprocess->readAllStandardError();
    });


}






void Widget::on_pushButtontest222_clicked()
{

    QString id_rsa_pub_path= QDir::homePath() +"/.ssh/id_rsa.pub";
    QFile id_rsa_pubFile(id_rsa_pub_path);

    if(!id_rsa_pubFile.exists())
    {
        qDebug()<< "  exist failed  ";
        return;
    }
    else
    {
        qDebug()<< "  exist OK ";
    }



    createConnection();


    qDebug()<<" IP  is " <<QNetworkInterface::allAddresses()<<" -- "<< QNetworkInterface::allInterfaces()  ;


    //QString strCmd = "wget -P/root/.ssh http://" + getIP()+":/" + id_rsa_pub_path;
      QString strCmd = "wget -P/opt http://" + getIP()+"/" + id_rsa_pub_path;

    QEventLoop myloop;

    QTimer::singleShot(5000,&myloop, &QEventLoop::quit );
    myloop.exec();



    qDebug()<<" strCmd is " <<strCmd;



    // wget -P/tmp/ http://%s/recover/product_002/board_initres
    // wget -P/opt http://192.168.15.72/C:/Users/86158/.ssh/id_rsa.pub\n


    if(strCmd.isEmpty())
    {
        strCmd ="ls -l";
    }
    strCmd += "\n"; //添加回车
    qDebug()<<"send "<< strCmd;
    emit sendCMDsignal(strCmd);





}

void Widget::on_pushButtontest333_clicked()
{




    return;



    qDebug()<<tr("on_pushButtontest333_clicked");
    myQprocess = new QProcess;

    //QString cmdtostart= QString("OpenSSH\\ssh-keygen \n");
QString cmdtostart= QString("sss.bat\n");

    myQprocess->start(cmdtostart);

    //myQprocess->start("C:\\Windows\\WinSxS\\amd64_microsoft-windows-powershell-exe_31bf3856ad364e35_10.0.18362.1_none_3b736eaf7f6b1264\\powershell.exe");
   //myQprocess.setProcessChannelMode(QProcess::ForwardedChannels);
    myQprocess->setProcessChannelMode(QProcess::MergedChannels);

    myQprocess->waitForStarted();

    connect(myQprocess, &QProcess::readyReadStandardOutput, [](){

        qDebug()<< " output normal " << myQprocess->readAllStandardOutput();
    });

    connect(myQprocess, &QProcess::readyReadStandardError, [](){

        qDebug()<< " output error " << myQprocess->readAllStandardError();
    });

}

void Widget::on_pushButtonAdmin_clicked()
{
    qDebug()<<"admin in";
    QString adminStr= ui->lineEditAdmin->text();
    if(adminStr != "tiger")
    {
        return;
    }

    ui->lineEdit_3username->setVisible(true);
    ui->label_3username->setVisible(true);

    ui->lineEdit_4password->setVisible(true);
    ui->label_4password->setVisible(true);


}
