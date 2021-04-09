
#include "widget.h"

#include <QApplication>



// JQLibrary import
#include "JQHttpServer.h"
#include<QDebug>






int main(int argc, char *argv[])
{
    QApplication a(argc, argv);



#if 1
    JQHttpServer::TcpServerManage tcpServerManage( 2 ); // ����������߳�����Ĭ��2��

    tcpServerManage.setHttpAcceptedCallback( []( const QPointer< JQHttpServer::Session > &session )
    {
        // �ص��������µ��߳��ڣ��������̣߳���ע���̰߳�ȫ
        // �������˴˻ص�����ô�µ����ӽ�����õ�����Ĭ���������2���߳̿�������2�Σ���3�����ӽ����ᱻ����

        QString ssss=QString( "url %1  body %2" ).arg( session->requestUrl(), QString( session->requestBody() ) ) ;
        qDebug()<<"\n   --retutn--  \n"<<ssss;


        QString Needfile=QString( "%1" ).arg( session->requestUrl() ) ;
        Needfile= Needfile.remove(0,1);
        Needfile=QDir::toNativeSeparators(Needfile);
        qDebug()<<"\n   --Needfile "<<Needfile;

        session->replyFile( Needfile );



/*
on Linux side, the format must be:

wget -P/tmp http://192.168.15.72:80/bbb
bbb was put with exe together
*/


//        session->replyRedirects( QUrl( "http://www.baidu.com" ) );
//        session->replyJsonObject( { { { "message", "ok" } } } );
//        session->replyJsonArray( { "a", "b", "c" } );
//        session->replyFile( "/Users/jason/Desktop/Test1.Test2" );
//        session->replyImage( QImage( "/Users/jason/Desktop/Test.png" ) );
//        session->replyBytes(QByteArray(4,'\x24')); // $$$$

        // ע1����Ϊһ��session��Ӧһ����һ��HTTP��������sessionֻ��replyһ��
        // ע2����reply��session���������ڲ��ɿأ�����reply��Ҫ�ٵ���session�Ľӿ���
    } );

    qDebug() << "listen:" << tcpServerManage.listen( QHostAddress::Any, 80 );

#endif


    Widget w;
    w.show();
    return a.exec();
}
