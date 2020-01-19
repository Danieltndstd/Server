#ifndef MATCH_FRAME_CONTROL_HEAD_FILE
#define MATCH_FRAME_CONTROL_HEAD_FILE

#pragma once

#include "GameFrameHead.h"
#include "GameFrameUserView.h"
#include "GameFrameMedalView.h"
#include "UserListControl2.h"

//////////////////////////////////////////////////////////////////////////////////

//�û�ѡ��
#define MAX_HISTOY_USER				8									//�����Ŀ

//�û�����
typedef CWHArray<IClientUserItem *> CClientUserItemArray;

//////////////////////////////////////////////////////////////////////////////////

//���ƿ��
class GAME_FRAME_CLASS CMatchFrameControl : public CDialog, public IMatchFrameControl, public IUserEventSink
{
	//�������
protected:
	bool							m_bCreateFlag;						//������־

	//��������
protected:
	CRect							m_rcUserView;
	CRect							m_rcUserList;
	CRect							m_rcChat;

	//�������
protected:
	CClientUserItemArray			m_MenuUserItemArray;				//�û�����

	//�ؼ�����
protected:
	CUserListControl2				m_UserListControl;					//�û��б�
	CGameFrameUserView				m_GameFrameUserView;				//�û���Ϣ


	//�ӿڱ���
protected:
	IClientKernel *					m_pIClientKernel;					//�ں˽ӿ�
	IClientUserItem *				m_pIMySelfUserItem;					//�Լ�ָ��

	CPngImage						m_ImageBackground;
	CPngImage						m_ImageBase;
	CPngImage						m_ImageRank;
	CPngImage						m_ImageTime[4];
	CPngImage						m_ImageTurnDraw;
	CPngImage						m_ImageNumberOrange;
	CPngImage						m_ImageNumberGreen;
	CPngImage						m_ImageNumTime;

	tagMatchViewInfo				m_MatchViewInfo;
	CRichEditMessage				m_ChatMessage;						//������Ϣ

	WORD							m_HourTime;
	WORD							m_MinuteTime;
	WORD							m_SecondTime;
	BOOL							m_bMatchOver;

	//��̬����
protected:
	static	CMatchFrameControl*		m_pMatchFrameControl;

	//��������
public:
	//���캯��
	CMatchFrameControl();
	//��������
	virtual ~CMatchFrameControl();

	static CMatchFrameControl* GetInstance() {return m_pMatchFrameControl;}

	void SetMatchOverStatus(BOOL bMatchOver) ;

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release() { delete this; }
	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//���غ���
protected:
	//�ؼ���
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//���ú���
	virtual BOOL OnInitDialog();
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//�����
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

public:
	virtual VOID SetMatchViewInfo(tagMatchViewInfo* pMatchViewInfo);

	//�û��¼�
public:
	//�û�����
	virtual VOID OnEventUserEnter(IClientUserItem * pIClientUserItem, bool bLookonUser);
	//�û��뿪
	virtual VOID OnEventUserLeave(IClientUserItem * pIClientUserItem, bool bLookonUser);
	//�û�����
	virtual VOID OnEventUserScore(IClientUserItem * pIClientUserItem, bool bLookonUser);
	//�û�״̬
	virtual VOID OnEventUserStatus(IClientUserItem * pIClientUserItem, bool bLookonUser);
	//�û�����
	virtual VOID OnEventUserAttrib(IClientUserItem * pIClientUserItem, bool bLookonUser);
	//�û�ͷ��
	virtual VOID OnEventCustomFace(IClientUserItem * pIClientUserItem, bool bLookonUser);
	//
	virtual VOID OnUserTaskUpdate(IClientUserItem * pIClientUserItem, bool bLookonUser);

	//���溯��
private:
	//�����ؼ�
	VOID RectifyControl(INT nWidth, INT nHeight);
	//��������
	VOID RectifyControl(CSkinRenderManager * pSkinRenderManager);
	//�滭����
	VOID DrawNumberString(CDC * pDC, CPngImage &PngNumber, LONG lNumber, INT nXPos, INT nYPos,bool bZero=false,BYTE cbAlign=TA_LEFT);

	//��Ϣ����
protected:
	//����ǿͻ�������
	LRESULT OnNcHitTest(CPoint point);
	//�滭����
	BOOL OnEraseBkgnd(CDC * pDC);
	VOID OnPaint();
	//λ����Ϣ
	VOID OnSize(UINT nType, INT cx, INT cy);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

//////////////////////////////////////////////////////////////////////////////////

#endif