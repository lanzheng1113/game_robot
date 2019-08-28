
// jlMap2BmpDoc.cpp : CjlMap2BmpDoc 类的实现
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


// CjlMap2BmpDoc 构造/析构

CjlMap2BmpDoc::CjlMap2BmpDoc()
{
	// TODO: 在此添加一次性构造代码

}

CjlMap2BmpDoc::~CjlMap2BmpDoc()
{
}

BOOL CjlMap2BmpDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CjlMap2BmpDoc 序列化

void CjlMap2BmpDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}


// CjlMap2BmpDoc 诊断

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


// CjlMap2BmpDoc 命令
