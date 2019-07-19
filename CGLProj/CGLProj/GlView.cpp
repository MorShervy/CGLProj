// GlView.cpp: implementation of the CGlView class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GlView.h"

#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGlView::CGlView(CWnd *pclWnd)
{
	m_pclWnd = pclWnd;
	m_hDC  = ::GetDC(pclWnd->m_hWnd);
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
	if(wglGetCurrentContext()!=NULL) 
		wglMakeCurrent(NULL, NULL);
	
	// delete the Context
  if (m_hGLContext!=NULL)
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
	glTranslatef(0.0f, 0.0f, -8.0f);						
	
	//axes
	glBegin(GL_LINES);	
	    //x  RED
	    glColor3f(1.0f,0.0f,0.0f);						
		glVertex3f( -3.0f, 0.0f, 0.0f);	
		glVertex3f( 3.0f, 0.0f, 0.0f);	
	    //y  GREEN 
	    glColor3f(0.0f,1.0f,0.0f);						
		glVertex3f( 0.0f, -3.0f, 0.0f);	
		glVertex3f( 0.0f, 3.0f, 0.0f);	
	    //z  BLUE
	    glColor3f(0.0f,0.0f,1.0f);						
		glVertex3f( 0.0f, 0.0f, -3.0f);	
		glVertex3f( 0.0f, 0.0f, 3.0f);	
    glEnd();
			

	glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);

    GLUquadricObj* obj;
        obj = gluNewQuadric();
		glRotatef (-90, 1, 0, 0);
		glColor3f (0.2f, 0.8f, 0.0f);
		gluCylinder( obj, 1.5, 0, 2, 16, 16);	
		glRotatef (90, 1, 0, 0);
        glColor3f (1, 0, 0);
        gluSphere( obj, 1.1, 32, 32);
        glRotatef (90, 1, 0, 0);
        glColor3f (0.1f, 0.1f, 0.7f);
        gluCylinder( obj, 0.5, 0.5, 6, 16, 16);

	gluDeleteQuadric(obj);

	// swap the buffer
	SwapBuffers(m_hDC);
	
}

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

    pixelDesc.nSize     = sizeof(PIXELFORMATDESCRIPTOR);
    pixelDesc.nVersion	= 1;

    pixelDesc.dwFlags   =    
	
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

    int iGLPixelIndex = ChoosePixelFormat( hDC, &pixelDesc);
		ASSERT(&iGLPixelIndex);

    if (SetPixelFormat( hDC, iGLPixelIndex, &pixelDesc))
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

    if (wglMakeCurrent(hDC, m_hGLContext)==FALSE)
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

	glViewport(0,0,iWidth,iHeigth);						

	glMatrixMode(GL_PROJECTION);						
	glLoadIdentity();								

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f,(GLfloat)iWidth/(GLfloat)iHeigth,0.45f,100.0f);

	glMatrixMode(GL_MODELVIEW);							
	glLoadIdentity();		
}


