#include "graphwidget.h"

#include <QApplication>
#include <QWidget>
#include <QTextCodec>

int main(int argc, char* argv[]){
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("Windows-1251"));

    QApplication a(argc, argv);

    GraphWidget g;
    g.show();
    return a.exec();
}
