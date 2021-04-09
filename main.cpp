
#include "widget.h"

#include <QApplication>



// JQLibrary import
#include "JQHttpServer.h"
#include<QDebug>






int main(int argc, char *argv[])
{
    QApplication a(argc, argv);



#if 1
    JQHttpServer::TcpServerManage tcpServerManage( 2 ); // 设置最大处理线程数，默认2个

    tcpServerManage.setHttpAcceptedCallback( []( const QPointer< JQHttpServer::Session > &session )
    {
        // 回调发生在新的线程内，不是主线程，请注意线程安全
        // 若阻塞了此回调，那么新的连接将不会得到处理（默认情况下有2个线程可以阻塞2次，第3个连接将不会被处理）

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

        // 注1：因为一个session对应一个单一的HTTP请求，所以session只能reply一次
        // 注2：在reply后，session的生命周期不可控，所以reply后不要再调用session的接口了
    } );

    qDebug() << "listen:" << tcpServerManage.listen( QHostAddress::Any, 80 );

#endif


    Widget w;
    w.show();
    return a.exec();
}
