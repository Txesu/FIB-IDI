#version 330 core

in vec4 vertexF;
in vec3 normalF;
in vec3 matambF; //ambiental
in vec3 matdiffF; //diferencial
in vec3 matspecF; //especular
in float matshinF;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 TG;
uniform int camera;

// Valors per als components que necessitem del focus de llum

vec3 llumAmbient = vec3(0.2, 0.2, 0.2);
uniform vec3 colorFocusC;
uniform vec3 colorFocusE;

uniform vec3 posFocusC;  // en OBS
uniform vec3 posFocusE;  //en SCA

//in vec3 fcolor;
out vec4 FragColor;


vec3 Ambient() {
    return llumAmbient * matambF;
}

vec3 Difus (vec3 NormSCO, vec3 L, vec3 colFocus) // NormSCO esta en SC i la necessitem en SCO-> mat3 NormalMatrix = inverce(transpose(mat3(VM*TG))), NormSCO = NrmalMatrix*normal -> normalitzat -> NormSCO =normalize(NormSCO). L=posició del focus(la tenim en SCA i l'hem de passar a SCO-> L*VM i normalitzar)
{
    // Fixeu-vos que SOLS es retorna el terme difús
    // S'assumeix que els vectors que es reben com a paràmetres estan normalitzats
    vec3 colRes = vec3(0);
    // Càlcul component difusa, si n'hi ha
    if (dot (L, NormSCO) > 0)
      colRes = colFocus * matdiffF * dot (L, NormSCO);
    return (colRes);
}

vec3 Especular (vec3 NormSCO, vec3 L, vec4 vertexF, vec3 colFocus) 
{
    // Fixeu-vos que SOLS es retorna el terme especular!
    // Assumim que els vectors estan normalitzats
    vec3 colRes = vec3 (0);
    // Si la llum ve de darrera o el material és mate no fem res
    if ((dot(NormSCO,L) < 0) || (matshinF == 0))
      return colRes;  // no hi ha component especular

    // Calculem R i V
    vec3 R = reflect(-L, NormSCO); // equival a: 2.0*dot(NormSCO,L)*NormSCO - L;
    vec3 V = normalize(-vertexF.xyz); // perquè la càmera està a (0,0,0) en SCO

    if (dot(R, V) < 0)
      return colRes;  // no hi ha component especular
    
    float shine = pow(max(0.0, dot(R, V)), matshinF);
    return (matspecF * colFocus * shine); 
}

void main()
{	
//normal
    vec3 NormSCO = normalize(normalF);
    NormSCO = normalize(normalF);
//PosFocus
    //camara
    vec4 posFSCOC;
    posFSCOC = vec4(posFocusC, 1.0);
    //escena
    vec4 posFSCOE;
    posFSCOE = view*vec4(posFocusE, 1.0);
    
    //LC
    vec3 LC = posFSCOC.xyz-vertexF.xyz;
    LC = normalize(LC);
    //LE
    vec3 LE = posFSCOE.xyz-vertexF.xyz;
    LE = normalize(LE);
    
    vec3 fcolor = Ambient()+Difus(NormSCO, LC, colorFocusC) + Especular(NormSCO, LC, vertexF, colorFocusC);
    fcolor += (Difus(NormSCO, LE, colorFocusE)+Especular(NormSCO, LE, vertexF, colorFocusE));
	FragColor = vec4(fcolor,1);	
}
