#version 400

layout(location = 0, index = 0) out vec4 FragColor;

uniform mat4 MVPInverse;
uniform vec4 camPosition;

in vec4 vPosition;

void main()
{
	vec4 rayOrigin = vPosition;
	vec4 rayDir = normalize (MVPInverse * (vPosition - camPosition));
	
	vec4 exitBB;
	intersectBB(rayDir, exitBB);
	
	vec4 pos = rayOrigin;
	float dist = 0;
	float maxDist = exitBB - rayOrigin
	
	while (dist < maxDist)
	{
	
	}
	
	
	FragColor = vec4(vPosition.xyz, 1.0);

}


void intersectBB(in vec4 rayDir, out vec4 exitBB)
{
	float tmin, tmax, tymin, tymax, tzmin, tzmax;

	vec3 boxMin = vec3(0.0);
	vec3 boxMax = vec3(1.0);
	vec3 recDir(vec3(1.0) / rayDir.xyz);

	if (rayDir.x >= 0.0) 
	{
		tmax = (boxMax.x - rayOrigin.x) * recDir.x;
	}
	else 
	{
		tmax = (boxMin.x - rayOrigin.x) * recDir.x;
	}

	if (rayDir.y >= 0.0) 
	{
		tymax = (boxMax.y - rayOrigin.y) * recDir.y;
	}
	else 
	{
		tymax = (boxMin.y - rayOrigin.y) * recDir.y;
	}
	if (tymax < tmax) 
	{
		tmax = tymax;
	}

	if (rayDir.z >= 0.0) 
	{
		tzmax = (boxMax.z - rayOrigin.z) * recDir.z;
	}
	else 
	{
		tzmax = (boxMin.z - rayOrigin.z) * recDir.z;
	}

	if (tzmax < tmax) 
	{
		tmax = tzmax;
	}
	exitBB  = rayOrigin + (tmax * rayDir);
}

