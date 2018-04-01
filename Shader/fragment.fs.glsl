#version 460 core

out vec4 outColor;

in Vec2Out{
	vec2 v_outFragmentUV;
} vec2In;

uniform sampler2D u_textureSampler;

void main()
{
	outColor = texture(u_textureSampler, vec2In.v_outFragmentUV);
}