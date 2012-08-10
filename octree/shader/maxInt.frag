#version 400


layout(location = 0, index = 0) 
out vec4 FragColor;

uniform usampler3D indexTexture;
uniform usamplerBuffer textureAtlas;
uniform sampler1D transferFunction;

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
	
	vec4 dst = vec4(0.0);
	float indWidth = width/float(BRICKSIZE);
	float indHeight= height/float(BRICKSIZE);
	float indDepth = depth/float(BRICKSIZE);
	
	float bricksizeMinusOne = float(BRICKSIZE) - 1.0;
	int brickSizeCube = BRICKSIZE * BRICKSIZE * BRICKSIZE;
	
//	if((vPosition.x <= 0.01 && vPosition.y <= 0.01) || (vPosition.x <= 0.01 && vPosition.z <= 0.01) || (vPosition.z <= 0.01 && vPosition.y <= 0.01) ||
//	   (vPosition.x >= 0.99 && vPosition.y >= 0.99) || (vPosition.x >= 0.99 && vPosition.z >= 0.99) || (vPosition.z >= 0.99 && vPosition.y >= 0.99) ||
//	   (vPosition.x >= 0.99 && vPosition.y <= 0.01) || (vPosition.x >= 0.99 && vPosition.z <= 0.01) || (vPosition.z >= 0.99 && vPosition.y <= 0.01) || 
//	   (vPosition.x <= 0.01 && vPosition.y >= 0.99) || (vPosition.x <= 0.01 && vPosition.z >= 0.99) || (vPosition.z <= 0.01 && vPosition.y >= 0.99) )
//	FragColor = vec4(0.0, 1.0, 0.0, 1.0);
//	else
//	{
	while (dist < maxDist)
	{
		ivec3 index = ivec3(floor(pos.x * indWidth), floor(pos.y * indHeight), floor(pos.z * indDepth));
		uvec4 bla = texelFetch(indexTexture , index, 0);
		
		if(bla.z == 0)
		{
			vec4 pos2 = vec4 ( fract(pos.x * bla.y) * bricksizeMinusOne, fract(pos.y * bla.y) * bricksizeMinusOne, fract(pos.z * bla.y) * bricksizeMinusOne, brickSizeCube * bla.x );

			float value = trilinearSample(pos2);
		
			vec4 src = texture(transferFunction, value * inverseVALUERANGE + 0.5 * inverseVALUERANGE);
			dst.r = max(src.r * src.a, dst.r);
			dst.g = max(src.g * src.a, dst.g);
			dst.b = max(src.b * src.a, dst.b);
//			dst.a = max(src.a, dst.a);
		
			// without transferfunction
//			float v = value*inverseVALUERANGE;
//			vec3 src = vec3(v, v, v);
//			float alpha = v;	
//			dst.r = max(src.r , dst.r);
//			dst.g = max(src.g, dst.g);
//			dst.b = max(src.b, dst.b);
		}
		pos += stepSize * rayDir;
		dist = length(pos - rayOrigin);
	}
	
	FragColor = dst;
//	}

//	FragColor = vec4(maxDist, maxDist, maxDist, 1.0);
//	FragColor = exitBB;
//	FragColor = rayOrigin;
}





void intersectBB(in vec4 rayDir, in vec4 rayOrigin, out vec4 exitBB)
{
//	float tmin, tmax, tymin, tymax, tzmin, tzmax;

//	vec3 boxMin = vec3(0.0);
//	vec3 boxMax = vec3(1.0);
//	vec3 recDir = vec3(vec3(1.0) / rayDir.xyz);

//	if (rayDir.x >= 0.0) 
//	{
//		tmax = (boxMax.x - rayOrigin.x) * recDir.x;
//	}
//	else 
//	{
//		tmax = (boxMin.x - rayOrigin.x) * recDir.x;
//	}

//	if (rayDir.y >= 0.0) 
//	{
//		tymax = (boxMax.y - rayOrigin.y) * recDir.y;
//	}
//	else 
//	{
//		tymax = (boxMin.y - rayOrigin.y) * recDir.y;
//	}
//	if (tymax < tmax) 
//	{
//		tmax = tymax;
//	}

//	if (rayDir.z >= 0.0) 
//	{
//		tzmax = (boxMax.z - rayOrigin.z) * recDir.z;
//	}
//	else 
//	{
//		tzmax = (boxMin.z - rayOrigin.z) * recDir.z;
//	}

//	if (tzmax < tmax) 
//	{
//		tmax = tzmax;
//	}
//	exitBB  = rayOrigin + (tmax * rayDir);
	
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
	
//	float v000 = (texelFetch(textureAtlas, int(pos.w) + p000.x + BRICKSIZE * p000.y + BRICKSIZE * BRICKSIZE * p000.z));
//	float v001 = (texelFetch(textureAtlas, int(pos.w) + p001.x + BRICKSIZE * p001.y + BRICKSIZE * BRICKSIZE * p001.z));
//	float v010 = (texelFetch(textureAtlas, int(pos.w) + p010.x + BRICKSIZE * p010.y + BRICKSIZE * BRICKSIZE * p010.z));
//	float v011 = (texelFetch(textureAtlas, int(pos.w) + p011.x + BRICKSIZE * p011.y + BRICKSIZE * BRICKSIZE * p011.z));
//	float v100 = (texelFetch(textureAtlas, int(pos.w) + p100.x + BRICKSIZE * p100.y + BRICKSIZE * BRICKSIZE * p100.z));
//	float v101 = (texelFetch(textureAtlas, int(pos.w) + p101.x + BRICKSIZE * p101.y + BRICKSIZE * BRICKSIZE * p101.z));
//	float v110 = (texelFetch(textureAtlas, int(pos.w) + p110.x + BRICKSIZE * p110.y + BRICKSIZE * BRICKSIZE * p110.z));
//	float v111 = (texelFetch(textureAtlas, int(pos.w) + p111.x + BRICKSIZE * p111.y + BRICKSIZE * BRICKSIZE * p111.z));

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










