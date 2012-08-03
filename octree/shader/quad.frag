#version 400

layout(location = 0, index = 0) out vec4 FragColor;

in vec4 vPosition;

void main()
{
	FragColor = vec4(vPosition.z/(192.0), 0.0, 0.0, 1.0);
}


