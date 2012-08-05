#version 400

uniform mat4 MVP;

layout(location = 0) in vec3 Position;
layout(location = 1) in uint level;

out vec4 vPosition;
out uint vLevel;

void main()
{
	gl_Position = MVP * vec4(Position, 1.0);
	vPosition = gl_Position;
	vLevel = level;
}
