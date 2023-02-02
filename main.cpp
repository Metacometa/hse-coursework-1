#include "QtArchiver.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QtArchiver w;
    //w.setWindowIcon(QIcon(":/QtArchiver/ArchiverIcon.ico"));

    w.show();

    return a.exec();
}
