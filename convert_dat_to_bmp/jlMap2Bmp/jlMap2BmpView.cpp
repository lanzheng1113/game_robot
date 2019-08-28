
// jlMap2BmpView.cpp : CjlMap2BmpView 类的实现
//

#include "stdafx.h"
#include "jlMap2Bmp.h"

#include "jlMap2BmpDoc.h"
#include "jlMap2BmpView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CjlMap2BmpView

IMPLEMENT_DYNCREATE(CjlMap2BmpView, CView)

BEGIN_MESSAGE_MAP(CjlMap2BmpView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CjlMap2BmpView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CjlMap2BmpView 构造/析构

CjlMap2BmpView::CjlMap2BmpView()
{
	// TODO: 在此处添加构造代码

}

CjlMap2BmpView::~CjlMap2BmpView()
{
}

BOOL CjlMap2BmpView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CjlMap2BmpView 绘制

void CjlMap2BmpView::OnDraw(CDC* /*pDC*/)
{
	CjlMap2BmpDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CjlMap2BmpView 打印


void CjlMap2BmpView::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}

BOOL CjlMap2BmpView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CjlMap2BmpView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CjlMap2BmpView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CjlMap2BmpView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CjlMap2BmpView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}


// CjlMap2BmpView 诊断

#ifdef _DEBUG
void CjlMap2BmpView::AssertValid() const
{
	CView::AssertValid();
}

void CjlMap2BmpView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CjlMap2BmpDoc* CjlMap2BmpView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CjlMap2BmpDoc)));
	return (CjlMap2BmpDoc*)m_pDocument;
}
#endif //_DEBUG


// CjlMap2BmpView 消息处理程序
