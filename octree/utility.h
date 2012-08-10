#ifndef __UTILITY_H__
#define __UTILITY_H__


#define BRICKSIZE 64
#define CUTSIZE 1024
#define MAXREPLACEMENTS 128

#define BIT 16

#if BIT == 8
typedef unsigned char valueType;
#define TEXTURETYPE GL_R8UI
#define VALUERANGE 256

#elif BIT == 12
typedef unsigned short valueType;
#define TEXTURETYPE GL_R16UI
#define VALUERANGE 4096

#elif BIT == 16 
typedef unsigned short valueType;
#define TEXTURETYPE GL_R16UI
#define VALUERANGE 65536

#elif BIT == 32
typedef unsigned int valueType;
#define TEXTURETYPE GL_R32UI
#define VALUERANGE 4294967296

#endif

#endif /* __UTILITY_H__ */

