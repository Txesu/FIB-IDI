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

void MyGLWidget::paintGL () 
{
    LL4GLWidget::paintGL();
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Activem el VAO per a pintar el terra 
  glBindVertexArray (VAO_Terra);

  modelTransformTerra ();

  // pintem
  glDrawArrays(GL_TRIANGLES, 0, 12);

  // Activem el VAO per a pintar el Patricio
  glBindVertexArray (VAO_Patr);

  modelTransformPatricio ();

  // Pintem l'escena
  glDrawArrays(GL_TRIANGLES, 0, patr.faces().size()*3);
  
  glBindVertexArray(0);
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *e)
{
  makeCurrent();
  // Aqui cal que es calculi i s'apliqui la rotacio o el zoom com s'escaigui...
  if (DoingInteractive == ROTATE)
  {
    // Fem la rotació
    angleY += (e->x() - xClick) * M_PI / ample;
    viewTransform ();
  }

  xClick = e->x();
  yClick = e->y();

  update ();
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) {
  makeCurrent();
  switch (event->key()) {
  case Qt::Key_F: {
      // ...
    break;
	}
  case Qt::Key_Right: {
      anguloGiro -= float(M_PI*30/180);
    break;
	}
  case Qt::Key_Left: {
      anguloGiro += float(M_PI*30/180);
    break;
	}
    case Qt::Key_C: {
        if (LuzC) {
            //std::cout << LuzC << std::endl;
            colorFocusC = glm::vec3(0.0, 0.0, 0.0);
            glUniform3fv(colFocusLocC, 1, &colorFocusC[0]);
            LuzC = 0;
        }
        else if (!LuzC) {
            colorFocusC = glm::vec3(0.9, 0.0, 0.9);
            glUniform3fv(colFocusLocC, 1, &colorFocusC[0]);
            LuzC = 1;
        }
    break;
    }
    case Qt::Key_E: {
      if (LuzE) {
            //std::cout << LuzC << std::endl;
            colorFocusFB = glm::vec3(0.0, 0.0, 0.0);
            glUniform3fv(colFocusLocFB, 1, &colorFocusFB[0]);
            LuzE = 0;
        }
        else if (!LuzE) {
            colorFocusFB = glm::vec3(0.9, 0.9, 0.9);
            glUniform3fv(colFocusLocFB, 1, &colorFocusFB[0]);
            LuzE = 1;
        }
    break;
    }
    case Qt::Key_P: {
      if (LuzP) {
            //std::cout << LuzC << std::endl;
            colorFocusFG = glm::vec3(0.0, 0.0, 0.0);
            glUniform3fv(colFocusLocFG, 1, &colorFocusFG[0]);
            LuzP = 0;
        }
        else if (!LuzP) {
            colorFocusFG = glm::vec3(0.9, 0.9, 0.2);
            glUniform3fv(colFocusLocFG, 1, &colorFocusFG[0]);
            LuzP = 1;
        }
    break;
    }
  default: LL4GLWidget::keyPressEvent(event); break;
  }
  update();
}

void MyGLWidget::initializeGL ()
{
    LL4GLWidget::initializeGL();
  // Cal inicialitzar l'ús de les funcions d'OpenGL
  initializeOpenGLFunctions();  

  glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  glEnable(GL_DEPTH_TEST);
  carregaShaders();
  creaBuffersPatricio();
  creaBuffersTerraIParet();
  
 //FocusBlanc
  posFocusFB = glm::vec3(10.0,3.0,5.0);
  glUniform3fv(PosFocusLocFB, 1, &posFocusFB[0]);
  colorFocusFB = glm::vec3(0.9, 0.9, 0.9);
  glUniform3fv(colFocusLocFB, 1, &colorFocusFB[0]);
  
  //camara
  posFocusC = glm::vec3(0, 0 ,0);//camara en 0 0 0 siempre pq esta en SCoordsOBS
  glUniform3fv(PosFocusLocC, 1, &posFocusC[0]);
  colorFocusC = glm::vec3(0.9, 0.0, 0.9);
  glUniform3fv(colFocusLocC, 1, &colorFocusC[0]);

 //FocusGroc
  posFocusFG = glm::vec3(5.0, 3 ,2.0);
  //std::cout << "centro patricio" << centreBasePatr[0] << ' ' << maxYPat << ' ' << centreBasePatr[2] << std::endl;
  glUniform3fv(PosFocusLocFG, 1, &posFocusFG[0]);
  colorFocusFG = glm::vec3(0.9, 0.9, 0.2);
  glUniform3fv(colFocusLocFG, 1, &colorFocusFG[0]);
  iniEscena();
  iniCamera();
}

void MyGLWidget::carregaShaders()
{
    LL4GLWidget::carregaShaders();
    colFocusLocC = glGetUniformLocation(program->programId(), "colorFocusC");
    PosFocusLocC = glGetUniformLocation(program->programId(), "posFocusC");
    colFocusLocFB = glGetUniformLocation(program->programId(), "colorFocusFB");
    PosFocusLocFB = glGetUniformLocation(program->programId(), "posFocusFB");
    colFocusLocFG = glGetUniformLocation(program->programId(), "colorFocusFG");
    PosFocusLocFG = glGetUniformLocation(program->programId(), "posFocusFG");    
}

void MyGLWidget::modelTransformPatricio ()
{
  //glm::mat4 TG(1.f);  // Matriu de transformació

  TG = glm::translate(TG, glm::vec3(5,0,5));
  TG = glm::rotate(TG, anguloGiro, glm::vec3(0.0, 1.0, 0.0));
  TG = glm::translate(TG, glm::vec3(-5,0,-5));

  TG = glm::translate(TG, glm::vec3(5,0,2));
  TG = glm::scale(TG, glm::vec3(escala, escala, escala));
  TG = glm::translate(TG, -centreBasePatr);

  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}
