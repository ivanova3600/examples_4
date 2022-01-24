#include "plnm.h"

int k = 0;
//Шейдеры
const char *vsrc = "#version 330 core\n" // вершинный шейдер
                   "layout (location = 0) in vec3 aPos;\n"
                   "uniform mat4 model;\n"
                   "uniform vec3 color;\n"
                   "out vec4 vertexColor;\n"
                   "void main()\n"
                   "{\n"
                   "gl_Position = model * vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                   "vertexColor = vec4(color.x, color.y, color.z, 1.0);\n"
                   "}\0";
const char *fsrc = "#version 330 core\n" // фрагментный шейдер
                   "out vec4 FragColor;\n"
                   "in vec4 vertexColor;\n"
                   "void main()\n"
                   "{\n"
                   "FragColor = vertexColor;\n"
                   "}\n\0";
// конструктор
plnm::plnm(QWidget *prt) : QGLWidget(prt), scale_(0.1)
{
    QGLFormat frmt; // создать формат по умолчанию
    frmt.setDoubleBuffer(false); // задать простую буферизацию
    setFormat(frmt);
    // установка таймера
    QTimer *timer = new QTimer(this);
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(10);
// начальные значения точек
    x_[0] = -1; x_[1] = 0; x_[2] = 1; x_[3] = 2;
    y_[0] = 0; y_[1] = 2; y_[2] = -2; y_[3] = -1;
}
// интер. многочлен лагранжа
float plnm::GetF(float x)
{
    float res = 0;
    float s1, s2;
    for (int i = 0; i < 4; i++){
        s1 = 1; s2 = 1;
        for (int j = 0; j < 4; j++){
            if (i != j){
                s1 = s1 * (x - x_[j]); // числитель
                s2 = s2 * (x_[i] - x_[j]); // знаменатель
            }
        }
        res = res + y_[i] * (s1 / s2); // прибавляем к итоговому результату
     }
    return res;
}

void plnm::initializeGL()
{
    initializeOpenGLFunctions();
    // точки кривой
    int kk = 0;
    float points[20000*3];
    for(float i = -10; i <= 10; i+=0.001){
        points[kk] = i; kk++;
        points[kk] = GetF(i); kk++;
        points[kk] = 0.0; kk++;
    }
    // координатные оси
    float x_line[] = {-10, 0, 0, 10, 0, 0};
    float y_line[] = {0, -10, 0, 0, 10, 0};
    //стрелки
    float arrows[] = {
        9.7f, 0.3f, 0.0f, 10.f, 0.f, 0.f, 9.7f, -0.3f, 0.f,
        -0.3f, 9.7f, 0.f, 0.f, 10.f, 0.f, 0.3f, 9.7f, 0.f
    };
    // 4 точки
    float x[12] = {-1,0,0, 0,2,0, 1,-2,0, 2,-1,0};

    shaderProgram_.addShaderFromSourceCode(QGLShader::Vertex, vsrc);
    shaderProgram_.addShaderFromSourceCode(QGLShader::Fragment, fsrc);

    shaderProgram_.bindAttributeLocation("inPosition", vertexLocationInShader_);
    shaderProgram_.link();
    // создание буферов
    glGenBuffers(5, VBO_);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(x_line), x_line, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(y_line), y_line, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_[3]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(arrows), arrows, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_[4]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(x), x, GL_DYNAMIC_DRAW);

    glGenVertexArrays(5, VAO_);

    for(int i = 0; i < 5; i++){
        glBindVertexArray(VAO_[i]);
        glBindBuffer(GL_ARRAY_BUFFER, VBO_[i]);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
}
// отрисовка
void plnm::paintGL()
{
   glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   // при изменении координат точек
   if(k){
       k = 0;
       int kk = 0;
       float x[12];
       float points[20000*3];
       for(float i = -10; i <= 10; i+=0.001){
           points[kk] = i; kk++;
           points[kk] = GetF(i); kk++;
           points[kk] = 0.0; kk++;
       }
       kk = 0;
       for(int i = 0; i < 12; i+=3){
           x[i] = x_[kk]; x[i+1] = y_[kk]; x[i+2] = 0;
           kk++;
       }
       glBindVertexArray(VAO_[0]);
       glBindBuffer(GL_ARRAY_BUFFER, VBO_[0]);
       glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
       glBindBuffer(GL_ARRAY_BUFFER, 0);

       glBindVertexArray(VAO_[4]);
       glBindBuffer(GL_ARRAY_BUFFER, VBO_[4]);
       glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(x), x);
       glBindBuffer(GL_ARRAY_BUFFER, 0);
   }

   QMatrix4x4 Model;
   Model.setToIdentity();
   scale_ = 0.1;
   Model.scale(QVector3D(scale_, scale_, scale_));

   shaderProgram_.bind(); 
   shaderProgram_.setUniformValue("model",Model);
   shaderProgram_.setUniformValue("color",QVector3D(1,0,0));

   glBindVertexArray(VAO_[0]);
   glEnableVertexAttribArray(0);
   glDrawArrays(GL_LINES, 0, 20000);
   glDisableVertexAttribArray(0);
   glBindVertexArray(0);

   glPointSize(5.f);
   glBindVertexArray(VAO_[4]);
   glEnableVertexAttribArray(0);
   glDrawArrays(GL_POINTS, 0, 4);
   glDisableVertexAttribArray(0);
   glBindVertexArray(0);

   shaderProgram_.setUniformValue("color",QVector3D(0,0,0));

   glBindVertexArray(VAO_[1]);
   glEnableVertexAttribArray(0);
   glDrawArrays(GL_LINES, 0, 3);
   glDisableVertexAttribArray(0);
   glBindVertexArray(0);

   glBindVertexArray(VAO_[2]);
   glEnableVertexAttribArray(0);
   glDrawArrays(GL_LINES, 0, 3);
   glDisableVertexAttribArray(0);
   glBindVertexArray(0);

   glBindVertexArray(VAO_[3]);
   glEnableVertexAttribArray(0);
   glDrawArrays(GL_TRIANGLES, 0, 6);
   glDisableVertexAttribArray(0);
   glBindVertexArray(0);

   shaderProgram_.release();
}
// изменение размера окна
void plnm::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}
// изменение координат точек
void plnm::setCoords(QVector<float> coords)
{
    for(int i = 0; i < 4; i++){
        x_[i] = coords[i];
        y_[i] = coords[i+4];
    }

    k = 1;
    this->paintGL();
}
// деструктор
plnm::~plnm()
{
    glDeleteVertexArrays(5, VAO_);
    glDeleteBuffers(5, VBO_);
}
