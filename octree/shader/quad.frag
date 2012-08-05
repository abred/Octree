#version 400

layout(location = 0, index = 0) out vec4 FragColor;

in vec4 vPosition;
in uint vLevel;

void main()
{
	if(vLevel == 5)
	FragColor = vec4(1.0, 0.0, 0.0, 1.0);
	else if(vLevel == 4)
	FragColor = vec4(0.0, 1.0, 0.0, 1.0);
	else if(vLevel == 3)
	FragColor = vec4(0.0, 0.0, 1.0, 1.0);
	else if(vLevel == 2)
	FragColor = vec4(1.0, 1.0, 0.0, 1.0);
	else if(vLevel == 6)
	FragColor = vec4(0.0, 1.0, 1.0, 1.0);
	else
	FragColor = vec4(0.0, 0.0, 0.0, 1.0);
}


