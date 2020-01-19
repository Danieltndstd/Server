#ifndef GAME_FRAME_WND_HEAD_FILE
#define GAME_FRAME_WND_HEAD_FILE

#pragma once

#include "GameFrameHead.h"
#include "GameFrameControl.h"
#include "DlgGameRule.h"
#include "MatchFrameControl.h"
#include "GameGladMessage.h"

//////////////////////////////////////////////////////////////////////////////////

class CDlgVolumnControl;
class CDlgTopMsg;

//�����Դ
class CFrameEncircle : public CEncircleBMP
{
	//��������
public:
	//���캯��
	CFrameEncircle();
	//��������
	virtual ~CFrameEncircle();

	//���غ���
protected:
	//�滭����
	virtual bool PreDrawEncircleImage(tagEncircleBMP & EncircleImage);
};

//////////////////////////////////////////////////////////////////////////////////

//��Ϸ����
class GAME_FRAME_CLASS CGameFrameWnd : public CFrameWnd, public IGameFrameWnd,public IFlashControlSink,public IClientPropertySink
{
	//״̬����
protected:
	bool							m_bMaxShow;							//����־
	bool							m_bShowControl;						//��ʾ��־
	CRect							m_rcNormalSize;						//����λ��

	//��������
protected:
	INT								m_nScrollXPos;						//����λ��
	INT								m_nScrollYPos;						//����λ��
	INT								m_nScrollXMax;						//���λ��
	INT								m_nScrollYMax;						//���λ��

	//����ӿ�
protected:
	IClientKernel *					m_pIClientKernel;					//�ں˽ӿ�
	IGameFrameView *				m_pIGameFrameView;					//��ͼ�ӿ�

	//�ؼ�����
public:
	CSkinButton						m_btMin;							//��С��ť
	CSkinButton						m_btClose;						//�رհ�ť
	CSkinButton						m_btOption;						//���ð�ť
	CSkinButton						m_btRule;							//����ť
	CSkinButton						m_btChat;							//��/�ջ�����
	CSkinScrollBar					m_SkinScrollBar;				//��������

	//�ؼ�ָ��
protected:
	CDlgGameRule *					m_pDlgGameRule;						//��Ϸ����
	CDlgVolumnControl*				m_pDlgVolumn;

	//���ܿؼ�
public:
	CFlashControl					m_FlashControl;						//�����ؼ�
	CToolTipCtrl					m_ToolTipCtrl;						//��ʾ�ؼ�
	//CSkinSplitter					m_SkinSplitter;						//��ֿؼ�
	CGameFrameControl				m_GameFrameControl;					//���ƿ��
	CMatchFrameControl				m_MatchFrameControl;
	CPngImage					m_splitPng;							//�ָ���
	CGameGladMessage				m_GameGladMessage;					//��Ϸ����

	//��Դ����
protected:
	CFrameEncircle					m_FrameEncircle;					//�����Դ

	IPC_GR_PropertyNoticeMsg		m_noticeMsg;	//	������Ϣ
	int											m_strLen;
	int											m_strX;
	CPngImage						m_processTitle;

	WORD							m_LayerWndMapIndex;
	IPC_GF_LayerWndInfo				m_LayerWndInfo;

	//��������
public:
	//���캯��
	CGameFrameWnd();
	//��������
	virtual ~CGameFrameWnd();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release() { delete this; }
	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//���ڿ���
public:
	//��Ϸ����
	virtual bool ShowGameRule();
	//��󴰿�
	virtual bool MaxSizeWindow(){ return true; }
	//��ԭ����
	virtual bool RestoreWindow(){ return true; }

	//���ƽӿ�
public:
	//��������
	virtual bool AllowGameSound(bool bAllowSound) { return true; }
	//�Թۿ���
	virtual bool AllowGameLookon(DWORD dwUserID, bool bAllowLookon);
	//���ƽӿ�
	virtual bool OnGameOptionChange();

	//���߽ӿ�
public:
	//�������
	virtual VOID OnEventFlashFinish();
	//���߳ɹ�
	virtual VOID OnPropertySuccess(LPCTSTR szTargerNickName,DWORD dwSourceUserID,DWORD dwTargerUserID,WORD wPropertyIndex,WORD wPropertyCount);
	//����ʧ��
	virtual VOID OnPropertyFailure( LONG lErrorCode,LPCTSTR szDescribeString);

	//���غ���
protected:
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//�����
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	//���溯��
protected:
	//��ԭ����
//	bool RestoreWindow();
	//��󴰿�
//	bool MaxSizeWindow();

	//�������
protected:
	//�����ؼ�
	VOID RectifyControl(INT nWidth, INT nHeight);
	void UpdateGladControll();
	BOOL Snapshot(CRect rcRect,CString strFileName);
	int GetEncoderClsid(const WCHAR* format,CLSID* pClsid);
	//��Ϣ����
protected:
	//�滭����
	BOOL OnEraseBkgnd(CDC * pDC);
	//��������
	LRESULT OnNcHitTest(CPoint Point);
	//λ����Ϣ
	VOID OnSize(UINT nType, INT cx, INT cy);
	//������Ϣ
	INT OnCreate(LPCREATESTRUCT lpCreateStruct);
	//�����Ϣ
	VOID OnLButtonDown(UINT nFlags, CPoint Point);
	//�ı���Ϣ
	VOID OnSettingChange(UINT uFlags, LPCTSTR lpszSection);

	bool SetGameLayerHandle(IPC_GF_LayerWndInfo& LayerWndInfo);

	//�Զ���Ϣ
protected:
	//������Ϣ
	LRESULT	OnSetTextMesage(WPARAM wParam, LPARAM lParam);

	//ϵͳ��Ϣ
public:
	//��������
	afx_msg void OnDestroy();
	//��ʱ����Ϣ
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//�ػ���Ϣ
	afx_msg void OnPaint();
	afx_msg void OnMove(int x, int y);

	DECLARE_MESSAGE_MAP()
	
//	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnMoving(UINT fwSide, LPRECT pRect);
};

//////////////////////////////////////////////////////////////////////////////////

#endif