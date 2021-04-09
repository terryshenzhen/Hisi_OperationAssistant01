#ifndef CCONNECTIONFORSSHCLIENT_H
#define CCONNECTIONFORSSHCLIENT_H

/* Func:���û��������ʽ����ssh������  �������shellִ��  ���\n
 * Note:��ʱ�������״̬���߼���  ���ӳɹ��رն�ʱ�����
 *      ���ӶϿ�  ������ʱ����鲢��������  ֱ�����ӳɹ�
 *      ���رն�ʱ�����
 * Use:�������ź�
 *      ���״̬:sigConnectStateChanged(bState,strIp,nPort);
 *      ������Ϣ:sigDataArrived(QString strMsg,QString strIp, int nPort);
 *     ��һ����
 *      ������Ϣ:void slotSend(QString strMsg);
 */
#include <sshconnection.h>
#include <sshremoteprocess.h>
#include <sftpchannel.h>
#include <QTimer>
#include <QHostAddress>
#include <QThread>


#define RECONNET_SPAN_TIME (1000*10)  //����״̬����


//class CDefaultNotifierSSH : public INetworkInterface{
//    void slotDataArrived(QByteArray arrMessage,QString strIp, int nPort){
//        Q_UNUSED(arrMessage);
//        Q_UNUSED(strIp);
//        Q_UNUSED(nPort);
//    }
//    void slotConnectStateChanged(bool bState,QString strIp,int nPort){
//        Q_UNUSED(bState);
//        Q_UNUSED(strIp);
//        Q_UNUSED(nPort);
//    }
//};

class  CConnectionForSshClient : public QObject
{
    Q_OBJECT
public:
    explicit CConnectionForSshClient(QString strIp, int nPort = 22,QString strPwd = "qq554869603",QString strUser = "root");

    void init();
    void unInit();

    ~CConnectionForSshClient();
    int32_t reconnectTimes;
private:
    QThread *m_pThread = nullptr;
    bool m_bConnected = false;
    bool m_bIsFirstConnect = true;
    bool m_bSendAble = false;

    QTimer *m_pTimer;

    QString m_strIp = "";
    int m_nPort = -1;
    QString m_strUser;
    QString m_strPwd;
    QString m_strIpPort;

    QSsh::SshConnectionParameters m_argParameters;
    QSsh::SshConnection *m_pSshSocket = nullptr;
    QSharedPointer<QSsh::SshRemoteProcess> m_shell;
signals:
    void sigInitForClild();
    void sigConnectStateChanged(bool bState,QString strIp,int nPort);
    void sigDataArrived(QString strMsg,QString strIp, int nPort);
private:
    int send(QString strMessage);
    QString getIpPort(){return m_strIp + ":" + QString::number(m_nPort);}
public slots:
    void slotResetConnection(QString strIpPort);
    void slotSend(QString strIpPort,QString strMessage);
    void slotSend(QString strMsg);
    void slotSendByQByteArray(QString strIpPort,QByteArray arrMsg);
    void slotDisconnected();
    void slotDataReceived();
private slots:
    void slotInitForClild();
    void slotCreateConnection();
    void slotConnected();

    void slotThreadFinished();

    void slotSshConnectError(QSsh::SshError sshError);
    void slotShellStart();
    void slotShellError();
};

#endif // CCONNECTIONFORSSHCLIENT_H
