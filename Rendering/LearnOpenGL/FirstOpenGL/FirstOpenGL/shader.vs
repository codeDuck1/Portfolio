#version 330 core 
layout (location = 0) in vec3 aPos; // pos var has attribute pos 0
layout (location = 1) in vec3 aColor; // color var has attribute pos 1
layout (location = 2) in vec2 aTexCoord;

out vec3 ourColor; // output a color to fragment shader
out vec2 TexCoord; // for forwarding coords to fragment shader


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0f); // read mult from right to lefts

   
    //ourColor = aColor; // set ourColor to the input color got from vertex data (for rainbow)
    TexCoord = aTexCoord; 
}