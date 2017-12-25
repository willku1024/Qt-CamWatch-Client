#ifndef CONNDIALOG_H
#define CONNDIALOG_H

#include <QDialog>
#include <QGridLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
class ConnDialog : public QDialog
{
    Q_OBJECT
public:
    ConnDialog(QWidget *parent = nullptr);
    virtual ~ ConnDialog();
    bool isAccept = false;
    QString ipAddr;
    int portNum;
private:
    QGridLayout *gridLay;
    QPushButton *okBtn;
    QPushButton *cancelBtn;
    QLabel *ipLab;
    QLabel *portLab;
    QLineEdit *ipLine;
    QLineEdit *portLine;



private slots:
    void okBtnSlot();
    void cancelBtnSlot();
};

#endif // CONNDIALOG_H
