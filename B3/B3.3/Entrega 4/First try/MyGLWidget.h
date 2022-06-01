// MyGLWidget.h
#include "LL4GLWidget.h"

class MyGLWidget : public LL4GLWidget {
  Q_OBJECT
  public:
    MyGLWidget(QWidget *parent=0) : LL4GLWidget(parent) {}
    ~MyGLWidget();
  protected:
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void initializeGL();
    virtual void paintGL();
    virtual void carregaShaders();
    virtual void modelTransformPatricio();
  private:
    int printOglError(const char file[], int line, const char func[]);
    GLuint colFocusLocC;
    GLuint PosFocusLocC;
    GLuint colFocusLocFB;
    GLuint PosFocusLocFB;
    GLuint colFocusLocFG;
    GLuint PosFocusLocFG;

    glm::vec3 posFocusFB;
    glm::vec3 colorFocusFB;
    glm::vec3 posFocusC;
    glm::vec3 colorFocusC;
    glm::vec3 posFocusFG;
    glm::vec3 colorFocusFG;

    int LuzP = 1; 
    int LuzE = 1; 
    int LuzC = 1;
    float anguloGiro = 0.0;
};
