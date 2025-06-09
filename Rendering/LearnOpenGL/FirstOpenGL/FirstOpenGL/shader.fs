#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// fragment shader needs access to texture object
uniform sampler2D texture1;
uniform sampler2D texture2;

uniform float visVal;

void main()
{
    // sample color of texture using our texture object we setup and coords
    // output is filtered color of texture at the interpolated texture coordinate
    //FragColor = texture(ourTexture, TexCoord) * vec4(ourColor, 1.0);
    
    // final output color is combo of 2 txture look ups. 
    // 3rd param as 0.2 returns 80% of first input color
    FragColor = mix(texture(texture1, TexCoord), texture(texture2, vec2(1.0 - TexCoord.x, TexCoord.y)), visVal); //* vec4(ourColor, 1.0); // invert x axis
}