// GLSObject.h: interface for the GLSObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLSOBJECT_H__7FDFA204_9F04_11D2_84E6_00A0C9A250C5__INCLUDED_)
#define AFX_GLSOBJECT_H__7FDFA204_9F04_11D2_84E6_00A0C9A250C5__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000





// Draw flags
#define DF_FLAT			(1 << 0)
#define DF_SMOOTH		(1 << 1)

struct GLSObject  
{
						GLSObject ();
	virtual				~GLSObject ();

			void		Draw (WORD wFlags = 0);

	protected:
			void		ComputeVerticeNormal (int ixVertice);

		int				m_nbFaces;
		int				m_nbVertices;
		GLpoint	*		m_pFaceNormals;
		GLpoint	*		m_pVertNormals;
	private:
		int	*			m_pStorage;
};

#endif // !defined(AFX_GLSOBJECT_H__7FDFA204_9F04_11D2_84E6_00A0C9A250C5__INCLUDED_)
