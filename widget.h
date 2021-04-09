#ifndef WIDGET_H
#define WIDGET_H


#include <QWidget>
#include <QTextEdit>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include "CConnectionForSshClient.h"

#include<QDir>
#include <QDebug>
#include <QGridLayout>
#include<QNetworkInterface>
#include<QHostAddress>
#include<QAction>

// JQLibrary import
#include "JQHttpServer.h"

#include<QStandardItemModel>
#include<QStandardItem>


#include<QThread>



QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

#define IPMinNum 220
//220
#define IPMaxNum 223
//225



class MyThreadcmdHost : public QObject

{
    Q_OBJECT

public:

    explicit MyThreadcmdHost(QObject *parent = nullptr);

signals:
    void on_threadFinishedSignal(QString IPqstring,qint32 IPindex);

public slots:
    void startThreadSlot();



private:

    ////volatile bool isStop;

};




class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();



    QString getIP();

    void iPlistQListwidgetInit();

    void cmdlistQListviewInit();
    QStandardItemModel *cmdlistStandarditemModel;
    void on_Sendcmd2IP();

    //
    bool sshFileCreated(QString destIP);
private:
    void createConnection();
    void createConnection(QString IP);
    void disConnection();
    void initUI();
    void setConnectState(bool bState);
    void httpListen();


    //========================IPlist-- qlistwidget table
QStandardItemModel *pIPlistStandarditemModel;
QAction *RemoveCmditemAction;


private slots:

    void slotConnectStateChanged(bool bState,QString strIp,int nPort);
    void slotDataArrived(QString strMsg,QString strIp, int nPort);

    void on_pushButtonConnect_clicked();

    void on_pushButton_2send_clicked();

    void on_pushButton_2CleanOutput_clicked();

    void on_pushButton_SearchIP_clicked();

    void on_pushButton_allexe_clicked();

    void on_pushButton_addCmd_clicked();


void on_threadFinishedSignalSlot(QString IPqstring, qint32 IPindex);
void searchHost_finishedThreadSlot();
void searchHost_closeThreadSlot();

void on_pushButtontest111_clicked();

void on_pushButtontest222_clicked();

void on_pushButtontest333_clicked();

void on_pushButtonAdmin_clicked();

signals:
    void sendCMDsignal(QString strMsg);
    void sigDisconnected();
private:

    bool m_bConnectState;



private:
    Ui::Widget *ui;

    QThread *MychildThread[255];
    MyThreadcmdHost *myObjectThread[255];



};









#endif // WIDGET_H
