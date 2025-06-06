#version 450 core

layout(location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec4 ourColor;
out vec2 TexCoord;

uniform mat4 transform;
uniform mat4 view;
uniform mat4 projection;
uniform float alpha;

void main()
{
    gl_Position = projection * view * transform * vec4(aPos, 1.0);
	ourColor = vec4(aColor, alpha);
	TexCoord = aTexCoord;
}