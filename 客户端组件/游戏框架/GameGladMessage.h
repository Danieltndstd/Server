#ifndef _GameGladMessage_H_
#define _GameGladMessage_H_

#include <list>
#include "GameFrameHead.h"
#include "../../../��Ϸ���/������/oops/easing/Easing.h"

class CMsgBlock;
//������Ϣ
class CGameGladMessage : public CDialog
{
private:
	static CGameGladMessage* s_instance;
public:
	static CGameGladMessage* GetInstance();
	//��������
public:
	//���캯��
	CGameGladMessage();
	//��������
	virtual ~CGameGladMessage();

	// ��Ӵ�������Ϣ
	bool InsertUserTyphon(LPCTSTR pszSendUser,LPCTSTR pszString,COLORREF crColor);
	// ϲ����Ϣ
	bool InsertGladMessage(LPCTSTR pszContent, LPCTSTR pszNickName, LPCTSTR pszNumber, COLORREF colText, COLORREF colName,COLORREF colNum);

	//���غ���
protected:
	//�ؼ���
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//���ú���
	virtual BOOL OnInitDialog();
	//�����ؼ�
	VOID RectifyControl(INT nWidth, INT nHeight);
	//
	VOID OnTimer(UINT_PTR nIDEvent);
	//�滭����
	BOOL OnEraseBkgnd(CDC * pDC);
	//����
	VOID OnPaint();
	//λ����Ϣ
	VOID OnSize(UINT nType, INT cx, INT cy);
	//������ɫ
	HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);
	DECLARE_MESSAGE_MAP()

private:
	// �����Ϣ
	void AddMsg(CMsgBlock* msg);
	// ��Դ��ʼ��
	void Init();
	// ��Դ�ͷ�
	void Deinit();	
	// ��Ⱦ����
	void RenderBackground();
	// ��Ⱦ
	void Render();
private:
	// ����
	CImage m_imgBgBuffer;
	CImage m_imgCanvas;

	CPngImage	ImageTL;							//λͼ����
	CPngImage	ImageTM;							//λͼ����
	CPngImage	ImageTR;							//λͼ����

	CRgn m_rgnContent;								//���ݲü�����
	// ����
	std::list<CMsgBlock *> m_ltMsg;

	bool m_IsTimering;								//ʱ��ˢ��
	// ��Ϣ������
	easing::CTween<int> m_twBar;
	// ����״̬
	int m_iAniState; 
	// ��ʼ��ʾ��Ϣʱ��
	DWORD m_dwShowStart;	
};

//////////////////////////////////////////////////////////////////////////////////

#endif