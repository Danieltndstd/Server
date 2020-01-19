#ifndef MODULE_MANAGER_HEAD_HEAD_FILE
#define MODULE_MANAGER_HEAD_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////

//ƽ̨����
#include "..\..\ȫ�ֶ���\Platform.h"

//�������
#include "..\..\�������\�������\ServiceCoreHead.h"
#include "..\..\���������\��Ϸ����\GameServiceHead.h"
#include "..\..\���������\�ں�����\KernelEngineHead.h"

//////////////////////////////////////////////////////////////////////////////////
//��������

//��������
#ifndef MODULE_MANAGER_CLASS
	#ifdef  MODULE_MANAGER_DLL
		#define MODULE_MANAGER_CLASS _declspec(dllexport)
	#else
		#define MODULE_MANAGER_CLASS _declspec(dllimport)
	#endif
#endif

//ģ�鶨��
#ifndef _DEBUG
	#define MODULE_MANAGER_DLL_NAME	TEXT("ModuleManager.dll")			//��� DLL ����
#else
	#define MODULE_MANAGER_DLL_NAME	TEXT("ModuleManagerD.dll")			//��� DLL ����
#endif

//////////////////////////////////////////////////////////////////////////////////
//�ṹ����

//ģ����Ϣ
struct tagGameModuleInfo
{
	//�汾��Ϣ
	DWORD							dwClientVersion;					//��Ϸ�汾
	DWORD							dwServerVersion;					//����汾
	DWORD							dwNativeVersion;					//���ذ汾

	//��������
	WORD							wGameID;							//ģ���ʶ
	TCHAR							szDataBaseAddr[16];					//���ݿ��ַ
	TCHAR							szDataBaseName[32];					//���ݿ���

	//ģ������
	TCHAR							szGameName[LEN_KIND];				//��Ϸ����
	TCHAR							szServerDLLName[LEN_PROCESS];		//��������
	TCHAR							szClientEXEName[LEN_PROCESS];		//��������
};

//������Ϣ
struct tagGameServerInfo
{
	//��������
	WORD							wGameID;							//ģ���ʶ
	WORD							wServerID;							//�������

	//�ҽ�����
	WORD							wKindID;							//�ҽ�����
	WORD							wNodeID;							//�ҽӽڵ�
	WORD							wSortID;							//���б�ʶ

	//�汾��Ϣ
	DWORD							dwClientVersion;					//��Ϸ�汾
	DWORD							dwServerVersion;					//����汾
	DWORD							dwNativeVersion;					//���ذ汾

	//ģ������
	TCHAR							szGameName[LEN_KIND];				//��Ϸ����
	TCHAR							szServerDLLName[LEN_PROCESS];		//��������
	TCHAR							szClientEXEName[LEN_PROCESS];		//��������

	//˰������
	LONG							lCellScore;							//��λ����
	WORD							wRevenueRatio;						//˰�ձ���
	SCORE							lServiceScore;						//�������

	//��������
	SCORE							lRestrictScore;						//���ƻ���
	SCORE							lMinTableScore;						//��ͻ���
	SCORE							lMinEnterScore;						//��ͻ���
	SCORE							lMaxEnterScore;						//��߻���
	SCORE							lMinEnterTiLi;						//�������

	//��Ա����
	BYTE							cbMinEnterMember;					//��ͻ�Ա
	BYTE							cbMaxEnterMember;					//��߻�Ա

	//��������
	DWORD							dwServerRule;						//�������
	DWORD							dwAttachUserRight;					//����Ȩ��

	//��������
	WORD							wMaxPlayer;							//�����Ŀ
	WORD							wAndroidCount;						//Ԥ���ػ�������Ŀ
	WORD							wTableCount;						//������Ŀ
	WORD							wServerPort;						//����˿�
	WORD							wServerType;						//��������
	//---------------
	DWORD							dwRoomID;							//ROOMID
	WORD							wRoomKind;							//�������� 1���� 2ͬ��
	WORD							wRoomType;							//�������� 0��� 1���� 2�м� 3�߼�
	DWORD							dwBloodScore;						//����Ѫ��
	DWORD							dwBloodLower;						//��Ѫ����
	WORD							wLockPoint;							//���˵�
	BYTE							cbRestTime;							//��Ϣʱ��
	WORD							wBloodIndex;						//��Ѫ����
	WORD							wCardBoxIndex;						//��������
	WORD							wRankIndex;							//��������
	WORD							wRankTime;							//��������
	WORD							wRewardExp;							//ÿ�־���
	WORD							wCashIn;							//ÿ100�һ����
	WORD							wRoomTimeOut;						//����ر�ʱ�䣬ͬ�淿
	BYTE							cbIsKickOut;						//��������
	BYTE							cbIsSendRed;						//���������
	//----------------
	TCHAR							szServerName[LEN_SERVER];			//��������

	//��������
	BYTE							cbDistributeRule;					//�������
	WORD							wMinDistributeUser;					//��������
	WORD							wMaxDistributeUser;					//�������
	WORD							wDistributeTimeSpace;				//������
	WORD							wDistributeDrawCount;				//�������
	WORD							wDistributeStartDelay;				//��ʼ��ʱ

	//��������
	TCHAR							szDataBaseName[32];					//���ݿ���
	TCHAR							szDataBaseAddr[32];					//��ַ��Ϣ

	//��������
	BYTE							cbCustomRule[1024];					//�Զ�����
	TCHAR							szServiceMachine[LEN_MACHINE_ID];	//��������
};

//������Ϣ
struct tagGameServerCreate
{
	//��������
	WORD							wGameID;							//ģ���ʶ
	WORD							wServerID;							//�������

	//�ҽ�����
	WORD							wKindID;							//�ҽ�����
	WORD							wNodeID;							//�ҽӽڵ�
	WORD							wSortID;							//���б�ʶ

	//˰������
	LONG							lCellScore;							//��λ����
	WORD							wRevenueRatio;						//˰�ձ���
	SCORE							lServiceScore;						//�������

	//��������
	SCORE							lRestrictScore;						//���ƻ���
	SCORE							lMinTableScore;						//��ͻ���
	SCORE							lMinEnterScore;						//��ͻ���
	SCORE							lMaxEnterScore;						//��߻���
	SCORE							lMinEnterTiLi;						//�������

	WORD							wBloodIndex;						//��Ѫ����
	WORD							wCardBoxIndex;						//��������


	//��Ա����
	BYTE							cbMinEnterMember;					//��ͻ�Ա
	BYTE							cbMaxEnterMember;					//��߻�Ա

	//��������
	DWORD							dwServerRule;						//�������
	DWORD							dwAttachUserRight;					//����Ȩ��

	//��������
	WORD							wMaxPlayer;							//�����Ŀ
	WORD							wAndroidCount;						//Ԥ���ػ�������Ŀ
	WORD							wTableCount;						//������Ŀ
	WORD							wServerPort;						//����˿�
	WORD							wServerType;						//��������
	DWORD							dwRoomID;							//RoomID
	TCHAR							szServerName[LEN_SERVER];			//��������

	//��������
	BYTE							cbDistributeRule;					//�������
	WORD							wMinDistributeUser;					//��������
	WORD							wMaxDistributeUser;					//�������
	WORD							wDistributeTimeSpace;				//������
	WORD							wDistributeDrawCount;				//�������
	WORD							wDistributeStartDelay;				//��ʼ��ʱ

	//��������
	TCHAR							szDataBaseName[32];					//���ݿ���
	TCHAR							szDataBaseAddr[32];					//��ַ��Ϣ

	//��������
	BYTE							cbCustomRule[1024];					//�Զ�����
	TCHAR							szServiceMachine[LEN_MACHINE_ID];	//��������

};

//////////////////////////////////////////////////////////////////////////////////

//���ò���
struct tagModuleInitParameter
{
	tagGameServiceAttrib			GameServiceAttrib;					//��������
	tagGameServiceOption			GameServiceOption;					//��������
};

//////////////////////////////////////////////////////////////////////////////////

//�����ļ�
#ifndef MODULE_MANAGER_DLL 
	
	#include "ListControl.h"
	#include "DlgServerItem.h"
	#include "DlgServerWizard.h"
	
	#include "ModuleListControl.h"
	#include "ModuleDBParameter.h"
	#include "ModuleInfoManager.h"

	#include "ServerCustomRule.h"
	#include "ServerListControl.h"
	#include "ServerInfoManager.h"

#endif

//////////////////////////////////////////////////////////////////////////////////

#endif