#version 400


layout(location = 0, index = 0) 
out vec4 FragColor;

uniform usampler3D indexTexture;
uniform usamplerBuffer textureAtlas;
uniform mat4 MVInverse;
uniform vec4 camPosition;

uniform vec2 windowSize;
uniform float focalLength;
uniform float stepSize;
uniform int numSamples;

in vec4 vPosition;


struct Ray
{
	vec3 origin;
	vec3 direction;
};


void intersectBB(in vec4 rayDir, in vec4 rayOrigin, out vec4 exitBB);
float trilinearSample(in vec4 pos);

bool intersectBox(in Ray r , out float t0 , out float t1);

void main()
{
	vec4 rayOrigin = vPosition ;
	vec4 rayDir = normalize ((vPosition - camPosition));
	
	vec4 exitBB;
	intersectBB(rayDir, rayOrigin, exitBB);
	
	vec4 pos = rayOrigin;
	float dist = 0.0;
	float maxDist = length(exitBB - rayOrigin);
	
	vec3 dst = vec3(0.0);
	while (dist < maxDist)
	{
		ivec3 index = ivec3(floor(pos.x * 32.0), floor(pos.y * 32.0), floor(pos.z * 32.0));
		uvec4 bla = texelFetch(indexTexture , index, 0);
		
//		vec4 pos2 = vec4 (fract(pos.x * 255.0/8.0), fract(pos.y * 255.0/8.0), fract(pos.z * 255.0/8.0), 8.0 * 8.0 * 8.0 * bla.x );
		vec4 pos2 = vec4 ( fract(pos.x *bla.y)*7.0, fract(pos.y * bla.y) *7.0, fract(pos.z * bla.y) *7.0, 8.0 * 8.0 * 8.0 * bla.x );

		
		float value = trilinearSample(pos2);
		vec3 src;
//		if (value < 1.0)
//		src = vec3(0.0);
//		else
		src = vec3((value/255.0), (value/255.0), (value/255.0));
		float alpha = value/255.0;
		
		dst.r = max(src.r , dst.r);
		dst.g = max(src.g , dst.g);
		dst.b = max(src.b , dst.b);
		
		pos += 0.001 * rayDir;
		dist = length(pos - rayOrigin);
	}
////		FragColor= vec4(dst, 1.0);
	
//	FragColor = vec4((texelFetch(indexTexture, ivec3(0, 3, 0), 0)).x/633.0, 0.0, 0.0, 1.0);
//	FragColor = vec4((float((texelFetch(textureAtlas, ivec3(0,0,0), 1)).x)/255.0, 0.0, 1.3, 1.0));
//	vec4 blub = texelFetch(textureAtlas, ivec3(vPosition.x * 8, vPosition.y * 8, 32.0), 0);
//	FragColor = vec4(blub.x/255.0, blub.y/255.0, blub.z/255.0, 1.0);
//	FragColor = vec4(dst, 1.0);


//	vec3 rD;
//	rD.xy = 2.0 * gl_FragCoord.xy/windowSize -1.0;
//	rD.z = -focalLength;
//	rD = normalize((MVInverse * vec4(rD, 0)).xyz);
//	Ray r = Ray(camPosition.xyz , vec3(0.0,0.0,1.0));
//	
//	float tNear;
//	float tFar;
//	
//	intersectBox(r , tNear , tFar);
//	
//	if(tNear < 0.0 )
//		tNear = 0.0;

//	vec3 rayStart = r.origin + r.direction * tNear;
//	vec3 rayEnd   = r.origin + r.direction * tFar;
//	
//	rayStart = 0.5 * (rayStart + 1.0);
//	rayEnd 	 = 0.5 * (rayEnd + 1.0);
//	
//	vec3 pos = rayStart;
//	vec3 ste = normalize(rayEnd - rayStart) * stepSize; 
//	
//	float dist = distance(rayEnd , rayStart);

//	vec3 dst = vec3(0.0);
//	
//	for(int i = 0 ; i < numSamples && dist > 0.0 ; ++i , pos += ste, dist -= stepSize)
//	{
//		ivec3 index = ivec3(floor(pos.x * 256.0/8.0), floor(pos.y * 256.0/8.0), floor(pos.z * 256.0/8.0));
//		uvec4 bla = texelFetch(indexTexture , index, 0);
//		
////		vec4 posAtl = vec4 (fract(pos.x * 256.0/8.0), fract(pos.y * 256.0/8.0), fract(pos.z * 256.0/8.0), 8.0 * 8.0 * 8.0 * bla.x );
//		vec4 posAtl = vec4 ( (pos.x - bla.y)/bla.y, (pos.y - bla.z) / bla.z*8.0, (pos.z - bla.w) / bla.w *8.0, 8.0 * 8.0 * 8.0 * bla.x );
//		float value = trilinearSample(posAtl);
//		vec3 src = vec3((value/255.0), (value/255.0), (value/255.0));
//		float alpha = value/255.0;
//		
//		
//		
//		dst.r = max(src.r , dst.r);
//		dst.g = max(src.g , dst.g);
//		dst.b = max(src.b , dst.b);
//		
////		if(src.x > 0.0)
////			dst= vec3(0.0,1.0,0.0);
//	}	
		
//	ivec3 index = ivec3(floor(vPosition.x * 256.0/8.0), floor(vPosition.y * 256.0/8.0), floor(vPosition.z * 256.0/8.0));
//	uvec4 bla = texelFetch(indexTexture , index, 0);

//	int index = int(vPosition.x * 8 + vPosition.y * 8 * 8+ vPosition.z * 8 * 8 * 8);
//	uvec4 bla = texelFetch(textureAtlas , index);
//		
//	if(bla.x == 0)
//	FragColor = vec4(1.0, 0.0, 0.0, 1.0);
//	else if(bla.x == 1)
//	FragColor = vec4(0.0, 1.0, 0.0, 1.0);
//	else if(bla.x == 2)
//	FragColor = vec4(0.0, 0.0, 1.0, 1.0);
//	else if(bla.x == 3)
//	FragColor = vec4(1.0, 1.0, 0.0, 1.0);
//	else if(bla.x == 4)
//	FragColor = vec4(1.0, 0.0, 1.0, 1.0);
//	else if(bla.x == 5)
//	FragColor = vec4(0.0, 1.0, 1.0, 1.0);
//	else if(bla.x >= 6)
//	FragColor = vec4(1.0, 1.0, 1.0, 1.0);
//	else
//	FragColor = vec4(0.5, 0.5, 0.5, 1.0);
	
		FragColor= vec4(dst, 1.0);
}





bool intersectBox(in Ray r , out float t0 , out float t1)
{

	vec3 recDir = (1.0)/r.direction;
	
	vec3 tmp = recDir * (vec3(-1.0) - r.origin);
	vec3 tmp1 = recDir * (vec3(1.0) - r.origin);

	vec3 tMin = min(tmp , tmp1);
	vec3 tMax = max(tmp , tmp1);
	
	vec2 t = max(tMin.xx , tMin.yz);
	
	t0 = max(t.x , t.y);
	t= min(tMax.xx , tMax.yz);
	
	t1 = min(t.x , t.y);
	
	return (t0 <= t1);


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

float trilinearSample(in vec4 pos)
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

	float v000 = (texelFetch(textureAtlas, int(pos.w) + p000.x + 8 * p000.y + 8 * 8 * p000.z));
	float v001 = (texelFetch(textureAtlas, int(pos.w) + p001.x + 8 * p001.y + 8 * 8 * p001.z));
	float v010 = (texelFetch(textureAtlas, int(pos.w) + p010.x + 8 * p010.y + 8 * 8 * p010.z));
	float v011 = (texelFetch(textureAtlas, int(pos.w) + p011.x + 8 * p011.y + 8 * 8 * p011.z));
	float v100 = (texelFetch(textureAtlas, int(pos.w) + p100.x + 8 * p100.y + 8 * 8 * p100.z));
	float v101 = (texelFetch(textureAtlas, int(pos.w) + p101.x + 8 * p101.y + 8 * 8 * p101.z));
	float v110 = (texelFetch(textureAtlas, int(pos.w) + p110.x + 8 * p110.y + 8 * 8 * p110.z));
	float v111 = (texelFetch(textureAtlas, int(pos.w) + p111.x + 8 * p111.y + 8 * 8 * p111.z));

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










