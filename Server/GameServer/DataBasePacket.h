#pragma once

#pragma pack(1)
//�������ݰ�

//�û��¼�
#define	DBR_GR_LOGON_USERID			100									//I D ��¼
#define	DBR_GR_LOGON_MOBILE			101									//�ֻ���¼
#define	DBR_GR_LOGON_ACCOUNTS		102									//�ʺŵ�¼
#define DBR_GR_LOGON_TOKENID		103									//Token��¼

//�û��¼�
#define	DBR_GR_LOGON_USERID_BCode			150									//I D ��¼
#define	DBR_GR_LOGON_MOBILE_BCode			151									//�ֻ���¼
#define	DBR_GR_LOGON_ACCOUNTS_BCode			152									//�ʺŵ�¼

//ϵͳ�¼�
#define DBR_GR_WRITE_GAME_SCORE		200									//��Ϸ����
#define DBR_GR_LEAVE_GAME_SERVER	201									//�뿪����
#define DBR_GR_GAME_SCORE_RECORD	202									//���ּ�¼
#define DBR_GR_MANAGE_USER_RIGHT	203									//Ȩ�޹���
#define DBR_GR_LOAD_SYSTEM_MESSAGE	204									//ϵͳ��Ϣ
#define DBR_GR_LOAD_SENSITIVE_WORDS	205									//���д�
#define DBR_GR_USER_SITUP			206									//��������
#define DBR_GR_WRITE_GAME_MEDAL		207									//��Ϸ����
#define DBR_GP_GAME_FISH_RECORD		208									// ������Ϸ��¼
#define DBR_GP_GAME_SCROLL_MESSAGE	209									// ��Ϸ������Ϣ

#define DBR_GR_SCROLL_MESSAGE				221							// ������Ϣ��ȡ
#define DBR_GR_LOAD_TIP_MESSAGE		222									//������Ϣ
#define DBR_GR_LOAD_SCROLL_MESSAGE		223									//��ȡ������Ϣ
#define DBR_GR_CLEAR_SCROLL_MESSAGE		224									//���������Ϣ

#define DBR_GR_SET_USER_OFFLINE_RECORD     225								//�����û����߼�¼
#define DBR_GR_GET_FIRST_REWARD_STATUS     226								//��ȡһ�ֽ���״̬

#define  DRB_GR_STATUS_USER_OFFLINE			227								//�������


//�����¼�
#define DBR_GR_LOAD_PARAMETER		300									//���ز���
#define DBR_GR_LOAD_GAME_COLUMN		301									//�����б�
#define DBR_GR_LOAD_ANDROID_USER	302									//���ػ���
#define DBR_GR_LOAD_GAME_PROPERTY	303									//���ص���

//�����¼�
#define DBR_GR_USER_SAVE_SCORE		400									//����Ԫ��
#define DBR_GR_USER_TAKE_SCORE		401									//��ȡԪ��
#define DBR_GR_USER_TRANSFER_SCORE	402									//ת��Ԫ��
#define DBR_GR_QUERY_INSURE_INFO	403									//��ѯ����
#define DBR_GR_QUERY_TRANSFER_USER_INFO	    404							//��ѯ�û�
#define DBR_GR_USER_INSURE_SYNC		405									//ͬ��Ԫ��

//��Ϸ�¼�
#define DBR_GR_PROPERTY_REQUEST		500									//��������
#define DBR_GR_GAME_FRAME_REQUEST	502									//��Ϸ����
#define DBR_GR_KILL_PALAYER_LOCKER  503									//�������


//�����˼����¼�
#define DBR_GR_ANDRIOD_APPLY		700									//���������id
#define DBR_GR_ANDRIOD_CURRENTVALID 701									//��֤�Ϸ���
#define DBR_GR_ANDRIOD_REFRESH		702									//ˢ��ʱ���
#define DBR_GR_ANDRIOD_RELEASE      703									//�ͷŵ���������
#define DBR_GR_ANDRIOD_ALL			704									//�ͷ����Ի�����
#define DBR_GR_ANDRIOD_ARRAY		705									//�����ͷŻ�����
#define DBR_GR_ANDRIOD_RELEASE_DELETE	706								//�ͷŵ��������ˣ���ɾ��������

//�����¼�
#define  DBR_GR_ITEM_LOAD			820									//���ص���

//////////////////////////////////////////////////////////////////////////////////
//�����Ϣ

//�߼��¼�
#define DBO_GR_LOGON_SUCCESS		100									//��¼�ɹ�
#define DBO_GR_LOGON_FAILURE		101									//��¼ʧ��

#define DBO_GR_LOGON_SUCCESS_H5		110									//��¼�ɹ�
#define DBO_GR_LOGON_SUCCESS_BCode	150									//��¼�ɹ�

//�����¼�
#define DBO_GR_GAME_PARAMETER		200									//������Ϣ
#define DBO_GR_GAME_COLUMN_INFO		201									//�б���Ϣ
#define DBR_GR_GAME_ANDROID_INFO	202									//������Ϣ
#define DBO_GR_GAME_PROPERTY_INFO	203									//������Ϣ

#define DBO_GP_UPDATE_ATTRIBUTE			256								//czg��������

//��������
#define DBO_GR_USER_INSURE_INFO		300									//��������
#define DBO_GR_USER_INSURE_SUCCESS	301									//���гɹ�
#define DBO_GR_USER_INSURE_FAILURE	302									//����ʧ��
#define DBO_GR_USER_INSURE_USER_INFO   303								//�û�����

//��Ϸ�¼�
#define DBO_GR_PROPERTY_SUCCESS			400								//���߳ɹ�
#define DBO_GR_PROPERTY_FAILURE			401								//����ʧ��
#define DBO_GR_GAME_FRAME_RESULT		402								//��Ϸ���

//�����¼�
#define DBO_GR_MATCH_EVENT_START		500								//�¼���Χ
#define DBO_GR_MATCH_RANK				501								//��������
#define DBO_GR_MATCH_APPLY_ANDROID		503								//�������������

#define	DBO_GR_MATCH_TAGOUT				512								//��������
#define	DBO_GR_MATCH_BASE				513								//��������
#define DBO_GR_MATCH_FEE				514								//
#define DBO_GR_MATCH_SOLUTION			515								//
#define DBO_GR_MATCH_JOIN				516								//��������
#define DBO_GR_MATCH_QUIT				517								//��������
#define	DBO_GR_MATCH_RESULT				518								//��ȡ����
#define	DBO_GR_MATCH_STAGE				519	
#define	DBO_GR_MATCH_DATE				520								//����ʱ��
#define DBO_GR_MATCH_FINISH				521								//�������
#define	DBO_GR_MATCH_REVIVE				522								//��������
#define DBO_GR_MATCH_REWARD				523								//��������
#define DBO_GR_MATCH_EXP_REWARD			524								//���齱��

#define DBO_GR_MATCH_EVENT_END		    599								//�¼���Χ

//ϵͳ�¼�
#define DBO_GR_SYSTEM_MESSAGE_RESULT	600									//��Ϣ���
#define DBO_GR_SENSITIVE_WORDS		    601									//���д�
#define DBO_GR_LOAD_SCROLL_MESSAGE		602									//��ȡ������Ϣ
#define DBO_GR_GET_FIRST_REWARD_STATUS		603								//��ȡһ�ֽ���״̬

#define DBO_GR_SCROLL_MESSAGE			604									// ���ع�����Ϣ


			

//�����˼����¼�
#define DBO_GR_ANDRIOD_APPLY		700									//���������id
#define DBO_GR_ANDRIOD_CURRENTVALID 701									//��֤�Ϸ���
#define DBO_GR_ANDRIOD_REFRESH		702									//ˢ��ʱ���
#define DBO_GR_ANDRIOD_RELEASE      703									//�ͷŵ���������
#define DBO_GR_ANDRIOD_ALL			704									//�ͷ����Ի�����
#define DBO_GR_ANDRIOD_ARRAY		705									//�����ͷŻ�����
#define DBO_GR_ANDRIOD_RELEASE_DELTE	706								//�ͷŵ��������ˣ���ɾ��������

//////////////////////////////////////////////////////////////////////////////////

//ID ��¼
struct DBR_GR_LogonUserID
{		
	//��¼��Ϣ
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_MD5];				//��¼����

	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//������ʶ
};

//Token��¼
struct DBR_GR_LogonTokenID
{
	//��¼��Ϣ
	DWORD							dwMerchantID;						//�̻�ID
	DWORD							dwUserID;							//�û� I D
	TCHAR							szTokenID[LEN_MD5];				//��¼����

	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//������ʶ
	TCHAR							szClientIP[16];						//�ͻ���ip
	BYTE                            cbDeviceType;                       //�豸����
};

//ID ��¼
struct DBR_GR_LogonMobile
{		
	//��¼��Ϣ
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_MD5];				//��¼����
	BYTE                            cbDeviceType;                       //�豸����
	WORD                            wBehaviorFlags;                     //��Ϊ��ʶ
	WORD                            wPageTableCount;                    //��ҳ����

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//������ʶ
};

//�ʺŵ�¼
struct DBR_GR_LogonAccounts
{
	//��¼��Ϣ
	TCHAR							szPassword[LEN_MD5];				//��¼����
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//������ʶ
};

//��Ϸ����
struct DBR_GR_WriteGameScore
{
	//�û���Ϣ
	DWORD							dwUserID;							//�û���ʶ
	DWORD							dwClientAddr;						//���ӵ�ַ

	//�û���Ϣ
	DWORD							dwDBQuestID;						//�����ʶ
	DWORD							dwInoutIndex;						//��������

	WORD							wTableID;							//����ID
	//�ɼ�����
	tagVariationInfo				VariationInfo;						//��ȡ��Ϣ
};


//�뿪����
struct DBR_GR_LeaveGameServer
{
	//�û���Ϣ
	DWORD							dwUserID;							//�û���ʶ
	DWORD							dwInoutIndex;						//��¼����
	DWORD							dwLeaveReason;						//�뿪ԭ��
	DWORD							dwOnLineTimeCount;					//����ʱ��

	//�ɼ�����
	tagVariationInfo				RecordInfo;							//��¼��Ϣ
	tagVariationInfo				VariationInfo;						//��ȡ��Ϣ

	//ϵͳ��Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//������ʶ
};

//������Ϸ��¼
struct DBR_GP_GameFishRecord
{
	//������Ϣ
	WORD							wTableID;							//���Ӻ���
	//���ּ�¼
	WORD							wRecordCount;						//��¼��Ŀ
	tagFishRecordInfo				FishRecord[MAX_CHAIR];				//��Ϸ��¼
};

//��Ϸ��¼
struct DBR_GR_GameScoreRecord
{
	//������Ϣ
	WORD							wTableID;							//���Ӻ���
	WORD							wUserCount;							//�û���Ŀ
	WORD							wAndroidCount;						//������Ŀ

	//���˰��
	SCORE							lWasteCount;						//�����Ŀ
	SCORE							lRevenueCount;						//˰����Ŀ

	//ͳ����Ϣ
	DWORD							dwUserMemal;						//������Ŀ
	DWORD							dwPlayTimeCount;					//��Ϸʱ��

	//ʱ����Ϣ
	SYSTEMTIME						SystemTimeStart;					//��ʼʱ��
	SYSTEMTIME						SystemTimeConclude;					//����ʱ��

	//���ּ�¼
	WORD							wRecordCount;						//��¼��Ŀ
	tagGameScoreRecord				GameScoreRecord[MAX_CHAIR];			//��Ϸ��¼
};

struct DBR_GP_UserSitUp
{
	DWORD							dwUserID;							//�û�ID
	WORD							wKindID;							//��ϷID
	WORD							wServerID;							//����ID
	WORD							wTableID;							//����ID
	WORD							wChairID;							//����ID
	SCORE							lScore;								//�û�����
	WORD							wInOutFlag;							//�������
};

//����Ԫ��
struct DBR_GR_UserSaveScore
{
	BYTE                            cbActivityGame;                     //��Ϸ����
	DWORD							dwUserID;							//�û� I D
	SCORE							lSaveScore;							//����Ԫ��
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//ȡ��Ԫ��
struct DBR_GR_UserTakeScore
{
	BYTE                            cbActivityGame;                     //��Ϸ����
	DWORD							dwUserID;							//�û� I D
	SCORE							lTakeScore;							//��ȡԪ��
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szPassword[LEN_PASSWORD];			//��������
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//ת��Ԫ��
struct DBR_GR_UserTransferScore
{
	BYTE                            cbActivityGame;                     //��Ϸ����
	DWORD							dwUserID;							//�û� I D
	DWORD							dwClientAddr;						//���ӵ�ַ
	BYTE                            cbByNickName;                       //�ǳ�����
	SCORE							lTransferScore;						//ת��Ԫ��
	TCHAR							szNickName[LEN_NICKNAME];			//Ŀ���û�
	TCHAR							szPassword[LEN_PASSWORD];			//��������
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//��ѯ����
struct DBR_GR_QueryInsureInfo
{
	BYTE                            cbActivityGame;                     //��Ϸ����
	DWORD							dwUserID;							//�û� I D
	DWORD							dwClientAddr;						//���ӵ�ַ
};

//��ѯ�û�
struct DBR_GR_QueryTransferUserInfo
{
	BYTE                            cbActivityGame;                     //��Ϸ����
	BYTE                            cbByNickName;                       //�ǳ�����
	TCHAR							szNickName[LEN_NICKNAME];			//Ŀ���û�
	DWORD							dwUserID;							//�û� I D
};

//��������
struct DBR_GR_PropertyRequest
{
	//������Ϣ
	WORD							wItemCount;							//������Ŀ
	WORD							wPropertyIndex;						//��������
	DWORD							dwSourceUserID;						//�������
	DWORD							dwTargetUserID;						//ʹ�ö���

	//����ģʽ
	BYTE                            cbRequestArea;			            //ʹ�÷�Χ 
	BYTE							cbConsumeScore;						//��������
	SCORE							lFrozenedScore;						//�������

	//�û�Ȩ��
	DWORD                           dwUserRight;						//��ԱȨ��

	//ϵͳ��Ϣ
	WORD							wTableID;							//���Ӻ���
	DWORD							dwInoutIndex;						//�����ʶ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//¼������
struct DBR_GR_WriteVideo
{
	BYTE							szRoomName[31];						//��������
	BYTE							szFileName[100];						//�ļ�����
	BYTE							cbStreamContent[VIDEO_LEN];			//���ݳ���
};


//////////////////////////////////////////////////////////////////////////////////

//��¼�ɹ�
struct DBO_GR_LogonSuccess
{
	//��������
	DWORD							wFaceID;							//ͷ���ʶ
	DWORD							dwUserID;							//�û� I D
	DWORD							dwGameID;							//��Ϸ I D
	DWORD							dwGroupID;							//���� I D
	DWORD							dwCustomID;							//�Զ�ͷ��
	TCHAR							szNickName[LEN_NICKNAME];			//�ʺ��ǳ�
	TCHAR							szGroupName[LEN_GROUP_NAME];		//��������

	//�û�����
	BYTE							cbGender;							//�û��Ա�
	BYTE							cbMemberOrder;						//��Ա�ȼ�
	BYTE							cbMasterOrder;						//����ȼ�
	TCHAR							szUnderWrite[LEN_UNDER_WRITE];		//����ǩ��
	BYTE							cbIsAndroid;						

	//������Ϣ
	SCORE							lScore;								//�û�����
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
	DWORD							wAvatarID;							//����ID

	//�û�Ȩ��
	DWORD							dwUserRight;						//�û�Ȩ��
	DWORD							dwMasterRight;						//����Ȩ��

	//��������
	DWORD							dwInoutIndex;						//��¼����
	TCHAR							szMachineID[LEN_MACHINE_ID];		//������ʶ

	//�ֻ�����
	BYTE                            cbDeviceType;                       //�豸����
	WORD                            wBehaviorFlags;                     //��Ϊ��ʶ
	WORD                            wPageTableCount;                    //��ҳ����

	//������Ϣ
	TCHAR							szPassword[LEN_MD5];				//��¼����
	TCHAR							szDescribeString[128];				//������Ϣ
};

//��¼ʧ��
struct DBO_GR_LogonFailure
{
	LONG							lResultCode;						//�������
	TCHAR							szDescribeString[128];				//������Ϣ
};

//������Ϣ
struct DBO_GR_GameParameter
{
	//������Ϣ
	WORD							wMedalRate;							//���ƻ���
	WORD							wRevenueRate;						//����˰��

	//�汾��Ϣ
	DWORD							dwClientVersion;					//�ͻ��汾
	DWORD							dwServerVersion;					//����汾
};

//�б���Ϣ
struct DBO_GR_GameColumnInfo
{
	LONG							lResultCode;						//�������
	BYTE							cbColumnCount;						//�б���Ŀ
	tagColumnItem					ColumnItemInfo[MAX_COLUMN];			//�б���Ϣ
};

//������Ϣ
struct DBO_GR_GameAndroidInfo
{
	LONG							lResultCode;						//�������
	WORD							wAndroidCount;						//�û���Ŀ
	tagAndroidParameter				AndroidParameter[MAX_ANDROID];		//������Ϣ
};

//������Ϣ
struct DBO_GR_GamePropertyInfo
{
	LONG							lResultCode;						//�������
	BYTE							cbPropertyCount;					//������Ŀ
	tagPropertyInfo					PropertyInfo[MAX_PROPERTY];			//������Ϣ
};

//��������
struct DBO_GR_UserInsureInfo
{
	BYTE                            cbActivityGame;                     //��Ϸ����
	WORD							wRevenueTake;						//˰�ձ���
	WORD							wRevenueTransfer;					//˰�ձ���
	WORD							wServerID;							//�����ʶ
	SCORE							lUserScore;							//�û�Ԫ��
	SCORE							lUserInsure;						//����Ԫ��
	SCORE							lTransferPrerequisite;				//ת������
};

//���гɹ�
struct DBO_GR_UserInsureSuccess
{
	BYTE                            cbActivityGame;                     //��Ϸ����
	DWORD							dwUserID;							//�û� I D
	SCORE							lSourceScore;						//ԭ��Ԫ��
	SCORE							lSourceInsure;						//ԭ��Ԫ��
	SCORE							lInsureRevenue;						//����˰��
	SCORE							lFrozenedScore;						//�������
	SCORE							lVariationScore;					//Ԫ���仯
	SCORE							lVariationInsure;					//���б仯
	TCHAR							szDescribeString[128];				//������Ϣ
};

//����ʧ��
struct DBO_GR_UserInsureFailure
{
	BYTE                            cbActivityGame;                     //��Ϸ����
	LONG							lResultCode;						//��������
	SCORE							lFrozenedScore;						//�������
	TCHAR							szDescribeString[128];				//������Ϣ
};

//�û�����
struct DBO_GR_UserTransferUserInfo
{
	BYTE                            cbActivityGame;                     //��Ϸ����
	DWORD							dwGameID;							//�û� I D
	TCHAR							szNickName[LEN_NICKNAME];			//�ʺ��ǳ�
};

//���߳ɹ�
struct DBO_GR_S_PropertySuccess
{
	//������Ϣ
	WORD							wItemCount;							//������Ŀ
	WORD							wPropertyIndex;						//��������
	DWORD							dwSourceUserID;						//�������
	DWORD							dwTargetUserID;						//ʹ�ö���

	//����ģʽ
	BYTE                            cbRequestArea;						//����Χ
	BYTE							cbConsumeScore;						//��������
	SCORE							lFrozenedScore;						//�������

	//��ԱȨ��
	DWORD                           dwUserRight;						//��ԱȨ��

	//�����Ϣ
	SCORE							lConsumeGold;						//����Ԫ��
	LONG							lSendLoveLiness;					//��������
	LONG							lRecvLoveLiness;					//��������

	//��Ա��Ϣ
	BYTE							cbMemberOrder;						//��Ա�ȼ�
};

//����ʧ��
struct DBO_GR_PropertyFailure
{
	BYTE                            cbRequestArea;						//����Χ
	LONG							lResultCode;						//��������
	SCORE							lFrozenedScore;						//�������
	TCHAR							szDescribeString[128];				//������Ϣ
};

//��������
struct DBO_GR_MatchRank
{
	BYTE							cbRank;								//��������
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�
	LONG							lMatchScore;						//�û��÷�
};

//Ȩ�޹���
struct DBR_GR_ManageUserRight
{
	DWORD							dwUserID;							//Ŀ���û�
	DWORD							dwAddRight;							//���Ȩ��
	DWORD							dwRemoveRight;						//ɾ��Ȩ��
	bool							bGameRight;							//��ϷȨ��
};

//��������
struct DBR_GR_UserMatchJoin
{
	DWORD							dwUserID;							//�û�ID
	WORD							wServerID;							//����ID
	DWORD							dwMatchID;							//����ID
	BYTE							cbQuitMatch;						//�Ƿ�����
	DWORD							dwJoinType;
	DWORD							dwJoinFee;
	DWORD							dwJoinTypeTwo;
	DWORD							dwJoinFeeTwo;
	DWORD							dwJoinTypeThree;
	DWORD							dwJoinFeeThree;
	DWORD							dwJoinTypeFour;
	DWORD							dwJoinFeeFour;
	DWORD							dwJoinTypeFive;
	DWORD							dwJoinFeeFive;
};

struct tagRewardItem
{
	DWORD							dwRewardType;
	DWORD							dwCount;
};

//��������
struct DBR_GR_MatchReward
{
	DWORD								dwUserID;							//�û� I D
	WORD								wServerID;
	DWORD								dwMatchID;							//���� I D 
	DWORD								dwRank;								//��������
	SCORE								lMatchScore;						//�����÷�
	tagRewardItem						Item[5];
};

//���齱��
struct DBR_GR_MatchExpReward
{
	DWORD								dwUserID;							//�û� I D
	WORD								wServerID;
	DWORD								dwMatchID;							//���� I D 
	DWORD								dwRank;								//��������
	DWORD								dwExpValue;
};

struct DBO_GR_MatchRewardProp
{
	LONG							lResultCode;						//�������
	WORD							wCount;								//�û���Ŀ
	tagMatchPropertyName			MatchPropertyName[32];
};

//ϵͳ��Ϣ
struct DBR_GR_SystemMessage
{
	BYTE                            cbMessageID;                        //��ϢID
	BYTE							cbMessageType;						//��Ϣ����
	BYTE                            cbAllRoom;                          //ȫ�巿��
	DWORD							dwTimeRate;						    //ʱ��Ƶ��
	TCHAR							szSystemMessage[LEN_USER_CHAT];		//ϵͳ��Ϣ
};

// ��ѯ�ڰ�������
struct DBR_BlackOrWhite
{
	DWORD							UserID;								//��ϢID
	char*							pBlackOrWhite;						//��Ϣ����
};

//�����������
struct DBR_GR_GameAndroidApply
{		
	WCHAR							guidRoom[40];						//����guid
	DWORD							dwCount;							//������Ŀ	
	DWORD							dwRoomID;							//����id
};

//������Ϣ
struct DBO_GR_GameAndroidApply
{
	LONG							lResultCode;						//�������
	WORD							wAndroidCount;						//�û���Ŀ
	tagAndroidParameter				AndroidParameter[MAX_ANDROID];		//������Ϣ
};

//��ǰ��Ч����������
struct DBR_GR_GameAndriodCurrentValid
{
	WCHAR							guidRoom[40];						//����id
};

//��ǰ��Ч������
struct DBO_GR_GameAndriodCurrentValid
{
	LONG							lResultCode;						//�������
	WORD							wAndroidCount;						//�û���Ŀ
	DWORD							dwUserID[MAX_ANDROID];		//������Ϣ
};

//ˢ��ʱ�������
struct DBR_GR_GameAndriodRefresh
{
	WCHAR							guidRoom[40];						//����id
};

//�ͷŵ���������
struct DBR_GR_GameAndriodRelease
{
	WCHAR							guidRoom[40];						//����id
	DWORD							dwUserId;							//�û�id	
};

struct DBR_GR_GameAndriodReleaseAndDelete
{
	WCHAR							guidRoom[40];						//����id
	DWORD							dwUserId;							//�û�id	
	WORD							wBingId;							//��id
};


//�����ͷŻ�����
struct DBR_GR_GameAndriodArray
{
	TCHAR							szUserArray[8024];					//��Ӧ���ݿ��max����ʽ��id,...,id,id
	WCHAR							guidRoom[40];						//����id
};
//�ͷ�ȫ��������
struct DBR_GR_GameAndriodAll
{
	TCHAR							guidRoom[40];						//����id
};

struct DBO_GR_GameAndriodRelease
{
	DWORD							dwUserId;							//������ID
	WORD							wBindId;							//��ID							
};


//��������
struct DBO_GR_GameTaskLoad
{
	WORD							wKindID;
	WORD							wServerID;
	DWORD							dwUserID;
};

//д������
struct DBO_GR_GameTaskWrite
{
	WORD							wKindID;
	WORD							wServerID;
	WORD							wTaskID;
	WORD							wCount;
	DWORD							dwUserID;
};
//�ύ����
struct DBO_GR_GameTaskSubmit
{
	WORD							wKindID;
	WORD							wServerID;
	WORD							wTaskID;
	DWORD							dwUserID;
};

//����ɹ�
struct DBO_GR_GameTaskSuccess
{
	WORD							wCount;
	DWORD							dwUserID;
	tagUserTaskInfo				TaskSuccess[10];
};

//�������а�
struct DBO_GR_RankTopSuccess
{
	WORD							wCount;
	WORD							wTableID;
	tagRankTopInfo					RankInfo[6];
};

//���ص���
struct DBO_GR_GameItemLoad
{
	WORD							wKindID;
	WORD							wServerID;
	DWORD							dwUserID;
};

//���ص���
struct DBO_GR_GameItemSuccess
{
	WORD							wKindID;
	WORD							wServerID;
	WORD							wCount;
	DWORD							dwUserID;
	tagGameItemInfo					GameItemInfo[8];
};


////////////////////////////��������//////////////////////////////////////////////
//�������
struct DBO_GR_UserMatchJoin
{
	WORD				wResultCode;
	TCHAR				szMsg[128];
};

//�����ӹ�
struct DBO_GR_MatchResult
{
	DWORD							dwRank;								//����
};

//
struct tagRewardInfo
{
	DWORD							dwOrderID;
	DWORD							dwRankUpper;
	DWORD							dwRankLower;
	DWORD							dwRewardType;
	DWORD							dwRewardCount;
};

//��������
struct CMD_GR_RewardArray
{
	tagRewardInfo					Reward[50];
	DWORD							dwCount;
	DWORD							dwSolutionID;
};

//
struct tagExpReward
{
	DWORD							dwOrderID;
	DWORD							dwRankUpper;
	DWORD							dwRankLower;
	DWORD							dwExpValue;
};

//���齱��
struct CMD_GR_ExpReward
{
	tagExpReward					Exp[5];
	DWORD							dwCount;
	DWORD							dwSolutionID;
};

//��Ѫ����
struct	DBR_GR_BloodInfo
{
	WORD	wBloodOrder;			//��Ѫ����
	WORD	wPayType;				//֧������ 12���� 1���
	DWORD	dwBloodPrice;			//�۸�
	DWORD	dwBloodValue;			//���Ѫ��

};

struct	DBR_GR_BloodArray
{
	WORD			 wCount;
	DBR_GR_BloodInfo  BloodInfo[4];
};

//�Ծֱ���
struct tagCardBoxInfo
{
	WORD		wBoxID;
	WORD		wCardCount;
	DWORD		dwRewardType;
	DWORD		dwRewardCount;
};

struct DBR_GR_CardBoxArray
{
	WORD				wCount;
	tagCardBoxInfo		BoxInfo[4];
};

// ������Ϣ
struct DBO_GP_ExpRank
{
	DWORD						dwUserID;					// ����ID
	DWORD						wFaceID;					// ͷ��ID
	DWORD						dwExpValue;					// ����
	TCHAR						szNickName[LEN_NICKNAME];	// �ǳ�
	TCHAR						szRemark[LEN_NICKNAME];		// ����
};

//��ʱ����
struct tagTimeRank
{
	WORD						wRewardGroup;
	WORD						RankUpper;
	WORD						RankLower;
	DWORD						dwRewardType;
	DWORD						dwRewardCount;
};

struct DBR_GR_TimeRankArray
{
	WORD						wCount;
	tagTimeRank					Rank[15];
};

//��������
struct DBR_GR_RankRatio
{
	DWORD						dwRatioID;
	WORD						wInningsFirst;
	BYTE						cbRatioFirst;
	WORD						wInningsSecond;
	BYTE						cbRatioSecond;
	WORD						wInningsThird;
	BYTE						cbRatioThird;
	WORD						wInningsFour;
	BYTE						cbRatioFour;
};

struct DBR_GR_GetTimeRank
{
	WORD						wRank;
	DWORD						dwRankScore;
	TCHAR						szNickName[LEN_NICKNAME];
};

struct DBR_GR_GameHappyLand
{
	DWORD							dwUserID;
	BYTE							cbBanker;							//������
	BYTE							cbRobBanker;						//������
	BYTE							cbFarmer;							//��ũ��
	BYTE							cbSpring;							//�������
	BYTE							cbRocket;							//�������
};

struct DBR_GR_GameLand
{
	DWORD							dwUserID;
	BYTE							cbBanker;							//������
	BYTE							cbRobBanker;						//������
	BYTE							cbFarmer;							//��ũ��
	BYTE							cbSpring;							//�������
	BYTE							cbRocket;							//�������
};

struct DBR_GR_GameDoubleLand
{
	DWORD							dwUserID;
	BYTE							cbBanker;							//������
	BYTE							cbRobBanker;						//������
	BYTE							cbFarmer;							//��ũ��
	BYTE							cbSpring;							//�������
	BYTE							cbRocket;							//�������
};

//������Ϣ
struct DBO_GR_TipMessage
{
	TCHAR szSystemMessage[1024];
	SYSTEMTIME startTime;
	SYSTEMTIME endTime;
};

//������Ϣ
struct DBO_GR_ScrollMessage
{
	TCHAR szScrollMessage[256];
};

struct DBO_GR_LoadScrollMessage
{
	DWORD dwID;							// ��ϢID
	DWORD dwMessageType;				// ��Ϣ���� 0 ϵͳ 1 ��Ϸ
	TCHAR szScrollMessage[1024];		// ��Ϣ����
};


//////////////////////////////////////////////////////////////////////////////////
//czg����ˢ��
struct DBO_UPDATE_ATTRIBUTE
{
	enum  attribute{
		enumAttr_error		=0,		//����
		enumAttr_exp			=1,		//������ֵ
		enumAttr_vip			=2,		//VIP�ɳ�ֵ
		enumAttr_liness		=3,		//�û�����
		enumAttr_order		=4,		//��Ա�ȼ�
		enumAttr_Medal		=5,		//�û�����
		enumAttr_gold			=6,		//�û����
		enumAttr_Insure		=7,		//�û�����
		enumAttr_HonorID		=8,		//����id �ƺ�
		enumAttr_AvatarID		=9,		//����id
		enumAttr_Power		=10,		//�û�����
		enumAttr_Jewel		=11,		//�û���ʯ
		enumAttr_lRevenue		=12,		//�û�˰��
		enumAttr_lLobbyLv		=13,		//�����ȼ�
		enumAttr_lMasterScore	=14,		//��ʦ��
		enumAttr_lHonorScore	=15,		//ս����
		enumAttr_lRaceScore	=16,		//������
		enumAttr_lAchieveScore	=17,		//�ɾͷ�
		enumAttr_FaceID		=18,		//ͷ��id
		enumAttr_Gender		=19,		//�Ա�

	};

	WORD wType;
	SCORE wScore;
};

//���������eg:���롢��;��������Ϣ��
struct DBO_GP_OperateResult
{	
	WORD							wSubCommandID;						//��Ϣ��;
	DWORD							dwResultCode;						//��������
	TCHAR							szDescribeString[128];				//������Ϣ
};

#pragma pack()