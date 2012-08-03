#version 400

uniform mat4 MVP;

layout(location = 0) in vec3 Position;


out vec4 vPosition;

void main()
{
	gl_Position = MVP * vec4(Position, 1.0);
	vPosition = gl_Position;
}
