// Иванова Елена
// 301 группа
// ЛР7: вариант 13
// Интерполяционный многочлен Лагранжа по четырем точкам.
#ifndef PLNM_H
#define PLNM_H

#include <QGLWidget>
#include <QGLShaderProgram>
#include <QMatrix4x4>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLExtraFunctions>
#include <QtMath>
#include <QTimer>
#include <QPointF>

class plnm: public QGLWidget, protected QOpenGLExtraFunctions
{
    Q_OBJECT
private:
    const int vertexLocationInShader_ = 0;
    GLuint VAO_[5], VBO_[5]; // массивы для вершин, их буферов и нормалей
    QGLShaderProgram shaderProgram_; // шейдерная программа
    float x_[4], y_[4]; // координаты 4 точек
    float scale_; // для регулирования масштаба
    float GetF(float x); // построение интерполяционного многочлена
protected:
    void initializeGL(); // инициализация
    void resizeGL(int w, int h); // изменение размара
    void paintGL(); // отрисовка
public:
    plnm(QWidget *prt = 0);
    void setCoords(QVector<float> coords); // изменение координат 4 точек
    ~plnm();
};

#endif // PLNM_H
