// MyGLWidget.h
#include "BL2GLWidget.h"

class MyGLWidget : public BL2GLWidget {
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : BL2GLWidget(parent) {}
    ~MyGLWidget();

  private:
    void carregaShaders();
    void projectTransform();
    void viewTransform();
    void paintGL();
    GLuint projLoc;
    GLuint viewLoc;
    int printOglError(const char file[], int line, const char func[]);
};
