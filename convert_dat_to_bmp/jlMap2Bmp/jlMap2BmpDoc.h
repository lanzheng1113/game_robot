
// jlMap2BmpDoc.h : CjlMap2BmpDoc ��Ľӿ�
//


#pragma once


class CjlMap2BmpDoc : public CDocument
{
protected: // �������л�����
	CjlMap2BmpDoc();
	DECLARE_DYNCREATE(CjlMap2BmpDoc)

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ʵ��
public:
	virtual ~CjlMap2BmpDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};


