#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QTimer>
#include "plnm.h"

// для создания окна
class MainWidget : public QWidget
{
    Q_OBJECT
public:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();
public slots: // слоты
    void on_ok_clicked();
private:
    QPushButton *button; // кнопка
    QVector<QLineEdit*> lineEdits; // поля ввода
    plnm *scene;
};

#endif // WIDGET_H
