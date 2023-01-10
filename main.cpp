#include "QtArchiver.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QtArchiver w;
    w.show();
    return a.exec();
}
