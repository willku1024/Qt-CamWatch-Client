#include "camwatch.h"


CamWatch::CamWatch(QWidget *parent)
    : QMainWindow(parent)
{

    scrollArea = new QScrollArea;
    widget = new QWidget;
    widget->resize(450*3,360*6);
    scrollArea->setWidget(widget);
    this->setCentralWidget(scrollArea);
    this->setMinimumSize(800,550);
    this->statusBar()->show();
    this->setWindowIcon(QIcon(":/bg.png"));
    createMenuBar();
    createToolBar();


    btn = new QPushButton(QString("+"),widget);
    btn->setGeometry(0,0,200,200);
    btn->setFont(QFont("微软雅黑",55,75));
    btn->show();
    count++;

    timer1 = new QTimer;
    connect(timer1,SIGNAL(timeout()),this,SLOT(getCurTime()));
    timer1->start(1000);


    connect(btn,SIGNAL(clicked(bool)),this,SLOT(addVideoSlot(bool)));
}

void CamWatch::createMenuBar()
{


    fileMenu = menuBar()->addMenu("文件(&F)");
    helpMenu = menuBar()->addMenu("帮助(&H)");

    openAction = fileMenu->addAction("新建(&N)");
    openAction->setStatusTip("新建一个摄像头的监控回话");
    fileMenu->addSeparator();
    quitAction = fileMenu->addAction("退出(&Q)");
    quitAction->setStatusTip("退出本程序");
    connect(quitAction,  &QAction::triggered, [&](){
        this->close();
    });

    aboutAction = helpMenu->addAction("关于");
    aboutAction->setStatusTip("关于本软件");
    connect(aboutAction,  &QAction::triggered, [&](){
        QMessageBox::information(this,"关于","版权木有，翻版不究");
    });

    qtAction = helpMenu->addAction("关于Qt");
    qtAction->setStatusTip("本软件基于Qt-GUI框架");
    connect(qtAction,  &QAction::triggered, [&](){
        QMessageBox::information(this,"关于Qt","Based on Qt 5.9.3 (MSVC 2015, 32 bit)\n");
    });


    connect(openAction,SIGNAL(triggered(bool)),this,SLOT(addVideoSlot(bool)));

}

void CamWatch::createToolBar()
{
    toolBar = addToolBar("toolbar");
    toolBar->setMovable(false);
    openAction = toolBar->addAction(QIcon(":/New.png"),"新建");
    openAction->setToolTip("新建一个摄像头的监控回话");
    openAction->setStatusTip("新建一个摄像头的监控回话");
    openAction->setShortcut(QKeySequence("Alt+N"));
    toolBar->addSeparator();

    quitAction = toolBar->addAction(QIcon(":/Exit.png"),"退出");
    quitAction->setToolTip("退出本程序");
    quitAction->setStatusTip("退出本程序");
    connect(quitAction,  &QAction::triggered, [&](){
        this->close();
    });

    spacer = new QWidget;
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    toolBar->addWidget(spacer);

    timeLabel = new QLabel;
    toolBar->addWidget(timeLabel);
    toolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    connect(openAction,SIGNAL(triggered(bool)),this,SLOT(addVideoSlot(bool)));
}

CamWatch::~CamWatch()
{

}



void CamWatch::getCurTime()
{
    QString curTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    this->timeLabel->setFont(QFont("微软雅黑",12,75));
    this->timeLabel->setText(curTime+" ");
}

void CamWatch::addVideoSlot(bool ok)
{

    Q_UNUSED(ok);

    VideoWindow * v = new VideoWindow(widget);

    v->setGeometry(getPosition(count-1).width()*v->width(),
                   getPosition(count-1).height()*v->height(),
                   v->width(),v->height());
    v->show();
    showList.append(v);
    connect(v,SIGNAL(closeWindow(VideoWindow*)),this,SLOT(delVideoSlot(VideoWindow*)));

    btn->setGeometry(getPosition(count).width()*v->width(),
                     getPosition(count).height()*v->height(),
                     v->width(),v->height());
    btn->setFont(QFont("微软雅黑",55,75));
    btn->show();

    count++;


}

void CamWatch::delVideoSlot(VideoWindow * video)
{
    //获取下标
    int index = showList.indexOf(video);
    //移动按钮到 最后一个video的位置
    btn->setGeometry(showList.last()->geometry());
    showList.at(index)->setParent(this);
    //循环移动
    for(int i = showList.size() - 1; i > index; i--)
    {
        //获取位置
        QRect videoRect = showList.at(i-1)->geometry();
        showList.at(i)->setGeometry(videoRect);
    }
    count--;
    showList.removeOne(video);
    delete video;
}

void CamWatch::closeEvent(QCloseEvent *event)
{

    QMessageBox::StandardButton btn;
    if(showList.size()>0)
    {
        btn = QMessageBox::question(this, tr("退出程序"),
                                    QString(tr("退出本程序将关闭所有监控窗口，\n确定要退出么?")),
                                    QMessageBox::Yes | QMessageBox::No,QMessageBox::No);
        for(int i = 0; i<showList.size(); ++i)
        {
            showList.at(i)->setParent(this);
            //delete showList.at(i);
        }
    }else
    {
        btn = QMessageBox::question(this, tr("退出程序"),
                                    QString(tr("是否退出?")),
                                    QMessageBox::Yes | QMessageBox::No,QMessageBox::No);
    }

    if (btn == QMessageBox::Yes)
    {
        event->accept();
    }
    else
    {
        event->ignore();
    }

}

QSize CamWatch::getPosition(int count)
{
    int row = count%(widget->width()/450);// 6/4 = 1...2
    int col = count/(widget->width()/450);

    return QSize(row,col);
}
