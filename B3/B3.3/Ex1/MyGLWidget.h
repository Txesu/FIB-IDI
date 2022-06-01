// MyGLWidget.h
#include "BL3GLWidget.h"

class MyGLWidget : public BL3GLWidget {
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : BL3GLWidget(parent) {}
    ~MyGLWidget();

  protected:
    virtual void mouseMoveEvent(QMouseEvent *e);
    //virtual void keyPressEvent(QKeyEvent* event);
    virtual void iniMaterialTerra();
    virtual void carregaShaders();
    virtual void initializeGL ();
    GLuint colFocusLocC;
    GLuint PosFocusLocC;
    GLuint colFocusLocE;
    GLuint PosFocusLocE;
    //GLuint camLoc;
    //int cam;
    glm::vec3 posFocusE;
    glm::vec3 colorFocusE;
    glm::vec3 posFocusC;
    glm::vec3 colorFocusC;
    bool first = true;
  private:
    int printOglError(const char file[], int line, const char func[]);
};
