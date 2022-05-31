#pragma once  
#define GLUT_DISABLE_ATEXIT_HACK
//#include "glut.h"
#include "GL/glut.h"
#include<stdio.h>  
struct TextureTga            // ����һ���ṹ��  
{
	GLubyte *imageData;           // ͼ������ (���32bit)  
	GLuint bpp;            // ÿһ���ص�ͼ����ɫ���  
	GLuint width;            // ͼ����  
	GLuint height;            // ͼ��߶�  
	GLuint texID;            // ����ID  
};

// ����BMP,JPG,GIF���ļ�  
bool BuildTexture(const char *szPathName, GLuint &texid);

// ����TGA�ļ�  
bool BuildTexture(const char *filename, TextureTga *texture); 
