#version 330 core

in vec3  fmatamb;
in vec3  fmatdiff;
in vec3  fmatspec;
in float fmatshin;
in vec3  fvertex;
in vec3  fnormal;

const vec3 llumAmbient = vec3(0.2, 0.2, 0.2);

uniform mat4 TG;
uniform mat4 Proj;
uniform mat4 View;

out vec4 FragColor;

uniform vec3 colorFocusC;
uniform vec3 colorFocusFB;
uniform vec3 colorFocusFG; 

uniform vec3 posFocusC;     //camara 
uniform vec3 posFocusFB;    //focus blanc
uniform vec3 posFocusFG;    //focus groc  

vec3 Ambient() {
  return llumAmbient*fmatamb;
}

vec3 Difus (vec3 NormSCO, vec3 L, vec3 colFocus)
{
  // Fixeu-vos que SOLS es retorna el terme de Lambert!
  // S'assumeix que els vectors que es reben com a paràmetres estan normalitzats
  vec3 colRes = vec3(0);
  if (dot (L, NormSCO) > 0)
    colRes = colFocus * fmatdiff * dot (L, NormSCO);
  return (colRes);
}

vec3 Especular (vec3 NormSCO, vec3 L, vec3 vertSCO, vec3 colFocus)
{
  // Fixeu-vos que SOLS es retorna el terme especular!
  // Assumim que els vectors estan normalitzats
  vec3 colRes = vec3 (0);
  // Si la llum ve de darrera o el material és mate no fem res
  if ((dot(NormSCO,L) < 0) || (fmatshin == 0))
    return colRes;  // no hi ha component especular

  vec3 R = reflect(-L, NormSCO); // equival a: 2.0*dot(NormSCO,L)*NormSCO - L;
  vec3 V = normalize(-vertSCO); // perquè la càmera està a (0,0,0) en SCO

  if (dot(R, V) < 0)
    return colRes;  // no hi ha component especular

  float shine = pow(max(0.0, dot(R, V)), fmatshin);
  return (colRes + fmatspec * colFocus * shine);
}

void main()
{
    //normal
    vec3 NormSCO = normalize(fnormal);
    NormSCO = normalize(fnormal);
    //---POSFOCUS---
    //camara
    vec4 posFSCOC;
    posFSCOC = vec4(posFocusC, 1.0);//no se multiplica por la view, ya esta en OBS
    //Focus blanc
    vec4 posFSCOFB;
    posFSCOFB = View*vec4(posFocusFB, 1.0);
    //Focus grox
    vec4 posFSCOFG;
    posFSCOFG = View*vec4(posFocusFG, 1.0);


    //LC
    vec3 LC = posFSCOC.xyz-fvertex.xyz;
    LC = normalize(LC);
    //LFB
    vec3 LFB = posFSCOFB.xyz-fvertex.xyz;
    LFB = normalize(LFB);
    //LFG
    vec3 LFG = posFSCOFG.xyz-fvertex.xyz;
    LFG = normalize(LFG);
    
    vec3 fcolor = Ambient()+Difus(NormSCO, LC, colorFocusC) + Especular(NormSCO, LC, fvertex.xyz, colorFocusC);
    fcolor += (Difus(NormSCO, LFB, colorFocusFB)+Especular(NormSCO, LFB, fvertex.xyz, colorFocusFB));
    fcolor += (Difus(NormSCO, LFG, colorFocusFG)+Especular(NormSCO, LFG, fvertex.xyz, colorFocusFG));
    FragColor = vec4(fcolor,1); 


    //FragColor = vec4(fmatdiff, 1);
}
