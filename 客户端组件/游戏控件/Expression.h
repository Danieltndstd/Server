#ifndef EXPRESSION_HEAD_FILE
#define EXPRESSION_HEAD_FILE

#pragma once

#include "ShareControlHead.h"

//////////////////////////////////////////////////////////////////////////////////
//���Ͷ���

//��˵��
class CExpression;
class CExpression2;
class CExpressionManager;
class CExpressionManager2;
class CExpressionControl;

//������Ϣ
struct tagExpressionInfo
{
	INT								nLength[3];							//��������
	TCHAR							szExpression[3][8];					//��������
};

//���ͽ��
struct tagTranslateResult
{
	//���ݱ���
	INT								nStartPos;							//��ʼλ��
	INT								nDescribeLen;						//��������

	//�������
	CExpression *					pExpressionItem;					//������Ϣ
};

struct tagTranslateResult2
{
	//���ݱ���
	INT								nStartPos;							//��ʼλ��
	INT								nDescribeLen;						//��������

	//�������
	CExpression2 *					pExpressionItem;					//������Ϣ
};
//���鶨��
typedef CWHArray<CExpression *>		CExpressionArray;					//��������
typedef CWHArray<CExpression2 *>		CExpressionArray2;					//��������

//////////////////////////////////////////////////////////////////////////////////

//����ӿ�
interface IExpressionSink
{
	//ѡ�����
	virtual VOID OnExpressionSelect(CExpression * pExpression, tagExpressionInfo * pExpressionInfo)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

//�������
class SHARE_CONTROL_CLASS CExpression
{
	//��Ԫ����
	friend class CExpressionControl;
	friend class CExpressionManager;

	//��������
protected:
	WORD							m_wIndex;							//������־
	CGIFImage						m_ExpressionItem;					//������Դ
	tagExpressionInfo				m_ExpressionInfo;					//������Ϣ

	//��������
protected:
	//���캯��
	CExpression();
	//��������
	virtual ~CExpression();

	//���ܺ���
public:
	//��ȡ����
	WORD GetIndex() { return m_wIndex; }
	//��ȡ·��
	bool GetExpressionPath(LPTSTR pszImagePath, WORD wMaxCount);
	//���ñ���
	bool SetExpressionInfo(WORD wIndex, tagExpressionInfo&ExpressionInfo);
};

//////////////////////////////////////////////////////////////////////////////////

//�������
class SHARE_CONTROL_CLASS CExpressionManager
{
	//�������
protected:
	CExpressionArray				m_ExpressionArray;					//��������

	//��̬����
protected:
	static CExpressionManager *		m_pExpressionManager;				//����ָ��

	//��������
public:
	//���캯��
	CExpressionManager();
	//��������
	virtual ~CExpressionManager();

	//���ܺ���
public:
	//���ر���
	bool LoadExpression();
	//���ͱ���
	bool TranslateString(LPCTSTR pszString, tagTranslateResult&TranslateResult);

	//������Ϣ
public:
	//������Ŀ
	WORD GetExpressionCount();
	//ö�ٱ���
	CExpression * GetExpressionItem(WORD wIndex);

	//��̬����
public:
	//��ȡ����
	static CExpressionManager * GetInstance() { return m_pExpressionManager; }
};

//////////////////////////////////////////////////////////////////////////////////

//���鴰��
class SHARE_CONTROL_CLASS CExpressionControl : public CDialog
{
	//��������
protected:
	WORD							m_wItemCount;							//������Ŀ
	WORD							m_wSelectIndex;							//ѡ������

	//�������
protected:
	IExpressionSink *				m_pIExpressionSink;						//����ӿ�

	//��������
public:
	//���캯��
	CExpressionControl();
	//��������
	virtual ~CExpressionControl();

	//���ܺ���
public:
	//��ʾ�ؼ�
	VOID ShowExpression(CWnd * pParentWnd, INT nXPos, INT nYPos, IExpressionSink * pIExpressionSink);

	//��������
protected:
	//�滭ͷ��
	VOID DrawExpression(CDC * pDC, WORD wItemIndex, bool bSelected);

	//��Ϣ����
public:
	//�ػ���Ϣ
	VOID OnPaint();
	//�滭����
	BOOL OnEraseBkgnd(CDC * pDC);
	//������Ϣ
	VOID OnKillFocus(CWnd * pNewWnd);
	//�����Ϣ
	VOID OnLButtonDown(UINT nFlags, CPoint Point);
	//�����Ϣ
	BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);

	DECLARE_MESSAGE_MAP()
};



//�������
class SHARE_CONTROL_CLASS CExpression2
{
	//��Ԫ����
	friend class CExpressionControl;
	friend class CExpressionManager2;

	//��������
protected:
	WORD							m_wIndex;							//������־
	CGIFImage						m_ExpressionItem;					//������Դ
	tagExpressionInfo				m_ExpressionInfo;					//������Ϣ
	CExpressionManager2				*m_pMgr;
	//��������
protected:
	//���캯��
	CExpression2(CExpressionManager2 *mgr);
	//��������
	virtual ~CExpression2();

	//���ܺ���
public:
	//��ȡ����
	WORD GetIndex() { return m_wIndex; }
	//��ȡͼƬ
	CGIFImage* GetImage() {return &m_ExpressionItem;}
	//��ȡ·��
	bool GetExpressionPath(LPTSTR pszImagePath, WORD wMaxCount);
	//���ñ���
	bool SetExpressionInfo(WORD wIndex, tagExpressionInfo&ExpressionInfo);
	// ���Ʊ���
	void DrawImageFrame(CDC* pDC, INT x, INT y, INT_PTR nFrameIndex = 0L);
	// ��ȡ��������
	INT GetImageFrames() ;

};

//////////////////////////////////////////////////////////////////////////////////

//�������
class SHARE_CONTROL_CLASS CExpressionManager2
{
	//�������
protected:
	CExpressionArray2				m_ExpressionArray;					//��������
	TCHAR							m_szDirectory[MAX_PATH];			//�����·��

	//��̬����
protected:
	static CExpressionManager2 *		m_pExpressionManager;				//����ָ��

	//��������
public:
	//���캯��
	CExpressionManager2();
	//��������
	virtual ~CExpressionManager2();

	//���ܺ���
public:
	//�����·��
	LPCTSTR GetDirectory() const;
	//���ر���
	bool LoadExpression(LPCTSTR szDirectory);
	//���ͱ���
	bool TranslateString(LPCTSTR szDirectory, tagTranslateResult2&TranslateResult);

	//������Ϣ
public:
	//������Ŀ
	WORD GetExpressionCount();
	//ö�ٱ���
	CExpression2 * GetExpressionItem(WORD wIndex);

	//��̬����
public:
	//��ȡ����
	static CExpressionManager2 * GetInstance() { return m_pExpressionManager; }
};
//////////////////////////////////////////////////////////////////////////////////

#endif