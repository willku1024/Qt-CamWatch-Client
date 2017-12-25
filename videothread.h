#ifndef VIDEOTHREAD_H
#define VIDEOTHREAD_H

#include <QThread>
#include <QImage>
#include <QTcpSocket>

class VideoThread : public QThread
{
    Q_OBJECT
public:
    QString ip;
    int port;
    int isPause = false;
    VideoThread(QString ip, int port,QObject *ptr = nullptr);
    void run();
signals:
    void hasConnected(bool isConn);
    void hasDataArrived(QByteArray frame);

};

#endif // VIDEOTHREAD_H
