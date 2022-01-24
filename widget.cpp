#include "widget.h"
#include <QMessageBox>
#include <QLabel>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>

//Создание окна
MainWidget::MainWidget(QWidget *parent) : QWidget(parent)
{
    scene = new plnm;
    button = new QPushButton("ok"); //  кнопка
    QLabel *line1 = new QLabel("X");
    QLabel *line2 = new QLabel("Y");
    // поля ввода
    lineEdits.resize(8);
    for(int i = 0; i < 8; i++){
        lineEdits[i] = new QLineEdit;
    }

    QGridLayout *grid = new QGridLayout(this); // слой, на который все будет добавляться
    grid->addWidget(scene, 0, 0, 3, 5);

    grid->addWidget(line1, 3, 0);

    lineEdits[0]->setText("-1");
    lineEdits[1]->setText("0");
    lineEdits[2]->setText("1");
    lineEdits[3]->setText("2");
    for(int i = 0; i < 4; i++)
        grid->addWidget(lineEdits[i], 3, i+1);

    grid->addWidget(line2, 4, 0);
    lineEdits[4]->setText("1");
    lineEdits[5]->setText("2");
    lineEdits[6]->setText("-2");
    lineEdits[7]->setText("-1");
    for(int i = 0; i < 4; i++)
        grid->addWidget(lineEdits[i+4], 4, i+1);

    grid->addWidget(button, 5, 0);

    // связывание кнопки и слайдеров со слотами */
    QObject::connect(button, &QPushButton::clicked, this, &MainWidget::on_ok_clicked);
}
// отправка параметров
void MainWidget::on_ok_clicked()
{
    QVector<float> params;
    for(int i = 0; i < 8; i++)
        params.push_back(lineEdits[i]->text().toFloat());
    scene->setCoords(params);
}

MainWidget::~MainWidget() {}


