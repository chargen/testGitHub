#pragma once
#include "opengl/glew.h"
#include "opengl/include/GL/glut.h"
#include <iostream>
#include "LoardShaderProgram.h"
//#include "BW_READBMP.h"
using namespace std;

struct COLOR
{
	float r, g, b, w;
};
float rot = 0.1;
float transX = 0;
float transY = 0;
float transZ = 0;
GLint program;
GLint lightPosLoc;
GLint lightAmbientLoc;
GLint lightLambertLoc;
GLint lightProjLoc;
GLint lightViewLoc;
GLint mLoc;
GLint ksLoc;
GLint kdLoc;
GLint kaLoc;
GLint texLoc;

GLint tex;
GLint mapWidth = 1024;
GLint mapHight = 1024;
GLuint depthTxe;
GLuint frameBuff;
GLfloat lightProj[16];
GLfloat lightView[16];
float alf = 0;
float ks = 1;
float kd = 0.5;
float ka = 1.0;
float m = 0.0035410088;
COLOR lightPos;
void DrawSence()
{
	glTranslated(0, 0, -8);
	glBegin(GL_QUADS);
	glColor3f(1, 0, 0);

	// ground
	//glNormal3f( 0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f);
	glNormal3f(0, 1, 0);
	glVertex3f(4.0f, -2.0f, -4.0f);
	glNormal3f(0, 1, 0);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-4.0f, -2.0f, -4.0f);
	glNormal3f(0, 1, 0);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-4.0f, -2.0f, 4.0f);
	glNormal3f(0, 1, 0);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(4.0f, -2.0f, 4.0f);

	glEnd();

	glTranslated(transX, transY, transZ);
	glRotatef(rot, 1.0, 0.0, 1.0);
	glutSolidTeapot(1);
}
void ProcessKeyboard(unsigned char key, int x, int y)
{
	if (key == 'x' || key == 'X')
	{
		rot += 0.8;
	}
	if (key == 'c' || key == 'C')
	{
		m += 0.00001;
	}
	if (key == 'd' || key == 'D')
	{
		m -= 0.00001;
	}
	if (key == 'u' || key == 'U')
	{
		transX += 0.3;
	}
	if (key == 'i' || key == 'I')
	{
		transX -= 0.3;
	}
	if (key == 'j' || key == 'J')
	{
		transY += 0.3;
	}
	if (key == 'k' || key == 'K')
	{
		transY -= 0.3;
	}
	if (key == 'o' || key == 'O')
	{
		transZ += 0.3;
	}
	if (key == 'p' || key == 'P')
	{
		transZ -= 0.3;
	}

	glutPostRedisplay();
}

void ReSizeScene(int width, int height)
{
	if (height == 0)
	{
		height = 1;
	}//if

	 //设置视口
	glViewport(0, 0, width, height);
	//设置透视矩阵
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
	//开始对模型矩阵进行操作
	glMatrixMode(GL_MODELVIEW);
	//复位
	glLoadIdentity();
}
void GenDepthMap()
{
	lightPos.r = 5; lightPos.g = 5; lightPos.b = -5; lightPos.w = 1;
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuff);
	glDrawBuffer(GL_NONE);
	glViewport(0, 0, mapWidth, mapHight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90, 4.0 / 3, 0.1, 100);
	glGetFloatv(GL_PROJECTION_MATRIX, lightProj);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(lightPos.r, lightPos.g, lightPos.b, 0, 0, -10, 0, 1, 0);
	glGetFloatv(GL_MODELVIEW_MATRIX, lightView);
	glDisable(GL_TEXTURE_2D);
	// glUseProgram(shadowPro) ;
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	DrawSence();
	glUseProgram(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDrawBuffer(GL_FRONT);
}
void ShowSence()
{
	ReSizeScene(400, 300);
	COLOR lightAmbient, lightLambert;
	COLOR CameralDir, CameralPos;
	lightAmbient.r = 0.1; lightAmbient.g = lightAmbient.b = 0.1; lightAmbient.w = 1;
	lightLambert.r = 1; lightLambert.g = lightLambert.b = 1; lightLambert.w = 1;

	CameralDir.r = 0;
	CameralDir.g = 0;
	CameralDir.b = -1;
	CameralDir.w = 0;

	CameralPos.r = 0;
	CameralPos.g = 0;
	CameralPos.b = 0;
	CameralPos.w = 1;
	ka = 1;
	kd = 0.7;
	ks = 0.4;
	tex = 0;
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, depthTxe);
	glUseProgram(program);
	glUniform4f(lightPosLoc, lightPos.r, lightPos.g, lightPos.b, lightPos.w);
	glUniform4f(lightAmbientLoc, lightAmbient.r, lightAmbient.g, lightAmbient.b, lightAmbient.w);
	glUniform4f(lightLambertLoc, lightLambert.r, lightLambert.g, lightLambert.b, lightLambert.w);
	glUniformMatrix4fv(lightProjLoc, 1, GL_FALSE, lightProj);
	glUniformMatrix4fv(lightViewLoc, 1, GL_FALSE, lightView);
	glUniform1f(mLoc, m);
	glUniform1f(kdLoc, kd);
	glUniform1f(ksLoc, ks);
	glUniform1f(kaLoc, ka);
	glUniform1i(texLoc, tex);
	gluLookAt(CameralPos.r, CameralPos.g, CameralPos.b, CameralDir.r, CameralDir.g, CameralDir.b, 0, 1, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	DrawSence();
	glUseProgram(0);
	glFlush();
}
void display()
{
	GenDepthMap();
	ShowSence();
}

void InitScene()
{
	glClearColor(1.0, 1.0, 0.0, 1.0);
	int err = glGetError();
	glGenFramebuffers(1, &frameBuff);
	err = glGetError();
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuff);
	err = glGetError();
	glGenTextures(1, &depthTxe);
	glBindTexture(GL_TEXTURE_2D, depthTxe);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, mapWidth, mapHight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTxe, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (result == GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER)
	{
		cout << "frambuffer GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER " << endl;
	}

	if (result == GL_FRAMEBUFFER_COMPLETE) {
		cout << "Framebuffer is complete.\n" << endl;
	}
	else {
		cout << "Framebuffer is not complete.\n" << endl;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	string shaderPath[2];
	shaderPath[0] = "ShadowMap.vertex";
	shaderPath[1] = "ShadowMap.frag";

	program = loadShader(shaderPath[0].c_str(), shaderPath[1].c_str());
	lightPosLoc = glGetUniformLocation(program, "lightPos");
	lightAmbientLoc = glGetUniformLocation(program, "lightAmbient");
	lightLambertLoc = glGetUniformLocation(program, "lightLambert");
	lightProjLoc = glGetUniformLocation(program, "lightProj");
	lightViewLoc = glGetUniformLocation(program, "lightView");
	mLoc = glGetUniformLocation(program, "m");
	kdLoc = glGetUniformLocation(program, "kd");
	ksLoc = glGetUniformLocation(program, "ks");
	kaLoc = glGetUniformLocation(program, "ka");
	texLoc = glGetUniformLocation(program, "tex");
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glShadeModel(GL_FLAT);

}