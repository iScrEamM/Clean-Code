#version 460 core

layout (location = 0) in vec3 a_position;
layout (location = 1) in vec2 a_uv;

out Vec2Out {
	vec2 v_outFragmentUV;
} vec2Out;

uniform mat4 u_mvp;

void main()
{
	gl_Position = u_mvp * vec4(a_position, 1.0);

	vec2Out.v_outFragmentUV = vec2(a_uv.x, 1 - a_uv.y);
}