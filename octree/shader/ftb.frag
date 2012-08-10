#version 400


layout(location = 0, index = 0) 
out vec4 FragColor;

uniform usampler3D indexTexture;
uniform usamplerBuffer textureAtlas;
uniform samplerBuffer transferFunction;

uniform mat4 MVInverse;
uniform vec4 camPosition;

uniform float stepSize;

uniform int BRICKSIZE;
uniform float inverseVALUERANGE;

uniform float width;
uniform float height;
uniform float depth;


in vec4 vPosition;




void intersectBB(in vec4 rayDir, in vec4 rayOrigin, out vec4 exitBB);
float trilinearSample(in vec4 pos);


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
	float indWidth = width/float(BRICKSIZE);
	float indHeight= height/float(BRICKSIZE);
	float indDepth = depth/float(BRICKSIZE);
	
	float bricksizeMinusOne = float(BRICKSIZE) - 1.0;
	int brickSizeCube = BRICKSIZE * BRICKSIZE * BRICKSIZE;
	
//	float prevA = 1.0;
//	float transparency = 1.0;
	float accA = 0.0;
//	while (dist < maxDist && transparency > 0.1)
	while (dist < maxDist && accA < 0.95)
	{
		ivec3 index = ivec3(floor(pos.x * indWidth), floor(pos.y * indHeight), floor(pos.z * indDepth));
		uvec4 bla = texelFetch(indexTexture , index, 0);
		
		if(bla.z == 0)
		{
			vec4 pos2 = vec4 ( fract(pos.x * bla.y) * bricksizeMinusOne, fract(pos.y * bla.y) * bricksizeMinusOne, fract(pos.z * bla.y) * bricksizeMinusOne, brickSizeCube * bla.x );

			float value = trilinearSample(pos2);
		
			vec4 src = texelFetch(transferFunction, int(value));
			
//			transparency = transparency * prevA;
//			dst.r = transparency * src.r * src.a + dst.r;
//			dst.g = transparency * src.g * src.a + dst.g;
//			dst.b = transparency * src.b * src.a + dst.b;

//			prevA = 1.0 - src.a;

//			A'(i) = (1-A'(i-1))*A(i) + A'(i-1)
//			C'(i) = (1-A'(i-1))*C(i)*A(i)+C'(i-1)			
			dst  = (1.0 - accA) * src.xyz * src.a + dst;
			accA = (1.0 - accA) * src.a           + accA;
		}
		pos += stepSize * rayDir;
		dist = length(pos - rayOrigin);
	}
	
	FragColor = vec4(dst, accA);

}





void intersectBB(in vec4 rayDir, in vec4 rayOrigin, out vec4 exitBB)
{
	vec3 recDir = (1.0)/rayDir;
	
	vec3 tmp = recDir * (vec3(0.0) - rayOrigin);
	vec3 tmp1 = recDir * (vec3(1.0) - rayOrigin);

	vec3 tMin = min(tmp , tmp1);
	vec3 tMax = max(tmp , tmp1);
	
	vec2 t = max(tMin.xx , tMin.yz);
	
	float t0 = max(t.x , t.y);
	t = min(tMax.xx , tMax.yz);
	
	float t1 = min(t.x , t.y);
	
	exitBB = rayOrigin + max(t0, t1) * rayDir;
//	return (t0 <= t1);
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

	float v000 = (texelFetch(textureAtlas, int(pos.w) + fx + BRICKSIZE * fy + BRICKSIZE * BRICKSIZE * fz));
	float v001 = (texelFetch(textureAtlas, int(pos.w) + fx + BRICKSIZE * fy + BRICKSIZE * BRICKSIZE * cz));
	float v010 = (texelFetch(textureAtlas, int(pos.w) + fx + BRICKSIZE * cy + BRICKSIZE * BRICKSIZE * fz));
	float v011 = (texelFetch(textureAtlas, int(pos.w) + fx + BRICKSIZE * cy + BRICKSIZE * BRICKSIZE * cz));
	float v100 = (texelFetch(textureAtlas, int(pos.w) + cx + BRICKSIZE * fy + BRICKSIZE * BRICKSIZE * fz));
	float v101 = (texelFetch(textureAtlas, int(pos.w) + cx + BRICKSIZE * fy + BRICKSIZE * BRICKSIZE * cz));
	float v110 = (texelFetch(textureAtlas, int(pos.w) + cx + BRICKSIZE * cy + BRICKSIZE * BRICKSIZE * fz));
	float v111 = (texelFetch(textureAtlas, int(pos.w) + cx + BRICKSIZE * cy + BRICKSIZE * BRICKSIZE * cz));

	// 4 linear
	float l00 = mix(v000, v100, fract(pos.x));
	float l01 = mix(v001, v101, fract(pos.x));
	float l10 = mix(v010, v110, fract(pos.x));
	float l11 = mix(v011, v111, fract(pos.x));	

	// 2 bilinear
	float b0 = mix(l00, l10, fract(pos.y));
	float b1 = mix(l01, l11, fract(pos.y));
	
	// 1 trilinear
	return mix (b0, b1, fract(pos.z));

}










