
// jlMap2BmpDoc.cpp : CjlMap2BmpDoc ���ʵ��
//

#include "stdafx.h"
#include "jlMap2Bmp.h"

#include "jlMap2BmpDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CjlMap2BmpDoc

IMPLEMENT_DYNCREATE(CjlMap2BmpDoc, CDocument)

BEGIN_MESSAGE_MAP(CjlMap2BmpDoc, CDocument)
END_MESSAGE_MAP()


// CjlMap2BmpDoc ����/����

CjlMap2BmpDoc::CjlMap2BmpDoc()
{
	// TODO: �ڴ����һ���Թ������

}

CjlMap2BmpDoc::~CjlMap2BmpDoc()
{
}

BOOL CjlMap2BmpDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CjlMap2BmpDoc ���л�

void CjlMap2BmpDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}


// CjlMap2BmpDoc ���

#ifdef _DEBUG
void CjlMap2BmpDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CjlMap2BmpDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CjlMap2BmpDoc ����
