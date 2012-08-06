#version 400


layout(location = 0, index = 0) out vec4 FragColor;

uniform usampler3D indexTexture;
uniform usampler3D textureAtlas;
uniform mat4 MVPInverse;
uniform vec4 camPosition;

in vec4 vPosition;

void intersectBB(in vec4 rayDir, in vec4 rayOrigin, out vec4 exitBB);
float trilinearSample(in vec3 pos);

void main()
{
	vec4 rayOrigin = vPosition;
	vec4 rayDir = normalize ((vPosition - camPosition));
	
	vec4 exitBB;
	intersectBB(rayDir, rayOrigin, exitBB);
	
	vec4 pos = rayOrigin;
	float dist = 0;
	float maxDist = length(exitBB - rayOrigin);
	
	vec3 dst = vec3(0.0);
	int counter = 0;
	while (dist < maxDist && counter <10000)
	{
		vec4 indextmp = floor(pos);
		ivec3 index = ivec3(indextmp.x, indextmp.y, indextmp.z);
		uvec4 bla = texelFetch(indexTexture , index, 0);
		vec3 pos2 = vec3 (bla.x * 128 + pos.x/bla.y * 128 , 
				  pos.y/bla.z * 128 , pos.z/bla.w * 128);
		
		float value = trilinearSample(pos2);
		vec3 src = vec3((value/255.0), (value/255.0), (value/255.0));
//		float alpha = value/255.0;
		
		dst.r = max(src.r , dst.r);
		dst.g = max(src.g , dst.g);
		dst.b = max(src.b , dst.b);
		
		pos += 0.01 * rayDir;
		dist = length(pos - rayOrigin);
		++counter;
	}
	
	
//	FragColor = vec4((texelFetch(indexTexture, ivec3(0, 3, 0), 0)).x/73.0, 0.0, 0.0, 1.0);
//	FragColor = vec4((float((texelFetch(textureAtlas, ivec3(0,0,0), 1)).x)/255.0, 0.0, 1.3, 1.0));
	vec4 blub = texelFetch(textureAtlas, ivec3(vPosition.x * 128, vPosition.y * 128, 32.0), 0);
//	FragColor = vec4(blub.x/255.0, 0.0, 0.0, 1.0);
//	FragColor = vec4(dst, 1.0);

}


void intersectBB(in vec4 rayDir, in vec4 rayOrigin, out vec4 exitBB)
{
	float tmin, tmax, tymin, tymax, tzmin, tzmax;

	vec3 boxMin = vec3(0.0);
	vec3 boxMax = vec3(1.0);
	vec3 recDir = vec3(vec3(1.0) / rayDir.xyz);

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

float trilinearSample(in vec3 pos)
{
	int fx = int(floor(pos.x));
	int fy = int(floor(pos.y));
	int fz = int(floor(pos.z));
	int cx = int(ceil (pos.x));
	int cy = int(ceil (pos.y));
	int cz = int(ceil (pos.z));
	
	ivec3 p000 = ivec3(fx, fy, fz);
	ivec3 p001 = ivec3(fx, fy, cz);
	ivec3 p010 = ivec3(fx, cy, fz);
	ivec3 p011 = ivec3(fx, cy, cz);
	ivec3 p100 = ivec3(cx, fy, fz);
	ivec3 p101 = ivec3(cx, fy, cz);
	ivec3 p110 = ivec3(cx, cy, fz);
	ivec3 p111 = ivec3(cx, cy, cz);

	float v000 = (texelFetch(textureAtlas, p000, 0));
	float v001 = (texelFetch(textureAtlas, p001, 0));
	float v010 = (texelFetch(textureAtlas, p010, 0));
	float v011 = (texelFetch(textureAtlas, p011, 0));
	float v100 = (texelFetch(textureAtlas, p100, 0));
	float v101 = (texelFetch(textureAtlas, p101, 0));
	float v110 = (texelFetch(textureAtlas, p110, 0));
	float v111 = (texelFetch(textureAtlas, p111, 0));

	// 4 linear
	float l00 = mix(v000, v100, fract(pos.x));
	float l01 = mix(v001, v101, fract(pos.x));
	float l10 = mix(v010, v110, fract(pos.x));
	float l11 = mix(v011, v111, fract(pos.x));	

	// 2 bilinear
	float b0 = mix(l00, l10, fract(pos.y));
	float b1 = mix(l01, l11, fract(pos.y));
	
	// 1 trilinear
	float t = mix (b0, b1, fract(pos.z));
	
	return t;
}










