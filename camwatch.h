#ifndef CAMWATCH_H
#define CAMWATCH_H

#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QToolBar>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QWidget>
#include <QCloseEvent>
#include <QLabel>
#include <QTimer>
#include <QMessageBox>
#include <QString>
#include <QDebug>
#include <QStatusBar>
#include <QDateTime>
#include <QPushButton>
#include <QKeySequence>
#include <QSize>
#include <QVector>
#include "videowindow.h"
class CamWatch : public QMainWindow
{
    Q_OBJECT

public:
    CamWatch(QWidget *parent = 0);
    void createMenuBar();
    void createToolBar();
    ~CamWatch();

    QMenu *fileMenu;
    QMenu *helpMenu;

    QAction *openAction ;
    QAction *quitAction ;
    QAction *aboutAction ;
    QAction *qtAction ;

    QToolBar *toolBar;

    QVBoxLayout *v;
    QScrollArea * scrollArea;
    QWidget * widget;
    QWidget *spacer ;
    QLabel *timeLabel;
    QTimer *timer1;
    QPushButton *btn;
    QVector<VideoWindow *> showList;
    int count = 0;



private slots:
    void getCurTime();
    void addVideoSlot(bool ok);
    void delVideoSlot(VideoWindow * video);

protected:
    void closeEvent(QCloseEvent *event);
    QSize getPosition(int count);
};

#endif // CAMWATCH_H
