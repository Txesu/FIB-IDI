#version 330 core

in vec3 vertex;
in vec3 normal;

out vec4 vertexF;
out vec3 normalF;
out vec3 matambF; //ambiental
out vec3 matdiffF; //diferencial
out vec3 matspecF; //especular
out float matshinF;


in vec3 matamb; //ambiental
in vec3 matdiff; //diferencial
in vec3 matspec; //especular
in float matshin;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 TG;

// Valors per als components que necessitem del focus de llum
uniform vec3 colorFocusC;
uniform vec3 colorFocusE;
vec3 llumAmbient = vec3(0.2, 0.2, 0.2);
uniform vec3 posFocusC;  // en OBS
uniform vec3 posFocusE;    //en SCA

//out vec3 fcolor; //Ambient()+Difus()+Especular()

void main()
{
    matambF = matamb;
    matdiffF = matdiff;
    matspecF = matspec;
    matshinF = matshin;
    
    mat3 NormalMatrix = inverse (transpose (mat3 (view * TG)));
    vec3 NormSCO = NormalMatrix*normal;
    normalF = NormSCO;
    
    //vec4 posFSCO = view*vec4(posFocus, 1.0);
    
    vec4 vertSCO = view*TG*vec4(vertex, 1.0);
    vertexF = vertSCO;
    
    gl_Position = proj * view * TG * vec4 (vertex, 1.0);
}
