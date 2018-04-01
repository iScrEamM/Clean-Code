#version 460 core

out vec4 outColor;

in vec2 v_uv;

uniform sampler2D texture_diffuse1;

void main()
{    
    outColor = texture(texture_diffuse1, v_uv);
}