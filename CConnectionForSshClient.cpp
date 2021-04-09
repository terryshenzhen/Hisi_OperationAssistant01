#include "CConnectionForSshClient.h"
#include <QDebug>


CConnectionForSshClient::CConnectionForSshClient(QString strIp, int nPort, QString strPwd, QString strUser)
{
    m_strIp = strIp;
    m_nPort = nPort;
    m_strUser = strUser;
    m_strPwd = strPwd;
    m_strIpPort = m_strIp + ":" + QString::number(m_nPort);
}

void CConnectionForSshClient::init()
{
    m_pThread = new QThread();
    connect(m_pThread,SIGNAL(finished()),this,SLOT(slotThreadFinished()));
    this->moveToThread(m_pThread);
    m_pThread->start();

    //֮����߼�����ͨ���źźͲ۽�ͨ
    connect(this,SIGNAL(sigInitForClild()),this,SLOT(slotInitForClild()));
    emit sigInitForClild();

    reconnectTimes=0;
}

void CConnectionForSshClient::unInit()
{
    m_pThread->quit();
}



CConnectionForSshClient::~CConnectionForSshClient()
{
    if(nullptr != m_pSshSocket){
        delete m_pSshSocket;
        m_pSshSocket = nullptr;
    }
}

void CConnectionForSshClient::slotResetConnection(QString strIpPort)
{
    if(this->getIpPort() == strIpPort){
        this->slotDisconnected();
    }
}




void CConnectionForSshClient::slotInitForClild()
{
    m_argParameters.host = m_strIp;
    m_argParameters.port = m_nPort;
    m_argParameters.userName = m_strUser;
    m_argParameters.password = m_strPwd;
    m_argParameters.authenticationType = QSsh::SshConnectionParameters::AuthenticationTypePassword; //���뷽ʽ����

    m_argParameters.timeout = 10;

    slotCreateConnection(); //����

    m_pTimer = new QTimer(this);
    m_pTimer->setInterval(RECONNET_SPAN_TIME);
    connect(m_pTimer,SIGNAL(timeout()),this,SLOT(slotCreateConnection()));
    m_pTimer->start();//����������ʱ����ÿ��һ��ʱ�����slotCreateConnection�ж��Ƿ���Ҫ����
    reconnectTimes=0;

}

void CConnectionForSshClient::slotCreateConnection()
{

    qDebug()<<"CConnectionForSshClient::slotCreateConnection Check the connection" ;

    if(true == m_bConnected)
        return;

    if(nullptr == m_pSshSocket){
        m_pSshSocket = new QSsh::SshConnection(m_argParameters);
        connect(m_pSshSocket,SIGNAL(connected()),SLOT(slotConnected()));
        connect(m_pSshSocket,SIGNAL(error(QSsh::SshError)),SLOT(slotSshConnectError(QSsh::SshError)));
    }


    m_pSshSocket->connectToHost();


    qDebug()<<QString::fromLocal8Bit("CConnectionForSshClient::��ssh��ʽ try to connect times:")<< reconnectTimes;//getIpPort();
}

void CConnectionForSshClient::slotConnected()
{
    qDebug()<<"CConnectionForSshClient::slotConnected ssh has been connected to:"<<getIpPort();
    m_pTimer->stop();

    m_shell = m_pSshSocket->createRemoteShell();
    connect(m_shell.data(), SIGNAL(started()), SLOT(slotShellStart()));
    connect(m_shell.data(), SIGNAL(readyReadStandardOutput()), SLOT(slotDataReceived()));
    connect(m_shell.data(), SIGNAL(readyReadStandardError()), SLOT(slotShellError()));
    m_shell.data()->start();

    m_bConnected = true;
    emit sigConnectStateChanged(m_bConnected,m_strIp,m_nPort);
}

void CConnectionForSshClient::slotDisconnected()
{
    m_pSshSocket->disconnectFromHost();
}

void CConnectionForSshClient::slotThreadFinished()
{
    m_pThread->deleteLater();
    this->deleteLater();
}

void CConnectionForSshClient::slotSshConnectError(QSsh::SshError sshError)
{
    m_bSendAble = false;
    m_bConnected = false;
    emit sigConnectStateChanged(m_bConnected,m_strIp,m_nPort);

    reconnectTimes++;
    if(reconnectTimes>=3){
        m_pTimer->stop();
        reconnectTimes=0;
        return;
    }

    m_pTimer->start();

    switch(sshError){
    case QSsh::SshNoError:
        qDebug()<<"slotSshConnectError SshNoError"<<getIpPort();
        break;
    case QSsh::SshSocketError:
        qDebug()<<"slotSshConnectError SshSocketError"<<getIpPort(); //�ε����������ִ���
        break;
    case QSsh::SshTimeoutError:
        qDebug()<<"slotSshConnectError SshTimeoutError"<<getIpPort();
        break;
    case QSsh::SshProtocolError:
        qDebug()<<"slotSshConnectError SshProtocolError"<<getIpPort();
        break;
    case QSsh::SshHostKeyError:
        qDebug()<<"slotSshConnectError SshHostKeyError"<<getIpPort();
        break;
    case QSsh::SshKeyFileError:
        qDebug()<<"slotSshConnectError SshKeyFileError"<<getIpPort();
        break;
    case QSsh::SshAuthenticationError:
        qDebug()<<"slotSshConnectError SshAuthenticationError"<<getIpPort();
        break;
    case QSsh::SshClosedByServerError:
        qDebug()<<"slotSshConnectError SshClosedByServerError"<<getIpPort();
        break;
    case QSsh::SshInternalError:
        qDebug()<<"slotSshConnectError SshInternalError"<<getIpPort();
        break;
    default:
        break;
    }

}

void CConnectionForSshClient::slotShellStart()
{
    m_bSendAble = true;
    qDebug()<<"CConnectionForSshClient::slotShellStart Shell������:"<<getIpPort();
}

void CConnectionForSshClient::slotShellError()
{
    qDebug()<<"CConnectionForSshClient::slotShellError Shell��������:"<<getIpPort();
}






void CConnectionForSshClient::slotSendByQByteArray(QString strIpPort, QByteArray arrMsg)
{
    if(0 != m_strIpPort.compare(strIpPort)){
        return;
    }

    if(m_bConnected){
       m_shell->write(arrMsg);
    }else{
       qDebug()<<"CConnectionForSshClient::send(QString strMessage) ����ʧ�� δ��������:"<<getIpPort();
    }
}

int CConnectionForSshClient::send(QString strMessage)
{
    qDebug()<<"CConnectionForSshClient ssh send "<<strMessage;

    int nSize = 0;
    if(m_bConnected && m_bSendAble){

       nSize = m_shell->write(strMessage.toLatin1().data());

    }else{
       qDebug()<<"CConnectionForSshClient::send() sshδ���� �� shellδ����:"<<getIpPort();
    }

    return nSize;
}


void CConnectionForSshClient::slotSend(QString strIpPort, QString strMessage)
{
    if(0 != m_strIpPort.compare(strIpPort)){
        return;
    }

    send(strMessage);
}

void CConnectionForSshClient::slotSend(QString strMessage)
{
    send(strMessage);
}

void CConnectionForSshClient::slotDataReceived()
{
    QByteArray byteRecv = m_shell->readAllStandardOutput();
    QString strRecv = QString::fromUtf8(byteRecv);

//    if(strRecv.contains("password for")){
//        m_shell->write(m_strPwd.toLatin1().data());
//    }

    if(!strRecv.isEmpty()) //���˿���
        emit sigDataArrived(strRecv, m_strIp, m_nPort);

}
