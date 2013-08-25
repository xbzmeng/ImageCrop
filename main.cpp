#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>
#include <QLocale>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTranslator translator;
    translator.load(":/" + QLocale::system().name());
    a.installTranslator(&translator);
    QTranslator qtTranslator;
    qtTranslator.load(":/qt_" + QLocale::system().name());
    a.installTranslator(&qtTranslator);

    MainWindow w;
    w.show();
    
    return a.exec();
}
