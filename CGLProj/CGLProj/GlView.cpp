// GlView.cpp: implementation of the CGlView class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GlView.h"

#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library


#include <math.h>

#define PI 3.14159265

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//

CGlView::CGlView(CWnd *pclWnd)
{
	m_pclWnd = pclWnd;
	m_hDC = ::GetDC(pclWnd->m_hWnd);
	BOOL bSuccess;

	RECT rect;
	m_pclWnd->GetClientRect(&rect);
	int iWidth = rect.right - rect.left;
	int iHeight = rect.bottom - rect.top;

	bSuccess = bSetWindowPixelFormat(m_hDC);
	ASSERT(&bSuccess);

	bSuccess = bCreateViewGLContext(m_hDC);
	ASSERT(&bSuccess);

	vInitGl(iWidth, iHeight);

	generateTextureFromImageFile();
}

// +
// *****************************************************************************
//   Module      :  CGlView::~CGlView()
//   Access      :  Public
//   Input       :  None
//   Output      :  None
//   Return Val  :  None
//   Exceptions  :  None
//   Desc        :  This function is the one and only destructor. It provides
//									clean up and performs any needed shutdown procedures
//
//   Notes       :  None
// *****************************************************************************
// -
CGlView::~CGlView()
{
	// make the rendering context not current
	if (wglGetCurrentContext() != NULL)
		wglMakeCurrent(NULL, NULL);

	// delete the Context
	if (m_hGLContext != NULL)
	{
		wglDeleteContext(m_hGLContext);
		m_hGLContext = NULL;
	}

	
}


// +
// *****************************************************************************
//   Module      :  void CGlView::DrawGLScene()	
//   Access      :  Public
//   Input       :  None
//   Output      :  None
//   Return Val  :  None
//   Exceptions  :  None
//   Desc        :  This function is called contiously to provide realtime OpenGl
//									drawing
//
//   Notes       :  Place all reatime rendering code here
// *****************************************************************************
// -
void CGlView::vDrawGLScene()
{

	//glClearColor(1.0f, 1.0f, 1.0f, 0.5f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -16.0f);
	//glRotatef(45,1,0, 0);


	
	//glEnable(GL_TEXTURE_2D);
	//glBindTexture(GL_TEXTURE_2D, idTexture[FLOWER_IMAGE]);//select idTexture[FLOWER_IMAGE]
	//axes
	//glBegin(GL_LINES);
	////x  RED
	//glColor3f(1.0f, 0.0f, 0.0f);
	//glVertex3f(-3.0f, 0.0f, 0.0f);
	//glVertex3f(3.0f, 0.0f, 0.0f);
	////y  GREEN 
	//glColor3f(0.0f, 1.0f, 0.0f);
	//glVertex3f(0.0f, -3.0f, 0.0f);
	//glVertex3f(0.0f, 3.0f, 0.0f);
	////z  BLUE
	//glColor3f(0.0f, 0.0f, 1.0f);
	//glVertex3f(0.0f, 0.0f, -3.0f);
	//glVertex3f(0.0f, 0.0f, 3.0f);
	//glEnd();

	// Translate
	glTranslatef(xShift, yShift, zShift);

	// Rotate
	glRotatef(xAngle, 1, 0, 0);
	glRotatef(yAngle, 0, 1, 0);
	glRotatef(zAngle, 0, 0, 1);

	enableLights();


	glColor3f(1.0f, 1.0f, 1.0f); // color white for texture
	GLUquadricObj* obj;
	obj = gluNewQuadric();

	// head - sphere
	glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		gluQuadricTexture(obj, true);
		glBindTexture(GL_TEXTURE_2D, idTexture[REALFACE_IMAGE]);//select idTexture[EARTH_IMAGE]
		glTranslatef(0.0f, 4.0f, 0.0f); // move add y+=4
		glRotatef(-90, 1, 0, 0);
		//glColor3f(0.1f, 0.1f, 0.7f);
		gluSphere(obj, 1.1, 32, 32);
		glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	// neck
	glPushMatrix();
		glTranslatef(0.0f, 3.0f, 0.0f);
		glRotatef(90, 1, 0, 0);
		glColor3f(0.1f, 0.7f, 0.7f);
		gluSphere(obj, 0.5, 32, 32);
	glPopMatrix();

	// body top cover
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	gluQuadricTexture(obj, true);
	glBindTexture(GL_TEXTURE_2D, idTexture[LEG_IMAGE]);//select idTexture[EARTH_IMAGE]
		disableLights();
		glTranslatef(0, 2.5, 0);
		glRotatef(45, 0, 1, 0);
		glRotatef(90, 1, 0, 0);
		//glColor3f(0.5f, 1.0f, 0.7f);
		glColor3f(1.0, 1.0, 1.0);
		gluDisk(obj, 0, 1.5, 4, 4);
		enableLights();
		
	glPopMatrix();
	
	//body 
	glPushMatrix();

		//glTranslatef(0.0f, 2.3f, 0.0f);
		glRotatef(45, 0, 1, 0); // rotate y 45 degrees
		glRotatef(-90, 1, 0, 0); // rotate x
		//glColor3f(0.5f, 1.0f, 0.7f);
		gluCylinder(obj, 1.06, 1.5, 2.5, 32, 32);
		
	glPopMatrix();

	// body bottom cover
	glPushMatrix();
		disableLights();
		//glTranslatef(0, 2.5, 0);
		glRotatef(45, 0, 1, 0);
		glRotatef(90, 1, 0, 0);
		//glColor3f(0.5f, 1.0f, 0.7f);
		gluDisk(obj, 0, 1.5, 4, 4);
		enableLights();
	glPopMatrix();

	//left circle for left hand
	glPushMatrix();
		glTranslatef(1.2f, 2.0f, 0.0f);
		glRotatef(90, 1, 0, 0); // rotate x
		//glColor3f(1.0f, 1.0f, 0.7f);
		gluSphere(obj, 0.6, 32, 32);
	glPopMatrix();

	//right circle for left hand
	glPushMatrix();
		glTranslatef(-1.2f, 2.0f, 0.0f);
		glRotatef(90, 1, 0, 0); // rotate x
		//glColor3f(1.0f, 1.0f, 0.7f);
		gluSphere(obj, 0.6, 32, 32);
		glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	// circle for down body
	glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		gluQuadricTexture(obj, true);
		glBindTexture(GL_TEXTURE_2D, idTexture[UNDERWEAR_IMAGE]);//select idTexture[EARTH_IMAGE]
		glTranslatef(0.0f, 0.0f, 0.0f);
		glRotatef(90, 1, 0, 0); // rotate x
		glColor3f(1.0f, 1.0f, 1.7f);
		gluSphere(obj, 1.06, 32, 32);
		glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	// left leg
	glPushMatrix();
		glTranslatef(0.6f, 0.0f, 0.0f);
		//glRotatef(45, 0, 1, 0); // rotate y 45 degrees
		glRotatef(90, 1, 0, 0); // rotate x
		glColor3f(0.5f, 0.5f, 1.7f);
		gluCylinder(obj, 0.35,0.35, 2.5, 32, 32);
	glPopMatrix();

	// right leg
	glPushMatrix();
		glTranslatef(-0.6f, 0.0f, 0.0f);
		//glRotatef(45, 0, 1, 0); // rotate y 45 degrees
		glRotatef(90, 1, 0, 0); // rotate x
		glColor3f(0.5f, 0.5f, 1.7f);
		gluCylinder(obj, 0.35, 0.35, 2.5, 32, 32);
	glPopMatrix();


	// rectangle for left leg
	//glPushMatrix();

	//	glTranslatef(0.0f, -2.5f, 0.0f);
	//	glRotatef(-45, 0, 0, 1); // rotate y 45 degrees
	//	//glRotatef(0, 1, 0, 0); // rotate x
	//	glColor3f(0.5f, 0.5f, 1.7f);
	//	gluCylinder(obj, 0.5,2,1,4,2);
	//glPopMatrix();



	//glRotatef(-180, 1, 0, 0);
	//glTranslatef(0.0f, 15.0f, 0.0f);
	//glColor3f(0.1f, 0.1f, 0.7f);
	//gluCylinder(obj, 0.5, 0.5, 6, 16, 16);

	disableLights();
	gluDeleteQuadric(obj);

	// swap the buffer
	SwapBuffers(m_hDC);

}


float CGlView::getZShift() {
	return this->zShift;
}

float CGlView::getYShift() {
	return this->yShift;
}
float CGlView::getXShift() {
	return this->xShift;
}

void CGlView::setZShift(float zShift) {
	this->zShift = zShift;
}
void CGlView::setYShift(float yShift) {
	this->yShift = yShift;
}
void CGlView::setXShift(float xShift) {
	this->xShift = xShift;
}

float CGlView::getZAngle() {
	return this->zAngle;
}
float CGlView::getYAngle() {
	return this->yAngle;
}
float CGlView::getXAngle() {
	return this->xAngle;
}

void CGlView::setZAngle(float zAngle) {
	this->zAngle = zAngle;
}
void CGlView::setYAngle(float yAngle) {
	this->yAngle = yAngle;
}
void CGlView::setXAngle(float xAngle) {
	this->xAngle = xAngle;
}

void CGlView::disableLights()
{
	//Disable color and light effects
	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHTING);
}

void CGlView::enableLights()
{
	//Enable color and light effects
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
}

void CGlView::generateTextureFromImageFile()
{
	glGenTextures(SIZE, idTexture);//generates ID for each texture.

	const char* imageFileName[SIZE] = { "res\\earth.bmp", "res\\env.bmp", "res\\flower.bmp","res\\leg.bmp","res\\underwear.bmp","res\\realface.bmp" };

	for (int i = 0; i < SIZE; i++)
	{
		texture = auxDIBImageLoad(imageFileName[i]);//loads the width, height, colors from the image file (*.bmp) into opengl structure (AUX_RGBImageRec).

		if (texture)
		{
			glBindTexture(GL_TEXTURE_2D, idTexture[i]);//working on idTexture[i]
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//if the target is smaller than the source then minimize it using LINEAR algorithm.
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//if the target is bigger than the source then maximize it using LINEAR algorithm.
			glTexImage2D(GL_TEXTURE_2D, 0, 3, texture->sizeX, texture->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, texture->data);//save texture in memory.
			free(texture);
			texture = NULL;
		}
	}
}

//
//void CGlView::vDrawHoleInAPit() {
//	glBegin(GL_QUADS);
//	glVertex3d(-5, 0, 5);
//	glVertex3d(-5, 10, 5);
//	glVertex3d(5, 10, 5);
//	glVertex3d(5, 0, 5);
//	glEnd();
//}
//void CGlView::vDrawChest() {
//	//wglSwapLayerBuffers(m_hDC,)
//
//
//	glPushMatrix();// save starting position of drawing
//	glTranslatef(-5.0f, 0.0f, 0.0f);
//	glColor3f(1.0f, 1.0f, 1.0f);
//	glEnable(GL_TEXTURE_2D);
//	//glBindTexture(GL.GL_TEXTURE_2D, Textures[(0 + textureOffset) % Textures.Length]);
//	glBegin(GL_QUADS);
//
//	//bottom
//	glTexCoord2f(0.0f, 0.0f);
//	glVertex3d(0, 0, 0);
//	glTexCoord2f(0.0f, 1.0f);
//	glVertex3d(0, 10, 0);
//	glTexCoord2f(1.0f, 1.0f);
//	glVertex3d(10, 10, 0);
//	glTexCoord2f(1.0f, 0.0f);
//	glVertex3d(10, 0, 0);
//
//	//top
//	glTexCoord2f(0.0f, 0.0f);
//	glVertex3d(0, 0, 5);
//	glTexCoord2f(0.0f, 1.0f);
//	glVertex3d(0, 10, 5);
//	glTexCoord2f(1.0f, 1.0f);
//	glVertex3d(10, 10, 5);
//	glTexCoord2f(1.0f, 0.0f);
//	glVertex3d(10, 0, 5);
//
//	glEnd();
//	//GL.glBindTexture(GL.GL_TEXTURE_2D, Textures[(1 + textureOffset) % Textures.Length]);
//	glBegin(GL_QUADS);
//
//	//top - other texture
//	glTexCoord2f(0.0f, 0.0f);
//	glVertex3d(0.5, 0.5, 5.01f);
//	glTexCoord2f(0.0f, 1.0f);
//	glVertex3d(0.5, 9.5, 5.01f);
//	glTexCoord2f(1.0f, 1.0f);
//	glVertex3d(9.5, 9.5, 5.01f);
//	glTexCoord2f(1.0f, 0.0f);
//	glVertex3d(9.5, 0.5, 5.01f);
//
//	glEnd();
//	//GL.glBindTexture(GL.GL_TEXTURE_2D, Textures[(0 + textureOffset) % Textures.Length]);
//	glBegin(GL_QUADS);
//
//	//front
//	glTexCoord2f(0.0f, 0.0f);
//	glVertex3d(0, 10, 0);
//	glTexCoord2f(0.0f, 1.0f);
//	glVertex3d(0, 10, 5);
//	glTexCoord2f(1.0f, 1.0f);
//	glVertex3d(10, 10, 5);
//	glTexCoord2f(1.0f, 0.0f);
//	glVertex3d(10, 10, 0);
//
//	//back
//	glTexCoord2f(0.0f, 0.0f);
//	glVertex3d(0, 0, 0);
//	glTexCoord2f(0.0f, 1.0f);
//	glVertex3d(0, 0, 5);
//	glTexCoord2f(1.0f, 1.0f);
//	glVertex3d(10, 0, 5);
//	glTexCoord2f(1.0f, 0.0f);
//	glVertex3d(10, 0, 0);
//
//	//left
//	glTexCoord2f(0.0f, 0.0f);
//	glVertex3d(0, 0, 0);
//	glTexCoord2f(0.0f, 1.0f);
//	glVertex3d(0, 0, 5);
//	glTexCoord2f(1.0f, 1.0f);
//	glVertex3d(0, 10, 5);
//	glTexCoord2f(1.0f, 0.0f);
//	glVertex3d(0, 10, 0);
//
//	//right
//	glTexCoord2f(0.0f, 0.0f);
//	glVertex3d(10, 0, 0);
//	glTexCoord2f(0.0f, 1.0f);
//	glVertex3d(10, 0, 5);
//	glTexCoord2f(1.0f, 1.0f);
//	glVertex3d(10, 10, 5);
//	glTexCoord2f(1.0f, 0.0f);
//	glVertex3d(10, 10, 0);
//
//	glEnd();
//	glDisable(GL_TEXTURE_2D);
//	glPopMatrix();
//
//
//}
//
//void CGlView::vDrawChestLidTop()  //draws only top of chest lid - for use of shadow stencil
//{
//	glPushMatrix();// save starting position of drawing
//
//	glColor3f(1.0f, 1.0f, 1.0f);
//	glTranslatef(-5.0f, 0.0f, 0.0f);
//	glTranslatef(0, 10.0f, 5.0f);
//	glRotated(mirrorAngle, 1, 0, 0);
//	glBegin(GL_QUADS);
//	//top
//	glVertex3d(0, 0, 0);
//	glVertex3d(0, 10, 0);
//	glVertex3d(10, 10, 0);
//	glVertex3d(10, 0, 0);
//	glEnd();
//	glPopMatrix();
//}
//
//void CGlView::vDrawChestLid()
//{
//	glPushMatrix();// save starting position of drawing
//
//	glColor3f(1.0f, 1.0f, 1.0f);
//	glEnable(GL_TEXTURE_2D);
//	//glBindTexture(GL_TEXTURE_2D, Textures[(0 + textureOffset) % Textures.Length]);
//	glTranslatef(-5.0f, 0.0f, 0.0f);
//	glTranslatef(0, 10.0f, 5.0f);
//	glRotated(mirrorAngle, 1, 0, 0);
//
//	glBegin(GL_QUADS);
//
//	//bottom
//	glTexCoord2f(0.0f, 0.0f);
//	glVertex3d(0, 0, -2);
//	glTexCoord2f(0.0f, 1.0f);
//	glVertex3d(0, 10, -2);
//	glTexCoord2f(1.0f, 1.0f);
//	glVertex3d(10, 10, -2);
//	glTexCoord2f(1.0f, 0.0f);
//	glVertex3d(10, 0, -2);
//
//	//top
//	glTexCoord2f(0.0f, 0.0f);
//	glVertex3d(0, 0, 0);
//	glTexCoord2f(0.0f, 1.0f);
//	glVertex3d(0, 10, 0);
//	glTexCoord2f(1.0f, 1.0f);
//	glVertex3d(10, 10, 0);
//	glTexCoord2f(1.0f, 0.0f);
//	glVertex3d(10, 0, 0);
//
//	//front
//	glTexCoord2f(0.0f, 0.0f);
//	glVertex3d(0, 10, -2);
//	glTexCoord2f(0.0f, 1.0f);
//	glVertex3d(0, 10, 0);
//	glTexCoord2f(1.0f, 1.0f);
//	glVertex3d(10, 10, 0);
//	glTexCoord2f(1.0f, 0.0f);
//	glVertex3d(10, 10, -2);
//
//	//back
//	glTexCoord2f(0.0f, 0.0f);
//	glVertex3d(0, 0, -2);
//	glTexCoord2f(0.0f, 1.0f);
//	glVertex3d(0, 0, 0);
//	glTexCoord2f(1.0f, 1.0f);
//	glVertex3d(10, 0, 0);
//	glTexCoord2f(1.0f, 0.0f);
//	glVertex3d(10, 0, -2);
//
//	//left
//	glTexCoord2f(0.0f, 0.0f);
//	glVertex3d(0, 0, -2);
//	glTexCoord2f(0.0f, 1.0f);
//	glVertex3d(0, 0, 0);
//	glTexCoord2f(1.0f, 1.0f);
//	glVertex3d(0, 10, 0);
//	glTexCoord2f(1.0f, 0.0f);
//	glVertex3d(0, 10, -2);
//
//	//right
//	glTexCoord2f(0.0f, 0.0f);
//	glVertex3d(10, 0, -2);
//	glTexCoord2f(0.0f, 1.0f);
//	glVertex3d(10, 0, 0);
//	glTexCoord2f(1.0f, 1.0f);
//	glVertex3d(10, 10, 0);
//	glTexCoord2f(1.0f, 0.0f);
//	glVertex3d(10, 10, -2);
//
//	glEnd();
//	glDisable(GL_TEXTURE_2D);
//	glPopMatrix();
//}
//
//void CGlView::vDrawTheWomanInRed()
//{
//	GLUquadricObj* obj;
//	obj = gluNewQuadric();
//	glPushMatrix();
//	glColor3f(1.0f, 1.0f, 1.0f);
//	glTranslatef(0.0f, 5.0f, 5.2f);
//	glRotated(intOptionB, 0, 0, 1); //rotating the dancer
//	glPushMatrix();// save position of dancer
//	//if (shadow == false)
//	//	pre_MapSphereTexture(2);
//	glRotated(90, 1, 0, 0);
//	//glutSolidTeapot(0.5);
//	glRotated(-90, 1, 0, 0);
//	end_MapTexture();
//	/*if (shadow == false)
//		pre_MapCylinderTexture(3);*/
//	gluCylinder(obj, 0.2, 0.4, 4, 20, 20);
//	glTranslated(0.4f, 0.0f, 2.0f);
//	end_MapTexture();
//
//	//right leg - rotate to leg angle
//	glRotated(50, 0, 1, 0);
//	//right shoe
//	//if (shadow == false)
//	//	pre_MapSphereTexture(2);
//	glRotated(90, 1, 0, 0);
//	//glutSolidTeapot(0.5);
//	glRotated(-90, 1, 0, 0);
//	end_MapTexture();
//
//	//lower right leg
//	//if (shadow == false)
//	//	pre_MapCylinderTexture(3);
//	gluCylinder(obj, 0.2, 0.25, 2, 20, 20);
//	glTranslated(0.0f, 0.0f, 2.0f);
//	glRotated(-100, 0, 1, 0);
//
//	//right knee
//	gluSphere(obj, 0.25, 20, 20);
//
//	//upper second leg
//	gluCylinder(obj, 0.25, 0.4, 2, 20, 20);
//	glPopMatrix(); //reset position to the first leg
//	end_MapTexture();
//
//	//skirt
//	//if (shadow == false)
//	//	pre_MapCylinderTexture(4);
//	glTranslated(0.0f, 0.0f, 4.0f);
//	gluCylinder(obj, 2, 0.75, 1.5, 20, 20);
//	glTranslated(0.0f, 0.0f, 1.5f);
//	end_MapTexture();
//	//if (shadow == false)
//	//	pre_MapCylinderTexture(5);
//	gluCylinder(obj, 0.75, 0.75, 2.5, 20, 20);
//	glTranslated(0.0f, 0.0f, 2.5f);
//	end_MapTexture();
//
//	//upper blob
//	//if (shadow == false)
//	//	pre_MapSphereTexture(5);
//	gluSphere(obj, 0.75, 20, 20);
//	end_MapTexture();
//	//Right
//	glPushMatrix();
//	glTranslated(0.75f, 0.0f, 0.0f);
//	//right shoulder
//	//if (shadow == false)
//	//	pre_MapSphereTexture(5);
//	gluSphere(obj, 0.3, 20, 20);
//	end_MapTexture();
//	glRotated(shoulderAngle, 1, 0, 0);
//	//right arm
//	//if (shadow == false)
//	//	pre_MapCylinderTexture(5);
//	gluCylinder(obj, 0.2, 0.19, 1.5, 20, 20);
//	glTranslated(0.0f, 0.0f, 1.5f);
//	end_MapTexture();
//	//right elbow
//	//if (shadow == false)
//	//	pre_MapSphereTexture(5);
//	gluSphere(obj, 0.3, 20, 20);
//	glRotated(-15, 1, 0, 0);
//	end_MapTexture();
//	//right forearm
//	//if (shadow == false)
//	//	pre_MapCylinderTexture(3);
//	gluCylinder(obj, 0.2, 0.19, 1.5, 20, 20);
//	glTranslated(0.0f, 0.0f, 1.5f);
//	//right hand
//	gluCylinder(obj, 0.2, 0, .5, 20, 20);
//	glPopMatrix();
//	end_MapTexture();
//	//Left
//	glPushMatrix();
//	glTranslated(-0.75f, 0.0f, 0.0f);
//	//left shoulder
//	//if (shadow == false)
//	//	pre_MapSphereTexture(5);
//	gluSphere(obj, 0.3, 20, 20);
//	glRotated(shoulderAngle, 1, 0, 0);
//	end_MapTexture();
//	//left arm
//	//if (shadow == false)
//	//	pre_MapCylinderTexture(5);
//	gluCylinder(obj, 0.2, 0.19, 1.5, 20, 20);
//	glTranslated(0.0f, 0.0f, 1.5f);
//	end_MapTexture();
//	//left elbow
//	//if (shadow == false)
//	//	pre_MapSphereTexture(5);
//	gluSphere(obj, 0.3, 20, 20);
//	glRotated(-15, 1, 0, 0);
//	end_MapTexture();
//	//left forearm
//	//if (shadow == false)
//	//	pre_MapCylinderTexture(3);
//	gluCylinder(obj, 0.2, 0.19, 1.5, 20, 20);
//	glTranslated(0.0f, 0.0f, 1.5f);
//	//left hand
//	gluCylinder(obj, 0.2, 0, .5, 20, 20);
//	glPopMatrix();
//	//neck
//	gluCylinder(obj, 0.2, 0.19, 0.5 + 0.75, 20, 20);
//	glTranslated(0.0f, 0.0f, 1.2f);
//	end_MapTexture();
//	//head
//	//if (shadow == false)
//	//	pre_MapCylinderTexture(6);
//	gluSphere(obj, 0.5, 20, 20);
//	glPopMatrix();
//	end_MapTexture();
//
//	gluDeleteQuadric(obj);
//
//}
//
//void CGlView::vDrawMirror() {
//	glPushMatrix();
//	glTranslated(-5, 10, 5);
//	glRotated(mirrorAngle, 1, 0, 0);
//	glEnable(GL_LIGHTING);
//	glBegin(GL_QUADS);
//
//	glColor4d(0, 0, 1, 0.5);
//	glVertex3d(0.5, 0.5, 0.05f);
//	glVertex3d(0.5, 9.5, 0.05f);
//	glVertex3d(9.5, 9.5, 0.05f);
//	glVertex3d(9.5, 0.5, 0.05f);
//	glEnd();
//	glPopMatrix();
//}
//
//void CGlView::Draw() {
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
//	glLoadIdentity();
//
//	//Handeling of translate rotate - mathematically correct
//	double ModelVievMatrixBeforeSpecificTransforms[16];
//	double CurrentRotationTraslation[16];
//
//	glTranslatef(0.0f, 0.0f, -1.0f);
//	glTranslatef(0.0f, 0.0f, -10.0f);
//
//	//glEnable(GL_COLOR_MATERIAL);
//	//glEnable(GL_LIGHT0);
//	//glEnable(GL_LIGHTING);
//	//save current ModelView Matrix values
//	//in ModelVievMatrixBeforeSpecificTransforms array
//	//ModelView Matrix ========>>>>>> ModelVievMatrixBeforeSpecificTransforms
//	glGetDoublev(GL_MODELVIEW_MATRIX, ModelVievMatrixBeforeSpecificTransforms);
//	//ModelView Matrix was saved, so
//	glLoadIdentity(); // make it identity matrix
//	
//	//make transformation in accordance to KeyCode
//	float delta;
//	if (intOptionC != 0)
//	{
//		delta = 5.0f * abs(intOptionC) / intOptionC; // signed 5
//
//		switch (abs(intOptionC))
//		{
//		case 1:
//			glRotatef(delta, 1, 0, 0);
//			break;
//		case 2:
//			glRotatef(delta, 0, 1, 0);
//			break;
//		case 3:
//			glRotatef(delta, 0, 0, 1);
//			break;
//		case 4:
//			glTranslatef(delta / 20, 0, 0);
//			break;
//		case 5:
//			glTranslatef(0, delta / 20, 0);
//			break;
//		case 6:
//			glTranslatef(0, 0, delta / 20);
//			break;
//		}
//	}
//
//	//as result - the ModelView Matrix now is pure representation
//			//of KeyCode transform and only it !!!
//
//			//save current ModelView Matrix values
//			//in CurrentRotationTraslation array
//			//ModelView Matrix =======>>>>>>> CurrentRotationTraslation
//	glGetDoublev(GL_MODELVIEW_MATRIX, CurrentRotationTraslation);
//
//	//The GL.glLoadMatrix function replaces the current matrix with
//	//the one specified in its argument.
//	//The current matrix is the
//	//projection matrix, modelview matrix, or texture matrix,
//	//determined by the current matrix mode (now is ModelView mode)
//	glLoadMatrixd(AccumulatedRotationsTraslations); //Global
//
//	//The GL.glMultMatrix function multiplies the current matrix by
//			//the one specified in its argument.
//			//That is, if M is the current matrix and T is the matrix passed to
//			//GL.glMultMatrix, then M is replaced with M • T
//	glMultMatrixd(CurrentRotationTraslation);
//
//	//save the matrix product in AccumulatedRotationsTraslations
//	glGetDoublev(GL_MODELVIEW_MATRIX, AccumulatedRotationsTraslations);
//
//	//replace ModelViev Matrix with stored ModelVievMatrixBeforeSpecificTransforms
//	glLoadMatrixd(ModelVievMatrixBeforeSpecificTransforms);
//	//multiply it by KeyCode defined AccumulatedRotationsTraslations matrix
//	glMultMatrixd(AccumulatedRotationsTraslations);
//
//	//end of - Handeling of translate rotate mathematically correct
//
//	//Animation Values
//	//intOptionC += 2;
//	//intOptionB += 2;    //dancer rotation
//	//sin_index++;
//
//	//for arms animation
//	shoulderAngle = (int)(45 + 45 * sin((2 * PI) / 100 * sin_index));
//
//	//mising code
//	vDrawChest();
//	vDrawChestLid();
//	vDrawTheWomanInRed();
//
//	glPopMatrix();
//	glDisable(GL_LIGHTING);
//	glDisable(GL_STENCIL_TEST);
//	glFlush();
//
//	
//
//	// swap the buffer
//	SwapBuffers(m_hDC);
//}



//void CGlView::end_MapTexture()
//{
//	//GL.glDisable(GL.GL_TEXTURE_GEN_S);
//	//GL.glDisable(GL.GL_TEXTURE_GEN_T);
//	glDisable(GL_TEXTURE_2D);
//}

// +
// *****************************************************************************
//   Module      :  BOOL CGlView::SetWindowPixelFormat(HDC hDC)
//   Access      :  Private
//   Input       :  hDC - Handle to the GL windwos device context
//   Output      :  None
//   Return Val  :  TRUE  - success
//									FALSE - failure
//   Exceptions  :  None
//   Desc        :  This function sets up the required pixelformat descriptor
//
//   Notes       :  None
// *****************************************************************************
// -
BOOL CGlView::bSetWindowPixelFormat(HDC hDC)
{
	PIXELFORMATDESCRIPTOR pixelDesc;

	pixelDesc.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pixelDesc.nVersion = 1;

	pixelDesc.dwFlags =

		PFD_DRAW_TO_WINDOW |// Format Must Support Window
		PFD_SUPPORT_OPENGL |// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,	// Must Support Double Buffering
		PFD_TYPE_RGBA,		// Request An RGBA Format
		32,					// Select A 16Bit Color Depth
		0, 0, 0, 0, 0, 0,	// Color Bits Ignored (?)
		0,					// No Alpha Buffer
		0,					// Shift Bit Ignored (?)
		0,					// No Accumulation Buffer
		0, 0, 0, 0,			// Accumulation Bits Ignored (?)
		16,					// 16Bit Z-Buffer (Depth Buffer)  
		0,					// No Stencil Buffer
		0,					// No Auxiliary Buffer (?)
		PFD_MAIN_PLANE,		// Main Drawing Layer
		0,					// Reserved (?)
		0, 0, 0;			// Layer Masks Ignored (?)

	int iGLPixelIndex = ChoosePixelFormat(hDC, &pixelDesc);
	ASSERT(&iGLPixelIndex);

	if (SetPixelFormat(hDC, iGLPixelIndex, &pixelDesc))
		return false;

	return TRUE;
}

// +
// *****************************************************************************
//   Module      :  BOOL CGlView::CreateViewGLContext(HDC hDC)
//   Access      :  Private
//   Input       :  hDC - Handle to the GL windwos device context
//   Output      :  None
//   Return Val  :  TRUE  - success
//									FALSE - failure
//   Exceptions  :  None
//   Desc        :  This function sets up the required GLContext
//
//   Notes       :  None
// *****************************************************************************
// -
BOOL CGlView::bCreateViewGLContext(HDC hDC)
{
	m_hGLContext = wglCreateContext(hDC);
	if (m_hGLContext == NULL)
		return FALSE;

	if (wglMakeCurrent(hDC, m_hGLContext) == FALSE)
		return FALSE;

	return TRUE;
}

// +
// *****************************************************************************
//   Module      :  void CGlView::InitGl(int iWidth, int iHeigth)
//   Access      :  Private
//   Input       :  iWidth  - width of the window
//									iHeight - height of the window
//   Output      :  None
//   Return Val  :  None
//   Exceptions  :  None
//   Desc        :  This function initializes all OpenGL rendering specifics
//
//   Notes       :  None
// *****************************************************************************
// -
void CGlView::vInitGl(int iWidth, int iHeigth)
{
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glViewport(0, 0, iWidth, iHeigth);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f, (GLfloat)iWidth / (GLfloat)iHeigth, 0.45f, 100.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


