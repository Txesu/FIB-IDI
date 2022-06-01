#version 330 core

in vec3 vertex;
in vec3 normal;

in vec3  matamb;
in vec3  matdiff;
in vec3  matspec;
in float matshin;

uniform mat4 TG;
uniform mat4 Proj;
uniform mat4 View;

out vec3  fmatamb;
out vec3  fmatdiff;
out vec3  fmatspec;
out float fmatshin;
out vec3  fvertex;
out vec3  fnormal;

uniform vec3 colorFocusC;
uniform vec3 colorFocusFB;
uniform vec3 colorFocusFG; 

uniform vec3 posFocusC;     //camara 
uniform vec3 posFocusFB;    //focus blanc
uniform vec3 posFocusFG;    //focus groc  

void main()
{	
    // Passem les dades al fragment shader
    fmatamb  = matamb;
    fmatdiff = matdiff;
    fmatspec = matspec;
    fmatshin = matshin;
    //fvertex = vertex; // CALCULEU CORRECTAMENT
    //fnormal = normal; // CALCULEU CORRECTAMENT

    mat3 NormalMatrix = inverse (transpose (mat3 (View * TG)));
    vec3 NormSCO = NormalMatrix*normal;
    fnormal = NormSCO;

    vec4 vertSCO = View*TG*vec4(vertex, 1.0);
    fvertex = vertSCO.xyz;

    gl_Position = Proj * View * TG * vec4 (vertex, 1.0);
}
