#include "camwatch.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CamWatch w;
    w.show();

    return a.exec();
}
