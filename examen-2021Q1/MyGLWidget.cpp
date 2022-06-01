// MyGLWidget.cpp
#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>

#define printOpenGLError() printOglError(__FILE__, __LINE__)
#define CHECK() printOglError(__FILE__, __LINE__,__FUNCTION__)
#define DEBUG() std::cout << __FILE__ << " " << __LINE__ << " " << __FUNCTION__ << std::endl;

int MyGLWidget::printOglError(const char file[], int line, const char func[]) 
{
    GLenum glErr;
    int    retCode = 0;

    glErr = glGetError();
    const char * error = 0;
    switch (glErr)
    {
        case 0x0500:
            error = "GL_INVALID_ENUM";
            break;
        case 0x501:
            error = "GL_INVALID_VALUE";
            break;
        case 0x502: 
            error = "GL_INVALID_OPERATION";
            break;
        case 0x503:
            error = "GL_STACK_OVERFLOW";
            break;
        case 0x504:
            error = "GL_STACK_UNDERFLOW";
            break;
        case 0x505:
            error = "GL_OUT_OF_MEMORY";
            break;
        default:
            error = "unknown error!";
    }
    if (glErr != GL_NO_ERROR)
    {
        printf("glError in file %s @ line %d: %s function: %s\n",
                             file, line, error, func);
        retCode = 1;
    }
    return retCode;
}

MyGLWidget::~MyGLWidget() {
}

void MyGLWidget::paintGL ()   // Mètode que has de modificar
{
  ExamGLWidget::paintGL();
    // Pintem el terra
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glBindVertexArray (VAO_Terra);
  modelTransformTerra ();
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
if (v) {
  // Pintem el Patricio
  glBindVertexArray (VAO_Patr);
  modelTransformPatricio ();
  glDrawArrays(GL_TRIANGLES, 0, patr.faces().size()*3);
}
else if (!v) {
  // Pintem el cub
  glBindVertexArray(VAO_Cub);
  modelTransformCub (2.0, 0.0);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  
  glBindVertexArray(VAO_Cub);
  modelTransformCub (2.5, -120.0);
  glDrawArrays(GL_TRIANGLES, 0, 36);
  
  glBindVertexArray(VAO_Cub);
  modelTransformCub (3.0, 120.0);
  glDrawArrays(GL_TRIANGLES, 0, 36);
}
  glBindVertexArray(0);
}

void MyGLWidget::modelTransformCub (float escala, float angle) 
{
  ExamGLWidget::modelTransformCub (1.0, 0.0);
  TG = glm::mat4(1.f);

  TG = glm::rotate(TG,float((angle+extra)*M_PI/180),glm::vec3(0.0,1.0,0.0));
  TG = glm::translate(TG, glm::vec3(5.0,0.0,0.0));
  TG = glm::scale(TG, glm::vec3 (escala/0.5, escala/0.5, escala/0.5));
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformPatricio ()    // Mètode que has de modificar
{
  ExamGLWidget::modelTransformPatricio ();
  TG = glm::mat4(1.f);
  if (dos) TG = glm::rotate(TG,float(-120.0*M_PI/180),glm::vec3(0.0,1.0,0.0));
  if (tres) TG = glm::rotate(TG,float(120.0*M_PI/180),glm::vec3(0.0,1.0,0.0));
  TG = glm::rotate(TG,float(extra*M_PI/180),glm::vec3(0.0,1.0,0.0));
  TG = glm::translate(TG,glm::vec3(5.0,0.0,1.0));
  TG = glm::rotate(TG,-float(90*M_PI/180),glm::vec3(0.0,1.0,0.0));
  TG = glm::scale(TG, glm::vec3 (escala*2.0, escala*2.0, escala*2.0));
  TG = glm::translate(TG, -centreBasePat);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::viewTransform ()    // Mètode que has de modificar
{
  if (!camPlanta)
    ExamGLWidget::viewTransform();
  else
  {
      glm::mat4 View(1.f);
    View = glm::lookAt(glm::vec3(0.0,radiEsc*2,0.0),glm::vec3(0.0,0.0,0.0),glm::vec3(1.0,0.0,0.0));
    glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
  }
}

void MyGLWidget::projectTransform ()
{
  if (!camPlanta)
    ExamGLWidget::projectTransform();
  else
  {
  glm::mat4 Proj;  // Matriu de projecció
  Proj = glm::ortho(-radiEsc, radiEsc, -radiEsc, radiEsc,radiEsc,float(radiEsc*3.0));
  glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
  }
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) {
  makeCurrent();
  switch (event->key()) {
  case Qt::Key_V: {
      if (v) v = 0;
      else if (!v) v = 1;
    break;
	}
  case Qt::Key_1: {
      if (uno) dos = tres = 0;
      else if (!uno) {
          uno = 1;
          dos = tres = 0;
      }
    break;
	}
  case Qt::Key_2: {
      if (dos)  uno = tres = 0;
      else if (!dos) {
          dos = 1;
          tres = uno = 0;
      }
          
    break;
	}
  case Qt::Key_3: {
      if (tres) dos = uno = 0;
      else if (!tres) {
          tres = 1;
          uno = dos = 0;
      }
    break;
	}
  case Qt::Key_F: {
      if (f) {
          f = 0;
          colFoc = glm::vec3(1.f,1.f,1.f);
           enviaColFocus();
    }
    else if (!f) {
        f = 1;
         colFoc = glm::vec3(1.f,1.f,0.f);
        enviaColFocus();
    }
    break;
	}
  case Qt::Key_C: {
      if (camPlanta) camPlanta = 0;
    else if (!camPlanta) camPlanta = 1;
    projectTransform ();
    viewTransform();
    break;
	}
  case Qt::Key_Right: {
      extra -= 120.0;
      if (extra < 0.0) extra = 360.0;
    break;
	}
  case Qt::Key_Left: {
      extra += 120.0;
      if (extra > 360.0) extra = 0.0;
    break;
	}
  case Qt::Key_R: {
      // ...
    break;
	}
  default: ExamGLWidget::keyPressEvent(event); break;
  }
  update();
}

