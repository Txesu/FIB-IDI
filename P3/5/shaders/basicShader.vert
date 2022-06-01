#version 330 core

in vec3 vertex;
uniform float tam;
uniform mat4 TG;//matriz de transformacion, mover, rotar y escalar
void main()  {
    gl_Position = TG * vec4 (vertex*tam, 1.0);//las rotaciones en orden inverso, multiplicacion de matrices por la izquierda
    //gl_Position = vec4 (vertex, 1.0);
    
}
