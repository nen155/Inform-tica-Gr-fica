// objviewerView.cpp : implementation of the CNormalsView class
//

#include "stdafx.h"
#include "glincludes.h"
#include "Normals.h"

#include "NormalsDoc.h"
#include "NormalsView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CNormalsView

IMPLEMENT_DYNCREATE(CNormalsView, CView)

BEGIN_MESSAGE_MAP(CNormalsView, CView)
	//{{AFX_MSG_MAP(CNormalsView)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	ON_WM_SIZE()
	ON_COMMAND(ID_DISP_FLAT, OnDispFlat)
	ON_COMMAND(ID_DISP_SMOOTH, OnDispSmooth)
	ON_UPDATE_COMMAND_UI(ID_DISP_FLAT, OnUpdateDispFlat)
	ON_UPDATE_COMMAND_UI(ID_DISP_SMOOTH, OnUpdateDispSmooth)
	ON_COMMAND(ID_DISP_NONE, OnDispNone)
	ON_UPDATE_COMMAND_UI(ID_DISP_NONE, OnUpdateDispNone)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNormalsView construction/destruction

CNormalsView::CNormalsView() : 
	m_pGLDC(NULL),
	m_nTimerID (0),	
	m_shading (SHADING_NONE)
{
	m_center.x = m_center.y = m_center.z = 0;
	m_oyAngle = (float)_PI/3;
	m_radius = 400;
}


CNormalsView::~CNormalsView()
{
}


/////////////////////////////////////////////////////////////////////////////
// CNormalsView drawing
void CNormalsView::OnDraw(CDC* pDC)
{
}

/////////////////////////////////////////////////////////////////////////////
// CNormalsView diagnostics

#ifdef _DEBUG
void CNormalsView::AssertValid() const
{
	CView::AssertValid();
}

void CNormalsView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CNormalsDoc* CNormalsView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CNormalsDoc)));
	return (CNormalsDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CNormalsView message handlers

void CNormalsView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	
	ModifyStyle(0, WS_CLIPCHILDREN|WS_CLIPSIBLINGS);

	m_pGLDC = new CClientDC(this);

	PIXELFORMATDESCRIPTOR pfd = { 
		sizeof(PIXELFORMATDESCRIPTOR),  // size of this pfd
		1,                              // version number
		PFD_DRAW_TO_WINDOW |            // support window
		  PFD_SUPPORT_OPENGL |          // support OpenGL
		  PFD_DOUBLEBUFFER,             // double buffered
		PFD_TYPE_RGBA,                  // RGBA type
		24,                             // 24-bit color depth
		0, 0, 0, 0, 0, 0,               // color bits ignored
		0,                              // no alpha buffer
		0,                              // shift bit ignored
		0,                              // no accumulation buffer
		0, 0, 0, 0,                     // accum bits ignored
		16,                             // 16-bit z-buffer
		0,                              // no stencil buffer
		0,                              // no auxiliary buffer
		PFD_MAIN_PLANE,                 // main layer
		0,                              // reserved
		0, 0, 0                         // layer masks ignored
	};

	// get the best available match of pixel format for the device context  
	int iPixelFormat = ChoosePixelFormat(m_pGLDC->GetSafeHdc(), &pfd);  

	// make that the pixel format of the device context 
	SetPixelFormat(m_pGLDC->GetSafeHdc(), iPixelFormat, &pfd);
	
	// Get the pixel format
	iPixelFormat = ::GetPixelFormat(m_pGLDC->GetSafeHdc());
	::DescribePixelFormat(m_pGLDC->GetSafeHdc(), iPixelFormat, sizeof(pfd), &pfd);

	HGLRC hglrc = wglCreateContext(m_pGLDC->GetSafeHdc());
	ASSERT(hglrc);

	if (hglrc)
	{
		VERIFY(wglMakeCurrent(m_pGLDC->GetSafeHdc(), hglrc));

		GL_VERIFY(glMatrixMode(GL_PROJECTION));
		RECT r;
		GetClientRect(&r);

		// Compute the field of view angle for the perspective matrix
		#define EYE_TO_SCREEN	(13.5)	// Distance from eye to screen in inches
		double yinch = (double)r.bottom / m_pGLDC->GetDeviceCaps(LOGPIXELSY);
		m_fovAngle = asin(yinch / EYE_TO_SCREEN);
		m_aspectRatio = (GLdouble)r.right / r.bottom;
		gluPerspective(RAD2DEG(m_fovAngle), m_aspectRatio, 100, 10000);

		GL_VERIFY(glClearColor(0, 0, 0, 0));
		GL_VERIFY(glEnable(GL_DEPTH_TEST));
		GL_VERIFY(glShadeModel(GL_FLAT));
		GL_VERIFY(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));

		// Set a default lightning
		GL_VERIFY(glEnable(GL_LIGHTING));
		GL_VERIFY(glEnable(GL_LIGHT0));
		float ld[4] = { 1, 1, 1, 0 };
		GL_VERIFY(glLightfv(GL_LIGHT0, GL_AMBIENT, ld));
		GL_VERIFY(glLightfv(GL_LIGHT0, GL_DIFFUSE, ld));
		GL_VERIFY(glLightfv(GL_LIGHT0, GL_SPECULAR, ld));

		// Set a default model view matrix
		SetEyePos();

		// Set a default material
		GLfloat a[4] = { 0, 0, 0, 1 };
		glMaterialfv(GL_FRONT, GL_AMBIENT, a);
		GLfloat d[4] = { 1, 0, 0, 1 };
		glMaterialfv(GL_FRONT, GL_DIFFUSE, d);
		glMaterialfv(GL_FRONT, GL_SPECULAR, d);
		glMaterialf(GL_FRONT, GL_SHININESS, 128);

		// Start redraw timer
		m_nTimerID = SetTimer(1, 10, NULL);
	}
}

void CNormalsView::OnTimer(UINT nIDEvent) 
{
	// Redraw scene
	DrawScene();	
	
	CView::OnTimer(nIDEvent);
}


static void glDrawAxis ()
{
	GLboolean bLightning = glIsEnabled(GL_LIGHTING);
	glDisable(GL_LIGHTING);

	glBegin(GL_LINES );
		glColor3f(1, 0, 0);
		glVertex3f(-1000, 0,  0);
		glVertex3f(1000, 0,  0);
	glEnd();
	glBegin(GL_LINES );
		glColor3f(0, 1, 0);
		glVertex3f(0, -1000,  0);
		glVertex3f(0, 1000,  0);
	glEnd();
	glBegin(GL_LINES );
		glColor3f(0, 0, 1);
		glVertex3f(0, 0,  -1000);
		glVertex3f(0, 0,  1000);
	glEnd();

	if (bLightning)
		glEnable(GL_LIGHTING);
}


void CNormalsView::DrawScene()
{
	// Protect ourself against reentrance calls
    static bool bBusy = false;
    if (bBusy)
		return;
    bBusy = true;

    GL_VERIFY(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

	// Draw the static object
	WORD wFlags = 0;
	switch (m_shading)
	{
		case SHADING_FLAT: wFlags |= DF_FLAT; break;
		case SHADING_SMOOTH: wFlags |= DF_SMOOTH; break;
	}
	m_object.Draw(wFlags);

	// Draw axis if the shift key is down
	if (GetAsyncKeyState(VK_SHIFT) < 0)
		glDrawAxis();

	glFlush();
    SwapBuffers(wglGetCurrentDC());

    bBusy = false;
}


void CNormalsView::OnDestroy() 
{
	CView::OnDestroy();
	
	if (m_nTimerID)
		KillTimer(m_nTimerID);

	HGLRC hglrc = wglGetCurrentContext();
	if (hglrc)
	{
		wglMakeCurrent(NULL,  NULL);
		VERIFY(wglDeleteContext(hglrc));
	}

	if (m_pGLDC)
		delete m_pGLDC;	
}


void CNormalsView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	float f = 10;
	switch (nChar)
	{
		case '+': m_radius -= f; break;
		case '-': m_radius += f; break;
	}
	SetEyePos();
	
	CView::OnChar(nChar, nRepCnt, nFlags);
}


void CNormalsView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	float f = (float)DEG2RAD(5);
	switch (nChar)
	{
		case VK_LEFT: m_oyAngle -= f; break;
		case VK_RIGHT: m_oyAngle += f; break;
	}
	SetEyePos();
	
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CNormalsView::SetEyePos()
{
	GL_VERIFY(glMatrixMode(GL_MODELVIEW));
    glLoadIdentity();

	// Set camera position
	GLpoint eyePos = m_center;
	eyePos.y = (float)(m_radius * cos(m_oyAngle));
	eyePos.x = (float)(m_radius * -sin(m_oyAngle));
	eyePos.z = m_radius;
	gluLookAt(eyePos.x, eyePos.y, eyePos.z, m_center.x, m_center.y, m_center.z, 0, 0, 1);

	// Set light position
	GLfloat f[4];
	f[0] = eyePos.x;
	f[1] = eyePos.y;
	f[2] = eyePos.z;
	f[3] = 1;
	glLightfv(GL_LIGHT0, GL_POSITION, f);
}


void CNormalsView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
		
	if (cx && m_pGLDC)
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		glViewport(0, 0, cx, cy);

		// Compute the field of view angle for the perspective matrix
		#define EYE_TO_SCREEN	(13.5)	// Distance from eye to screen in inches
		double yinch = (double)cy / m_pGLDC->GetDeviceCaps(LOGPIXELSY);
		m_fovAngle = asin(yinch / EYE_TO_SCREEN);
		m_aspectRatio = (GLdouble)cx / cy;
		gluPerspective(RAD2DEG(m_fovAngle), m_aspectRatio, 1, 10000);

		// Set the default model view matrix
		SetEyePos();
	}
}


void CNormalsView::OnDispFlat() 
{
	m_shading = SHADING_FLAT;
	GL_VERIFY(glShadeModel(GL_FLAT));
}


void CNormalsView::OnDispSmooth() 
{
	m_shading = SHADING_SMOOTH;
	GL_VERIFY(glShadeModel(GL_SMOOTH));
}


void CNormalsView::OnDispNone() 
{
	m_shading = SHADING_NONE;
	GL_VERIFY(glShadeModel(GL_FLAT));
}


void CNormalsView::OnUpdateDispFlat(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_shading == SHADING_FLAT);
}


void CNormalsView::OnUpdateDispSmooth(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_shading == SHADING_SMOOTH);
}


void CNormalsView::OnUpdateDispNone(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_shading == SHADING_NONE);
}
