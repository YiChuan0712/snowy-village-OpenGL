#define NDEBUG
#define UNICODE
#define _CRT_SECURE_NO_WARNINGS      
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")
#pragma comment(lib,"glut32.lib")
#include <stdlib.h>       
#include<time.h>    
#include<windows.h>
int FFlag = 1;//控制白天黑夜切换
int COUNT = 1;//控制音乐播放
//#include"stdafx.h"
#include <mmsystem.h>	// PlaySound()
#pragma commment(lib,"Winmm.lib");
#include"texture.h"  
#include"collision.h"
// 鼠标移动事件 
double moveX = 0;//鼠标X位置
double moveY = 0;//鼠标y位置
double Xx = 0; double Yy = 0;//记录鼠标移动位置
int FLAG = 1;//鼠标右键恢复初始状态
const int skySizeX = 300;
const int skySizeY = 100;
const int skySizeZ = 300;
int flakenum = 1000;
float scale[1024];
float centerx[1024];
float centery[1024];
float centerz[1024];
float rotateangle[1024];
float thickness = 1;
float RIGHT = -1;
float LEFT = 1;
float TOP = 30;//反
float BOTTOM = 0;
int angle = 0;//树桩旋转角度
int Flag = 0;//控制树桩旋转tab键
float XXX = 200, YYY = 200, ZZZ = 200;//控制白天黑夜
#define MAXFLAKE 1
#define MINFLAKE 0.1

int snowFlag = 1;

int getRand(int a, int b)
{
	return (rand() % (b - a + 1)) + a;
}

//对雪花的参数初始化
//包括雪花的大小 生成位置 生成范围等
void initFlakes()
{
	for (int i = 0; i < flakenum; i++)
	{
		scale[i] = getRand(MINFLAKE, MAXFLAKE);
		centerx[i] = getRand(-160, 160);
		centery[i] = getRand(-60, 60);
		centerz[i] = getRand(-160, 0);
	}
	//第一片雪花的位置和大小固定
	scale[0] = 2;
	centerx[0] = 0;
	centery[0] = 0;
	centerz[0] = 0;
}

void initFlakes_anime()
{
	for (int i = 0; i < flakenum; i++)
	{
		scale[i] = getRand(MINFLAKE, MAXFLAKE);
		centerx[i] = getRand(-80, 240);
		centery[i] = getRand(60, 180);
		centerz[i] = getRand(-160, 0);
	}
}

//画雪花 注意此函数只画出1/6雪花 之后会通过旋转得到完整的雪花
void drawFlake(int index)
{
	//glBegin(GL_LINE_LOOP);
	glBegin(GL_POLYGON);
	glVertex2f(0 * scale[index], 0 * scale[index]);

	glVertex2f(0.35*scale[index], 0.15*scale[index]);

	glVertex2f(0.4*scale[index], 0.0 * scale[index]);

	glVertex2f(0.35*scale[index], -0.15*scale[index]);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex2f(0.4*scale[index], 0.0 * scale[index]);

	glVertex2f(0.5*scale[index], 0.24*scale[index]);

	glVertex2f(0.5*scale[index], 0.0 * scale[index]);

	glVertex2f(0.5*scale[index], -0.24*scale[index]);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex2f(0.5*scale[index], 0.0 * scale[index]);

	glVertex2f(0.7*scale[index], 0.3*scale[index]);

	glVertex2f(0.6*scale[index], 0.0 * scale[index]);

	glVertex2f(0.7*scale[index], -0.3*scale[index]);

	glEnd();

	glBegin(GL_POLYGON);
	glVertex2f(0.6*scale[index], 0.0 * scale[index]);

	glVertex2f(0.9*scale[index], 0.1*scale[index]);

	glVertex2f(0.85*scale[index], 0 * scale[index]);

	glVertex2f(0.9*scale[index], -0.1*scale[index]);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex2f(0.85*scale[index], 0 * scale[index]);
	glVertex2f(1.2*scale[index], 0.1*scale[index]);

	glVertex2f(1.3*scale[index], -0.0 * scale[index]);

	glVertex2f(1.2*scale[index], -0.1*scale[index]);
	glEnd();

}

//画出一片完整的雪花
void displayOnce(int index)
{
	glPushMatrix();
	glTranslatef(centerx[index], centery[index], centerz[index]);
	float tempangle = 60;
	int rotatetimes = 360 / tempangle;
	glRotatef(rotateangle[index], 0, 0, 1);
	for (int i = 0; i < rotatetimes; i++)
	{
		glRotatef(tempangle, 0, 0, 1);
		drawFlake(index);
	}
	glPopMatrix();
}
void timerFunc(int value)
{

	for (int i = 0; i < flakenum; i++)
	{
		centery[i] -= scale[i] * 0.3;
		centerx[i] -= scale[i] * 0.1;
		rotateangle[i] += 5 / scale[i];
		if (centery[i] < -60 && snowFlag == 1)
		{
			centery[i] = 60;
			centerx[i] += 40;
		}
	}
	if (Flag)
	{
		angle--; Flag = 0;
	}

	glutPostRedisplay();
	glutTimerFunc(30, timerFunc, 1);

}
#define BUFSIZE 100  
GLuint texture[20];
GLint houseList = 0;
GLint baseList = 0;
//视区      
float whRatio;
int wHeight = 0;
int wWidth = 0;


//视点      
float center[] = { 0, 0, 0 };
float eye[] = { 0, 0, 50 };
GLfloat dx = 0, dy = 0, dz = 0;
GLfloat ax = 0, ay = 0, az = 0;

float ratio = 180;

int rotate_state = 1;
bool isRotate = false;


GLint GenTableList()
{
	GLint lid = glGenLists(1);

	glNewList(lid, GL_COMPILE);
	glPushMatrix();
	glTranslatef(0, -1.0f*skySizeY / 2.0f - 2.0f, -1.0f*skySizeZ / 2.0f + 60.0f);
	glScalef(3.0f, 3.0f, 3.0f);
	glRotatef(90, 0, 1, 0);
	//loadObj(housename, housemap, house_matname);
	glPopMatrix();

	glNewList(lid, GL_COMPILE);
	glPushMatrix();
	glTranslatef(5.0f, -1.0f*skySizeY / 2.0f, -23.0f);
	glScalef(0.8f, 0.8f, 0.8f);
	//loadObj(basename, basemap,base_matname);
	glPopMatrix();

	glEndList();
	return lid;
}

int col = 4, num = 12;

void drawRect(GLuint texture, int i, int j)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);  //选择纹理texture[status]       

	const GLfloat x1 = -0.5, x2 = 0.5;
	const GLfloat y1 = -0.5, y2 = 0.5;
	const GLfloat x = 1.0 / col, y = 1.0 / (num / col);
	const GLfloat point[4][2] = { { x1,y1 }, { x2,y1 }, { x2,y2 }, { x1,y2 } };
	const GLfloat dir[4][2] = { { j*x,1 - (i + 1)*y }, { (j + 1)*x,1 - (i + 1)*y }, { (j + 1)*x ,1 - i * y }, { j*x,1 - i * y } };

	glBegin(GL_QUADS);

	for (int k = 0; k < 4; k++)
	{
		glTexCoord2fv(dir[k]);//用来定义纹理坐标
		glVertex2fv(point[k]);//用来定义几何顶点坐标
	}

	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void drawTop(GLuint texture)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);  //选择纹理texture[status]       
	int i = 0, j = 0;

	const GLfloat x = 1.0 / col, y = 1.0 / (num / col);
	const GLfloat x0[] = { 0,x,2 * x,3 * x,4 * x };
	const GLfloat x1 = -0.5f, x2 = 0.5f;
	const GLfloat y1 = -0.5f, y2 = 0.5f;
	const GLfloat point[12][2] = {
		{ x2,y1 },{ 0,0 },{ x1,y1 },
		{ x1,y1 },{ 0,0 },{ x1,y2 },
		{ x1,y2 },{ 0,0 },{ x2,y2 },
		{ x2,y2 },{ 0,0 },{ x2,y1 },
	};
	const GLfloat dir[12][2] = { { x0[0],1 - y },{ x0[2],1 },{ x0[1],1 - y },
	{ x0[1],1 - y },{ x0[2],1 },{ x0[2],1 - y },
	{ x0[2],1 - y },{ x0[2],1 },{ x0[3],1 - y },
	{ x0[3],1 - y },{ x0[2],1 },{ x0[4],1 - y } };

	glBegin(GL_TRIANGLES);

	for (int k = 0; k < 12; k++) {
		glTexCoord2fv(dir[k]);
		glVertex2fv(point[k]);
	}
	glEnd();


	glDisable(GL_TEXTURE_2D);
}

void drawBottom(GLuint texture)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);  //选择纹理texture[status]       
	int i = 0, j = 0;

	const GLfloat x = 1.0 / col, y = 1.0 / (num / col);
	const GLfloat x0[] = { 0,x,2 * x,3 * x,4 * x };
	const GLfloat x1 = -0.5f, x2 = 0.5f;
	const GLfloat y1 = -0.5f, y2 = 0.5f;
	const GLfloat point[12][2] = {
		{ x2,y1 },{ 0,0 },{ x1,y1 },
		{ x1,y1 },{ 0,0 },{ x1,y2 },
		{ x1,y2 },{ 0,0 },{ x2,y2 },
		{ x2,y2 },{ 0,0 },{ x2,y1 },
	};
	const GLfloat dir[12][2] = { { x0[0],y },{ x0[2],0 },{ x0[1],y },
	{ x0[1],y },{ x0[2],0 },{ x0[2],y },
	{ x0[2],y },{ x0[2],0 },{ x0[3],y },
	{ x0[3],y },{ x0[2],0 },{ x0[4],y } };

	glBegin(GL_TRIANGLES);

	for (int k = 0; k < 12; k++) {
		glTexCoord2fv(dir[k]);
		glVertex2fv(point[k]);
	}
	glEnd();


	glDisable(GL_TEXTURE_2D);
}

void drawRect(GLuint texture)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);  //选择纹理texture[status]       
	const GLfloat x1 = -0.5, x2 = 0.5;
	const GLfloat y1 = -0.5, y2 = 0.5;
	const GLfloat point[4][2] = { { x1,y1 },{ x2,y1 },{ x2,y2 },{ x1,y2 } };
	int dir[4][2] = { { 0,0 },{ 1,0 },{ 1,1 },{ 0,1 } };
	glBegin(GL_QUADS);

	for (int i = 0; i < 4; i++) {
		glTexCoord2iv(dir[i]);
		glVertex2fv(point[i]);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
}


void DrawCube1(void)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[4]);        //使用贴图纹理

	glPushMatrix();        //压入变换矩阵

	glRotatef(0, 1.0f, 0.0f, 0.0f);            //旋转矩阵，这里绕x轴旋转。
	glRotatef(30, 0.0f, 1.0f, 0.0f);            //旋转矩阵，这里绕y轴旋转。
	glRotatef(0, 0.0f, 0.0f, 1.0f);            //绕z轴旋转，这里zrot是角度制的度数。

	glTranslatef(30, -50, -130);

	glBegin(GL_QUADS);  //启用四边形带绘制模式绘制

	// 绘制前面，这里开始确定纹理坐标，然后是确定点的位置
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-40, -20, 15);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(40, -20, 15);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(40, 40, 15);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-40, 40, 15);

	// 绘制后面
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-40, -20, -15);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-40, 40, -15);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(40, 40, -15);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(40, -20, -15);

	// 上面
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-40, 40, -15);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-40, 40, 15);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(40, 40, 15);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(40, 40, -15);

	//底面
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-40, -20, -15);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(40, -20, -15);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(40, -20, 15);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-40, -20, 15);

	// 右面
	glTexCoord2f(1.0f, 0.0f); glVertex3f(40, -20, -15);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(40, 40, -15);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(40, 40, 15);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(40, -20, 15);

	// 左面
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-40, -20, -15);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-40, -20, 15);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-40, 40, 15);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-40, 40, -15);

	glEnd();

	glPopMatrix(); //弹出变换矩阵

	glDisable(GL_TEXTURE_2D);

}

void DrawCube1_1(void)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[5]);        //使用贴图纹理

	glPushMatrix();        //压入变换矩阵

	glRotatef(0, 1.0f, 0.0f, 0.0f);            //旋转矩阵，这里绕x轴旋转。
	glRotatef(30, 0.0f, 1.0f, 0.0f);            //旋转矩阵，这里绕y轴旋转。
	glRotatef(0, 0.0f, 0.0f, 1.0f);            //绕z轴旋转，这里zrot是角度制的度数。

	glTranslatef(30, -10, -130);

	glBegin(GL_QUADS);  //启用四边形带绘制模式绘制

	float bottomL = 44;
	float bottomW = 20;
	float bottomH = 0.01;

	float topL = 44;
	float topW = 1;
	float topH = 18;

	// 绘制前面，这里开始确定纹理坐标，然后是确定点的位置
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-bottomL, -bottomH, bottomW);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(bottomL, -bottomH, bottomW);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(topL, topH, topW);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-topL, topH, topW);

	// 绘制后面
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-bottomL, -bottomH, -bottomW);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-topL, topH, -topW);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(topL, topH, -topW);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(bottomL, -bottomH, -bottomW);

	// 上面
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-topL, topH, -topW);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-topL, topH, topW);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(topL, topH, topW);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(topL, topH, -topW);

	//底面
	/*glTexCoord2f(1.0f, 1.0f); glVertex3f(-bottomL, -bottomH, -bottomW);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(bottomL, -bottomH, -bottomW);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(bottomL, -bottomH, bottomW);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-bottomL, -bottomH, bottomW);*/

	// 右面
	glTexCoord2f(1.0f, 0.0f); glVertex3f(bottomL, -bottomH, -bottomW);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(topL, topH, -topW);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(topL, topH, topW);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(bottomL, -bottomH, bottomW);

	// 左面
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-bottomL, -bottomH, -bottomW);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-bottomL, -bottomH, bottomW);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-topL, topH, topW);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-topL, topH, -topW);

	glEnd();

	glPopMatrix(); //弹出变换矩阵

	glDisable(GL_TEXTURE_2D);
}

void DrawCube1_2(void)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[5]);        //使用贴图纹理

	glPushMatrix();        //压入变换矩阵

	glRotatef(0, 1.0f, 0.0f, 0.0f);            //旋转矩阵，这里绕x轴旋转。
	glRotatef(30, 0.0f, 1.0f, 0.0f);            //旋转矩阵，这里绕y轴旋转。
	glRotatef(0, 0.0f, 0.0f, 1.0f);            //绕z轴旋转，这里zrot是角度制的度数。

	glTranslatef(30, -10, -130);

	glBegin(GL_QUADS);  //启用四边形带绘制模式绘制

	float bottomL = 45;
	float bottomW = 35;
	float bottomH = 20;

	float topL = 40;
	float topW = 30;
	float topH = -15;

	// 绘制前面，这里开始确定纹理坐标，然后是确定点的位置
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-bottomL, -bottomH, bottomW);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(bottomL, -bottomH, bottomW);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(topL, topH, topW);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-topL, topH, topW);

	// 绘制后面
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-bottomL, -bottomH, -bottomW);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-topL, topH, -topW);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(topL, topH, -topW);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(bottomL, -bottomH, -bottomW);

	// 上面
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-topL, topH, -topW);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-topL, topH, topW);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(topL, topH, topW);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(topL, topH, -topW);

	//底面
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-bottomL, -bottomH, -bottomW);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(bottomL, -bottomH, -bottomW);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(bottomL, -bottomH, bottomW);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-bottomL, -bottomH, bottomW);

	// 右面
	glTexCoord2f(1.0f, 0.0f); glVertex3f(bottomL, -bottomH, -bottomW);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(topL, topH, -topW);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(topL, topH, topW);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(bottomL, -bottomH, bottomW);

	// 左面
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-bottomL, -bottomH, -bottomW);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-bottomL, -bottomH, bottomW);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-topL, topH, topW);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-topL, topH, -topW);

	glEnd();

	glPopMatrix(); //弹出变换矩阵

	glDisable(GL_TEXTURE_2D);
}

void DrawCube1_3(void)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[5]);        //使用贴图纹理

	glPushMatrix();        //压入变换矩阵

	glRotatef(0, 1.0f, 0.0f, 0.0f);            //旋转矩阵，这里绕x轴旋转。
	glRotatef(30, 0.0f, 1.0f, 0.0f);            //旋转矩阵，这里绕y轴旋转。
	glRotatef(0, 0.0f, 0.0f, 1.0f);            //绕z轴旋转，这里zrot是角度制的度数。

	glTranslatef(30, -87, -130);

	glBegin(GL_QUADS);  //启用四边形带绘制模式绘制

	// 绘制前面，这里开始确定纹理坐标，然后是确定点的位置
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-41, -20, 16);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(41, -20, 16);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(41, 40, 16);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-41, 40, 16);

	// 绘制后面
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-41, -20, -16);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-41, 40, -16);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(41, 40, -16);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(41, -20, -16);

	// 上面
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-41, 40, -16);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-41, 40, 16);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(41, 40, 16);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(41, 40, -16);

	//底面
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-41, -20, -16);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(41, -20, -16);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(41, -20, 16);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-41, -20, 16);

	// 右面
	glTexCoord2f(1.0f, 0.0f); glVertex3f(41, -20, -16);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(41, 40, -16);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(41, 40, 16);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(41, -20, 16);

	// 左面
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-41, -20, -16);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-41, -20, 16);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-41, 40, 16);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-41, 40, -16);

	glEnd();

	glPopMatrix(); //弹出变换矩阵

	glDisable(GL_TEXTURE_2D);

}

void DrawDoor(void)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[6]);        //使用贴图纹理

	glPushMatrix();        //压入变换矩阵

	glRotatef(0, 1.0f, 0.0f, 0.0f);            //旋转矩阵，这里绕x轴旋转。
	glRotatef(-60, 0.0f, 1.0f, 0.0f);            //旋转矩阵，这里绕y轴旋转。
	glRotatef(0, 0.0f, 0.0f, 1.0f);            //绕z轴旋转，这里zrot是角度制的度数。

	glTranslatef(-119.99, -40, -29);

	glBegin(GL_QUADS);  //启用四边形带绘制模式绘制


	// 右面
	glTexCoord2f(1.0f, 0.0f); glVertex3f(15, -10, -4.5);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(15, 7, -4.5);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(15, 7, 4.5);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(15, -10, 4.5);


	glEnd();

	glPopMatrix(); //弹出变换矩阵

	glDisable(GL_TEXTURE_2D);

}

void DrawWindow1(void)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);        //使用贴图纹理

	glPushMatrix();        //压入变换矩阵

	glRotatef(0, 1.0f, 0.0f, 0.0f);            //旋转矩阵，这里绕x轴旋转。
	glRotatef(-60, 0.0f, 1.0f, 0.0f);            //旋转矩阵，这里绕y轴旋转。
	glRotatef(0, 0.0f, 0.0f, 1.0f);            //绕z轴旋转，这里zrot是角度制的度数。

	glTranslatef(-119.99, -17, -29);

	glBegin(GL_QUADS);  //启用四边形带绘制模式绘制


	// 右面
	glTexCoord2f(1.0f, 0.0f); glVertex3f(15, -4.5, -4.5);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(15, 4.5, -4.5);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(15, 4.5, 4.5);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(15, -4.5, 4.5);


	glEnd();

	glPopMatrix(); //弹出变换矩阵

	glDisable(GL_TEXTURE_2D);

}

void DrawWindow2(void)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);        //使用贴图纹理

	glPushMatrix();        //压入变换矩阵

	glRotatef(0, 1.0f, 0.0f, 0.0f);            //旋转矩阵，这里绕x轴旋转。
	glRotatef(-60, 0.0f, 1.0f, 0.0f);            //旋转矩阵，这里绕y轴旋转。
	glRotatef(0, 0.0f, 0.0f, 1.0f);            //绕z轴旋转，这里zrot是角度制的度数。

	glTranslatef(-129, -17, -1);

	glBegin(GL_QUADS);  //启用四边形带绘制模式绘制


	// 右面
	glTexCoord2f(1.0f, 0.0f); glVertex3f(15, -4.5, -4.5);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(15, 4.5, -4.5);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(15, 4.5, 4.5);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(15, -4.5, 4.5);


	glEnd();

	glPopMatrix(); //弹出变换矩阵

	glDisable(GL_TEXTURE_2D);

}

void DrawWindow3(void)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);        //使用贴图纹理

	glPushMatrix();        //压入变换矩阵

	glRotatef(0, 1.0f, 0.0f, 0.0f);            //旋转矩阵，这里绕x轴旋转。
	glRotatef(-60, 0.0f, 1.0f, 0.0f);            //旋转矩阵，这里绕y轴旋转。
	glRotatef(0, 0.0f, 0.0f, 1.0f);            //绕z轴旋转，这里zrot是角度制的度数。

	glTranslatef(-129, -17, -57);

	glBegin(GL_QUADS);  //启用四边形带绘制模式绘制


	// 右面
	glTexCoord2f(1.0f, 0.0f); glVertex3f(15, -4.5, -4.5);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(15, 4.5, -4.5);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(15, 4.5, 4.5);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(15, -4.5, 4.5);


	glEnd();

	glPopMatrix(); //弹出变换矩阵

	glDisable(GL_TEXTURE_2D);

}

void DrawWindow4(void)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);        //使用贴图纹理

	glPushMatrix();        //压入变换矩阵

	glRotatef(0, 1.0f, 0.0f, 0.0f);            //旋转矩阵，这里绕x轴旋转。
	glRotatef(-60, 0.0f, 1.0f, 0.0f);            //旋转矩阵，这里绕y轴旋转。
	glRotatef(0, 0.0f, 0.0f, 1.0f);            //绕z轴旋转，这里zrot是角度制的度数。

	glTranslatef(-129, -37, -1);

	glBegin(GL_QUADS);  //启用四边形带绘制模式绘制


	// 右面
	glTexCoord2f(1.0f, 0.0f); glVertex3f(15, -4.5, -4.5);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(15, 4.5, -4.5);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(15, 4.5, 4.5);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(15, -4.5, 4.5);


	glEnd();

	glPopMatrix(); //弹出变换矩阵

	glDisable(GL_TEXTURE_2D);

}

void DrawWindow5(void)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);        //使用贴图纹理

	glPushMatrix();        //压入变换矩阵

	glRotatef(0, 1.0f, 0.0f, 0.0f);            //旋转矩阵，这里绕x轴旋转。
	glRotatef(-60, 0.0f, 1.0f, 0.0f);            //旋转矩阵，这里绕y轴旋转。
	glRotatef(0, 0.0f, 0.0f, 1.0f);            //绕z轴旋转，这里zrot是角度制的度数。

	glTranslatef(-129, -37, -57);

	glBegin(GL_QUADS);  //启用四边形带绘制模式绘制


	// 右面
	glTexCoord2f(1.0f, 0.0f); glVertex3f(15, -4.5, -4.5);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(15, 4.5, -4.5);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(15, 4.5, 4.5);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(15, -4.5, 4.5);


	glEnd();

	glPopMatrix(); //弹出变换矩阵

	glDisable(GL_TEXTURE_2D);

}

void DrawCube2(void)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[4]);        //使用贴图纹理

	glPushMatrix();        //压入变换矩阵

	glRotatef(0, 1.0f, 0.0f, 0.0f);            //旋转矩阵，这里绕x轴旋转。
	glRotatef(-60, 0.0f, 1.0f, 0.0f);            //旋转矩阵，这里绕y轴旋转。
	glRotatef(0, 0.0f, 0.0f, 1.0f);            //绕z轴旋转，这里zrot是角度制的度数。

	glTranslatef(-120, -40, -29);

	glBegin(GL_QUADS);  //启用四边形带绘制模式绘制

	// 绘制前面，这里开始确定纹理坐标，然后是确定点的位置
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-20, -10, 15);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(15, -10, 15);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(15, 25, 15);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-20, 25, 15);

	// 绘制后面
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-20, -10, -15);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-20, 25, -15);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(15, 25, -15);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(15, -10, -15);

	// 上面
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-20, 25, -15);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-20, 25, 15);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(15, 25, 15);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(15, 25, -15);

	//底面
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-20, -10, -15);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(15, -10, -15);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(15, -10, 15);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-20, -10, 15);

	// 右面
	glTexCoord2f(1.0f, 0.0f); glVertex3f(15, -10, -15);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(15, 25, -15);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(15, 25, 15);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(15, -10, 15);

	// 左面
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-20, -10, -15);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-20, -10, 15);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-20, 25, 15);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-20, 25, -15);

	glEnd();

	glPopMatrix(); //弹出变换矩阵

	glDisable(GL_TEXTURE_2D);

}

void DrawCube2_1(void)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[5]);        //使用贴图纹理

	glPushMatrix();        //压入变换矩阵

	glRotatef(0, 1.0f, 0.0f, 0.0f);            //旋转矩阵，这里绕x轴旋转。
	glRotatef(-60, 0.0f, 1.0f, 0.0f);            //旋转矩阵，这里绕y轴旋转。
	glRotatef(0, 0.0f, 0.0f, 1.0f);            //绕z轴旋转，这里zrot是角度制的度数。

	glTranslatef(-120, 5, -29);

	glBegin(GL_QUADS);  //启用四边形带绘制模式绘制

	float bottomL = 20;
	float bottomW = 20;
	float bottomH = 20;

	float topL = 10;
	float topW = 10;
	float topH = -5;

	// 绘制前面，这里开始确定纹理坐标，然后是确定点的位置
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-bottomL, -bottomH, bottomW);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(bottomL, -bottomH, bottomW);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(topL, topH, topW);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-topL, topH, topW);

	// 绘制后面
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-bottomL, -bottomH, -bottomW);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-topL, topH, -topW);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(topL, topH, -topW);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(bottomL, -bottomH, -bottomW);

	// 上面
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-topL, topH, -topW);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-topL, topH, topW);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(topL, topH, topW);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(topL, topH, -topW);

	//底面
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-bottomL, -bottomH, -bottomW);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(bottomL, -bottomH, -bottomW);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(bottomL, -bottomH, bottomW);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-bottomL, -bottomH, bottomW);

	// 右面
	glTexCoord2f(1.0f, 0.0f); glVertex3f(bottomL, -bottomH, -bottomW);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(topL, topH, -topW);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(topL, topH, topW);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(bottomL, -bottomH, bottomW);

	// 左面
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-bottomL, -bottomH, -bottomW);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-bottomL, -bottomH, bottomW);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-topL, topH, topW);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-topL, topH, -topW);

	glEnd();

	glPopMatrix(); //弹出变换矩阵

	glDisable(GL_TEXTURE_2D);

}

//柱子
void DrawCube4(void)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[5]);        //使用贴图纹理

	glPushMatrix();        //压入变换矩阵

	glRotatef(0, 1.0f, 0.0f, 0.0f);            //旋转矩阵，这里绕x轴旋转。
	glRotatef(-60, 0.0f, 1.0f, 0.0f);            //旋转矩阵，这里绕y轴旋转。
	glRotatef(0, 0.0f, 0.0f, 1.0f);            //绕z轴旋转，这里zrot是角度制的度数。

	glTranslatef(-98, -40, -66);

	glBegin(GL_QUADS);  //启用四边形带绘制模式绘制

	// 绘制前面，这里开始确定纹理坐标，然后是确定点的位置
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1, -10, 1);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1, -10, 1);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1, 10, 1);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1, 10, 1);

	// 绘制后面
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1, -10, -1);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1, 10, -1);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1, 10, -1);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1, -10, -1);

	// 上面
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1, 10, -1);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1, 10, 1);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1, 10, 1);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1, 10, -1);

	//底面
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1, -10, -1);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1, -10, -1);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1, -10, 1);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1, -10, 1);

	// 右面
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1, -10, -1);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1, 10, -1);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1, 10, 1);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1, -10, 1);

	// 左面
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1, -10, -1);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1, -10, 1);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1, 10, 1);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1, 10, -1);

	glEnd();

	glPopMatrix(); //弹出变换矩阵

	glDisable(GL_TEXTURE_2D);

}

//柱子
void DrawCube3(void)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[5]);        //使用贴图纹理

	glPushMatrix();        //压入变换矩阵

	glRotatef(0, 1.0f, 0.0f, 0.0f);            //旋转矩阵，这里绕x轴旋转。
	glRotatef(-60, 0.0f, 1.0f, 0.0f);            //旋转矩阵，这里绕y轴旋转。
	glRotatef(0, 0.0f, 0.0f, 1.0f);            //绕z轴旋转，这里zrot是角度制的度数。

	glTranslatef(-98, -40, 6);

	glBegin(GL_QUADS);  //启用四边形带绘制模式绘制

	// 绘制前面，这里开始确定纹理坐标，然后是确定点的位置
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1, -10, 1);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1, -10, 1);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1, 10, 1);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1, 10, 1);

	// 绘制后面
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1, -10, -1);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1, 10, -1);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1, 10, -1);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1, -10, -1);

	// 上面
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1, 10, -1);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1, 10, 1);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1, 10, 1);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1, 10, -1);

	//底面
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1, -10, -1);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1, -10, -1);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1, -10, 1);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1, -10, 1);

	// 右面
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1, -10, -1);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1, 10, -1);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1, 10, 1);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1, -10, 1);

	// 左面
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1, -10, -1);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1, -10, 1);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1, 10, 1);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1, 10, -1);

	glEnd();

	glPopMatrix(); //弹出变换矩阵

	glDisable(GL_TEXTURE_2D);

}
void Draw()
{
	DrawCube1(); DrawCube1_1(); DrawCube1_2(); DrawCube1_3();

	DrawCube2();
	DrawCube2_1();

	DrawCube3();

	DrawCube4();
}


//灯柱子
void DrawCube5(void)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[5]);        //使用贴图纹理

	glPushMatrix();        //压入变换矩阵

	glRotatef(0, 1.0f, 0.0f, 0.0f);            //旋转矩阵，这里绕x轴旋转。
	glRotatef(-60, 0.0f, 1.0f, 0.0f);            //旋转矩阵，这里绕y轴旋转。
	glRotatef(0, 0.0f, 0.0f, 1.0f);            //绕z轴旋转，这里zrot是角度制的度数。

	glTranslatef(-80, -40, -78);

	glBegin(GL_QUADS);  //启用四边形带绘制模式绘制

	float thickness = 0.6;

	// 绘制前面，这里开始确定纹理坐标，然后是确定点的位置
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-thickness, -10, thickness);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(thickness, -10, thickness);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(thickness, 20, thickness);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-thickness, 20, thickness);

	// 绘制后面
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-thickness, -10, -thickness);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-thickness, 20, -thickness);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(thickness, 20, -thickness);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(thickness, -10, -thickness);

	// 上面
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-thickness, 20, -thickness);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-thickness, 20, thickness);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(thickness, 20, thickness);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(thickness, 20, -thickness);

	//底面
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-thickness, -10, -thickness);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(thickness, -10, -thickness);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(thickness, -10, thickness);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-thickness, -10, thickness);

	// 右面
	glTexCoord2f(1.0f, 0.0f); glVertex3f(thickness, -10, -thickness);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(thickness, 20, -thickness);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(thickness, 20, thickness);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(thickness, -10, thickness);

	// 左面
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-thickness, -10, -thickness);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-thickness, -10, thickness);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-thickness, 20, thickness);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-thickness, 20, -thickness);

	glEnd();

	glPopMatrix(); //弹出变换矩阵

	glDisable(GL_TEXTURE_2D);

}

//灯箱
void DrawCube6(void)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[2]);        //使用贴图纹理
	glColor3f(1, 1, 0);
	glPushMatrix();        //压入变换矩阵

	glRotatef(0, 1.0f, 0.0f, 0.0f);            //旋转矩阵，这里绕x轴旋转。
	glRotatef(-60, 0.0f, 1.0f, 0.0f);            //旋转矩阵，这里绕y轴旋转。
	glRotatef(0, 0.0f, 0.0f, 1.0f);            //绕z轴旋转，这里zrot是角度制的度数。

	glTranslatef(-80, -40, -78);

	glBegin(GL_QUADS);  //启用四边形带绘制模式绘制

	// 绘制前面，这里开始确定纹理坐标，然后是确定点的位置
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.5, 18, 1.5);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.5, 18, 1.5);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(2, 23, 2);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-2, 23, 2);

	// 绘制后面
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.5, 18, -1.5);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-2, 23, -2);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(2, 23, -2);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.5, 18, -1.5);

	// 上面
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-2, 23, -2);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-2, 23, 2);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(2, 23, 2);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(2, 23, -2);

	//底面
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.5, 18, -1.5);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.5, 18, -1.5);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.5, 18, 1.5);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.5, 18, 1.5);

	// 右面
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.5, 18, -1.5);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(2, 23, -2);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(2, 23, 2);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.5, 18, 1.5);

	// 左面
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.5, 18, -1.5);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.5, 18, 1.5);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-2, 23, 2);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-2, 23, -2);

	glEnd();

	glPopMatrix(); //弹出变换矩阵

	glDisable(GL_TEXTURE_2D);
	glColor4f(1.0, 1.0, 1.0, 1.0f);

}

//灯盖1
void DrawCube7(void)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[5]);        //使用贴图纹理

	glPushMatrix();        //压入变换矩阵

	glRotatef(0, 1.0f, 0.0f, 0.0f);            //旋转矩阵，这里绕x轴旋转。
	glRotatef(-60, 0.0f, 1.0f, 0.0f);            //旋转矩阵，这里绕y轴旋转。
	glRotatef(0, 0.0f, 0.0f, 1.0f);            //绕z轴旋转，这里zrot是角度制的度数。

	glTranslatef(-80, -40, -78);

	glBegin(GL_QUADS);  //启用四边形带绘制模式绘制

	// 绘制前面，这里开始确定纹理坐标，然后是确定点的位置
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-2.5, 23, 2.5);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(2.5, 23, 2.5);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(2.5, 24, 2.5);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-2.5, 24, 2.5);

	// 绘制后面
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-2.5, 23, -2.5);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-2.5, 24, -2.5);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(2.5, 24, -2.5);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(2.5, 23, -2.5);

	// 上面
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-2.5, 24, -2.5);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-2.5, 24, 2.5);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(2.5, 24, 2.5);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(2.5, 24, -2.5);

	//底面
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-2.5, 23, -2.5);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(2.5, 23, -2.5);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(2.5, 23, 2.5);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-2.5, 23, 2.5);

	// 右面
	glTexCoord2f(1.0f, 0.0f); glVertex3f(2.5, 23, -2.5);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(2.5, 24, -2.5);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(2.5, 24, 2.5);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(2.5, 23, 2.5);

	// 左面
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-2.5, 23, -2.5);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-2.5, 23, 2.5);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-2.5, 24, 2.5);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-2.5, 24, -2.5);

	glEnd();

	glPopMatrix(); //弹出变换矩阵

	glDisable(GL_TEXTURE_2D);

}

//灯盖2
void DrawCube8(void)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[5]);        //使用贴图纹理

	glPushMatrix();        //压入变换矩阵

	glRotatef(0, 1.0f, 0.0f, 0.0f);            //旋转矩阵，这里绕x轴旋转。
	glRotatef(-60, 0.0f, 1.0f, 0.0f);            //旋转矩阵，这里绕y轴旋转。
	glRotatef(0, 0.0f, 0.0f, 1.0f);            //绕z轴旋转，这里zrot是角度制的度数。

	glTranslatef(-80, -40, -78);

	glBegin(GL_QUADS);  //启用四边形带绘制模式绘制

	// 绘制前面，这里开始确定纹理坐标，然后是确定点的位置
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5, 24, 0.5);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5, 24, 0.5);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5, 25, 0.5);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5, 25, 0.5);

	// 绘制后面
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5, 24, -0.5);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5, 25, -0.5);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.5, 25, -0.5);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.5, 24, -0.5);

	// 上面
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5, 25, -0.5);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5, 25, 0.5);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5, 25, 0.5);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5, 25, -0.5);

	//底面
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5, 24, -0.5);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.5, 24, -0.5);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.5, 24, 0.5);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5, 24, 0.5);

	// 右面
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5, 24, -0.5);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5, 25, -0.5);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.5, 25, 0.5);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.5, 24, 0.5);

	// 左面
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5, 24, -0.5);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5, 24, 0.5);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5, 25, 0.5);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5, 25, -0.5);

	glEnd();

	glPopMatrix(); //弹出变换矩阵

	glDisable(GL_TEXTURE_2D);

}

void DrawLight()
{
	DrawCube5();
	DrawCube6();
	DrawCube7();
	DrawCube8();
}

// 树干1
void DrawCube9(void)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[5]);        //使用贴图纹理

	glPushMatrix();        //压入变换矩阵

	glRotatef(0, 1.0f, 0.0f, 0.0f);            //旋转矩阵，这里绕x轴旋转。
	glRotatef(-60, 0.0f, 1.0f, 0.0f);            //旋转矩阵，这里绕y轴旋转。
	glRotatef(0, 0.0f, 0.0f, 1.0f);            //绕z轴旋转，这里zrot是角度制的度数。

	//glTranslatef(-90, -10, -50);
	glTranslatef(-80, -40, -115);

	glBegin(GL_QUADS);  //启用四边形带绘制模式绘制

	float bottomL = 3;
	float bottomW = 2;
	float bottomH = 50;

	float topL = 1;
	float topW = 1;
	float topH = 30;

	// 绘制前面，这里开始确定纹理坐标，然后是确定点的位置
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-bottomL, -bottomH, bottomW);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(bottomL, -bottomH, bottomW);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(topL, topH, topW);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-topL, topH, topW);

	// 绘制后面
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-bottomL, -bottomH, -bottomW);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-topL, topH, -topW);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(topL, topH, -topW);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(bottomL, -bottomH, -bottomW);

	// 上面
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-topL, topH, -topW);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-topL, topH, topW);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(topL, topH, topW);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(topL, topH, -topW);

	//底面
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-bottomL, -bottomH, -bottomW);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(bottomL, -bottomH, -bottomW);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(bottomL, -bottomH, bottomW);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-bottomL, -bottomH, bottomW);

	// 右面
	glTexCoord2f(1.0f, 0.0f); glVertex3f(bottomL, -bottomH, -bottomW);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(topL, topH, -topW);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(topL, topH, topW);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(bottomL, -bottomH, bottomW);

	// 左面
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-bottomL, -bottomH, -bottomW);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-bottomL, -bottomH, bottomW);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-topL, topH, topW);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-topL, topH, -topW);

	glEnd();

	glPopMatrix(); //弹出变换矩阵

	glDisable(GL_TEXTURE_2D);
}

void DrawCube15(void)

{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[5]);        //使用贴图纹理

	glPushMatrix();        //压入变换矩阵

	glTranslatef(59, -37, -130);

	glRotatef(0, 1.0f, 0.0f, 0.0f);            //旋转矩阵，这里绕x轴旋转。
	glRotatef(-60, 0.0f, 1.0f, 0.0f);            //旋转矩阵，这里绕y轴旋转。
	glRotatef(-35, 0.0f, 0.0f, 1.0f);            //绕z轴旋转，这里zrot是角度制的度数。

	//glTranslatef(-90, -10, -50);


	glBegin(GL_QUADS);  //启用四边形带绘制模式绘制

	float bottomL = 0.5;
	float bottomW = 0.7;
	float bottomH = 0;

	float topL = 0.2;
	float topW = 0.15;
	float topH = 20;

	// 绘制前面，这里开始确定纹理坐标，然后是确定点的位置
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-bottomL, -bottomH, bottomW);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(bottomL, -bottomH, bottomW);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(topL, topH, topW);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-topL, topH, topW);

	// 绘制后面
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-bottomL, -bottomH, -bottomW);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-topL, topH, -topW);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(topL, topH, -topW);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(bottomL, -bottomH, -bottomW);

	// 上面
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-topL, topH, -topW);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-topL, topH, topW);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(topL, topH, topW);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(topL, topH, -topW);

	//底面
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-bottomL, -bottomH, -bottomW);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(bottomL, -bottomH, -bottomW);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(bottomL, -bottomH, bottomW);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-bottomL, -bottomH, bottomW);

	// 右面
	glTexCoord2f(1.0f, 0.0f); glVertex3f(bottomL, -bottomH, -bottomW);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(topL, topH, -topW);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(topL, topH, topW);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(bottomL, -bottomH, bottomW);

	// 左面
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-bottomL, -bottomH, -bottomW);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-bottomL, -bottomH, bottomW);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-topL, topH, topW);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-topL, topH, -topW);

	glEnd();

	glPopMatrix(); //弹出变换矩阵

	glDisable(GL_TEXTURE_2D);
}

void DrawCube17(void)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[5]);        //使用贴图纹理

	glPushMatrix();        //压入变换矩阵

	glTranslatef(59, -30, -130);

	glRotatef(0, 1.0f, 0.0f, 0.0f);            //旋转矩阵，这里绕x轴旋转。
	glRotatef(-60, 0.0f, 1.0f, 0.0f);            //旋转矩阵，这里绕y轴旋转。
	glRotatef(80, 0.0f, 0.0f, 1.0f);            //绕z轴旋转，这里zrot是角度制的度数。

	//glTranslatef(-90, -10, -50);


	glBegin(GL_QUADS);  //启用四边形带绘制模式绘制

	float bottomL = 0.6;
	float bottomW = 0.5;
	float bottomH = 4;

	float topL = 0.3;
	float topW = 0.2;
	float topH = 23;

	// 绘制前面，这里开始确定纹理坐标，然后是确定点的位置
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-bottomL, -bottomH, bottomW);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(bottomL, -bottomH, bottomW);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(topL, topH, topW);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-topL, topH, topW);

	// 绘制后面
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-bottomL, -bottomH, -bottomW);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-topL, topH, -topW);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(topL, topH, -topW);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(bottomL, -bottomH, -bottomW);

	// 上面
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-topL, topH, -topW);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-topL, topH, topW);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(topL, topH, topW);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(topL, topH, -topW);

	//底面
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-bottomL, -bottomH, -bottomW);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(bottomL, -bottomH, -bottomW);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(bottomL, -bottomH, bottomW);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-bottomL, -bottomH, bottomW);

	// 右面
	glTexCoord2f(1.0f, 0.0f); glVertex3f(bottomL, -bottomH, -bottomW);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(topL, topH, -topW);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(topL, topH, topW);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(bottomL, -bottomH, bottomW);

	// 左面
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-bottomL, -bottomH, -bottomW);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-bottomL, -bottomH, bottomW);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-topL, topH, topW);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-topL, topH, -topW);

	glEnd();

	glPopMatrix(); //弹出变换矩阵

	glDisable(GL_TEXTURE_2D);



}
//树叶
void DrawCube20(void)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[9]);        //使用贴图纹理

	glPushMatrix();        //压入变换矩阵

	glRotatef(0, 1.0f, 0.0f, 0.0f);            //旋转矩阵，这里绕x轴旋转。
	glRotatef(-60, 0.0f, 1.0f, 0.0f);            //旋转矩阵，这里绕y轴旋转。
	glRotatef(0, 0.0f, 0.0f, 1.0f);            //绕z轴旋转，这里zrot是角度制的度数。

	//glTranslatef(-90, -10, -50);
	glTranslatef(-80, -10, -115);

	glBegin(GL_QUADS);  //启用四边形带绘制模式绘制

	float bottomL = 12;
	float bottomW = 12;
	float bottomH = 0;

	float topL = 9;
	float topW = 9;
	float topH = 5;

	// 绘制前面，这里开始确定纹理坐标，然后是确定点的位置
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-bottomL, -bottomH, bottomW);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(bottomL, -bottomH, bottomW);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(topL, topH, topW);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-topL, topH, topW);

	// 绘制后面
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-bottomL, -bottomH, -bottomW);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-topL, topH, -topW);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(topL, topH, -topW);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(bottomL, -bottomH, -bottomW);

	// 上面


	// 右面
	glTexCoord2f(1.0f, 0.0f); glVertex3f(bottomL, -bottomH, -bottomW);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(topL, topH, -topW);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(topL, topH, topW);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(bottomL, -bottomH, bottomW);

	// 左面
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-bottomL, -bottomH, -bottomW);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-bottomL, -bottomH, bottomW);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-topL, topH, topW);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-topL, topH, -topW);

	glEnd();

	glPopMatrix(); //弹出变换矩阵

	glDisable(GL_TEXTURE_2D);
}

void DrawCube21(void)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[9]);        //使用贴图纹理

	glPushMatrix();        //压入变换矩阵

	glRotatef(0, 1.0f, 0.0f, 0.0f);            //旋转矩阵，这里绕x轴旋转。
	glRotatef(-60, 0.0f, 1.0f, 0.0f);            //旋转矩阵，这里绕y轴旋转。
	glRotatef(0, 0.0f, 0.0f, 1.0f);            //绕z轴旋转，这里zrot是角度制的度数。

	//glTranslatef(-90, -10, -50);
	glTranslatef(-80, -6, -115);

	glBegin(GL_QUADS);  //启用四边形带绘制模式绘制

	float bottomL = 11;
	float bottomW = 11;
	float bottomH = 0;

	float topL = 8;
	float topW = 8;
	float topH = 5;

	// 绘制前面，这里开始确定纹理坐标，然后是确定点的位置
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-bottomL, -bottomH, bottomW);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(bottomL, -bottomH, bottomW);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(topL, topH, topW);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-topL, topH, topW);

	// 绘制后面
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-bottomL, -bottomH, -bottomW);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-topL, topH, -topW);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(topL, topH, -topW);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(bottomL, -bottomH, -bottomW);

	// 上面


	// 右面
	glTexCoord2f(1.0f, 0.0f); glVertex3f(bottomL, -bottomH, -bottomW);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(topL, topH, -topW);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(topL, topH, topW);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(bottomL, -bottomH, bottomW);

	// 左面
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-bottomL, -bottomH, -bottomW);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-bottomL, -bottomH, bottomW);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-topL, topH, topW);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-topL, topH, -topW);

	glEnd();

	glPopMatrix(); //弹出变换矩阵

	glDisable(GL_TEXTURE_2D);
}

void DrawCube24(void)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[9]);        //使用贴图纹理

	glPushMatrix();        //压入变换矩阵

	glRotatef(0, 1.0f, 0.0f, 0.0f);            //旋转矩阵，这里绕x轴旋转。
	glRotatef(-60, 0.0f, 1.0f, 0.0f);            //旋转矩阵，这里绕y轴旋转。
	glRotatef(0, 0.0f, 0.0f, 1.0f);            //绕z轴旋转，这里zrot是角度制的度数。

	//glTranslatef(-90, -10, -50);
	glTranslatef(-80, -14, -115);

	glBegin(GL_QUADS);  //启用四边形带绘制模式绘制

	float bottomL = 13;
	float bottomW = 13;
	float bottomH = 0;

	float topL = 10;
	float topW = 10;
	float topH = 5;

	// 绘制前面，这里开始确定纹理坐标，然后是确定点的位置
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-bottomL, -bottomH, bottomW);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(bottomL, -bottomH, bottomW);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(topL, topH, topW);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-topL, topH, topW);

	// 绘制后面
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-bottomL, -bottomH, -bottomW);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-topL, topH, -topW);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(topL, topH, -topW);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(bottomL, -bottomH, -bottomW);

	// 上面


	// 右面
	glTexCoord2f(1.0f, 0.0f); glVertex3f(bottomL, -bottomH, -bottomW);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(topL, topH, -topW);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(topL, topH, topW);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(bottomL, -bottomH, bottomW);

	// 左面
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-bottomL, -bottomH, -bottomW);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-bottomL, -bottomH, bottomW);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-topL, topH, topW);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-topL, topH, -topW);

	glEnd();

	glPopMatrix(); //弹出变换矩阵

	glDisable(GL_TEXTURE_2D);
}

void DrawCube25(void)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[9]);        //使用贴图纹理

	glPushMatrix();        //压入变换矩阵

	glRotatef(0, 1.0f, 0.0f, 0.0f);            //旋转矩阵，这里绕x轴旋转。
	glRotatef(-60, 0.0f, 1.0f, 0.0f);            //旋转矩阵，这里绕y轴旋转。
	glRotatef(0, 0.0f, 0.0f, 1.0f);            //绕z轴旋转，这里zrot是角度制的度数。

	//glTranslatef(-90, -10, -50);
	glTranslatef(-80, -2, -115);

	glBegin(GL_QUADS);  //启用四边形带绘制模式绘制

	float bottomL = 9;
	float bottomW = 9;
	float bottomH = 0;

	float topL = 6;
	float topW = 6;
	float topH = 5;

	// 绘制前面，这里开始确定纹理坐标，然后是确定点的位置
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-bottomL, -bottomH, bottomW);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(bottomL, -bottomH, bottomW);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(topL, topH, topW);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-topL, topH, topW);

	// 绘制后面
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-bottomL, -bottomH, -bottomW);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-topL, topH, -topW);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(topL, topH, -topW);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(bottomL, -bottomH, -bottomW);

	// 上面


	// 右面
	glTexCoord2f(1.0f, 0.0f); glVertex3f(bottomL, -bottomH, -bottomW);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(topL, topH, -topW);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(topL, topH, topW);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(bottomL, -bottomH, bottomW);

	// 左面
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-bottomL, -bottomH, -bottomW);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-bottomL, -bottomH, bottomW);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-topL, topH, topW);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-topL, topH, -topW);

	glEnd();

	glPopMatrix(); //弹出变换矩阵

	glDisable(GL_TEXTURE_2D);
}

void DrawCube26(void)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[9]);        //使用贴图纹理

	glPushMatrix();        //压入变换矩阵

	glRotatef(0, 1.0f, 0.0f, 0.0f);            //旋转矩阵，这里绕x轴旋转。
	glRotatef(-60, 0.0f, 1.0f, 0.0f);            //旋转矩阵，这里绕y轴旋转。
	glRotatef(0, 0.0f, 0.0f, 1.0f);            //绕z轴旋转，这里zrot是角度制的度数。

	//glTranslatef(-90, -10, -50);
	glTranslatef(-80, 2, -115);

	glBegin(GL_QUADS);  //启用四边形带绘制模式绘制

	float bottomL = 8;
	float bottomW = 8;
	float bottomH = 0;

	float topL = 4;
	float topW = 4;
	float topH = 5;

	// 绘制前面，这里开始确定纹理坐标，然后是确定点的位置
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-bottomL, -bottomH, bottomW);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(bottomL, -bottomH, bottomW);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(topL, topH, topW);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-topL, topH, topW);

	// 绘制后面
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-bottomL, -bottomH, -bottomW);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-topL, topH, -topW);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(topL, topH, -topW);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(bottomL, -bottomH, -bottomW);

	// 上面


	// 右面
	glTexCoord2f(1.0f, 0.0f); glVertex3f(bottomL, -bottomH, -bottomW);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(topL, topH, -topW);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(topL, topH, topW);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(bottomL, -bottomH, bottomW);

	// 左面
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-bottomL, -bottomH, -bottomW);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-bottomL, -bottomH, bottomW);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-topL, topH, topW);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-topL, topH, -topW);

	glEnd();

	glPopMatrix(); //弹出变换矩阵

	glDisable(GL_TEXTURE_2D);
}

void DrawCube27(void)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[9]);        //使用贴图纹理

	glPushMatrix();        //压入变换矩阵

	glRotatef(0, 1.0f, 0.0f, 0.0f);            //旋转矩阵，这里绕x轴旋转。
	glRotatef(-60, 0.0f, 1.0f, 0.0f);            //旋转矩阵，这里绕y轴旋转。
	glRotatef(0, 0.0f, 0.0f, 1.0f);            //绕z轴旋转，这里zrot是角度制的度数。

	//glTranslatef(-90, -10, -50);
	glTranslatef(-80, 5, -115);

	glBegin(GL_QUADS);  //启用四边形带绘制模式绘制

	float bottomL = 7;
	float bottomW = 7;
	float bottomH = 0;

	float topL = 1;
	float topW = 1;
	float topH = 7;

	// 绘制前面，这里开始确定纹理坐标，然后是确定点的位置
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-bottomL, -bottomH, bottomW);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(bottomL, -bottomH, bottomW);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(topL, topH, topW);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-topL, topH, topW);

	// 绘制后面
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-bottomL, -bottomH, -bottomW);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-topL, topH, -topW);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(topL, topH, -topW);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(bottomL, -bottomH, -bottomW);

	// 上面


	// 右面
	glTexCoord2f(1.0f, 0.0f); glVertex3f(bottomL, -bottomH, -bottomW);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(topL, topH, -topW);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(topL, topH, topW);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(bottomL, -bottomH, bottomW);

	// 左面
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-bottomL, -bottomH, -bottomW);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-bottomL, -bottomH, bottomW);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-topL, topH, topW);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-topL, topH, -topW);

	glEnd();

	glPopMatrix(); //弹出变换矩阵

	glDisable(GL_TEXTURE_2D);
}

void DrawCube22(void)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[9]);        //使用贴图纹理

	glPushMatrix();        //压入变换矩阵

	glRotatef(0, 1.0f, 0.0f, 0.0f);            //旋转矩阵，这里绕x轴旋转。
	glRotatef(-60, 0.0f, 1.0f, 0.0f);            //旋转矩阵，这里绕y轴旋转。
	glRotatef(0, 0.0f, 0.0f, 1.0f);            //绕z轴旋转，这里zrot是角度制的度数。

	//glTranslatef(-90, -10, -50);
	glTranslatef(-80, -16, -115);

	glBegin(GL_QUADS);  //启用四边形带绘制模式绘制

	float bottomL = 15;
	float bottomW = 15;
	float bottomH = 5;

	float topL = 9;
	float topW = 9;
	float topH = 5;

	// 绘制前面，这里开始确定纹理坐标，然后是确定点的位置
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-bottomL, -bottomH, bottomW);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(bottomL, -bottomH, bottomW);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(topL, topH, topW);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-topL, topH, topW);

	// 绘制后面
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-bottomL, -bottomH, -bottomW);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-topL, topH, -topW);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(topL, topH, -topW);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(bottomL, -bottomH, -bottomW);

	// 上面


	// 右面
	glTexCoord2f(1.0f, 0.0f); glVertex3f(bottomL, -bottomH, -bottomW);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(topL, topH, -topW);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(topL, topH, topW);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(bottomL, -bottomH, bottomW);

	// 左面
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-bottomL, -bottomH, -bottomW);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-bottomL, -bottomH, bottomW);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-topL, topH, topW);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-topL, topH, -topW);

	glEnd();

	glPopMatrix(); //弹出变换矩阵

	glDisable(GL_TEXTURE_2D);
}

void DrawCube23(void)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[9]);        //使用贴图纹理

	glPushMatrix();        //压入变换矩阵

	glRotatef(0, 1.0f, 0.0f, 0.0f);            //旋转矩阵，这里绕x轴旋转。
	glRotatef(-60, 0.0f, 1.0f, 0.0f);            //旋转矩阵，这里绕y轴旋转。
	glRotatef(0, 0.0f, 0.0f, 1.0f);            //绕z轴旋转，这里zrot是角度制的度数。

	//glTranslatef(-90, -10, -50);
	glTranslatef(-80, -20, -115);

	glBegin(GL_QUADS);  //启用四边形带绘制模式绘制

	float bottomL = 15;
	float bottomW = 15;
	float bottomH = 5;

	float topL = 9;
	float topW = 9;
	float topH = 5;

	// 绘制前面，这里开始确定纹理坐标，然后是确定点的位置
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-bottomL, -bottomH, bottomW);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(bottomL, -bottomH, bottomW);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(topL, topH, topW);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-topL, topH, topW);

	// 绘制后面
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-bottomL, -bottomH, -bottomW);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-topL, topH, -topW);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(topL, topH, -topW);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(bottomL, -bottomH, -bottomW);

	// 上面


	// 右面
	glTexCoord2f(1.0f, 0.0f); glVertex3f(bottomL, -bottomH, -bottomW);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(topL, topH, -topW);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(topL, topH, topW);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(bottomL, -bottomH, bottomW);

	// 左面
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-bottomL, -bottomH, -bottomW);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-bottomL, -bottomH, bottomW);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-topL, topH, topW);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-topL, topH, -topW);

	glEnd();

	glPopMatrix(); //弹出变换矩阵

	glDisable(GL_TEXTURE_2D);
}

void DrawTree()
{
	DrawCube9();
	DrawCube15();
	DrawCube17();
	DrawCube20();
	DrawCube21();
	DrawCube22();
	DrawCube23();
	DrawCube24();
	DrawCube25();
	DrawCube26();
	DrawCube27();
}
void init(void)
{
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);

	//glEnable(GL_COLOR_MATERIAL);
	glLightModeli(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);// 模型的正面接受环境光和散射光


	//定义光源的颜色和位置
	GLfloat ambient[] = { 0.5, 0.8, 0.1, 0.1 };
	GLfloat diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	//	GLfloat position[] = { -80.0, 50.0, 25.0, 1.0 };
	//GLfloat position[] = { 0.0,0.0, 0.0, 1.0 };
	GLfloat position[] = { eye[0],eye[1], eye[2], 1.0 };
	//选择光照模型
	GLfloat lmodel_ambient[] = { 0.4, 0.4, 0.4, 1.0 };
	GLfloat local_view[] = { 0.0 };
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);
	//设置环境光
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	//设置漫射光
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	//设置光源位置
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, local_view);
	//启动光照
	//glEnable(GL_LIGHTING);
	//启用光源
	glEnable(GL_LIGHT0);
	BuildTexture("background2.jpg", texture[1]);
	BuildTexture("wood_2.jpg", texture[4]);
	BuildTexture("roof_1.jpg", texture[5]);
	BuildTexture("door_1.jpg", texture[6]);
	BuildTexture("roof_1.jpg", texture[7]);
	BuildTexture("snowland.jpg", texture[8]);
	BuildTexture("window_4.jpg", texture[2]);
	BuildTexture("wdw_2.jpg", texture[3]);
	BuildTexture("LEAF2.jpg", texture[9]);
}

void drawScene()
{
	glPushMatrix();
	glRotatef(0, 1.0f, 0.0f, 0.0f);            //旋转矩阵，这里绕x轴旋转。
	glRotatef(30.0, 0.0f, 1.0f, 0.0f);            //旋转矩阵，这里绕y轴旋转。
	glRotatef(0, 0.0f, 0.0f, 1.0f);            //绕z轴旋转，这里zrot是角度制的度数。
	glColor3f(255 / 255.0, 250 / 255.0, 250 / 255.0);

	glNormal3f(0.0, 0.0, 1.0);
	glTranslatef(70.0, -45.0, -90.0);

	GLUquadricObj *q;
	q = gluNewQuadric();

	gluQuadricTexture(q, GL_TRUE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[8]);
	gluSphere(q, 8, 20, 20);
	// 画头
	glTranslatef(0.0, 10.0, 0.0);


	gluQuadricTexture(q, GL_TRUE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[8]);
	gluSphere(q, 6, 20, 20);

	glPushMatrix();
	glPushMatrix();
	glPopMatrix();
	// 画眼睛

	glPushMatrix();
	glColor3f(-0.4f, 0.0f, 0.0f);
	glTranslatef(0.0f, 3.0f, 6.0f);
	glutSolidSphere(0.80f, 20, 20);

	glTranslatef(-6.0f, 0.0f, -2.0f);
	glutSolidSphere(0.8f, 20, 20);



	// 画鼻子

	glTranslatef(3.00f, -2.0f, -0.0f);
	glColor3f(1.0f, 0.0, 0.0f);

	glutSolidCone(1.50f, 7.0f, 10, 2);
	glPushMatrix();
	glColor3f(-0.4f, 0.0f, 0.0f);
	glTranslatef(-3.0f, 2.0f, 0.0f);
	glTranslatef(6.0f, 0.0f, 2.0f);

	glTranslatef(-5.0f, -4.0f, -2.0f);
	glutSolidSphere(0.40f, 20, 20);

	glTranslatef(1.20f, -0.50f, 1.0f);
	glutSolidSphere(0.40f, 20, 20);

	glTranslatef(0.80f, -0.50f, 0.50f);
	//glutSolidSphere(0.40f, 20, 20);

	glTranslatef(0.80f, 0.50f, 0.10f);
	glutSolidSphere(0.40f, 20, 20);

	glTranslatef(1.20f, 0.50f, 0.4f);
	glutSolidSphere(0.40f, 20, 20);

	glTranslatef(2.50f, 1.50f, 0.0f);
	glColor3f(1.0f, 0.5f, 0.5f);
	glutSolidSphere(1.0f, 20, 20);

	glTranslatef(-7.80f, 0.0f, -4.0f);
	glColor3f(1.0f, 0.5f, 0.5f);
	glutSolidSphere(1.00f, 20, 20);


	glTranslatef(2.50f, -10.0f, 5.0f);
	glColor3f(205 / 255.0, 133 / 255.0, 63 / 255.0);
	glutSolidSphere(1.0f, 20, 20);


	glTranslatef(0.0f, 4.0f, 0.0f);
	glColor3f(205 / 255.0, 133 / 255.0, 63 / 255.0);
	glutSolidSphere(1.0f, 20, 20);


	glTranslatef(2.5, 12.0, -5.0f);


	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	glColor4f(1.0, 1.0, 1.0, 1.0f);
	gluQuadricTexture(q, GL_TRUE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[5]);




	gluCylinder(q, 3, 0.5, 6, 10, 2);

	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);

	//glRotatef(90,)
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[5]);        //使用贴图纹理

	glPushMatrix();        //压入变换矩阵


	glColor4f(1.0, 1.0, 1.0, 1.0f);
	glRotatef(-angle, 0, 1, 0);
	glTranslatef(0 - moveX / 5.0, -30 + moveY / 15.0, 30);
	gluCylinder(q, 0, 0, 1, 10, 2);
	if (FLAG) {
		glBegin(GL_QUADS);  //启用四边形带绘制模式绘制
		
		// 绘制前面，这里开始确定纹理坐标，然后是确定点的位置
		glTexCoord2f(0.0f, 0.0f); glVertex3f(LEFT, BOTTOM, thickness);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(RIGHT, BOTTOM, thickness);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(RIGHT, TOP, thickness);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(LEFT, TOP, thickness);

		// 绘制后面
		glTexCoord2f(1.0f, 0.0f); glVertex3f(LEFT, BOTTOM, -thickness);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(LEFT, TOP, -thickness);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(RIGHT, TOP, -thickness);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(RIGHT, BOTTOM, -thickness);

		// 上面
		glTexCoord2f(0.0f, 1.0f); glVertex3f(LEFT, TOP, -thickness);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(LEFT, TOP, thickness);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(RIGHT, TOP, thickness);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(RIGHT, TOP, -thickness);

		//底面
		glTexCoord2f(1.0f, 1.0f); glVertex3f(LEFT, BOTTOM, -thickness);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(RIGHT, BOTTOM, -thickness);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(RIGHT, BOTTOM, thickness);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(LEFT, BOTTOM, thickness);

		// 右面
		glTexCoord2f(1.0f, 0.0f); glVertex3f(RIGHT, BOTTOM, -thickness);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(RIGHT, TOP, -thickness);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(RIGHT, TOP, thickness);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(RIGHT, BOTTOM, thickness);

		// 左面
		glTexCoord2f(0.0f, 0.0f); glVertex3f(LEFT, BOTTOM, -thickness);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(LEFT, BOTTOM, thickness);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(LEFT, TOP, thickness);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(LEFT, TOP, -thickness);
		//*/


		glEnd();

		glPopMatrix(); //弹出变换矩阵

		glDisable(GL_TEXTURE_2D);
	}
	else
	{
		glBegin(GL_QUADS);  //启用四边形带绘制模式绘制
	// 绘制前面，这里开始确定纹理坐标，然后是确定点的位置
		glTranslatef(0.0, 0.0, 40.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(LEFT, BOTTOM, thickness);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(RIGHT, BOTTOM, thickness);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(RIGHT, TOP, thickness);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(LEFT, TOP, thickness);

		// 绘制后面
		glTexCoord2f(1.0f, 0.0f); glVertex3f(LEFT, BOTTOM, -thickness);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(LEFT, TOP, -thickness);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(RIGHT, TOP, -thickness);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(RIGHT, BOTTOM, -thickness);

		// 上面
		glTexCoord2f(0.0f, 1.0f); glVertex3f(LEFT, TOP, -thickness);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(LEFT, TOP, thickness);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(RIGHT, TOP, thickness);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(RIGHT, TOP, -thickness);

		//底面
		glTexCoord2f(1.0f, 1.0f); glVertex3f(LEFT, BOTTOM, -thickness);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(RIGHT, BOTTOM, -thickness);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(RIGHT, BOTTOM, thickness);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(LEFT, BOTTOM, thickness);

		// 右面
		glTexCoord2f(1.0f, 0.0f); glVertex3f(RIGHT, BOTTOM, -thickness);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(RIGHT, TOP, -thickness);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(RIGHT, TOP, thickness);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(RIGHT, BOTTOM, thickness);

		// 左面
		glTexCoord2f(0.0f, 0.0f); glVertex3f(LEFT, BOTTOM, -thickness);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(LEFT, BOTTOM, thickness);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(LEFT, TOP, thickness);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(LEFT, TOP, -thickness);
		//*/

		FLAG = 1;
		glEnd();

		glPopMatrix(); //弹出变换矩阵

		glDisable(GL_TEXTURE_2D);
	}
	glPopMatrix();
	glPopMatrix();
	glColor4f(1.0, 1.0, 1.0, 1.0f);
	//门
	glPushMatrix();
	glTranslatef(-17.0f, -9.0f, -23.0f);
	glRotatef(180, 1, 0, 0);
	glRotatef(180, 0, 0, 1);
	glRotatef(ratio, 0, 1, 0);
	glTranslatef(5.0f, 0, 0);
	glScalef(10, 11, 1);
	glPopMatrix();
	//地板
	glPushMatrix();
	glTranslatef(0.0f, -1.0f*skySizeY / 2.0f, 0.0f);
	glRotatef(90, 1, 0, 0);
	glScalef(skySizeX, skySizeZ, 1);
	drawBottom(texture[1]);
	glPopMatrix();

	//天花板	
	glPushMatrix();
	glTranslatef(0.0f, 1.0f*skySizeY / 2.0f, 0.0f);
	glRotatef(270, 1, 0, 0);
	glScalef(skySizeX, skySizeZ, 1);
	drawTop(texture[1]);
	glPopMatrix();

	//墙壁（前）
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -1.0f*skySizeZ / 2.0);
	glScalef(skySizeX, skySizeY, 1);
	drawRect(texture[1], 1, 2);
	glPopMatrix();

	//墙壁（后）
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 1.0f*skySizeZ / 2.0f);
	glRotatef(180, 0, 1, 0);
	glScalef(skySizeX, skySizeY, 1);
	drawRect(texture[1], 1, 0);
	glPopMatrix();

	//墙壁（左）
	glPushMatrix();
	glTranslatef(-1.0f*skySizeX / 2.0f, 0.0f, 0.0f);
	glRotatef(90, 0, 1, 0);
	glScalef(skySizeZ, skySizeY, 1);
	drawRect(texture[1], 1, 1);
	glPopMatrix();

	//墙壁（右）
	glPushMatrix();
	glTranslatef(1.0f*skySizeX / 2.0f, 0.0f, 0.0f);
	glRotatef(270, 0, 1, 0);
	glScalef(skySizeZ, skySizeY, 1);
	drawRect(texture[1], 1, 3);
	glPopMatrix();
	Draw();
	DrawDoor();
	DrawLight();
	DrawWindow1();
	DrawWindow2();
	DrawWindow3();
	DrawWindow4();
	DrawWindow5();
	DrawTree();
}

void mouseMove(int  x, int  y) {//鼠标移动获得位置
	glutPostRedisplay();
	moveX = x;
	moveY = y;
	Xx = x;
	Yy = y;
}

void mouseClick(int btn, int state, int x, int y) {
	if (btn == GLUT_RIGHT_BUTTON)
	{
		FLAG = 0; moveX = 0; moveY = 0; Xx = 0; Yy = 0;
		thickness = 1;
		RIGHT = -1;
		LEFT = 1;
		TOP = 30;//反
		BOTTOM = 0;
		angle = 0;
	}
	else;
}
void reshape(int width, int height)
{
	if (height == 0) { //如果高度为0     
		height = 1;   //让高度为1（避免出现分母为0的现象）          
	}
	wHeight = height;
	wWidth = width;

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);//设置矩阵模式为投影           
	glLoadIdentity();   //初始化矩阵为单位矩阵              
	whRatio = (GLfloat)width / (GLfloat)height;  //设置显示比例             
	gluPerspective(45, whRatio, 1, 400); //透视投影  
	glMatrixMode(GL_MODELVIEW);  //设置矩阵模式为模型        
}

void redraw()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//清除颜色和深度缓存     
	glClearColor(0, 0, 0, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();   //初始化矩阵为单位矩阵          
	gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], 0, 1, 0);
	glPushMatrix();
	drawScene();//绘制场景 

	glColor3f(1, 1, 1);
	for (int i = 0; i < flakenum; i++)
		displayOnce(i);

	glPopMatrix();

	glutSwapBuffers();//交换缓冲区      
	//GLUT_KEY_UP


}
int Lightflag=1;//1，亮到暗;0，暗到亮
void normalKey(unsigned char key, GLint x, GLint y)
{


	if (key == 27)
		exit(0);
	if (key == 9)
		Flag = 1;

	//	break;
	//if(key==1)
	//	Flag = 1;
	//	break;
	switch (key)
	{
		//case GLUT_KEY_
	case 'w':
		move(eye, center, direction::front);
		break;
	case 'a':
		move(eye, center, direction::left);
		break;
	case 's':
		move(eye, center, direction::back);
		break;
	case 'd':
		move(eye, center, direction::right);
		break;
	case 'W':
		move(eye, center, direction::front);
		break;
	case 'A':
		move(eye, center, direction::left);
		break;
	case 'S':
		move(eye, center, direction::back);
		break;
	case 'D':
		move(eye, center, direction::right);
		break;
	case 't':
		TOP += 1;
		break;
	case 'T':
		TOP += 1;
		break;
	case 'f':
		RIGHT -= 1;

		break;
	case 'F':
		RIGHT -= 1;
		break;
	case 'g':
		TOP -= 1;
		break;
	case 'G':
		TOP -= 1;
		break;
	case 'h':
		LEFT += 1;
		break;
	case 'H':
		LEFT += 1;
		break;
	case'b':
		//TRue = !TRue;
		if (snowFlag == 1)
			snowFlag = 0;
		else
		{
			initFlakes_anime();
			snowFlag = 1;
		}
		break;
	case'B':
		//TRue = !TRue;
		if (snowFlag == 1)
			snowFlag = 0;
		else
		{
			initFlakes_anime();
			snowFlag = 1;
		}
		break;
	case 'M':
		COUNT++;
		if (COUNT % 4 == 1)
			PlaySound(L"Data/林友树 (林ゆうき) - ヒカリヘ (向着光) (piano version).wav", NULL, SND_LOOP | SND_ASYNC);
		else if (COUNT % 4 == 2)
			PlaySound(L"Data/松谷卓 (まつたに すぐる) - 最後の森へ….wav", NULL, SND_LOOP | SND_ASYNC);
		else if (COUNT % 4 == 3)
			PlaySound(L"Data/神山純一 (かみやま じゅんいち) - 妖精の森 (妖精之森).wav", NULL, SND_LOOP | SND_ASYNC);
		else
		{
			PlaySound(L"Data/Taro Ishida - No Sound (无声).wav", NULL, SND_LOOP | SND_ASYNC);
			PlaySound(L"Data/Taro Ishida - No Sound (无声).wav", NULL, SND_LOOP | SND_ASYNC);
			PlaySound(L"Data/Taro Ishida - No Sound (无声).wav", NULL, SND_LOOP | SND_ASYNC);
		}

		break;
	case 'm':
		COUNT++;
		if (COUNT % 4 == 1)
			PlaySound(L"Data/林友树 (林ゆうき) - ヒカリヘ (向着光) (piano version).wav", NULL, SND_LOOP | SND_ASYNC);
		else if (COUNT % 4 == 2)
			PlaySound(L"Data/松谷卓 (まつたに すぐる) - 最後の森へ….wav", NULL, SND_LOOP | SND_ASYNC);
		else if (COUNT % 4 == 3)
			PlaySound(L"Data/神山純一 (かみやま じゅんいち) - 妖精の森 (妖精之森).wav", NULL, SND_LOOP | SND_ASYNC);
		else
		{
			PlaySound(L"Data/Taro Ishida - No Sound (无声).wav", NULL, SND_LOOP | SND_ASYNC);
			PlaySound(L"Data/Taro Ishida - No Sound (无声).wav", NULL, SND_LOOP | SND_ASYNC);
			PlaySound(L"Data/Taro Ishida - No Sound (无声).wav", NULL, SND_LOOP | SND_ASYNC);
		}
		break;
	case 'k':
	{
		GLfloat sun_mat_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };  //定义材质的环境光颜色，为0
		GLfloat sun_mat_diffuse[] = { 0.2f, 0.2f, 0.2f, 1.0f };  //定义材质的漫反射光颜色，为0
		GLfloat sun_mat_specular[] = { 0.2f, 0.2f, 0.2f, 1.0f };   //定义材质的镜面反射光颜色，为0
		GLfloat sun_mat_emission[] = { XXX / 255.0,YYY / 255.0,ZZZ / 255.0, 1.0f };   //定义材质的辐射广颜色，为偏红色
		GLfloat sun_mat_shininess = 0.0f;
		glMaterialfv(GL_FRONT, GL_AMBIENT, sun_mat_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, sun_mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, sun_mat_specular);
		glMaterialfv(GL_FRONT, GL_EMISSION, sun_mat_emission);
		glMaterialf(GL_FRONT, GL_SHININESS, sun_mat_shininess);

		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHTING);
		if (Lightflag&&XXX > 50)
		{
			XXX -= 5;
			YYY -= 5;
			ZZZ -= 5;
		}
		else if (XXX < 240)
		{
			Lightflag = 0;
			XXX += 5;
			YYY += 5;
			ZZZ += 5;
		}
		else
			Lightflag = 1;
		break;
	}
	case 'K':
	{
		GLfloat sun_mat_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };  //定义材质的环境光颜色，为0
		GLfloat sun_mat_diffuse[] = { 0.2f, 0.2f, 0.2f, 1.0f };  //定义材质的漫反射光颜色，为0
		GLfloat sun_mat_specular[] = { 0.2f, 0.2f, 0.2f, 1.0f };   //定义材质的镜面反射光颜色，为0
		GLfloat sun_mat_emission[] = { XXX / 255.0,YYY / 255.0,ZZZ / 255.0, 1.0f };   //定义材质的辐射广颜色，为偏红色
		GLfloat sun_mat_shininess = 0.0f;
		glMaterialfv(GL_FRONT, GL_AMBIENT, sun_mat_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, sun_mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, sun_mat_specular);
		glMaterialfv(GL_FRONT, GL_EMISSION, sun_mat_emission);
		glMaterialf(GL_FRONT, GL_SHININESS, sun_mat_shininess);

		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHTING);
		if (Lightflag&&XXX > 50)
		{
			XXX -= 5;
			YYY -= 5;
			ZZZ -= 5;
		}
		else if (XXX < 240)
		{
			Lightflag = 0;
			XXX += 5;
			YYY += 5;
			ZZZ += 5;
		}
		else
			Lightflag = 1;
		break;
	}
	case'l':
	{
		if (FFlag)
		{
			//Sleep(1000);
			//for (int i = 255; i > 50; i -= 20) {
			//	Sleep(100);
			int i = 100;
		
				GLfloat sun_mat_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };  //定义材质的环境光颜色，为0
				GLfloat sun_mat_diffuse[] = { 0.0f, 0.0f, 0.0f, 1.0f };  //定义材质的漫反射光颜色，为0
				GLfloat sun_mat_specular[] = { 0.0f, 0.0f, 0.0f, 1.0f };   //定义材质的镜面反射光颜色，为0
				GLfloat sun_mat_emission[] = { XXX / 255.0,YYY / 255.0,ZZZ / 255.0, 1.0f };   //定义材质的辐射广颜色，为偏红色
				GLfloat sun_mat_shininess = 0.0f;
				glMaterialfv(GL_FRONT, GL_AMBIENT, sun_mat_ambient);
				glMaterialfv(GL_FRONT, GL_DIFFUSE, sun_mat_diffuse);
				glMaterialfv(GL_FRONT, GL_SPECULAR, sun_mat_specular);
				glMaterialfv(GL_FRONT, GL_EMISSION, sun_mat_emission);
				glMaterialf(GL_FRONT, GL_SHININESS, sun_mat_shininess);

				glEnable(GL_LIGHT0);
				glEnable(GL_LIGHTING);
				if (XXX > 50)
				{
					XXX -= 20;
					YYY -= 20;
					ZZZ -= 20;
				}
				//glutIdleFunc(idle);//注册全局回调函数：空闲时调用           
				
			FFlag = 0;
		}
		else
		{
			
				
				glDisable(GL_LIGHTING);
				glEnable(GL_LIGHT0);
				FFlag = 1;
			
		}
		//FFlag = !FFlag;
		break; 
	}
	case'L':
	{
		if (FFlag)
		{

			int i = 100;

			GLfloat sun_mat_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };  //定义材质的环境光颜色，为0
			GLfloat sun_mat_diffuse[] = { 0.2f, 0.2f, 0.2f, 1.0f };  //定义材质的漫反射光颜色，为0
			GLfloat sun_mat_specular[] = { 0.2f, 0.2f, 0.2f, 1.0f };   //定义材质的镜面反射光颜色，为0
			GLfloat sun_mat_emission[] = { XXX / 255.0,YYY / 255.0,ZZZ / 255.0, 1.0f };   //定义材质的辐射广颜色，为偏红色
			GLfloat sun_mat_shininess = 0.0f;
			glMaterialfv(GL_FRONT, GL_AMBIENT, sun_mat_ambient);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, sun_mat_diffuse);
			glMaterialfv(GL_FRONT, GL_SPECULAR, sun_mat_specular);
			glMaterialfv(GL_FRONT, GL_EMISSION, sun_mat_emission);
			glMaterialf(GL_FRONT, GL_SHININESS, sun_mat_shininess);

			glEnable(GL_LIGHT0);
			glEnable(GL_LIGHTING);
			if (XXX > 50)
			{
				XXX -= 20;
				YYY -= 20;
				ZZZ -= 20;
			}
			//glutIdleFunc(idle);//注册全局回调函数：空闲时调用           

			FFlag = 0;
		}
		else
		{

			glDisable(GL_LIGHTING);
			glEnable(GL_LIGHT0);
			FFlag = 1;
		}
		break;
	}
	default:
		break;
	}

}
void specialKey(GLint key, GLint x, GLint y)
{
	switch (key)
	{
	case GLUT_KEY_UP:
		center[1] += 1;
		break;
	case GLUT_KEY_DOWN:
		center[1] -= 1;
		break;
	case GLUT_KEY_LEFT:
		rotate(eye, center, direction::left);
		break;
	case GLUT_KEY_RIGHT:
		rotate(eye, center, direction::right);
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

int main(int argc, char *argv[])
{
	srand(time(NULL));

	initFlakes();

	glutInit(&argc, argv);//对glut的初始化             
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	//初始化显示模式:RGB颜色模型，深度测试，双缓冲               
	glutInitWindowSize(800, 600);//设置窗口大小             
	int windowHandle = glutCreateWindow("张一川小组作业");//设置窗口标题  

	glutDisplayFunc(redraw); //注册绘制回调函数             
	glutReshapeFunc(reshape);   //注册重绘回调函数    
	glutKeyboardFunc(normalKey);
	glColor4f(1.0, 1.0, 1.0, 1.0f);
	glutSpecialFunc(specialKey);
	init();
	glutTimerFunc(30, timerFunc, 1);
	glutMouseFunc(mouseClick);
	//鼠标移动事件，鼠标按下并移动时调用
	glutMotionFunc(mouseMove);
	glutMainLoop();  // glut事件处理循环           

	return 0;
}