// Normals.h : main header file for the NORMALS application
//

#if !defined(AFX_NORMALS_H__99936C37_B691_11D2_8502_00A0C9A250C5__INCLUDED_)
#define AFX_NORMALS_H__99936C37_B691_11D2_8502_00A0C9A250C5__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CNormalsApp:
// See Normals.cpp for the implementation of this class
//

class CNormalsApp : public CWinApp
{
public:
	CNormalsApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNormalsApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CNormalsApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NORMALS_H__99936C37_B691_11D2_8502_00A0C9A250C5__INCLUDED_)
