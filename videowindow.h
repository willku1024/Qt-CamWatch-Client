#ifndef VIDEOWINDOW_H
#define VIDEOWINDOW_H

#include <QWidget>
#include <QPaintEvent>
#include <QImage>
#include <QPushButton>
#include <QStackedLayout>
#include <QCloseEvent>
#include <QByteArray>
#include <QTcpSocket>
#include <videothread.h>
#include <QMessageBox>
#include <QKeyEvent>
extern "C"
{
#include "avilib.h"
}
class VideoWindow : public QWidget
{
    Q_OBJECT
public:
    explicit VideoWindow(QWidget *parent = nullptr);
    void drawAllBtn();
    void drawBackground();
    void drawCameraPic();
    virtual ~VideoWindow();
protected:
    void paintEvent(QPaintEvent *event);
    void closeEvent(QCloseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);


    QImage paintImg;
    QPushButton *startBtn;
    QPushButton *pauseBtn;
    QPushButton *fullScreenBtn;
    QPushButton *photoBtn;
    QPushButton *videoBtn;
    QPushButton *stopRecordBtn;
    QPushButton *exitBtn;

    QStackedLayout *stackedLay_1;
    QStackedLayout *stackedLay_2;
    QWidget * parentWinPtr = nullptr;
    QRect  winRect;
    VideoThread *th = nullptr;
    QString ipAddr;
    int portNum;
    bool isMonitor = false;
    bool isVideo = false;
    avi_t * avi = NULL;
signals:
    void closeWindow(VideoWindow *);
public slots:
    void fullScreenBtnSlot(bool ok);
    void startBtnSlot();
    void pauseBtnSlot();
    void photoBtnSlot();
    void videoBtnSlot();
    void stopRecordBtnSlot();
    void hasConnectedSlot(bool isConn);
    void hasDataArrivedSlot(QByteArray frame);


};

#endif // VIDEOWINDOW_H
