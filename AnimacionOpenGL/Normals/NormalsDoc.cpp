// NormalsDoc.cpp : implementation of the CNormalsDoc class
//

#include "stdafx.h"
#include "Normals.h"

#include "NormalsDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNormalsDoc

IMPLEMENT_DYNCREATE(CNormalsDoc, CDocument)

BEGIN_MESSAGE_MAP(CNormalsDoc, CDocument)
	//{{AFX_MSG_MAP(CNormalsDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNormalsDoc construction/destruction

CNormalsDoc::CNormalsDoc()
{
	// TODO: add one-time construction code here

}

CNormalsDoc::~CNormalsDoc()
{
}

BOOL CNormalsDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CNormalsDoc serialization

void CNormalsDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CNormalsDoc diagnostics

#ifdef _DEBUG
void CNormalsDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CNormalsDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CNormalsDoc commands
