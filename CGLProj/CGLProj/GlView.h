// GlView.h: interface for the CGlView class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLVIEW_H__6EDA23AE_B764_11D5_8D52_0004AC3DB799__INCLUDED_)
#define AFX_GLVIEW_H__6EDA23AE_B764_11D5_8D52_0004AC3DB799__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CGlView  
{
public:
	CGlView(CWnd *pclWnd);
	~CGlView();

	void vDrawGLScene();

	protected:
	private:
		void vInitGl(int iWidth, int iHeigth);
		BOOL bSetWindowPixelFormat(HDC hDC);
		BOOL bCreateViewGLContext(HDC hDC);

	// members //////////////////////////////////////////////////////////
	public:
	protected:
	private:			
		HGLRC	m_hGLContext;	// required GLContext
		CWnd *m_pclWnd;			// window inwhich opengl will be drawn
		HDC		m_hDC;			// handle to m_pclWnds device context

};

#endif

