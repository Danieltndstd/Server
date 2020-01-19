#include "StdAfx.h"
#include "GameLevelParser.h"

//////////////////////////////////////////////////////////////////////////////////

//��̬����
CLevelDescribeArray		CGameLevelParser::m_LevelDescribeStorage;		//�洢����

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CGameLevelParser::CGameLevelParser()
{
	//���ñ���
	ZeroMemory(m_szKindName,sizeof(m_szKindName));

	return;
}

//��������
CGameLevelParser::~CGameLevelParser()
{
}

//�ӿڲ�ѯ
VOID * CGameLevelParser::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IGameLevelParser,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IGameLevelParser,Guid,dwQueryVer);
	return NULL;
}

//���صȼ�
bool CGameLevelParser::LoadGameLevelItem(LPCTSTR pszKindName, LPCTSTR pszDirectory, WORD wGameGenre)
{
	//Ч�����
	ASSERT(m_LevelDescribeActive.GetCount()==0);
	if (m_LevelDescribeActive.GetCount()!=0) return 0;

	//���ñ���
	lstrcpyn(m_szKindName,pszKindName,CountArray(m_szKindName));

	//����Ŀ¼
	TCHAR szWorkDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szWorkDirectory,CountArray(szWorkDirectory));

	//��������
	TCHAR szGameLevelName[MAX_PATH]=TEXT("");

	//Ԫ���ȼ�
	if (wGameGenre==GAME_GENRE_GOLD)
	{
		//�ļ�Ŀ¼
		_sntprintf(szGameLevelName,CountArray(szGameLevelName),TEXT("%s\\%s\\GoldGameLevel.ini"),szWorkDirectory,pszDirectory);

		//���صȼ�
		LoadGameLevelItem(szGameLevelName);

		//���ñ���
		if(m_LevelDescribeActive.GetCount()==0L)
		{
			//�ļ�Ŀ¼
			_sntprintf(szGameLevelName,CountArray(szGameLevelName),TEXT("%s\\GoldGameLevel.ini"),szWorkDirectory);

			//���صȼ�
			LoadGameLevelItem(szGameLevelName);
		}

		//���ñ���
		if(m_LevelDescribeActive.GetCount()==0L)
		{
			//���صȼ�
			LoadDefaultGameLevelItem();
		}
	}

	//���ֵȼ�
	if (wGameGenre&(GAME_GENRE_SCORE|(GAME_GENRE_EDUCATE|GAME_GENRE_MATCH)))
	{
		//�ļ�Ŀ¼
		_sntprintf(szGameLevelName,CountArray(szGameLevelName),TEXT("%s\\%s\\GameLevel.ini"),szWorkDirectory,pszDirectory);

		//���صȼ�
		LoadGameLevelItem(szGameLevelName);
	}

	//���ñ���
	if (m_LevelDescribeActive.GetCount()==0L)
	{
		ASSERT(FALSE);
		return false;
	}

	return true;
}

//���صȼ�
bool CGameLevelParser::LoadGameLevelItem(LPCTSTR pszGameLevelFileName)
{
	ASSERT(pszGameLevelFileName != NULL);

	//��������
	WORD wItemIndex=0;
	TCHAR szItemName[16]=TEXT("");
	TCHAR szReadData[256]=TEXT("");

	//��ȡ����
	do
	{
		//��ȡ�ַ�
		_sntprintf(szItemName,CountArray(szItemName),TEXT("LevelItem%d"),wItemIndex+1);
		GetPrivateProfileString(TEXT("LevelDescribe"),szItemName,TEXT(""),szReadData,sizeof(szReadData),pszGameLevelFileName);

		//��ȡ�ַ�
		if (szReadData[0]!=0)
		{
			//��ȡ����
			tagLevelDescribe * pLevelDescribe=CreateLevelDescribe();

			//������
			if (pLevelDescribe==NULL)
			{
				ASSERT(FALSE);
				return false;
			}

			//���ñ���
			pLevelDescribe->lLevelScore=0L;
			pLevelDescribe->szLevelName[0]=0;

			//��ȡ�ȼ�
			WORD wStringIndex=0;
			WORD i;
			for (i=0;i<CountArray(pLevelDescribe->szLevelName)-1;i++)
			{
				//���Ǵ���
				if ((szReadData[i]==TEXT(','))||(szReadData[i]==0)) break;
				if ((wStringIndex==0)&&(szReadData[i]==TEXT(' '))) continue;

				//�����ַ�
				pLevelDescribe->szLevelName[wStringIndex++]=szReadData[i];
			}
			pLevelDescribe->szLevelName[wStringIndex]=0;

			//Ѱ�ҿ�ʼ
			LPCTSTR pszScore=&szReadData[i];
			while (((pszScore[0]>0)&&(pszScore[0]<TEXT('0')))||(pszScore[0]>TEXT('9'))) pszScore++;

			//��ȡ����
			while ((pszScore[0]>=TEXT('0'))&&(pszScore[0]<=TEXT('9')))
			{
				pLevelDescribe->lLevelScore=pLevelDescribe->lLevelScore*10L+pszScore[0]-TEXT('0');
				++pszScore;
			}

			//���ñ���
			wItemIndex++;
			m_LevelDescribeActive.Add(pLevelDescribe);
		}
		else break;

	} while (true);

	return true;
}

//��ȡ�ȼ�
WORD CGameLevelParser::GetGameLevelItem(tagLevelItem LevelItem[], WORD wMaxCount)
{
	//Ч�����
	ASSERT(wMaxCount>=m_LevelDescribeActive.GetCount());
	if (wMaxCount<m_LevelDescribeActive.GetCount()) return 0;

	//��������
	for (INT_PTR i=0;i<m_LevelDescribeActive.GetCount();i++)
	{
		//��ȡ����
		tagLevelDescribe * pLevelDescribe=m_LevelDescribeActive[i];

		//��������
		LevelItem[i].lLevelScore=pLevelDescribe->lLevelScore;
		lstrcpyn(LevelItem[i].szLevelName,pLevelDescribe->szLevelName,CountArray(LevelItem[i].szLevelName));
	}

	return (WORD)m_LevelDescribeActive.GetCount();
}

//���õȼ�
bool CGameLevelParser::SetGameLevelItem(tagLevelItem LevelItem[], WORD wItemCount)
{
	//Ч�����
	ASSERT(m_LevelDescribeActive.GetCount()==0);
	if (m_LevelDescribeActive.GetCount()!=0) return 0;

	//���õȼ�
	for (WORD i=0;i<wItemCount;i++)
	{
		//��ȡ����
		tagLevelDescribe * pLevelDescribe=CreateLevelDescribe();

		//������
		if (pLevelDescribe==NULL)
		{
			ASSERT(FALSE);
			return false;
		}

		//���ñ���
		pLevelDescribe->lLevelScore=LevelItem[i].lLevelScore;
		lstrcpyn(pLevelDescribe->szLevelName,LevelItem[i].szLevelName,CountArray(pLevelDescribe->szLevelName));

		//���ñ���
		m_LevelDescribeActive.Add(pLevelDescribe);
	}

	//���ñ���
	if (m_LevelDescribeActive.GetCount()==0)
	{
		ASSERT(FALSE);
		return false;
	}

	return true;
}

//�ȼ�����
LPCTSTR CGameLevelParser::GetLevelDescribe(SCORE lScore)
{
	//״̬Ч��
	ASSERT(m_LevelDescribeActive.GetCount()>0L);
	if (m_LevelDescribeActive.GetCount()==0) return GetDefaultLevel(lScore);

	//��������
	WORD wItemCount=(WORD)m_LevelDescribeActive.GetCount();

	//�ȼ�����
	for (WORD i=0;i<wItemCount;i++)
	{
		tagLevelDescribe * pLevelDescribe=m_LevelDescribeActive[i];
		if ((lScore>=pLevelDescribe->lLevelScore)||(i==(wItemCount-1))) return pLevelDescribe->szLevelName;
	}

	return NULL;
}

//��Ϸ����
VOID CGameLevelParser::SetKindName(LPCTSTR pszKindName)
{
	//��������
	lstrcpyn(m_szKindName,pszKindName,CountArray(m_szKindName));

	return;
}

//�ȼ�����
LPCTSTR CGameLevelParser::GetLevelDescribe(IClientUserItem * pIClientUserItem)
{
	//״̬Ч��
	ASSERT(m_LevelDescribeActive.GetCount()>0L);
	if (m_LevelDescribeActive.GetCount()==0) return GetDefaultLevel(pIClientUserItem->GetUserScore());

	//��������
	SCORE lUserScore=pIClientUserItem->GetUserScore();
	INT_PTR nItemCount=m_LevelDescribeActive.GetCount();

	//�ȼ�����
	for (INT_PTR i=0;i<nItemCount;i++)
	{
		tagLevelDescribe * pLevelDescribe=m_LevelDescribeActive[nItemCount-i-1];
		if ((lUserScore>=pLevelDescribe->lLevelScore)||(i==(nItemCount-1))) return pLevelDescribe->szLevelName;
	}

	//�������
	ASSERT(FALSE);

	return NULL;
}

//���صȼ�
bool CGameLevelParser::LoadDefaultGameLevelItem()
{
	//�ȼ�����
	LONG lLevelScore[]=
	{	
		0L,2000L,4000L,8000L,20000L,40000L,80000L,150000L,300000L,500000L,1000000L,
			2000000L,5000000L,10000000L,50000000L,100000000L,500000000L,1000000000L,
	};	

	//�ȼ�����
	LPCTSTR pszLevelName[]=
	{		
		TEXT("��ũ"),TEXT("�軧"),TEXT("�͹�"),TEXT("������"),TEXT("ũ����"),TEXT("����"),
			TEXT("�����"),TEXT("����"),TEXT("����"),TEXT("����"),TEXT("С����"),TEXT("�����"),			
			TEXT("�Ĺ�"),TEXT("����"),TEXT("����"),TEXT("��ʥ"),TEXT("����"),TEXT("ְҵ����")	
	};

	//Ч������	
	ASSERT(CountArray(pszLevelName)==(CountArray(lLevelScore)));

	//���صȼ�
	for (WORD i=0;i<CountArray(lLevelScore);i++)
	{
		//��ȡ����
		tagLevelDescribe * pLevelDescribe=CreateLevelDescribe();

		//������
		if (pLevelDescribe==NULL)
		{
			ASSERT(FALSE);
			return false;
		}

		//���ñ���
		pLevelDescribe->lLevelScore=lLevelScore[i];
		lstrcpyn(pLevelDescribe->szLevelName,pszLevelName[i],CountArray(pLevelDescribe->szLevelName));

		//����ȼ�
		m_LevelDescribeActive.Add(pLevelDescribe);
	}

	return true;
}

//Ĭ�ϵȼ�
LPCTSTR CGameLevelParser::GetDefaultLevel(SCORE lScore)
{
	//�ȼ�����
	static SCORE lLevelScore[]=
	{	0L,			2000L,		4000L,		8000L,		20000L,
		40000L,		80000L,		150000L,	300000L,	500000L,
		1000000L,	2000000L,	5000000L,	10000000L,	50000000L,
		100000000L,	500000000L,	1000000000L,
	};	
	
	//�ȼ�����
	static LPCTSTR pszLevelName[]=	
	{		
		TEXT("��ũ"),TEXT("�軧"),TEXT("�͹�"),TEXT("������"),			
			TEXT("ũ����"),TEXT("����"),TEXT("�����"),TEXT("����"),			
			TEXT("����"),TEXT("����"),TEXT("С����"),TEXT("�����"),			
			TEXT("�Ĺ�"),TEXT("����"),TEXT("����"),TEXT("��ʥ"),			
			TEXT("����"),TEXT("ְҵ����")	
	};

	//Ч������	
	ASSERT(CountArray(pszLevelName)==(CountArray(lLevelScore)));	

	//��ȡ�ȼ�	
	for (INT i=0;i<CountArray(lLevelScore);i++)	
	{		
		if (lScore>=lLevelScore[CountArray(lLevelScore)-i-1]) return pszLevelName[i];	
	}	
	
	return pszLevelName[0];
}

//��ȡ����
tagLevelDescribe * CGameLevelParser::CreateLevelDescribe()
{
	//��ȡ����
	tagLevelDescribe * pLevelDescribe=NULL;
	INT_PTR nStorageCount=m_LevelDescribeStorage.GetCount();
	if (nStorageCount>0)
	{
		pLevelDescribe=m_LevelDescribeStorage[nStorageCount-1];
		ASSERT(pLevelDescribe!=NULL);
		m_LevelDescribeStorage.RemoveAt(nStorageCount-1);
	}
	else
	{
		pLevelDescribe=new tagLevelDescribe;
		ASSERT(pLevelDescribe!=NULL);
		if (pLevelDescribe==NULL) return NULL;
	}

	return pLevelDescribe;
}

WORD CGameLevelParser::GetUserLevelItem(IClientUserItem * pIClientUserItem)
{
	ASSERT(pIClientUserItem!=NULL);
	if (pIClientUserItem == NULL)
		return 0;

	DWORD dwUserExperience = pIClientUserItem->GetUserExperience();
	WORD wUserLevel = 0;

	if ( dwUserExperience > 0 && dwUserExperience <= 10 )
		wUserLevel = 1;
	else if ( dwUserExperience > 10 && dwUserExperience <= 30 )
		wUserLevel = 2;
	else if ( dwUserExperience > 30 && dwUserExperience <= 70 )
		wUserLevel = 3;
	else if ( dwUserExperience > 70 && dwUserExperience <= 170 )
		wUserLevel = 4;
	else if ( dwUserExperience > 170 && dwUserExperience <= 370 )
		wUserLevel = 5;
	else if ( dwUserExperience > 370 && dwUserExperience <= 770 )
		wUserLevel = 6;
	else if ( dwUserExperience > 770 && dwUserExperience <= 1770 )
		wUserLevel = 7;
	else if ( dwUserExperience > 1770 && dwUserExperience <= 3770 )
		wUserLevel = 8;
	else if ( dwUserExperience > 3770 && dwUserExperience <= 7770 )
		wUserLevel = 9;
	else if ( dwUserExperience > 7770 && dwUserExperience <= 17770 )
		wUserLevel = 10;
	else if ( dwUserExperience > 17770 && dwUserExperience <= 37770 )
		wUserLevel = 11;
	else if ( dwUserExperience > 37770 && dwUserExperience <= 77770 )
		wUserLevel = 12;
	else if ( dwUserExperience > 77770 && dwUserExperience <= 137770 )
		wUserLevel = 13;
	else if ( dwUserExperience > 137770 && dwUserExperience <= 217770 )
		wUserLevel = 14;
	else if ( dwUserExperience > 217770 && dwUserExperience <= 317770 )
		wUserLevel = 15;
	else if ( dwUserExperience > 317770 && dwUserExperience <= 437770 )
		wUserLevel = 16;
	else if ( dwUserExperience > 437770 && dwUserExperience <= 577770 )
		wUserLevel = 17;
	else if ( dwUserExperience > 577770 && dwUserExperience <= 737770 )
		wUserLevel = 18;
	else if ( dwUserExperience > 737770 && dwUserExperience <= 917770 )
		wUserLevel = 19;
	else if ( dwUserExperience > 917770 && dwUserExperience <= 1117770 )
		wUserLevel = 20;
	else if ( dwUserExperience > 1117770 && dwUserExperience <= 1517770 )
		wUserLevel = 21;
	else if ( dwUserExperience > 1517770 && dwUserExperience <= 2117770 )
		wUserLevel = 22;
	else if ( dwUserExperience > 2117770 && dwUserExperience <= 2917770 )
		wUserLevel = 23;
	else if ( dwUserExperience > 2917770 && dwUserExperience <= 3917770 )
		wUserLevel = 24;
	else if ( dwUserExperience > 3917770 && dwUserExperience <= 5117770 )
		wUserLevel = 25;
	else if ( dwUserExperience > 5117770 && dwUserExperience <= 6517770 )
		wUserLevel = 26;
	else if ( dwUserExperience > 6517770 && dwUserExperience <= 8117770 )
		wUserLevel = 27;
	else if ( dwUserExperience > 8117770 && dwUserExperience <= 9917770 )
		wUserLevel = 28;
	else if ( dwUserExperience > 9917770 && dwUserExperience <= 11917770 )
		wUserLevel = 29;
	else if ( dwUserExperience > 11917770 && dwUserExperience <= 15917770 )
		wUserLevel = 30;
	else if ( dwUserExperience > 15917770 && dwUserExperience <= 21917770 )
		wUserLevel = 31;
	else if ( dwUserExperience > 21917770 && dwUserExperience <= 29917770 )
		wUserLevel = 32;
	else if ( dwUserExperience > 29917770 && dwUserExperience <= 39917770 )
		wUserLevel = 33;
	else if ( dwUserExperience > 39917770 && dwUserExperience <= 51917770 )
		wUserLevel = 34;
	else if ( dwUserExperience > 51917770 && dwUserExperience <= 65917770 )
		wUserLevel = 35;
	else if ( dwUserExperience > 65917770 && dwUserExperience <= 81917770 )
		wUserLevel = 36;
	else if ( dwUserExperience > 81917770 && dwUserExperience <= 99917770 )
		wUserLevel = 37;
	else if ( dwUserExperience > 99917770 && dwUserExperience <= 119917770 )
		wUserLevel = 38;
	else if ( dwUserExperience > 119917770 && dwUserExperience <= 159917770 )
		wUserLevel = 39;
	else if ( dwUserExperience > 159917770 && dwUserExperience <= 219917770 )
		wUserLevel = 40;
	else if ( dwUserExperience > 219917770 && dwUserExperience <= 299917770 )
		wUserLevel = 41;
	else if ( dwUserExperience > 299917770 && dwUserExperience <= 399917770 )
		wUserLevel = 42;
	else if ( dwUserExperience > 399917770 && dwUserExperience <= 519917770 )
		wUserLevel = 43;
	else if ( dwUserExperience > 519917770 && dwUserExperience <= 659917770 )
		wUserLevel = 44;
	else if ( dwUserExperience > 659917770 && dwUserExperience <= 819917770 )
		wUserLevel = 45;
	else if ( dwUserExperience > 819917770 && dwUserExperience <= 999917770 )
		wUserLevel = 46;
	else if (dwUserExperience > 999917770 && dwUserExperience <= 1199917770)
		wUserLevel = 47;
	else if (dwUserExperience > 1199917770)
		wUserLevel = 48;
	else
		wUserLevel = 1;
	return wUserLevel;
}

//////////////////////////////////////////////////////////////////////////////////

//�����������
DECLARE_CREATE_MODULE(GameLevelParser);

//////////////////////////////////////////////////////////////////////////////////
