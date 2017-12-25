#include "videowindow.h"
#include <QLinearGradient>
#include <QPainter>
#include <QImage>
#include <QDebug>
#include <QApplication>
#include <QDesktopWidget>
#include <QRect>
#include <QDateTime>
#include "conndialog.h"
#include <QBuffer>
VideoWindow::VideoWindow(QWidget *parent) : QWidget(parent)
{
    this->resize(450,360);
    this->setMinimumSize(450,360);
    startBtn = new QPushButton(this);
    startBtn->setIcon(QIcon(":/Start.png"));
    startBtn->setIconSize(QSize(48,48));
    startBtn->setToolTip("开始");
    startBtn->setStyleSheet("border:0px;");


    pauseBtn = new QPushButton(this);
    pauseBtn->setIcon(QIcon(":/Pause.png"));
    pauseBtn->setIconSize(QSize(48,48));
    pauseBtn->setToolTip("暂停");
    pauseBtn->setStyleSheet("border:0px;");


    fullScreenBtn = new QPushButton(this);
    fullScreenBtn->setIcon(QIcon(":/FullScreen.png"));
    fullScreenBtn->setIconSize(QSize(48,48));
    fullScreenBtn->setToolTip("全屏");
    fullScreenBtn->setStyleSheet("border:0px");


    photoBtn = new QPushButton(this);
    photoBtn->setIcon(QIcon(":/Photo.png"));
    photoBtn->setIconSize(QSize(48,48));
    photoBtn->setToolTip("截图");
    photoBtn->setEnabled(false);
    photoBtn->setStyleSheet("border:0px;");


    videoBtn= new QPushButton(this);
    videoBtn->setIcon(QIcon(":/Video.png"));
    videoBtn->setIconSize(QSize(48,48));
    videoBtn->setEnabled(false);
    videoBtn->setStyleSheet("border:0px;");


    stopRecordBtn = new QPushButton(this);
    stopRecordBtn->setIcon(QIcon(":/StopRecord.png"));
    stopRecordBtn->setIconSize(QSize(48,48));
    stopRecordBtn->setStyleSheet("border:0px;");


    exitBtn = new QPushButton(this);
    exitBtn->setIcon(QIcon(":/Exit.png"));
    exitBtn->setIconSize(QSize(48,48));
    exitBtn->setToolTip("退出");
    exitBtn->setStyleSheet("border:0px;");



    //QStackedLayout
    stackedLay_1 = new QStackedLayout;
    stackedLay_1->addWidget(startBtn);
    stackedLay_1->addWidget(pauseBtn);

    stackedLay_2 = new QStackedLayout;
    stackedLay_2->addWidget(videoBtn);
    stackedLay_2->addWidget(stopRecordBtn);

    drawAllBtn();
    //    drawBackground();

    connect(fullScreenBtn,SIGNAL(clicked(bool)),this,SLOT(fullScreenBtnSlot(bool)));
    connect(startBtn,SIGNAL(clicked()),this,SLOT(startBtnSlot()));
    connect(pauseBtn,SIGNAL(clicked()),this,SLOT(pauseBtnSlot()));
    connect(photoBtn,SIGNAL(clicked()),this,SLOT(photoBtnSlot()));
    connect(exitBtn,&QPushButton::clicked,[this](){
        fullScreenBtnSlot(true);
        emit closeWindow(this);
    });

}

void VideoWindow::drawAllBtn()
{

    startBtn->move(width()/8*7-24,height()/8*1-24);
    pauseBtn->move(width()/8*7-24,height()/8*1-24);
    fullScreenBtn->move(width()/8*7-24,height()/8*2-24);
    photoBtn->move(width()/8*7-24,height()/8*3-24);
    videoBtn->move(width()/8*7-24,height()/8*4-24);
    stopRecordBtn->move(width()/8*7-24,height()/8*4-24);
    exitBtn->move(width()/8*7-24,height()/8*5-24);


}

void VideoWindow::drawBackground()
{
    paintImg = QImage(width(),height(),QImage::Format_ARGB32);
    QPainter p(&paintImg);

    p.setRenderHints(QPainter::Antialiasing |
                     QPainter::SmoothPixmapTransform);
    QLinearGradient linear(0,0,width(),height());
    p.setPen(Qt::NoPen);
    linear.setColorAt(0,QColor(Qt::white));
    linear.setColorAt(0.5,QColor(Qt::green));
    linear.setColorAt(1,QColor(Qt::blue));
    p.setBrush(QBrush(linear));
    p.drawRect(this->rect());
    QImage background(":/bg.png");
    p.drawImage(width()/2-background.width()/2,height()/2-background.height()/2,background);


}

void VideoWindow::drawCameraPic()
{

}

void VideoWindow::paintEvent(QPaintEvent *event)
{


    Q_UNUSED(event)
    QPainter p(this);
    if(!isMonitor)
    {
        drawBackground();
    }

    p.drawImage(QRect(0,0,width(),height()),paintImg);
    drawAllBtn();


}

void VideoWindow::closeEvent(QCloseEvent *event)
{
    fullScreenBtnSlot(true);
    event->ignore();
}



VideoWindow::~VideoWindow()
{
    delete startBtn;
    delete pauseBtn;
    delete fullScreenBtn;
    delete photoBtn;
    delete videoBtn;
    delete stopRecordBtn;
    delete exitBtn;
    delete stackedLay_1;
    delete stackedLay_2;


}

void VideoWindow::fullScreenBtnSlot(bool ok)
{
    if(!ok)
    {
        fullScreenBtn->setCheckable(true);
        fullScreenBtn->setToolTip("最小化");
        parentWinPtr = this->parentWidget();
        this->winRect = this->geometry();
        this->setParent(NULL);
        // this->move(QApplication::desktop()->width()/2-this->width()/2,
        //            QApplication::desktop()->height()/2-this->height()/2);
        this->showFullScreen();

    }else
    {
        fullScreenBtn->setCheckable(false);
        fullScreenBtn->setToolTip("全屏");
        this->setParent(parentWinPtr);
        this->setGeometry(this->winRect);
        this->showNormal();
    }
}

void VideoWindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Escape:
        fullScreenBtn->setCheckable(false);
        this->setParent(parentWinPtr);
        this->setGeometry(this->winRect);
        this->showNormal();
        break;
    default:
        break;
    }
}

void VideoWindow::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        if(fullScreenBtn->isCheckable())
        {
            this->showNormal();
            this->move(QApplication::desktop()->width()/2-this->width()/2,
                       QApplication::desktop()->height()/2-this->height()/2);

        }

    }
}
void VideoWindow::startBtnSlot()
{
    if(th == nullptr)
    {
        ConnDialog w;
        w.exec();

        if(w.isAccept)
        {
            this->portNum = w.portNum;
            this->ipAddr = w.ipAddr;
            th = new VideoThread(w.ipAddr,w.portNum);
            th->start();
            connect(th,SIGNAL(hasConnected(bool)),this,SLOT(hasConnectedSlot(bool)));
        }

    }else
    {
        this->stackedLay_1->setCurrentIndex(1);
        th->isPause = false;
    }



}

void VideoWindow::pauseBtnSlot()
{
    th->isPause = true;
    this->stackedLay_1->setCurrentIndex(0);
}

void VideoWindow::photoBtnSlot()
{
    QString fileName("%1.jpg");
    fileName = fileName.arg(QDateTime::currentDateTime().toString("p-yyyyMMddhhmmsszzz"));
    this->paintImg.save(fileName,"JPEG");
    QMessageBox::information(this,"提示","拍照成功！",QMessageBox::Ok);
}

void VideoWindow::videoBtnSlot()
{

    stackedLay_2->setCurrentIndex(1);
    if(avi == NULL)
    {

        QString fileName("%1.avi");
        fileName = fileName.arg(QDateTime::currentDateTime().toString("v-yyyyMMddhhmmsszzz"));
        if(NULL == (avi=AVI_open_output_file(fileName.toUtf8().data())))
        {
            QMessageBox::information(this,"提示","录像文件打开失败");
            stackedLay_2->setCurrentIndex(0);
            return ;
        }
        AVI_set_video(avi,450,360,9,QString("MJPG").toUtf8().data());
        //AVI_set_video(avi,450,360,9,"MJPG");
        this->isVideo = true;
    }

}

void VideoWindow::stopRecordBtnSlot()
{
    stackedLay_2->setCurrentIndex(0);
    this->isVideo = false;
    if(avi)
    {
        AVI_close(avi);
        avi = NULL;
        QMessageBox::information(this,"提示","录像完成");

    }
}

void VideoWindow::hasConnectedSlot(bool isConn)
{
    if(isConn)
    {
        this->stackedLay_1->setCurrentIndex(1);
        this->photoBtn->setEnabled(true);
        this->videoBtn->setEnabled(true);
        QMessageBox::information(this,"提示","连接成功");
        this->isMonitor = true;
        connect(th,SIGNAL(hasDataArrived(QByteArray)),this,SLOT(hasDataArrivedSlot(QByteArray)));
        connect(videoBtn,SIGNAL(clicked()),this,SLOT(videoBtnSlot()));
        connect(stopRecordBtn,SIGNAL(clicked()),this,SLOT(stopRecordBtnSlot()));
    }else
    {
        QMessageBox::information(this,"提示","连接失败");
    }

}

void VideoWindow::hasDataArrivedSlot(QByteArray frame)
{
    paintImg.loadFromData(frame,"JPEG");

    QPainter p(&paintImg);
    p.setPen(QColor(Qt::red));
    p.setFont(QFont("宋体",13,75));
    p.drawText(6,20,this->ipAddr+":"+QString::number(this->portNum));
    p.drawText(6,35,QDateTime::currentDateTime().toString("yy/MM/dd hh:mm:ss"));

    this->update();

    QBuffer buffer;
    buffer.open(QIODevice::ReadWrite);

    //pixmap不能为空，必须先将图片加载到pixmap中
    paintImg.save(&buffer,"JPEG");
    QByteArray pixArray;
    pixArray.append(buffer.data());

    if(this->isVideo)
    {
        //if(AVI_write_frame(this->avi,frame.data(),frame.size(),0)<0)
        if(AVI_write_frame(this->avi,pixArray.data(),pixArray.size(),0)<0)
        {
            QMessageBox::information(this,"提示","录像失败");
            stopRecordBtnSlot();
        }
    }


}




