// objviewerView.h : interface of the CNormalsView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_OBJVIEWERVIEW_H__C7114F8D_8853_11D2_84D0_00A0C9A250C5__INCLUDED_)
#define AFX_OBJVIEWERVIEW_H__C7114F8D_8853_11D2_84D0_00A0C9A250C5__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#include "glincludes.h"
#include "GLObject.h"


class CNormalsView : public CView
{
protected: // create from serialization only
	CNormalsView();
	virtual ~CNormalsView();
	DECLARE_DYNCREATE(CNormalsView)

// Attributes
public:
	CNormalsDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNormalsView)
	public:
	virtual void OnDraw(CDC* pDC);
	virtual void OnInitialUpdate();
	//}}AFX_VIRTUAL

// Implementation
public:

	void SetEyePos();
	void DrawScene();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CDC *		m_pGLDC;
	UINT		m_nTimerID;
	double		m_fovAngle;
	double		m_aspectRatio;
	GLpoint		m_center;
	float		m_radius;
	float		m_oyAngle;
	enum { SHADING_NONE, SHADING_FLAT, SHADING_SMOOTH}
				m_shading;
	GLSObject	m_object;

// Generated message map functions
protected:
	//{{AFX_MSG(CNormalsView)
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDispFlat();
	afx_msg void OnDispSmooth();
	afx_msg void OnUpdateDispFlat(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDispSmooth(CCmdUI* pCmdUI);
	afx_msg void OnDispNone();
	afx_msg void OnUpdateDispNone(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in objviewerView.cpp
inline CNormalsDoc* CNormalsView::GetDocument()
   { return (CNormalsDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OBJVIEWERVIEW_H__C7114F8D_8853_11D2_84D0_00A0C9A250C5__INCLUDED_)
