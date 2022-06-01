#define GLM_FORCE_RADIANS
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QKeyEvent>
#include "glm/glm.hpp"
#include "BL2GLWidget.h"
#include "glm/gtc/matrix_transform.hpp"
#include "../Model/model.h"

class MyGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core 
{
  Q_OBJECT

  public:
    MyGLWidget (QWidget *parent=0);
    ~MyGLWidget ();

  protected:
    // initializeGL - Aqui incluim les inicialitzacions del contexte grafic.
    virtual void initializeGL ( );
    // paintGL - Mètode cridat cada cop que cal refrescar la finestra.
    // Tot el que es dibuixa es dibuixa aqui.
    virtual void paintGL ( );
    // resizeGL - És cridat quan canvia la mida del widget
    virtual void resizeGL (int width, int height);
    // keyPressEvent - Es cridat quan es prem una tecla
    virtual void keyPressEvent (QKeyEvent *event);

  private:
    void createBuffers ();
    virtual void carregaShaders ();
    void modelTransform ();
    void projectTransform ();
    void viewTransform ();
    void modelTransformGiro();
    // attribute locations
    GLuint vertexLoc, colorLoc;
    // uniform locations
    GLuint transLoc, projLoc, viewLoc;
    // VAO names
    //GLuint VAO_Casa;
    GLuint  VAO_HomerProves;
    // Program
    QOpenGLShaderProgram *program;
    // Internal vars
    float angulogiro;
    float angulogiro1;
    float angulogiro2;
    float scale;
    glm::vec3 pos;
    //matriz para girar al homer
    glm::mat4 GiroTG;
    
    Model HomerProves;
};
