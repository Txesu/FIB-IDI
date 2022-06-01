#include "MyGLWidget.h"
#include <iostream>

MyGLWidget::MyGLWidget (QWidget* parent) : QOpenGLWidget(parent), program(NULL)
{
  setFocusPolicy(Qt::StrongFocus);  // per rebre events de teclat
}

MyGLWidget::~MyGLWidget ()
{
  if (program != NULL)
    delete program;
}

void MyGLWidget::initializeGL ()
{
  // Cal inicialitzar l'ús de les funcions d'OpenGL
  initializeOpenGLFunctions();  

  glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  carregaShaders();
  createBuffers();
  glEnable(GL_DEPTH_TEST); // NOMES UN COP! En paintGL() molt incorrecte
  
  scale = 1.0f;
  projectTransform();
  viewTransform();
}

void MyGLWidget::paintGL () 
{
  // Esborrem el frame-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Carreguem la transformació de model
  
  //modelTransform ();
  modelTransformGiro();
  glBindVertexArray(VAO_HomerProves);
  glDrawArrays(GL_TRIANGLES, 0, HomerProves.faces().size()*3);

  glBindVertexArray (0);
}

void MyGLWidget::modelTransformGiro ()
{
    GiroTG = glm::mat4(1.0);
    GiroTG = glm::rotate(GiroTG, angulogiro, glm::vec3(0.0,1.0,0.0));
    GiroTG = glm::rotate(GiroTG, angulogiro1, glm::vec3(1.0,0.0,0.0));
    GiroTG = glm::rotate(GiroTG, angulogiro2, glm::vec3(0.0,0.0,1.0));
    glUniformMatrix4fv(transLoc,1,GL_FALSE,&GiroTG[0][0]);
}

void MyGLWidget::modelTransform () 
{
  // Matriu de transformació de model
  glm::mat4 transform (1.0f);
  transform = glm::scale(transform, glm::vec3(scale));
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::resizeGL (int w, int h) 
{
  // Aquí anirà el codi que cal fer quan es redimensiona la finestra
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) 
{
  makeCurrent();
  switch (event->key()) {
    case Qt::Key_R: { // escalar a més gran
      angulogiro += float(M_PI*45/180);
      break;
    }
    case Qt::Key_E: { // escalar a més gran
      angulogiro1 += float(M_PI*45/180);
      break;
    }
    case Qt::Key_W: { // escalar a més gran
      angulogiro2 += float(M_PI*45/180);
      break;
    }
    default: event->ignore(); break;
  }
  update();
}

/*void MyGLWidget::creaBufferTerra() 
{
    glm::vec3 Vertices[6];
    Vertices[0] = glm::vec3();
    Vertices[1] = glm::vec3();
    Vertices[2] = glm::vec3();
    Vertices[3] = glm::vec3();
    Vertices[4] = glm::vec3();
    Vertices[5] = glm::vec3();
    
}
*/

void MyGLWidget::createBuffers () 
{
  // HomerProves
  HomerProves.load("../models/HomerProves.obj");
  
  // Creació del Vertex Array Object per pintar
  glGenVertexArrays(1, &VAO_HomerProves);
  glBindVertexArray(VAO_HomerProves);
  
  GLuint VBO_HomerProvesPos;
  glGenBuffers(1, &VBO_HomerProvesPos);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_HomerProvesPos);
  glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*HomerProves.faces().size()*3*3, HomerProves.VBO_vertices(), GL_STATIC_DRAW);
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);
  
  GLuint VBO_HomerProvesCol;
  glGenBuffers(1, &VBO_HomerProvesCol);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_HomerProvesCol);
  glBufferData(GL_ARRAY_BUFFER,sizeof(GLfloat)*HomerProves.faces().size()*3*3, HomerProves.VBO_matdiff(), GL_STATIC_DRAW);

  // Activem l'atribut colorLoc
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);              

  glBindVertexArray (0);
}

void MyGLWidget::carregaShaders()
{
  // Creem els shaders per al fragment shader i el vertex shader
  QOpenGLShader fs (QOpenGLShader::Fragment, this);
  QOpenGLShader vs (QOpenGLShader::Vertex, this);
  // Carreguem el codi dels fitxers i els compilem
  fs.compileSourceFile("shaders/basicShader.frag");
  vs.compileSourceFile("shaders/basicShader.vert");
  // Creem el program
  program = new QOpenGLShaderProgram(this);
  // Li afegim els shaders corresponents
  program->addShader(&fs);
  program->addShader(&vs);
  // Linkem el program
  program->link();
  // Indiquem que aquest és el program que volem usar
  program->bind();
  // Obtenim identificador per a l'atribut “vertex” del vertex shader
  vertexLoc = glGetAttribLocation (program->programId(), "vertex");
  // Obtenim identificador per a l'atribut “color” del vertex shader
  colorLoc = glGetAttribLocation (program->programId(), "color");
  // Uniform locations
  transLoc = glGetUniformLocation(program->programId(), "TG");
  projLoc = glGetUniformLocation(program->programId(), "proj");
  viewLoc = glGetUniformLocation(program->programId(), "view");
  
}

void MyGLWidget::projectTransform () 
{
    //glm::perspective (FOV en radians, ra window, Znear, Zfar);
    glm::mat4 Proj = glm::perspective (float(M_PI)/2.0f, 1.0f, 0.4f, 3.0f);
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void MyGLWidget::viewTransform () 
{
    //glm::lookAt(OBS, VRP, UP);
    glm::mat4 View = glm::lookAt (glm::vec3(0,0,1),glm::vec3(0,0,0), glm::vec3(0,1,0));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &View[0][0]);
}
