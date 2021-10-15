#define NDEBUG
#define UNICODE
#define _CRT_SECURE_NO_WARNINGS      
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")
#pragma comment(lib,"glut32.lib")
#include <stdlib.h>       
#include <iostream>
#include<time.h>    
#include<windows.h>
int Flag1 = 1;
int Flag2 = 1;
#include"texture.h"  
#include"collision.h"
using namespace std;

int flakenum = 200;
float scale[1024];
float centerx[1024];
float centery[1024];
float centerz[1024];
float rotateangle[1024];

#define MAXFLAKE 10
#define MINFLAKE 5
#define BUFSIZE 100  

GLuint texture[20];

//视区      
float whRatio;
int wHeight = 0;
int wWidth = 0;


//视点      
float center[] = { 0, 0, -1 };
float eye[] = { 0, 0, -50, 1}; // 第四维赋成1 不然会触发一个奇怪的bug
//float test = 99;

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
		centery[i] = getRand(-160, 160);
		centerz[i] = getRand(-160, 160);
	}
	//第一片雪花的位置和大小固定
	scale[0] = 10;
	centerx[0] = 0;
	centery[0] = 0;
	centerz[0] = 50;
}

//画雪花 注意此函数只画出1/6雪花 之后会通过旋转得到完整的雪花
void drawFlake(int index)
{
	int idx = index % 4;
	//glColor3f(0.5, 0.9, 1);
	//glBegin(GL_LINE_LOOP);
	if (idx == 0 || idx == 1)
	{

		glBegin(GL_POLYGON);
		glColor3f(1, 1, 1);
		glVertex2f(0 * scale[index], 0 * scale[index]);
		if (idx == 0)
			glColor3f(0.5, 0.9, 1);
		else if (idx == 1)
			glColor3f(1, 0.9, 1);


		glVertex2f(0.35*scale[index], 0.15*scale[index]);

		glVertex2f(0.4*scale[index], 0.0 * scale[index]);

		glVertex2f(0.35*scale[index], -0.15*scale[index]);
		glEnd();

		glBegin(GL_POLYGON);
		glVertex2f(0.4*scale[index], 0.0 * scale[index]);

		glColor3f(1, 1, 1);
		glVertex2f(0.5*scale[index], 0.24*scale[index]);
		if (idx == 0)
			glColor3f(0.5, 0.9, 1);
		else if (idx == 1)
			glColor3f(1, 0.9, 1);


		glVertex2f(0.5*scale[index], 0.0 * scale[index]);

		glColor3f(1, 1, 1);
		glVertex2f(0.5*scale[index], -0.24*scale[index]);
		if (idx == 0)
			glColor3f(0.5, 0.9, 1);
		else if (idx == 1)
			glColor3f(1, 0.9, 1);

		glEnd();


		glBegin(GL_POLYGON);
		glVertex2f(0.5*scale[index], 0.0 * scale[index]);

		glColor3f(1, 1, 1);
		glVertex2f(0.7*scale[index], 0.3*scale[index]);
		if (idx == 0)
			glColor3f(0.5, 0.9, 1);
		else if (idx == 1)
			glColor3f(1, 0.9, 1);


		glVertex2f(0.6*scale[index], 0.0 * scale[index]);

		glColor3f(1, 1, 1);
		glVertex2f(0.7*scale[index], -0.3*scale[index]);
		if (idx == 0)
			glColor3f(0.5, 0.9, 1);
		else if (idx == 1)
			glColor3f(1, 0.9, 1);


		glEnd();

		glBegin(GL_POLYGON);
		glVertex2f(0.6*scale[index], 0.0 * scale[index]);
		glColor3f(1, 1, 1);
		glVertex2f(0.9*scale[index], 0.1*scale[index]);
		if (idx == 0)
			glColor3f(0.5, 0.9, 1);
		else if (idx == 1)
			glColor3f(1, 0.9, 1);

		glVertex2f(0.85*scale[index], 0 * scale[index]);
		glColor3f(1, 1, 1);
		glVertex2f(0.9*scale[index], -0.1*scale[index]);
		if (idx == 0)
			glColor3f(0.5, 0.9, 1);
		else if (idx == 1)
			glColor3f(1, 0.9, 1);
		glEnd();

		glBegin(GL_POLYGON);

		glVertex2f(0.85*scale[index], 0 * scale[index]);
		glVertex2f(1.2*scale[index], 0.1*scale[index]);

		glColor3f(1, 1, 1);

		glVertex2f(1.3*scale[index], -0.0 * scale[index]);

		if (idx == 0)
			glColor3f(0.5, 0.9, 1);
		else if (idx == 1)
			glColor3f(1, 0.9, 1);

		glVertex2f(1.2*scale[index], -0.1*scale[index]);

		glEnd();

	}
	else 	if (idx == 2)
	{

		glBegin(GL_LINE_LOOP);
		glColor3f(1, 1, 1);
		glVertex2f(0 * scale[index], 0 * scale[index]);
		glVertex2f(0.35*scale[index], 0.15*scale[index]);
		glVertex2f(0.4*scale[index], 0.0 * scale[index]);
		glVertex2f(0.35*scale[index], -0.15*scale[index]);
		glEnd();

		glBegin(GL_LINE_LOOP);
		glVertex2f(0.4*scale[index], 0.0 * scale[index]);
		glVertex2f(0.5*scale[index], 0.24*scale[index]);
		glVertex2f(0.5*scale[index], 0.0 * scale[index]);
		glVertex2f(0.5*scale[index], -0.24*scale[index]);
		glEnd();


		glBegin(GL_LINE_LOOP);
		glVertex2f(0.5*scale[index], 0.0 * scale[index]);
		glVertex2f(0.7*scale[index], 0.3*scale[index]);
		glVertex2f(0.6*scale[index], 0.0 * scale[index]);
		glVertex2f(0.7*scale[index], -0.3*scale[index]);
		glEnd();

		glBegin(GL_LINE_LOOP);
		glVertex2f(0.6*scale[index], 0.0 * scale[index]);
		glVertex2f(0.9*scale[index], 0.1*scale[index]);
		glVertex2f(0.85*scale[index], 0 * scale[index]);
		glVertex2f(0.9*scale[index], -0.1*scale[index]);
		glEnd();

		glBegin(GL_LINE_LOOP);
		glVertex2f(0.85*scale[index], 0 * scale[index]);
		glVertex2f(1.2*scale[index], 0.1*scale[index]);
		glVertex2f(1.3*scale[index], -0.0 * scale[index]);
		glVertex2f(1.2*scale[index], -0.1*scale[index]);
		glEnd();

	}
	else 	if (idx == 3)
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture[0]);        //使用贴图纹理

		glBegin(GL_QUADS);

		//glColor3f(1, 1, 1);
		glTexCoord2f(0.0f, 0.0f); glVertex2f(0 * scale[index], 0 * scale[index]);
		glTexCoord2f(1.0f, 0.0f); glVertex2f(0.35*scale[index], 0.15*scale[index]);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(0.4*scale[index], 0.0 * scale[index]);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(0.35*scale[index], -0.15*scale[index]);
		//glEnd();

		//glBegin(GL_LINE_LOOP);
		glTexCoord2f(0.0f, 0.0f); glVertex2f(0.4*scale[index], 0.0 * scale[index]);
		glTexCoord2f(1.0f, 0.0f); glVertex2f(0.5*scale[index], 0.24*scale[index]);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(0.5*scale[index], 0.0 * scale[index]);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(0.5*scale[index], -0.24*scale[index]);
		//glEnd();


		//glBegin(GL_LINE_LOOP);
		glTexCoord2f(0.0f, 0.0f); glVertex2f(0.5*scale[index], 0.0 * scale[index]);
		glTexCoord2f(1.0f, 0.0f); glVertex2f(0.7*scale[index], 0.3*scale[index]);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(0.6*scale[index], 0.0 * scale[index]);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(0.7*scale[index], -0.3*scale[index]);
		//glEnd();

		//glBegin(GL_LINE_LOOP);
		glTexCoord2f(0.0f, 0.0f); glVertex2f(0.6*scale[index], 0.0 * scale[index]);
		glTexCoord2f(1.0f, 0.0f); glVertex2f(0.9*scale[index], 0.1*scale[index]);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(0.85*scale[index], 0 * scale[index]);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(0.9*scale[index], -0.1*scale[index]);
		//glEnd();

		//glBegin(GL_LINE_LOOP);
		glTexCoord2f(0.0f, 0.0f); glVertex2f(0.85*scale[index], 0 * scale[index]);
		glTexCoord2f(1.0f, 0.0f); glVertex2f(1.2*scale[index], 0.1*scale[index]);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(1.3*scale[index], -0.0 * scale[index]);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(1.2*scale[index], -0.1*scale[index]);
		glEnd();
		glDisable(GL_TEXTURE_2D);

	}
}
//int angle = 0;

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

//时间
void timerFunc(int value)
{

	for (int i = 0; i < flakenum; i++)
	{
		centery[i] -= scale[i] * 0.1;
		rotateangle[i] += 5 / scale[i];
		if (centery[i] < -160)
		{
			centery[i] = 160;
		}
	}

	glutPostRedisplay();
	glutTimerFunc(30, timerFunc, 1);

}




void init(void)
{
	///*
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);

	GLfloat mat_specular[] = { 1.0,1.0,1.0,1.0 };
	GLfloat mat_shininess[] = { 1000 };

	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);    //指定当前材质属性 镜面反射颜色
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);  //镜面反射指数

	//glEnable(GL_COLOR_MATERIAL);
	//glLightModeli(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);// 模型的正面接受环境光和散射光

	GLfloat ambient[] = { 0.1, 0.1, 0.1, 1.0f };  // 环境强度
	GLfloat diffuse[] = { 1, 1, 1, 1.0f };  // 散射强度
	GLfloat specular[] = { 1, 1, 1, 1.0f }; // 镜面强度

	// 点光源, GL_POSITION属性的最后一个参数为1
	GLfloat position[] = { 0, 0, -100, 1 };
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

	//glEnable(GL_LIGHT0);
	//*/

	///*

	glEnable(GL_DEPTH_TEST);

	//glEnable(GL_COLOR_MATERIAL);
	//glLightModeli(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);// 模型的正面接受环境光和散射光


	GLfloat lmodel_ambient[] = { 0.5,0.5,0.5,1.0 };//微弱环境光，使物体可见
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glShadeModel(GL_SMOOTH);//GL_SMOOTH

	//聚光灯light1

	GLfloat light1_diffuse[] = { 1.0,1.0,1.0,1.0 };
	GLfloat light1_specular[] = { 1.0,1.0,1.0,1.0 };
	GLfloat light1_ambient[] = { 1,1,1,1.0 };

	//GLfloat light1_position[] = { 0,0,-50,1.0 };
	GLfloat spot_direction[] = { 0.0,0.0,-1.0 };

	glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular);
	glLightfv(GL_LIGHT1, GL_POSITION, eye);
	cout << eye[3] << endl;
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);


	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 15.0);//角度
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction);//方向
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 500);

	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT1);
	glEnable(GL_DEPTH_TEST);
	//*/



	BuildTexture("crystal.jpg", texture[0]);
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


	for (int i = 0; i < flakenum; i++)
		displayOnce(i);

	glPopMatrix();

	glutSwapBuffers();//交换缓冲区      
}

void normalKey(unsigned char key, GLint x, GLint y)
{
	if (key == 27)
		exit(0);
	// 退出

	switch (key)
	{


	case'l':
	{
		if (Flag1)
		{
			glEnable(GL_LIGHT0);
			glEnable(GL_LIGHTING);
			Flag1 = 0;
		}
		else
		{
			glDisable(GL_LIGHTING);
			glDisable(GL_LIGHT0);
			Flag1 = 1;
		}
		break;
	}
	case'L':
	{
		if (Flag1)
		{
			glEnable(GL_LIGHT0);
			glEnable(GL_LIGHTING);
			Flag1 = 0;
		}
		else
		{
			glDisable(GL_LIGHTING);
			glDisable(GL_LIGHT0);
			Flag1 = 1;
		}
		break;
	}
	case's':
	{
		if (Flag2)
		{
			glEnable(GL_LIGHT1);
			glEnable(GL_LIGHTING);
			Flag2 = 0;
		}
		else
		{
			glDisable(GL_LIGHTING);
			glDisable(GL_LIGHT1);
			Flag2 = 1;
		}
		break;
	}
	case'S':
	{
		if (Flag2)
		{
			glEnable(GL_LIGHT1);
			glEnable(GL_LIGHTING);
			Flag2 = 0;
		}
		else
		{
			glDisable(GL_LIGHTING);
			glDisable(GL_LIGHT1);
			Flag2 = 1;
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
		move(eye, center, direction::front);
		break;
	case GLUT_KEY_DOWN:
		move(eye, center, direction::back);
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
}

int main(int argc, char *argv[])
{
	srand(time(NULL));

	initFlakes();

	glutInit(&argc, argv);//对glut的初始化             
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	//初始化显示模式:RGB颜色模型，深度测试，双缓冲               
	glutInitWindowSize(800, 600);//设置窗口大小             
	int windowHandle = glutCreateWindow("21180210张一川");//设置窗口标题  

	glutDisplayFunc(redraw); //注册绘制回调函数             
	glutReshapeFunc(reshape);   //注册重绘回调函数    

	glutKeyboardFunc(normalKey);
	glutSpecialFunc(specialKey);

	init();
	glutTimerFunc(30, timerFunc, 1);

	glutMainLoop();  // glut事件处理循环           
	return 0;
}