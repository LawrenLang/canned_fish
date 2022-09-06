#include "mainWindow.h"
#include <QApplication>
#include <QTextCodec>
#include <QTranslator>
#include <QDesktopWidget>

int main(int argc, char *argv[])
{
//    QTextCodec *codec = QTextCodec::codecForName("UTF-8");//或者"GBK",不分大小写
//    QTextCodec::setCodecForLocale(codec);

    QApplication a(argc, argv);


    MainWindow w;
    w.setWindowTitle("鱼罐头");
    w.setFixedSize(637, 337);


    w.show();
    return a.exec();
}
