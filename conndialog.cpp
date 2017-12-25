#include "conndialog.h"
#include <QMessageBox>
#include <QDebug>
ConnDialog::ConnDialog(QWidget *parent)
    :QDialog(parent)
{
    this->setWindowTitle("连接");
    //    this->resize();
    okBtn = new QPushButton("确定");
    cancelBtn = new QPushButton("取消");
    ipLab  = new QLabel("连接服务器地址");
    portLab = new QLabel("连接服务器端口");
    ipLine = new QLineEdit;
    ipLine->setInputMask("000.000.000.000");
    ipLine->setText("127.0.0.1");
    portLine = new QLineEdit;
    portLine->setInputMask("00000");
    portLine->setText("10001");
    okBtn->setFocus();

    gridLay = new QGridLayout;
    gridLay->addWidget(ipLab,0,0);
    gridLay->addWidget(ipLine,0,1);
    gridLay->addWidget(portLab,1,0);
    gridLay->addWidget(portLine,1,1);
    gridLay->addWidget(cancelBtn,2,0);
    gridLay->addWidget(okBtn,2,1);
    gridLay->setColumnStretch(1,1);
    gridLay->setColumnStretch(0,1);


    this->setLayout(gridLay);

    connect(okBtn,SIGNAL(clicked()),this,SLOT(okBtnSlot()));
    connect(cancelBtn,SIGNAL(clicked()),this,SLOT(cancelBtnSlot()));
}

ConnDialog::~ConnDialog()
{

}


void ConnDialog::okBtnSlot()
{

    if(ipLine->text().isEmpty() || portLine->text().isEmpty())
    {
        QMessageBox::information(this, tr("错误"), tr("请填写完整的信息"));
    }else
    {

        QRegExp regExp("\\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.)"
                       "{3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\b");

        //        QRegExp regExp("<([1-9]|[1-9][0-9]|1[0-9]{2}|2[01][0-9]|22[0-3])>"
        //                       "(\\.<([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-4])>)"
        //                       "{2}\\.<([1-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-4])>");
        ipAddr = ipLine->text();
        portNum = portLine->text().toInt();

        if(!regExp.exactMatch(ipAddr) || portNum<0 || portNum >65535)
        {
            QMessageBox::information(this, tr("错误提示"), tr("IP为广播地址或输入不正确！"));
            ipLine->clear();
            portLine->clear();
            return ;
        }
        this->isAccept = true;
        this->close();
    }

}

void ConnDialog::cancelBtnSlot()
{

    this->isAccept = false;
    this->close();
}
