#include "simpleqt.h"
#include <QtGui/QApplication>
#include <QTimer>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("Aptina Imaging");
    QCoreApplication::setOrganizationDomain("aptina.com");
    QCoreApplication::setApplicationName("SimpleQt");
    SimpleQt w;
    w.show();
    QTimer::singleShot(10, &w, SLOT(onFirstStartup()));
    return a.exec();
}
