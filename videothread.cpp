#include "videothread.h"
#include "videowindow.h"
#include <QDebug>
VideoThread::VideoThread(QString ip, int port, QObject *ptr)
    :QThread(ptr)
{
    this->ip = ip;
    this->port = port;
}

void VideoThread::run()
{
    QByteArray dataStream;
    QByteArray frame;
    QTcpSocket socket;
    socket.connectToHost(this->ip,this->port);
    if(socket.waitForConnected(3000))
    {
        qDebug()<<"conn success";
        emit hasConnected(true);
    }else
    {
        qDebug()<<"conn failed";
        emit hasConnected(false);
        return ;
    }

    QByteArray req;
    req.append("?action=stream\n");
    socket.write(req);
    if(socket.waitForBytesWritten(3000))
    {
        qDebug()<<"write success";
    }else
    {
        qDebug()<<"write failed";
        return ;
    }
    while (!dataStream.contains("\r\n\r\n"))
    {
        if(socket.waitForReadyRead())
        {
            dataStream.append(socket.readAll());
        }
    }
    dataStream.remove(0,dataStream.indexOf("\r\n\r\n")+4);

    while (1) {
        //帧头+图片
        if(isPause)
        {
            msleep(618);//睡眠黄金比例1000*0.618ms
//            socket.readAll();
            continue ;
        }
        while (!dataStream.contains("\r\n--")) {
            if(socket.waitForReadyRead())
            {
                dataStream.append(socket.readAll());
            }
        }
        dataStream.remove(0,dataStream.indexOf("\n\n")+2);
        frame = dataStream.mid(0,dataStream.indexOf("\r\n--"));

        emit hasDataArrived(frame);
        dataStream.remove(0,frame.size()+4);

    }





}
