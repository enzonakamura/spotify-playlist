#include "mainwindow.h"

#include <QApplication>
#include <QStyle>
#include <QDesktopWidget>
#include <QGuiApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setGeometry(
        QStyle::alignedRect(
            Qt::LeftToRight,
            Qt::AlignCenter,
            w.size(),
            QGuiApplication::screens()[0]->availableGeometry()
        )
    );
    ;
    w.show();
    return a.exec();
}
