
// jlMap2BmpView.cpp : CjlMap2BmpView ���ʵ��
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
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CjlMap2BmpView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CjlMap2BmpView ����/����

CjlMap2BmpView::CjlMap2BmpView()
{
	// TODO: �ڴ˴���ӹ������

}

CjlMap2BmpView::~CjlMap2BmpView()
{
}

BOOL CjlMap2BmpView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CjlMap2BmpView ����

void CjlMap2BmpView::OnDraw(CDC* /*pDC*/)
{
	CjlMap2BmpDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CjlMap2BmpView ��ӡ


void CjlMap2BmpView::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}

BOOL CjlMap2BmpView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CjlMap2BmpView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CjlMap2BmpView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
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


// CjlMap2BmpView ���

#ifdef _DEBUG
void CjlMap2BmpView::AssertValid() const
{
	CView::AssertValid();
}

void CjlMap2BmpView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CjlMap2BmpDoc* CjlMap2BmpView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CjlMap2BmpDoc)));
	return (CjlMap2BmpDoc*)m_pDocument;
}
#endif //_DEBUG


// CjlMap2BmpView ��Ϣ�������
