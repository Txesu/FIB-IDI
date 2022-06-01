#version 330 core

out vec4 FragColor;
in vec4 gl_FragCoord;
in vec3 fcolor;

void main() {
    FragColor = vec4(fcolor, 1.0);
    /*if (gl_FragCoord.x < 357.5 &&  gl_FragCoord.y > 357)
        FragColor = vec4(1, 0, 0, 1);
    else if (gl_FragCoord.x < 357.5 &&  gl_FragCoord.y < 357)
        FragColor = vec4(1, 1, 0, 1);
    else if (gl_FragCoord.x >= 357.5 &&  gl_FragCoord.y > 357)
        FragColor = vec4(0, 0, 1, 1);
    else if (gl_FragCoord.x >= 357.5 &&  gl_FragCoord.y < 357)
        FragColor = vec4(0, 1, 0, 1);*/
}

