#include "widget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWidget w;
    w.setWindowTitle("Интерполяционный многочлен Лагранжа по 4 точкам");
    w.show();
    return a.exec();
}
