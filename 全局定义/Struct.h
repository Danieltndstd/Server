#ifndef STRUCT_HEAD_FILE
#define STRUCT_HEAD_FILE


#pragma pack(1)

//////////////////////////////////////////////////////////////////////////////////
//��Ϸ����,��������
enum enScoreType
{
	ScoreType_Score=1,							//���
	ScoreType_Insure=2,							//����
	ScoreType_Medal=3,							//����
	ScoreType_Jewel=4,							//��ʯ
	ScoreType_LobbyLV=5,						//��������
	ScoreType_GameExp=104,						//��Ϸ����
	ScoreType_Master=9,							//��ʦ��
	ScoreType_Compete=10,						//������
	ScoreType_Race=11,							//��������
	ScoreType_Power=12,							//����
	ScoreType_LoveLiness=13,					//����
	ScoreType_Honor=14,							//�����ƺ�
	ScoreType_Achievements=15,					//�ɾ�
	ScoreType_VIP=20,							//VIP					
};


//�����ƹ㽱������
enum enCalculateSpreadRewardOperationType
{
	SpreadRewardOperationType_Record = 0,		//��������ֻ��¼
	SpreadRewardOperationType_Reward = 1		//����������¼
};

/////////////////////////////////////////////////////////////////////////////////
//��Ϸ�б�

//��Ϸ����
struct tagGameType
{
	WORD							wJoinID;							//�ҽ�����
	WORD							wSortID;							//��������
	WORD							wTypeID;							//��������
	TCHAR							szTypeName[LEN_TYPE];				//��������
};

//��Ϸ����
struct tagGameKind
{
	WORD							wTypeID;							//��������
	WORD							wJoinID;							//�ҽ�����
	WORD							wSortID;							//��������
	WORD							wKindID;							//��������
	WORD							wGameID;							//ģ������
	WORD							wIsRoomOnline;						//�Ƿ��з��俪��
	DWORD							dwOnLineCount;						//��������
	DWORD							dwFullCount;						//��Ա����
	TCHAR							szKindName[LEN_KIND];				//��Ϸ����
	TCHAR							szProcessName[LEN_PROCESS];			//��������
	TCHAR							szDownLoadUrl[100];					//���ص�ַ
};

//��Ϸ����
struct tagGameKind_
{
	WORD							wKindID;							//��������
	WORD							wGameID;							//ģ������
	WORD							wIsRoomOnline;						//�Ƿ��з��俪��
	DWORD							dwOnLineCount;						//��������
	DWORD							dwFullCount;						//��Ա����
	TCHAR								szKindName[LEN_KIND];				//��Ϸ����
};

//��Ϸ�ڵ�
struct tagGameNode
{
	WORD							wKindID;							//��������
	WORD							wJoinID;							//�ҽ�����
	WORD							wSortID;							//��������
	WORD							wNodeID;							//�ڵ�����
	TCHAR							szNodeName[LEN_NODE];				//�ڵ�����
};

struct tagGameCollection
{
	DWORD						dwUserID;
	WORD						wSortID;
	WORD						wKindID;
	TCHAR						szDisplayName[LEN_TYPE];
};

//��������
struct tagGamePage
{
	WORD							wPageID;							//ҳ������
	WORD							wKindID;							//��������
	WORD							wNodeID;							//�ڵ�����
	WORD							wSortID;							//��������
	WORD							wOperateType;						//��������
	TCHAR							szDisplayName[LEN_PAGE];			//��ʾ����
};

//��Ϸ����
struct tagGameServer
{
	WORD							wKindID;							//��������
	WORD							wNodeID;							//�ڵ�����
	WORD							wSortID;							//��������
	WORD							wServerID;							//��������
	WORD							wServerPort;						//����˿�
	WORD							wServerType;						//��������
	DWORD							dwOnLineCount;						//��������
	DWORD							dwFullCount;						//��Ա����
	SCORE							lCellScore;							//����ͷ�
	BYTE							cbRoomType;

	TCHAR							szServerAddr[32];					//��������
	TCHAR							szServerName[LEN_SERVER];			//��������

	//��������
	SCORE							lRestrictScore;						//���ƻ���
	SCORE							lMinTableScore;						//��ͻ���
	SCORE							lMinEnterScore;						//��ͻ���
	SCORE							lMaxEnterScore;						//��߻���
	SCORE							lMinEnterTiLi;						//�������


	WORD							wCardBoxIndex;						//��������
};

//��Ϸ�����ֻ���
struct tagGameServer_
{
	WORD							wServerID;							//��������
	WORD							wServerPort;							//����˿�
	WORD							wServerType;						//��������
	DWORD							dwOnLineCount;					//��������
	DWORD							dwFullCount;						//��Ա����
	long long 						lCellScore;							//����ͷ�	��Ϸ����
	BYTE							cbRoomType;							// 0��� 1���� 2�м� 3�߼�
	TCHAR							szServerAddr[LEN_SERVER];		//����IP
	TCHAR							szServerName[LEN_SERVER];	//��������

	//��������
	SCORE							lMinEnterScore;						//����볡��
	SCORE							lMinTableScore;						//������·�	
	SCORE							lMaxEnterScore;						//����볡��

	WORD							wKindID;							//��Ϸ����

};

//��������
struct tagGameMatch
{
	WORD				wServerID;						//��������
	/*
	WORD				wOrderID;						//��������
	WORD				wMatchTab;						//������ǩ
	WORD				wMatchType;						//��������
	SYSTEMTIME			sysMatchStartTime;				//����ʱ��
	SYSTEMTIME			sysMatchJoinTime;				//����ʱ��
	WORD				wJoinCount;						//��������
	//tagMatchJoin		MatchJoin[20];					//��������
	TCHAR				szMatchReward[32];				//�ھ���Ʒ
	TCHAR				szMatchName[80];				//����ȫ��
	*/

	//tagMatchRequireInfo			Require;				//��������
	//tagMatchFeeInfo				Fee;					//��������
	//tagMatchReviveInfo			ReviveInfo;				//��������
	//CMD_GR_MatchDateTime		DateTime;				//����ʱ��
	//tagMatchTagout				Tagout;					//����
};

//��Ƶ����
struct tagAVServerOption
{
	WORD							wAVServerPort;						//��Ƶ�˿�
	DWORD							dwAVServerAddr;						//��Ƶ��ַ
};

//������Ϣ
struct tagOnLineInfoKind
{
	WORD							wKindID;							//���ͱ�ʶ
	DWORD							dwOnLineCount;						//��������
};

//������Ϣ
struct tagOnLineInfoServer
{
	WORD							wServerID;							//�����ʶ
	DWORD							dwOnLineCount;						//��������
};

struct tagBroadcastMsg
{
	TCHAR							szNickName[LEN_NICKNAME];
	TCHAR							szKindName[LEN_KIND];
	TCHAR							szScore[32];
};

//////////////////////////////////////////////////////////////////////////////////
//�û���Ϣ

//����״̬
struct tagTableStatus
{
	BYTE							cbTableLock;						//������־
	BYTE							cbPlayStatus;						//��Ϸ��־
};

//�û�״̬
struct tagUserStatus
{
	WORD							wTableID;							//��������
	WORD							wChairID;							//����λ��
	BYTE							cbUserStatus;						//�û�״̬
};

//�û�����
struct tagUserAttrib
{
	BYTE							cbCompanion;						//�û���ϵ
};

//�û�����
struct tagUserScore
{
	//������Ϣ
	SCORE							lScore;								//�û�����
	SCORE							lGrade;								//�û��ɼ�
	SCORE							lInsure;							//�û�����

	//��Ӯ��Ϣ
	DWORD							dwWinCount;							//ʤ������
	DWORD							dwLostCount;						//ʧ������
	DWORD							dwDrawCount;						//�;�����
	DWORD							dwFleeCount;						//��������

	//ȫ����Ϣ
	DWORD							dwUserMedal;						//�û�����
	DWORD							dwExperience;						//�û�����
	LONG							lLoveLiness;						//�û�����
};

//�û�����
struct tagMobileUserScore
{
	//������Ϣ
	SCORE							lScore;								//�û�����
	SCORE							lInsure;							//�û�����

	//��Ӯ��Ϣ
	DWORD							dwWinCount;							//ʤ������
	DWORD							dwLostCount;						//ʧ������
	DWORD							dwDrawCount;						//�;�����
	DWORD							dwFleeCount;						//��������

	//ȫ����Ϣ
	DWORD							dwUserMedal;						//�û�����
	DWORD							dwExperience;						//�û�����
};


//����ʹ��
struct tagUsePropertyInfo
{
	WORD                            wPropertyCount;                     //������Ŀ
	WORD                            dwValidNum;						    //��Ч����
	DWORD                           dwEffectTime;                       //��Чʱ��
};


//�û�����
struct tagUserProperty
{
	WORD                            wPropertyUseMark;                   //���߱�ʾ
	tagUsePropertyInfo              PropertyInfo[MAX_PT_MARK];			//ʹ����Ϣ   
};

//���߰���
struct tagPropertyPackage
{
	WORD                            wTrumpetCount;                     //С������
	WORD                            wTyphonCount;                      //��������
};

//ʱ����Ϣ
struct tagTimeInfo
{
	DWORD						dwEnterTableTimer;						//��������ʱ��
	DWORD						dwLeaveTableTimer;						//�뿪����ʱ��
	DWORD						dwStartGameTimer;						//��ʼ��Ϸʱ��
	DWORD						dwEndGameTimer;							//�뿪��Ϸʱ��
};

struct tagBaseScore
{
	WORD						wScoreType;
	SCORE						lScore;
};

struct tagScoreList
{
	WORD						wCount;
	tagBaseScore				BaseScore[20];
};


//�û���Ϣ
struct tagUserInfo
{
	//��������
	DWORD							dwUserID;							//�û� I D
	DWORD							dwGameID;							//��Ϸ I D
	DWORD							dwGroupID;							//���� I D
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�
	TCHAR							szGroupName[LEN_GROUP_NAME];		//��������
	TCHAR							szUnderWrite[LEN_UNDER_WRITE];		//����ǩ��

	//ͷ����Ϣ
	DWORD							wFaceID;							//ͷ������
	DWORD							dwCustomID;							//�Զ���ʶ
	DWORD							wAvatarID;							//��������

	//�û�����
	BYTE							cbGender;							//�û��Ա�
	BYTE							cbMemberOrder;						//��Ա�ȼ�
	BYTE							cbMasterOrder;						//����ȼ�
	BYTE							cbClientKind;						//��������

	//�û�״̬
	WORD							wTableID;							//��������
	WORD							wChairID;							//��������
	BYTE							cbUserStatus;						//�û�״̬
	WORD							wLastTableID;						//�������
	WORD							wLastChairID;						//�������

	//������Ϣ
	SCORE							lScore;								//�û�����
	SCORE							lGrade;								//�û��ɼ�
	SCORE							lInsure;							//�û�����
	SCORE							lRechargeScore;						//��ֵ����
	SCORE							lExchangeScore;						//�һ�����
	SCORE							lRevenue;							//�û�˰��

	//��Ϸ��Ϣ
	DWORD							dwWinCount;							//ʤ������
	DWORD							dwLostCount;						//ʧ������
	DWORD							dwDrawCount;						//�;�����
	DWORD							dwFleeCount;						//��������
	DWORD							dwUserMedal;						//�û�����
	DWORD							dwExperience;						//�û�����
	LONG							lLoveLiness;						//�û�����

	//////////////////////////////////////////////////////////////////////////
};

//�û�����
struct tagUserCardBox
{
	WORD							wCardCount;							//�������
	BYTE							cbBoxAwardCount;					//ʣ���콱����
	BYTE							cbIsComplete;						//�Ƿ����
	BYTE							cbBoxIndex;							//��������
};

//�û���Ϣ
struct tagUserInfoHead
{
	//�û�����
	DWORD							dwGameID;							//��Ϸ I D
	DWORD							dwUserID;							//�û� I D
	DWORD							dwGroupID;							//���� I D

	//ͷ����Ϣ
	DWORD							wFaceID;							//ͷ������
	DWORD							dwCustomID;							//�Զ���ʶ
	DWORD							dwAvatarID;							//��������

	//�û�����
	BYTE							cbGender;							//�û��Ա�
	BYTE							cbMemberOrder;						//��Ա�ȼ�
	BYTE							cbMasterOrder;						//����ȼ�

	//�û�״̬
	WORD							wTableID;							//��������
	WORD							wChairID;							//��������
	BYTE							cbUserStatus;						//�û�״̬

	//������Ϣ
	SCORE							lScore;								//�û�����
	SCORE							lGrade;								//�û��ɼ�
	SCORE							lInsure;							//�û�����

	//��Ϸ��Ϣ
	DWORD							dwWinCount;							//ʤ������
	DWORD							dwLostCount;						//ʧ������
	DWORD							dwDrawCount;						//�;�����
	DWORD							dwFleeCount;						//��������
	DWORD							dwUserMedal;						//�û�����
	DWORD							dwExperience;						//��ǰ����
	LONG							lLoveLiness;						//�û�����


	SCORE							lRevenue;							//�û�˰��
	SCORE							lRechargeScore;						//��ֵ����
	SCORE							lExchangeScore;						//�һ�����
};

//�û���Ϣ
struct tagUserInfoH5
{
	//�û�����
	DWORD							dwUserID;							//�û� I D
	DWORD							wFaceID;							//ͷ������
	BYTE							cbGender;							//�û��Ա�
	WORD							wTableID;							//��������
	WORD							wChairID;							//��������
	BYTE							cbUserStatus;						//�û�״̬
	SCORE							lScore;								//�û�����
																
	TCHAR							szNickName[LEN_NICKNAME];
};

//ͷ����Ϣ
struct tagCustomFaceInfo
{
	DWORD							dwDataSize;							//���ݴ�С
	DWORD							dwCustomFace[FACE_CX*FACE_CY];		//ͼƬ��Ϣ
};

//�û���Ϣ
struct tagUserRemoteInfo
{
	//�û���Ϣ
	DWORD							dwUserID;							//�û���ʶ
	DWORD							dwGameID;							//��Ϸ��ʶ
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�

	//�ȼ���Ϣ
	BYTE							cbGender;							//�û��Ա�
	BYTE							cbMemberOrder;						//��Ա�ȼ�
	BYTE							cbMasterOrder;						//����ȼ�

	//λ����Ϣ
	WORD							wKindID;							//���ͱ�ʶ
	WORD							wServerID;							//�����ʶ
	TCHAR							szGameServer[LEN_SERVER];			//����λ��
};

//�ƺ���Ϣ
struct tagHonorInfo
{
	DWORD							dwHonorID;							//�ƺ�ID
	DWORD							dwTypeID;							//����ID
};

//////////////////////////////////////////////////////////////////////////////////

//�㳡����
struct tagGamePlaza
{
	WORD							wPlazaID;							//�㳡��ʶ
	TCHAR							szServerAddr[32];					//�����ַ
	TCHAR							szServerName[32];					//��������
};

//��������
struct tagLevelItem
{
	LONG							lLevelScore;						//�������
	TCHAR							szLevelName[16];					//��������
};

//��Ա����
struct tagMemberItem
{
	BYTE							cbMemberOrder;						//�ȼ���ʶ
	TCHAR							szMemberName[16];					//�ȼ�����
};

//��������
struct tagMasterItem
{
	BYTE							cbMasterOrder;						//�ȼ���ʶ
	TCHAR							szMasterName[16];					//�ȼ�����
};

//�б�����
struct tagColumnItem
{
	BYTE							cbColumnWidth;						//�б���
	BYTE							cbDataDescribe;						//�ֶ�����
	TCHAR							szColumnName[16];					//�б�����
};

//��ַ��Ϣ
struct tagAddressInfo
{
	TCHAR							szAddress[32];						//�����ַ
};

//������Ϣ
struct tagDataBaseParameter
{
	WORD							wDataBasePort;						//���ݿ�˿�
	TCHAR							szDataBaseAddr[32];					//���ݿ��ַ
	TCHAR							szDataBaseUser[32];					//���ݿ��û�
	TCHAR							szDataBasePass[32];					//���ݿ�����
	TCHAR							szDataBaseName[32];					//���ݿ�����
};

//��������
struct tagServerOptionInfo
{
	//�ҽ�����
	WORD							wKindID;							//�ҽ�����
	WORD							wNodeID;							//�ҽӽڵ�
	WORD							wSortID;							//���б�ʶ

	//˰������
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
	TCHAR							szServerName[LEN_SERVER];			//��������
};

//�û���Ϣ
struct tagMobileUserInfoHead
{
	//�û�����
	DWORD							dwGameID;							//��Ϸ I D
	DWORD							dwUserID;							//�û� I D

	//ͷ����Ϣ
	DWORD							wFaceID;							//ͷ������
	DWORD							dwCustomID;							//�Զ���ʶ

	//�û�����
	BYTE							cbGender;							//�û��Ա�
	BYTE							cbMemberOrder;						//��Ա�ȼ�

	//�û�״̬
	WORD							wTableID;							//��������
	WORD							wChairID;							//��������
	BYTE							cbUserStatus;						//�û�״̬

	//������Ϣ
	SCORE							lScore;								//�û�����

	//��Ϸ��Ϣ
	DWORD							dwWinCount;							//ʤ������
	DWORD							dwLostCount;						//ʧ������
	DWORD							dwDrawCount;						//�;�����
	DWORD							dwFleeCount;						//��������
	DWORD							dwExperience;						//�û�����
};

struct tagMatchStatus
{
	BYTE							cbMatchStatus;						//����״̬
	DWORD							dwUserID;							//�û� ID
};

struct tagMatchNotify
{
	BYTE							cbNotify;
	WORD							wServerID;							//����ID
	WORD							wArraySize;							
	DWORD							UserIDArray[MAX_PATH];				//�û�ID
};

struct tagMatchingNotify
{
	BYTE							cbNotify;
	WORD							wServerID;							//����ID
};

struct tagGameUserFace
{
	unsigned long					user_id;
	unsigned short					face_id;
};

struct tagUserTaskInfo
{
	BYTE							cbIsComplete;
	WORD							wTaskID;
	WORD							wCurrentProcess;
	WORD							wTotalProcess;
	WORD							wItemCount;
	DWORD							dwRewardMedal;
	DWORD							dwHours;
	SCORE							lRewardGold;
	TCHAR							szTaskKind[32];
	TCHAR							szItemName[32];
	TCHAR							szTaskName[32];
	TCHAR							szTaskDesc[MAX_PATH];
};

struct tagUserTaskList
{
	WORD						wCount;
	tagUserTaskInfo				TaskInfo[10];
};

struct tagRankTopInfo
{
	WORD						wKindID;
	WORD						wServerID;
	DWORD						dwUserID;
	DWORD						RewardMedal;
	SCORE						lScore;
	TCHAR						szNickName[32];
};

struct tagRankTopList
{
	WORD							wCount;
	tagRankTopInfo					RankInfo[6];
};

struct tagGameItemInfo
{
	WORD							wItemID;
	WORD							wCount;
	TCHAR							szItemName[32];
};

//���ص���
struct tagGameItemList
{
	WORD							wKindID;
	WORD							wServerID;
	WORD							wCount;
	DWORD							dwUserID;
	tagGameItemInfo					GameItemInfo[8];
};



/////////////////��Ϸ������/////////////////////////////////

enum enTaskType
{
	enUnknowTask=-1,
	enCardTask,			//�Ծ�
	enWinTask,			//ʤ��
	enWinningStreak,	//��ʤ
	enScoreTask,		//��������
	enLimitTimeTask		//��ʱ
};

struct tagTaskBase
{
	enTaskType							TaskType;						//�������� 
	WORD								wTaskID;						//�����ʶ
	WORD								wTotalProcess;					//�ܽ���
};

struct tagTaskReward
{
	SCORE								lGold;
	DWORD								dwMedal;
	WORD								wItem;
	TCHAR								szItemName[32];
};

struct tagTaskItem
{
	BYTE								cbCompleteState;				//��ɱ�ʶ 1 0��
	WORD								wCurProcess;					//��ǰ����
	tagTaskBase							TaskBase;
	tagTaskReward						TaskReward;
};

struct tagTaskInfo
{
	DWORD	dwTaskID;
	WORD	wTaskWeight;	
};

struct tagTaskRefresh
{
	DWORD	dwUserID;	
	DWORD	dwTaskID;
	BYTE	cbTaskNum;	//�������
};

struct tagTaskAward
{
	DWORD	dwUserID;	
	DWORD	dwTaskID;
	BYTE	cbTaskNum;	//�������
};
//////////////////////////////////////////////////////////////////////////////////

////////////////////////////��������//////////////////////////////////////////////


//�����뽱����ϸ��Ϣ
// struct tagInviteCodeRewardInfo
// {
// 	DWORD dwID;						//����ID
// 	BYTE cbCondition;				//��������
// 	DWORD dwTopUpLimit;				//��ֵ����
// 	DWORD dwLevelLimit;				//�ȼ�����
// 	DWORD dwRewardItem;				//������Ʒ
// 	WORD wRewardNumber;				//��������
// 
// 	TCHAR szConditionExplain[100];   //����˵��
// };
// 
// //�ƹ���ϸ��Ϣ
// struct tagSpreadInfo
// {
// 	DWORD dwSpreadID;				 //�ƹ㽱��ID
// 	WORD wSpreadLevel;				//�ƹ�ȼ�
// 	WORD wRewardLevel;				//�����ȼ�
// 	WORD wReachUser;				//���������׼
// 	DWORD dwReachRevenue;			//����ˮ��׼
// 	WORD wRewardPercent;			//�����ٷֱ�
// 	DWORD dwRewardTimeOne;			//����ʱ��1
// 	DWORD dwRewardTimeTwo;			//����ʱ��2
// };

#pragma pack()

#endif