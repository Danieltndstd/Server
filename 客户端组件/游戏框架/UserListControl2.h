#ifndef _UserListControl2_H_
#define _UserListControl2_H_

#include "GameFrameHead.h"

//�û��б�
class CUserListControl2 : public CSkinListCtrlEx
{
	//״̬����
protected:
	bool							m_bCreateColumn;					//��������
	DWORD							m_dwServerRule;						//�������
	DWORD							m_dwMySelfUserID;					//�Լ���Ϣ

	//�б���Ϣ
protected:
	WORD							m_wColumnCount;						//�б���Ŀ
	BYTE							m_cbDataDescribe[MAX_COLUMN];		//��������

	//�ӿڱ���
protected:
	IGameLevelParser *				m_pIGameLevelParser;				//�ȼ��ӿ�
	IGameUserManager *				m_pIGameUserManager;				//�û��ӿ�

	//��Դ����
protected:
	tagColorUserList				m_ColorUserList;					//��ɫ����
	static CImageList				m_ImageUserStatus;					//״̬λͼ

	//��������
public:
	//���캯��
	CUserListControl2();
	//��������
	virtual ~CUserListControl2();

	//���غ���
public:
	//�ؼ���
	virtual VOID PreSubclassWindow();

	//���ݿ���
protected:
	//��ȡλ��
	virtual INT GetInsertIndex(VOID * pItemData);
	//��������
	virtual INT SortItemData(LPARAM lParam1, LPARAM lParam2, WORD wColumnIndex, bool bAscendSort);
	//�����ַ�
	virtual VOID ConstructString(VOID * pItemData, WORD wColumnIndex, LPTSTR pszString, WORD wMaxCount);

	//�滭����
protected:
	//��ȡ��ɫ
	virtual VOID GetItemColor(LPDRAWITEMSTRUCT lpDrawItemStruct, COLORREF&crColorText, COLORREF&crColorBack);
	//�滭����
	virtual VOID DrawCustomItem(CDC * pDC, LPDRAWITEMSTRUCT lpDrawItemStruct, CRect&rcSubItem, INT nColumnIndex);

	//��չ����
protected:
	//�û�����
	virtual IClientUserItem * GetClientUserItem(VOID * pItemData);

	//���ú���
public:
	//���ýӿ�
	bool SetServerRule(DWORD dwServerRule);
	//���ýӿ�
	bool SetMySelfUserID(DWORD dwMySelfUserID);
	//���ýӿ�
	bool SetGameLevelParser(IUnknownEx * pIUnknownEx);
	//���ýӿ�
	bool SetGameUserManager(IUnknownEx * pIUnknownEx);

	//�б�����
public:
	//������ɫ
	bool SetColorUserList(tagColorUserList&ColorUserList);
	//�����б�
	bool SetColumnDescribe(tagColumnItem ColumnItem[], WORD wColumnCount);

	//��Ϣӳ��
protected:
	//������Ϣ
	VOID OnNcDestroy();

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif