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
  // En cas de voler canviar els paràmetres del viewport, descomenteu la crida següent i
  // useu els paràmetres que considereu (els que hi ha són els de per defecte)
  // glViewport (0, 0, ample, alt);
  
  // Esborrem el frame-buffer i el depth-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Activem el VAO per a pintar el terra 
  glBindVertexArray (VAO_Terra);

  modelTransformTerra ();
  // pintem el terra
  glDrawArrays(GL_TRIANGLES, 0, 12);

  // Activem el VAO per a pintar el Patricio
  glBindVertexArray (VAO_Patr);

  /*modelTransformPatricio ();
  // Pintem el Patricio
  glDrawArrays(GL_TRIANGLES, 0, patr.faces().size()*3);*/
  
  patrTrans();
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
void MyGLWidget::initializeGL ()
{
    
  // Cal inicialitzar l'ús de les funcions d'OpenGL
  initializeOpenGLFunctions();  
  glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  glEnable(GL_DEPTH_TEST);
  carregaShaders();
  creaBuffersPatricio();
  creaBuffersTerraIParet();
  
 //escena
  posFocusE = glm::vec3(1, 1 ,1);
  glUniform3fv(PosFocusLocE, 1, &posFocusE[0]);
  colorFocusE = glm::vec3(0.0, 0.8, 0.8);
  glUniform3fv(colFocusLocE, 1, &colorFocusE[0]);
  
  //camara
  posFocusC = glm::vec3(0, 0 ,0);//camara en 0 0 0 siempre pq esta en SCoordsOBS
  glUniform3fv(PosFocusLocC, 1, &posFocusC[0]);
  colorFocusC = glm::vec3(0.8, 0.8, 0.0);
  glUniform3fv(colFocusLocC, 1, &colorFocusC[0]);

 // cam = 0;
//  glUniform1i(camLoc, cam);

  iniEscena();
  iniCamera();
}


/*void MyGLWidget::keyPressEvent(QKeyEvent* event) {
  makeCurrent();
  switch (event->key()) {
      case Qt::Key_F :
      {
          if (first == true)
          {
            makeCurrent();
            cam = 1;
            glUniform1i(camLoc, cam);
            posFocus = glm::vec3(0, 0 ,0);
            glUniform3fv(PosFocusLoc, 1, &posFocus[0]);
            update();
            first = false;
          }
          else
          {
            makeCurrent();
            cam = 0;
            glUniform1i(camLoc, cam);
            posFocus = glm::vec3(1, 0 ,1);
            glUniform3fv(PosFocusLoc, 1, &posFocus[0]);
            update();
            first = true;
          }
      break;
      }
    default: BL3GLWidget::keyPressEvent(event); break;
  }
  update();
}
*/
void MyGLWidget::iniMaterialTerra ()
{
  // Donem valors al material del terra
  amb = glm::vec3(0.2,0,0.2);
  diff = glm::vec3(0,0,1.0);
  spec = glm::vec3(1,1,1); //lo qu influie la pos de la luz con la pos de la obs
  shin = 50; //a més petit més taca i a més gran menys taca
}

void MyGLWidget::carregaShaders()
{
    BL3GLWidget::carregaShaders();
    colFocusLocE = glGetUniformLocation(program->programId(), "colorFocusE");
    PosFocusLocE = glGetUniformLocation(program->programId(), "posFocusE");
    colFocusLocC = glGetUniformLocation(program->programId(), "colorFocusC");
    PosFocusLocC = glGetUniformLocation(program->programId(), "posFocusC");
    //camLoc = glGetUniformLocation(program->programId(), "camera");
    
}

void MyGLWidget::patrTrans()
{
    glm::mat4 TG(1.0);
    TG = glm::translate(TG,-glm::vec3(0.0,0.75,0.0));
    TG = glm::scale(TG,glm::vec3(escala*0.3/2,escala*0.3/2,escala*0.3/2));
    TG = glm::translate(TG,-glm::vec3(centrePatr[0],centrePatr[1],centrePatr[2]));
    glUniformMatrix4fv(transLoc,1,GL_FALSE,&TG[0][0]);
    
}


