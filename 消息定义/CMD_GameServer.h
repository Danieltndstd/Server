#ifndef CMD_GAME_SERVER_HEAD_FILE
#define CMD_GAME_SERVER_HEAD_FILE

#pragma pack(1)

// �ص�������
#define CALLBACK_0(__selector__, __target__, ...) std::bind(&__selector__, __target__, ##__VA_ARGS__)
#define CALLBACK_1(__selector__, __target__, ...) std::bind(&__selector__, __target__, std::placeholders::_1, ##__VA_ARGS__)
#define CALLBACK_2(__selector__, __target__, ...) std::bind(&__selector__, __target__, std::placeholders::_1, std::placeholders::_2, ##__VA_ARGS__)
#define CALLBACK_3(__selector__, __target__, ...) std::bind(&__selector__, __target__, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, ##__VA_ARGS__)
#define CALLBACK_4(__selector__, __target__, ...) std::bind(&__selector__, __target__, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, ##__VA_ARGS__)

// ע�ắ��
#define REGISTER_FUNCTION_0(__map__, __command__, __selector__, __target__, ...) __map__.insert(std::make_pair(__command__, CALLBACK_0(__selector__, __target__, ##__VA_ARGS__)))
#define REGISTER_FUNCTION_1(__map__, __command__, __selector__, __target__, ...) __map__.insert(std::make_pair(__command__, CALLBACK_1(__selector__, __target__, ##__VA_ARGS__)))
#define REGISTER_FUNCTION_2(__map__, __command__, __selector__, __target__, ...) __map__.insert(std::make_pair(__command__, CALLBACK_2(__selector__, __target__, ##__VA_ARGS__)))
#define REGISTER_FUNCTION_3(__map__, __command__, __selector__, __target__, ...) __map__.insert(std::make_pair(__command__, CALLBACK_3(__selector__, __target__, ##__VA_ARGS__)))
#define REGISTER_FUNCTION_4(__map__, __command__, __selector__, __target__, ...) __map__.insert(std::make_pair(__command__, CALLBACK_4(__selector__, __target__, ##__VA_ARGS__)))

//ҵ�������
#define  MDM_GR_HEART				65535
#define  SUB_GR_HEART				65535
//////////////////////////////////////////////////////////////////////////////////
//��¼����

#define MDM_GR_LOGON				1									//��¼��Ϣ
#define MDM_GR_LOGON_BCode			2									//��¼��Ϣ

//��¼ģʽ
#define SUB_GR_LOGON_USERID			1									//I D ��¼
#define SUB_GR_LOGON_MOBILE			2									//�ֻ���¼
#define SUB_GR_LOGON_ACCOUNTS		3									//�ʻ���¼
#define SUB_GR_LOGON_TOKEN			4									//Token��¼
#define SUB_GR_LOGON_TOKEN_LUA		9									//LUA��¼

//��¼���
#define SUB_GR_LOGON_SUCCESS		100									//��¼�ɹ�
#define SUB_GR_LOGON_FAILURE		101									//��¼ʧ��
#define SUB_GR_LOGON_FINISH			102									//��¼���


//////////////////////////////////////////////////////////////////////////////////

//I D ��¼
struct CMD_GR_LogonUserID
{
	//�汾��Ϣ
	DWORD							dwPlazaVersion;						//�㳡�汾
	DWORD							dwFrameVersion;						//��ܰ汾
	DWORD							dwProcessVersion;					//���̰汾

	//��¼��Ϣ
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_MD5];				//��¼����
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
	WORD							wKindID;							//��������
};

//�ֻ���¼
struct CMD_GR_LogonMobile
{	
	DWORD							dwUserID;							//�û� I D
	TCHAR							szPassword[LEN_MD5];				//��¼����
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//Token��¼
struct CMD_GR_LogonTokenID
{
	DWORD							dwMerchantID;						//�̻�ID
	DWORD							dwUserID;							//�û� I D
	TCHAR							szTokenID[LEN_MD5];					//TokenID
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
	TCHAR							szClientIP[16];						//�ͻ���ip
	BYTE                            cbDeviceType;                       //�豸����
};

//�ʺŵ�¼
struct CMD_GR_LogonAccounts
{
	//�汾��Ϣ
	DWORD							dwPlazaVersion;						//�㳡�汾
	DWORD							dwFrameVersion;						//��ܰ汾
	DWORD							dwProcessVersion;					//���̰汾

	//��¼��Ϣ
	TCHAR							szPassword[LEN_MD5];				//��¼����
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//��¼�ɹ�
struct CMD_GR_LogonSuccess
{
	DWORD							dwUserRight;						//�û�Ȩ��
	DWORD							dwMasterRight;						//����Ȩ��
};

struct CMD_GR_LogonUserBlock
{
	WORD							wErrorID;							//�����
	TCHAR							szAddr[32];							//�����ַ
	WORD							wPort;								//��������
	TCHAR							szErrorMsg[128];					//��������
};

//��¼ʧ��
struct CMD_GR_LogonFailure
{
	LONG							lErrorCode;							//�������
	TCHAR							szDescribeString[128];				//������Ϣ
};

//////////////////////////////////////////////////////////////////////////////////
//��������

#define MDM_GR_CONFIG				2									//������Ϣ

#define SUB_GR_CONFIG_COLUMN		100									//�б�����
#define SUB_GR_CONFIG_SERVER		101									//��������
#define SUB_GR_CONFIG_PROPERTY		102									//��������
#define SUB_GR_CONFIG_FINISH		103									//�������
#define SUB_GR_CONFIG_USER_RIGHT	104									//���Ȩ��

//////////////////////////////////////////////////////////////////////////////////

//�б�����
struct CMD_GR_ConfigColumn
{
	BYTE							cbColumnCount;						//�б���Ŀ
	tagColumnItem					ColumnItem[MAX_COLUMN];				//�б�����
};

//��������
struct CMD_GR_ConfigServer
{
	//��������
	WORD							wTableCount;						//������Ŀ
	WORD							wChairCount;						//������Ŀ

	//��������
	WORD							wServerType;						//��������
	DWORD							dwServerRule;						//�������
};

//��������
struct CMD_GR_ConfigServerEx
{
	//��������
	WORD							wTableCount;						//������Ŀ
	WORD							wChairCount;						//������Ŀ

	//��������
	WORD							wServerType;						//��������
	DWORD							dwServerRule;						//�������

	SCORE							lCellScore;							//����ͷ�
	DWORD							dwExpValue;							//ÿ�־���
	DWORD							dwBloodLower;
};

//��������
struct CMD_GR_ConfigProperty
{
	BYTE							cbPropertyCount;					//������Ŀ
	tagPropertyInfo					PropertyInfo[MAX_PROPERTY];			//��������
};

//���Ȩ��
struct CMD_GR_ConfigUserRight
{
	DWORD							dwUserRight;						//���Ȩ��
};
//////////////////////////////////////////////////////////////////////////////////
//�û�����

#define MDM_GR_USER					3									//�û���Ϣ

//�û�����
#define SUB_GR_USER_RULE			1									//�û�����
#define SUB_GR_USER_LOOKON			2									//�Թ�����
#define SUB_GR_USER_SITDOWN			3									//��������
#define SUB_GR_USER_STANDUP			4									//��������
#define SUB_GR_USER_INVITE			5									//�û�����
#define SUB_GR_USER_INVITE_REQ		6									//��������
#define SUB_GR_USER_REPULSE_SIT  	7									//�ܾ��������
#define SUB_GR_USER_KICK_USER       8                                   //�߳��û�
#define SUB_GR_USER_INFO_REQ        9                                   //�����û���Ϣ
#define SUB_GR_USER_CHAIR_REQ       10                                  //�������λ��
#define SUB_GR_USER_CHAIR_INFO_REQ  11                                  //���������û���Ϣ
#define SUB_GR_MB_USER_SITDOWN_REQ  12                                  //�ֻ��û�������Ϣ
#define SUB_GR_MB_USER_SITDOWN      13                                  //�ֻ��û�����������Ϣ

#define SUB_GR_USER_SITDOWN_H5		15									//H5����
#define SUB_GR_USER_STANDUP_H5		16									//H5����
#define SUB_GR_USER_CHANGE_TABLE_H5 17									//H5����

//�û�״̬
#define SUB_GR_USER_ENTER			100									//�û�����
#define SUB_GR_USER_SCORE			101									//�û�����
#define SUB_GR_USER_STATUS			102									//�û�״̬
#define SUB_GR_REQUEST_FAILURE		103									//����ʧ��
#define SUB_GR_USER_LEAVE			104									//�û��뿪

//��������
#define SUB_GR_USER_CHAT			201									//������Ϣ
#define SUB_GR_USER_EXPRESSION		202									//������Ϣ
#define SUB_GR_WISPER_CHAT			203									//˽����Ϣ
#define SUB_GR_WISPER_EXPRESSION	204									//˽�ı���
#define SUB_GR_COLLOQUY_CHAT		205									//�Ự��Ϣ
#define SUB_GR_COLLOQUY_EXPRESSION	206									//�Ự����

//��������
#define SUB_GR_PROPERTY_BUY			300									//�������
#define SUB_GR_PROPERTY_SUCCESS		301									//���߳ɹ�
#define SUB_GR_PROPERTY_FAILURE		302									//����ʧ��
#define SUB_GR_PROPERTY_MESSAGE     303                                 //������Ϣ
#define SUB_GR_PROPERTY_EFFECT      304                                 //����ЧӦ
#define SUB_GR_PROPERTY_TRUMPET		305                                 //������Ϣ


#define SUB_GR_GLAD_MESSAGE			400                                 //ϲ����Ϣ
#define SUB_GR_ROOM_RED				401                                 //������
#define SUB_GR_ROOM_GRAPRED			402									//���������
#define SUB_GR_S_SEND_RED			403									//�����������У�
#define SUB_GR_EXP_RANK				408									//��������
#define SUB_GR_GET_TIME_RANK		409									//��ʱ����

//////////////////////////////////////////////////////////////////////////////////

//�Թ�����
struct CMD_GR_UserLookon
{
	WORD							wTableID;							//����λ��
	WORD							wChairID;							//����λ��
};

//��������
struct CMD_GR_UserSitDown
{
	WORD							wTableID;							//����λ��
	WORD							wChairID;							//����λ��
	TCHAR							szPassword[LEN_PASSWORD];			//��������
};

//��������
struct CMD_GR_UserStandUp
{
	WORD							wTableID;							//����λ��
	WORD							wChairID;							//����λ��
	BYTE							cbForceLeave;						//ǿ���뿪
};

//�����û� 
struct CMD_GR_UserInvite
{
	WORD							wTableID;							//���Ӻ���
	DWORD							dwUserID;							//�û� I D
};

//�����û����� 
struct CMD_GR_UserInviteReq
{
	WORD							wTableID;							//���Ӻ���
	DWORD							dwUserID;							//�û� I D
};

//�û�����
struct CMD_GR_UserScore
{
	DWORD							dwUserID;							//�û���ʶ
	tagUserScore					UserScore;							//������Ϣ
};

//�û�����
struct CMD_GR_MobileUserScore
{
	DWORD							dwUserID;							//�û���ʶ
	tagMobileUserScore				UserScore;							//������Ϣ
};

//�û�״̬
struct CMD_GR_UserStatus
{
	DWORD							dwUserID;							//�û���ʶ
	tagUserStatus					UserStatus;							//�û�״̬
};

//�û�����
struct CMD_GR_UserAddress
{
	DWORD							dwUserID;							//�û���ʶ
	TCHAR							szAddress[50];						//����
};

//����ʧ��
struct CMD_GR_RequestFailure
{
	LONG							lErrorCode;							//�������
	TCHAR							szDescribeString[256];				//������Ϣ
};


//�û�����
struct CMD_GR_C_UserChat
{
	WORD							wChatLength;						//��Ϣ����
	DWORD							dwChatColor;						//��Ϣ��ɫ
	DWORD							dwTargetUserID;						//Ŀ���û�
	TCHAR							szChatString[LEN_USER_CHAT];		//������Ϣ
};

//�û�����
struct CMD_GR_S_UserChat
{
	WORD							wChatLength;						//��Ϣ����
	DWORD							dwChatColor;						//��Ϣ��ɫ
	DWORD							dwSendUserID;						//�����û�
	DWORD							dwTargetUserID;						//Ŀ���û�
	TCHAR							szChatString[LEN_USER_CHAT];		//������Ϣ
};

//�û�����
struct CMD_GR_C_UserExpression
{
	WORD							wItemIndex;							//��������
	DWORD							dwTargetUserID;						//Ŀ���û�
};

//�û�����
struct CMD_GR_S_UserExpression
{
	WORD							wItemIndex;							//��������
	DWORD							dwSendUserID;						//�����û�
	DWORD							dwTargetUserID;						//Ŀ���û�
};

//�û�˽��
struct CMD_GR_C_WisperChat
{
	WORD							wChatLength;						//��Ϣ����
	DWORD							dwChatColor;						//��Ϣ��ɫ
	DWORD							dwTargetUserID;						//Ŀ���û�
	TCHAR							szChatString[LEN_USER_CHAT];		//������Ϣ
};

//�û�˽��
struct CMD_GR_S_WisperChat
{
	WORD							wChatLength;						//��Ϣ����
	DWORD							dwChatColor;						//��Ϣ��ɫ
	DWORD							dwSendUserID;						//�����û�
	DWORD							dwTargetUserID;						//Ŀ���û�
	TCHAR							szChatString[LEN_USER_CHAT];		//������Ϣ
};

//˽�ı���
struct CMD_GR_C_WisperExpression
{
	WORD							wItemIndex;							//��������
	DWORD							dwTargetUserID;						//Ŀ���û�
};

//˽�ı���
struct CMD_GR_S_WisperExpression
{
	WORD							wItemIndex;							//��������
	DWORD							dwSendUserID;						//�����û�
	DWORD							dwTargetUserID;						//Ŀ���û�
};

//�û��Ự
struct CMD_GR_ColloquyChat
{
	WORD							wChatLength;						//��Ϣ����
	DWORD							dwChatColor;						//��Ϣ��ɫ
	DWORD							dwSendUserID;						//�����û�
	DWORD							dwConversationID;					//�Ự��ʶ
	DWORD							dwTargetUserID[16];					//Ŀ���û�
	TCHAR							szChatString[LEN_USER_CHAT];		//������Ϣ
};

//�����û�
struct CMD_GR_C_InviteUser
{
	WORD							wTableID;							//���Ӻ���
	DWORD							dwSendUserID;						//�����û�
};

//�����û�
struct CMD_GR_S_InviteUser
{
	DWORD							dwTargetUserID;						//Ŀ���û�
};

//�������
struct CMD_GR_C_PropertyBuy
{
	BYTE                            cbRequestArea;						//����Χ
	BYTE							cbConsumeScore;						//��������
	WORD							wItemCount;							//������Ŀ
	WORD							wPropertyIndex;						//��������	
	DWORD							dwTargetUserID;						//ʹ�ö���
};

//���߳ɹ�
struct CMD_GR_S_PropertySuccess
{
	BYTE                            cbRequestArea;						//ʹ�û���
	WORD							wItemCount;							//������Ŀ
	WORD							wPropertyIndex;						//��������
	DWORD							dwSourceUserID;						//Ŀ�����
	DWORD							dwTargetUserID;						//ʹ�ö���
};

//����ʧ��
struct CMD_GR_PropertyFailure
{
	WORD                            wRequestArea;                       //��������
	LONG							lErrorCode;							//�������
	TCHAR							szDescribeString[256];				//������Ϣ
};

//������Ϣ
struct CMD_GR_S_PropertyMessage
{
	//������Ϣ
	WORD                            wPropertyIndex;                     //��������
	WORD                            wPropertyCount;                     //������Ŀ
	DWORD                           dwSourceUserID;                     //Ŀ�����
	DWORD                           dwTargerUserID;                     //ʹ�ö���
};


//����ЧӦ
struct CMD_GR_S_PropertyEffect
{
	DWORD                           wUserID;					        //�� ��I D
	BYTE							cbMemberOrder;						//��Ա�ȼ�
};

//��������
struct CMD_GR_C_SendTrumpet
{
	BYTE                           cbRequestArea;                        //����Χ 
	WORD                           wPropertyIndex;                      //�������� 
	DWORD                          TrumpetColor;                        //������ɫ
	TCHAR                          szTrumpetContent[TRUMPET_MAX_CHAR];  //��������
};

//��������
struct CMD_GR_S_SendTrumpet
{
	WORD                           wPropertyIndex;                      //�������� 
	DWORD                          dwSendUserID;                         //�û� I D
	DWORD                          TrumpetColor;                        //������ɫ
	TCHAR                          szSendNickName[32];				    //����ǳ� 
	TCHAR                          szTrumpetContent[TRUMPET_MAX_CHAR];  //��������
};

//ϲ����Ϣ
struct CMD_GR_SendGladMsg 
{
	TCHAR							szContent[TRUMPET_MAX_CHAR];		//����
	TCHAR							szNickName[32];						//�ǳ�
	TCHAR							szNum[128];							//����
	DWORD							colText;							//������ɫ
	DWORD							colName;							//�ǳ���ɫ
	DWORD							colNum;								//������ɫ

};


//�û��ܾ�����������
struct CMD_GR_UserRepulseSit
{
	WORD							wTableID;							//���Ӻ���
	WORD							wChairID;							//����λ��
	DWORD							dwUserID;							//�û� I D
	DWORD							dwRepulseUserID;					//�û� I D
};

//���߳ɹ�
struct CMD_GR_GameItemSuccess
{
	WORD							wKindID;
	WORD							wServerID;
	WORD							wCount;
	DWORD							dwUserID;
	tagGameItemInfo					GameItemInfo[8];
};

//��������
struct CMD_GR_CreateTableNum
{
	WORD							wTableNum;							//����
	LONG							lCellScore;							//���ӵͷ�
	TCHAR							szPassword[LEN_PASSWORD];	//��������
};

//��ȡ����
struct CMD_GR_GetNullTableNum
{
	WORD							wNullTableNum;
	LONG							lCellScoreList[4];
};

struct tagLockTables
{
	BYTE							cbIsLock;		//������ʶ��������
	WORD							wTableNum;		//�������
	LONG							lCellScore;		//�����ͷ�
};

//��ȡ�����б�
struct CMD_GR_GetLockTables
{
	WORD							wTableCount;		//��������
	tagLockTables					LockTables[100];	//һ�����ⶥ
};
//////////////////////////////////////////////////////////////////////////////////

//�����־
#define UR_LIMIT_SAME_IP			0x01								//���Ƶ�ַ
#define UR_LIMIT_WIN_RATE			0x02								//����ʤ��
#define UR_LIMIT_FLEE_RATE			0x04								//��������
#define UR_LIMIT_GAME_SCORE			0x08								//���ƻ���

//�û�����
struct CMD_GR_UserRule
{
	BYTE							cbRuleMask;							//��������
	WORD							wMinWinRate;						//���ʤ��
	WORD							wMaxFleeRate;						//�������
	LONG							lMaxGameScore;						//��߷��� 
	LONG							lMinGameScore;						//��ͷ���
};

//�����û���Ϣ
struct CMD_GR_UserInfoReq
{
	DWORD                           dwUserIDReq;                        //�����û�
	WORD							wTablePos;							//����λ��
};

//�����û���Ϣ
struct CMD_GR_ChairUserInfoReq
{
	WORD							wTableID;							//���Ӻ���
	WORD							wChairID;							//����λ��
};
//////////////////////////////////////////////////////////////////////////////////
//״̬����

#define MDM_GR_STATUS				4									//״̬��Ϣ

#define SUB_GR_TABLE_INFO			100									//������Ϣ
#define SUB_GR_TABLE_STATUS			101									//����״̬

//////////////////////////////////////////////////////////////////////////////////

//������Ϣ
struct CMD_GR_TableInfo
{
	WORD							wTableCount;						//������Ŀ
	tagTableStatus					TableStatusArray[512];				//����״̬
};

//����״̬
struct CMD_GR_TableStatus
{
	WORD							wTableID;							//���Ӻ���
	tagTableStatus					TableStatus;						//����״̬
};

//////////////////////////////////////////////////////////////////////////////////
//��������

#define MDM_GR_INSURE				5									//�û���Ϣ

//��������
#define SUB_GR_QUERY_INSURE_INFO	1									//��ѯ����
#define SUB_GR_SAVE_SCORE_REQUEST	2									//������
#define SUB_GR_TAKE_SCORE_REQUEST	3									//ȡ�����
#define SUB_GR_TRANSFER_SCORE_REQUEST	4								//ȡ�����
#define SUB_GR_QUERY_USER_INFO_REQUEST	5								//��ѯ�û�

#define SUB_GR_USER_INSURE_INFO		100									//��������
#define SUB_GR_USER_INSURE_SUCCESS	101									//���гɹ�
#define SUB_GR_USER_INSURE_FAILURE	102									//����ʧ��
#define SUB_GR_USER_TRANSFER_USER_INFO	103								//�û�����

//////////////////////////////////////////////////////////////////////////////////

//��ѯ����
struct CMD_GR_C_QueryInsureInfoRequest
{
	BYTE                            cbActivityGame;                     //��Ϸ����
};

//�������
struct CMD_GR_C_SaveScoreRequest
{
	BYTE                            cbActivityGame;                     //��Ϸ����
	SCORE							lSaveScore;							//�����Ŀ
};

//ȡ������
struct CMD_GR_C_TakeScoreRequest
{
	BYTE                            cbActivityGame;                     //��Ϸ����
	SCORE							lTakeScore;							//ȡ����Ŀ
	TCHAR							szInsurePass[LEN_PASSWORD];			//��������
};

//ת�˽��
struct CMD_GP_C_TransferScoreRequest
{
	BYTE                            cbActivityGame;                     //��Ϸ����
	BYTE                            cbByNickName;                       //�ǳ�����
	SCORE							lTransferScore;						//ת�˽��
	TCHAR							szNickName[LEN_NICKNAME];			//Ŀ���û�
	TCHAR							szInsurePass[LEN_PASSWORD];			//��������
};

//��ѯ�û�
struct CMD_GR_C_QueryUserInfoRequest
{
	BYTE                            cbActivityGame;                     //��Ϸ����
	BYTE                            cbByNickName;                       //�ǳ�����
	TCHAR							szNickName[LEN_NICKNAME];			//Ŀ���û�
};

//��������
struct CMD_GR_S_UserInsureInfo
{
	BYTE                            cbActivityGame;                     //��Ϸ����
	WORD							wRevenueTake;						//˰�ձ���
	WORD							wRevenueTransfer;					//˰�ձ���
	WORD							wServerID;							//�����ʶ
	SCORE							lUserScore;							//�û����
	SCORE							lUserInsure;						//���н��
	SCORE							lTransferPrerequisite;				//ת������
};

//���гɹ�
struct CMD_GR_S_UserInsureSuccess
{
	BYTE                            cbActivityGame;                     //��Ϸ����
	SCORE							lUserScore;							//���Ͻ��
	SCORE							lUserInsure;						//���н��
	TCHAR							szDescribeString[128];				//������Ϣ
};

//����ʧ��
struct CMD_GR_S_UserInsureFailure
{
	BYTE                            cbActivityGame;                     //��Ϸ����
	LONG							lErrorCode;							//�������
	TCHAR							szDescribeString[128];				//������Ϣ
};

//�û���Ϣ
struct CMD_GR_S_UserTransferUserInfo
{
	BYTE                            cbActivityGame;                     //��Ϸ����
	DWORD							dwTargetGameID;						//Ŀ���û�
	TCHAR							szNickName[LEN_NICKNAME];			//Ŀ���û�
};
//////////////////////////////////////////////////////////////////////////////////
//��������

#define MDM_GR_MANAGE				6									//��������

#define SUB_GR_SEND_WARNING			1									//���;���
#define SUB_GR_SEND_MESSAGE			2									//������Ϣ
#define SUB_GR_LOOK_USER_IP			3									//�鿴��ַ
#define SUB_GR_KILL_USER			4									//�߳��û�
#define SUB_GR_LIMIT_ACCOUNS		5									//�����ʻ�
#define SUB_GR_SET_USER_RIGHT		6									//Ȩ������

//��������
#define SUB_GR_QUERY_OPTION			7									//��ѯ����
#define SUB_GR_OPTION_SERVER		8									//��������
#define SUB_GR_OPTION_CURRENT		9									//��ǰ����

#define SUB_GR_LIMIT_USER_CHAT		10									//��������

#define SUB_GR_KICK_ALL_USER		11									//�߳��û�
#define SUB_GR_DISMISSGAME		    12									//��ɢ��Ϸ
#define SUB_GR_SCORE_MODIFY			13									//�����޸�
#define SUB_GR_SCORE_MODIFY_RESULT	14									//�����޸Ľ��
#define SUB_GR_SCORE_QUERY			15									//��ѯ����
#define SUB_GR_SCORE_QUERY_RESULT	16									//��ѯ���
#define SUB_GR_GM2_LIST_QUERY		17									//��ѯGM2�б�
#define SUB_GR_GM2_LIST_ITEM		18									//gm2�б���
#define SUB_GR_GM2_OP_LIST_QUERY	19									//��ѯGM2�����б�
#define SUB_GR_GM2_OP_LIST_ITEM		20									//����gm2�б���

#define SUB_GR_SCORE_LIMIT_MODIFY			21							//�����޸�
#define SUB_GR_SCORE_LIMIT_MODIFY_RESULT	22							//�����޸Ľ��
#define SUB_GR_SCORE_LIMIT_QUERY			23							//��ѯ����
#define SUB_GR_SCORE_LIMIT_QUERY_RESULT		24							//��ѯ���ֽ��
#define SUB_GR_KILL_USER_LOCKER     13                                  //�忨��  

//////////////////////////////////////////////////////////////////////////////////

//���;���
struct CMD_GR_SendWarning
{
	WORD							wChatLength;						//��Ϣ����
	DWORD							dwTargetUserID;						//Ŀ���û�
	TCHAR							szWarningMessage[LEN_USER_CHAT];	//������Ϣ
};

//ϵͳ��Ϣ
struct CMD_GR_SendMessage
{
	BYTE							cbGame;								//��Ϸ��Ϣ
	BYTE							cbRoom;								//��Ϸ��Ϣ
	BYTE							cbAllRoom;							//��Ϸ��Ϣ
	WORD							wChatLength;						//��Ϣ����
	TCHAR							szSystemMessage[LEN_USER_CHAT];		//ϵͳ��Ϣ
};

//������Ϣ
struct CMD_GR_TipMessage
{
	TCHAR							szTipMessage[1024];		//������Ϣ
};


//�鿴��ַ
struct CMD_GR_LookUserIP
{
	DWORD							dwTargetUserID;						//Ŀ���û�
};

//�߳��û�
struct CMD_GR_KickUser
{
	DWORD							dwTargetUserID;						//Ŀ���û�
};

//�����ʻ�
struct CMD_GR_LimitAccounts
{
	DWORD							dwTargetUserID;						//Ŀ���û�
};

//Ȩ������
struct CMD_GR_SetUserRight
{
	//Ŀ���û�
	DWORD							dwTargetUserID;						//Ŀ���û�

	//�󶨱���
	BYTE							cbGameRight;						//�ʺ�Ȩ��
	BYTE							cbAccountsRight;					//�ʺ�Ȩ��

	//Ȩ�ޱ仯
	BYTE							cbLimitRoomChat;					//��������
	BYTE							cbLimitGameChat;					//��Ϸ����
	BYTE							cbLimitPlayGame;					//��ϷȨ��
	BYTE							cbLimitSendWisper;					//������Ϣ
	BYTE							cbLimitLookonGame;					//�Թ�Ȩ��
};

//��������
struct CMD_GR_OptionCurrent
{
	DWORD							dwRuleMask;							//��������
	tagServerOptionInfo				ServerOptionInfo;					//��������
};

//��������
struct CMD_GR_ServerOption
{
	tagServerOptionInfo				ServerOptionInfo;					//��������
};

//�߳������û�
struct CMD_GR_KickAllUser
{
	TCHAR							szKickMessage[LEN_USER_CHAT];		//�߳���ʾ
};

//��ɢ��Ϸ
struct CMD_GR_DismissGame
{
	WORD							wDismissTableNum;		            //��ɢ����
};

//�޸Ļ���
struct CMD_GR_ModifyScore
{
	TCHAR							szTargetAccount[32];				//Ŀ���˺�
	LONGLONG						lScore;								//�����Ƕ�
	LONGLONG						lMedal;								//����Ԫ��(�ɽ���)
	LONGLONG						lGameScore;							//��Ϸ�Ƕ�
	LONGLONG						lGameMedal;							//��ϷԪ��
	LONG							lLoveLiness;						//����(������)
	LONG							lUserPower;							//����
	LONG							lScoreLimit;						//��������
	TCHAR							szReason[32];						//�޸�ԭ��
};

//�����޸Ľ��
struct CMD_GR_ModifyScoreResult
{
	BYTE							cbResult;							//0:�ɹ�
	TCHAR							szResult[128];						//�����Ϣ
};

//��ѯ����
struct CMD_GR_CheckMedal
{
	WORD							wTable;								//���Ӻ�
	DWORD							dwUserID;							//�û�ID

};

//��ѯ����
struct CMD_GR_QueryScore
{
	TCHAR							szTargetAccount[32];				//Ŀ���˺�
};

//�����޸Ľ��
struct CMD_GR_QueryScoreResult
{
	BYTE							cbResult;							//��ѯ���,0:�ɹ�
	TCHAR							szTargetAccount[32];				//Ŀ���˺�
	LONGLONG						lScore;								//�����Ƕ�
	LONGLONG						lMedal;								//����Ԫ��(�ɽ���)
	LONGLONG						lGameScore;							//��Ϸ�Ƕ�
	LONGLONG						lGameMedal;							//��ϷԪ��
	LONG							lLoveLiness;						//����(������)
	LONG							lUserPower;							//�û�����
	LONG							lScoreLimit;						//��������
};

//��ѯgm2�б���
struct CMD_GR_Gm2ListItem
{
	TCHAR							szTargetAccount[32];				//Ŀ���˺�
	LONGLONG						lMedalLimit;						//����
	LONGLONG						lMedal;								//�ۼ�
};

//��ѯgm2�����б���
struct CMD_GR_Gm2OpListItem
{
	TCHAR							szTargetAccount[32];				//Ŀ���˺�
	LONGLONG						lMedal;								//�ۼ�
};

//�޸Ļ���
struct CMD_GR_ModifyScoreLimit
{
	TCHAR							szTargetAccount[32];				//Ŀ���˺�
	LONGLONG						lMedal;								//����Ԫ��(�ɽ���)
};

//�����޸Ľ��
struct CMD_GR_ModifyScoreLimitResult
{
	BYTE							cbResult;							//0:�ɹ�
	TCHAR							szResult[128];						//�����Ϣ
};


//��ѯ����
struct CMD_GR_QueryScoreLimit
{
};

//�����޸Ľ��
struct CMD_GR_QueryScoreLimitResult
{
	BYTE							cbResult;							//��ѯ���,0:�ɹ�
	LONGLONG						lMedalLimit;						//����Ԫ��
	LONGLONG						lMedal;								//�ѿ�Ԫ��
};
//////////////////////////////////////////////////////////////////////////////////

//���ñ�־
#define OSF_ROOM_CHAT				1									//��������
#define OSF_GAME_CHAT				2									//��Ϸ����
#define OSF_ROOM_WISPER				3									//����˽��
#define OSF_ENTER_TABLE				4									//������Ϸ
#define OSF_ENTER_SERVER			5									//���뷿��
#define OSF_SEND_BUGLE				12									//��������

//��������
struct CMD_GR_OptionServer
{
	BYTE							cbOptionFlags;						//���ñ�־
	BYTE							cbOptionValue;						//���ñ�־
};

//��������
struct CMD_GR_LimitUserChat
{
	DWORD							dwTargetUserID;						//Ŀ���û�
	BYTE							cbLimitFlags;						//���Ʊ�־
	BYTE							cbLimitValue;						//�������
};
//////////////////////////////////////////////////////////////////////////////////
//��������

#define MDM_GR_MATCH				7									//��������

#define SUB_GR_MATCH_FEE			400									//��������
#define SUB_GR_MATCH_NUM			401									//�ȴ�����
#define SUB_GR_LEAVE_MATCH			402									//�˳�����
#define SUB_GR_MATCH_INFO			403									//������Ϣ
#define SUB_GR_MATCH_WAIT_TIP		404									//�ȴ���ʾ
#define SUB_GR_MATCH_RESULT			405									//�������
#define SUB_GR_MATCH_STATUS			406									//����״̬
#define SUB_GR_MATCH_USER_COUNT		407									//��������
#define SUB_GR_MATCH_DESC			408									//��������

//����˷���
#define SUB_GR_MATCH_REWARD			409									//��������
#define SUB_GR_MATCH_TYPE			411									//��������
#define SUB_GR_MATCH_JOIN			412									//��������
#define SUB_GR_MATCH_REVIVE			413									//�����
#define SUB_GR_MATCH_RULE			414									//��������
#define SUB_GR_MATCH_USER			415									//����ѡ��
#define SUB_GR_MATCH_WARN			416									//������ʾ

//��������
struct CMD_GR_Match_Num
{
	DWORD							dwWaitting;							//�ȴ�����
	DWORD							dwTotal;							//��������
	DWORD							dwMatchTotal;						//������
};

//������Ϣ
struct CMD_GR_Match_Info
{
	TCHAR							szTitle[4][64];						//��Ϣ����
    WORD							wGameCount;							//��Ϸ����
};

//��ʾ��Ϣ
struct CMD_GR_Match_Warn
{
	SCORE							lScore;								//��ǰ����
	WORD							wRank;								//��ǰ����
	WORD							wCurTableRank;						//��������
	WORD							wUserCount;							//��ǰ����
	WORD							wPlayingTable;						//��Ϸ����
};

//
struct CMD_GR_MatchInfoNew
{
	BYTE							cbMatchMold;						//��ʼ��ʽ��Ԥ����ʼ��������ʼ
	BYTE							cbMatchType;						//�������ͣ���������ѭ��������ʱ��
	BYTE							cbMatchRule;						//�������򣬴������֣�ĩλ��̭
	BYTE							cbRoundCount;						//��������
	BYTE							cbGameCount;						//һ�־���
	BYTE							cbCurRound;							//��ǰ����
	BYTE							cbCurCount;							//���־���
	BYTE							cbMatchStatus;						//�����׶�
	WORD							wCurBase;							//��ǰ����
	WORD							wOutScore;							//���ֵͷ�
	WORD							wCurRank;							//��ǰ����
	WORD							wMatchUser;							//��������
	WORD							wPromotionCount;					//��������
	WORD							wRemainCount;						//��ֹ����
	TCHAR							szMatchStatus[64];					//�����׶�
	TCHAR							szMatchTitle[64];					//��������
};

struct CMD_GR_MatchCurProc
{
	BYTE							cbCurStage;							//��ǰ�׶�
	BYTE							cbCurRule;							//��ǰ����
	BYTE							cbRoundCount;						//һ������
	BYTE							cbGameCount;						//һ�־���
	BYTE							cbCurRound;							//��ǰ����
	BYTE							cbCurCount;							//���־���
	DWORD							dwCurBase;							//��ǰ����
	DWORD							dwOutScore;							//���ֵͷ�
	DWORD							dwCurRank;							//��ǰ����
	DWORD							dwMatchUser;						//��������
	DWORD							dwPromotionCount;					//��������
	DWORD							dwRemainCount;						//��ֹ����
};


//�����������
struct CMD_GR_MatchResult
{
	DWORD							dwRank;								//��������
};

//��������
struct CMD_GR_MatchDesc
{
};


//����״̬
struct CMD_GR_MatchStatus
{
	BYTE							cbMatchStatus;						//����״̬
	DWORD							dwUserID;							//�û� ID
};

//////////////////////////////////////////////////////////////////////////////////
//�������

#define MDM_GF_FRAME				100									//�������

//////////////////////////////////////////////////////////////////////////////////
//�������

//�û�����
#define SUB_GF_GAME_OPTION			1									//��Ϸ����
#define SUB_GF_USER_READY			2									//�û�׼��
#define SUB_GF_LOOKON_CONFIG		3									//�Թ�����

#define SUB_GF_GAME_OPTION_H5			21									//��Ϸ����
#define SUB_GF_USER_READY_H5			22									//�û�׼��
#define SUB_GF_LOOKON_CONFIG_H5			23									//�Թ�����

//��������
#define SUB_GF_USER_CHAT			10									//�û�����
#define SUB_GF_USER_EXPRESSION		11									//�û�����

//��Ϸ��Ϣ
#define SUB_GF_GAME_STATUS			100									//��Ϸ״̬
#define SUB_GF_GAME_SCENE			101									//��Ϸ����
#define SUB_GF_LOOKON_STATUS		102									//�Թ�״̬

//ϵͳ��Ϣ
#define SUB_GF_SYSTEM_MESSAGE		200									//ϵͳ��Ϣ
#define SUB_GF_ACTION_MESSAGE		201									//������Ϣ
#define SUB_GF_OPERATE_SUCCESS		202									//�����ɹ�
#define SUB_GF_OPERATE_FAILURE		203									//����ʧ��
#define SUB_GF_SCROLL_MESSAGE		204									//������Ϣ
#define SUB_GF_REDIS_SCROLL_MES		205									// Redis������Ϣ

#define SUB_GR_S_CARDBOX			301									//�Ծֱ���
#define SUB_GR_S_TIMERANK			311									//��ʱ����
#define SUB_GR_S_RANK_LIST			312									//��ʱ����

#define SUB_GR_GET_BLOOD			313									//��Ѫ����
#define SUB_GR_SUPPLY_BLOOD			314									//��Ѫ
#define SUB_GR_AWARD_CARDBOX		315									//��ȡ���佱��

#define SUB_GP_FINISHED_FIRST_GAME  320									//��ɵ�һ����Ϸ
//////////////////////////////////////////////////////////////////////////////////

//��Ϸ����
struct CMD_GF_GameOption
{
	BYTE							cbAllowLookon;						//�Թ۱�־
	DWORD							dwFrameVersion;						//��ܰ汾
	DWORD							dwClientVersion;					//��Ϸ�汾
};

//�Թ�����
struct CMD_GF_LookonConfig
{
	DWORD							dwUserID;							//�û���ʶ
	BYTE							cbAllowLookon;						//�����Թ�
};

//�Թ�״̬
struct CMD_GF_LookonStatus
{
	BYTE							cbAllowLookon;						//�����Թ�
};

//��Ϸ����
struct CMD_GF_GameStatus
{
	BYTE							cbGameStatus;						//��Ϸ״̬
	BYTE							cbAllowLookon;						//�Թ۱�־
};

//�û�����
struct CMD_GF_C_UserChat
{
	WORD							wChatLength;						//��Ϣ����
	DWORD							dwChatColor;						//��Ϣ��ɫ
	DWORD							dwTargetUserID;						//Ŀ���û�
	TCHAR							szChatString[LEN_USER_CHAT];		//������Ϣ
};

//�û�����
struct CMD_GF_S_UserChat
{
	WORD							wChatLength;						//��Ϣ����
	DWORD							dwChatColor;						//��Ϣ��ɫ
	DWORD							dwSendUserID;						//�����û�
	DWORD							dwTargetUserID;						//Ŀ���û�
	TCHAR							szChatString[LEN_USER_CHAT];		//������Ϣ
};

//�û�����
struct CMD_GF_C_UserExpression
{
	WORD							wItemIndex;							//��������
	DWORD							dwTargetUserID;						//Ŀ���û�
};

//�û�����
struct CMD_GF_S_UserExpression
{
	WORD							wItemIndex;							//��������
	DWORD							dwSendUserID;						//�����û�
	DWORD							dwTargetUserID;						//Ŀ���û�
};

//����
struct CMD_MB_S_CARDBOX
{
	WORD		wCurCount;			//��ǰ����
	WORD		wCardCount;			//�������
	BYTE		cbAwardCount;		//ʣ���콱
	BYTE		cbIsComplete;		//�Ƿ����
};

//��ʱ����
struct CMD_MB_S_TimeAward
{
	DWORD		dwCurRank;			//��ǰ����
	DWORD		dwRankScore;		//��������	
	DWORD		dwTimeLeft;			//�佱ʣ��ʱ�� ��λ��
};
//////////////////////////////////////////////////////////////////////////////////
//��Ϸ����

#define MDM_GF_GAME					200									//��Ϸ����

//////////////////////////////////////////////////////////////////////////////////
//Я����Ϣ

//������Ϣ
#define DTP_GR_TABLE_PASSWORD		1									//��������

//�û�����
#define DTP_GR_NICK_NAME			10									//�û��ǳ�
#define DTP_GR_GROUP_NAME			11									//��������
#define DTP_GR_UNDER_WRITE			12									//����ǩ��
#define DTP_GR_ADDRESS				13									//��ҵ�ַ

//������Ϣ
#define DTP_GR_USER_NOTE			20									//�û���ע
#define DTP_GR_CUSTOM_FACE			21									//�Զ�ͷ��

//////////////////////////////////////////////////////////////////////////////////

//�������
#define REQUEST_FAILURE_NORMAL		0									//����ԭ��
#define REQUEST_FAILURE_NOGOLD		1									//��Ҳ���
#define REQUEST_FAILURE_NOSCORE		2									//���ֲ���
#define REQUEST_FAILURE_PASSWORD	3									//�������
#define REQUEST_FAILURE_NETWORK		4									//����ԭ��
#define REQUEST_FAILURE_QUIT		5									//ʧ���˳�

//////////////////////////////////////////////////////////////////////////////////
//�Զ����¼�
#define SE_KICK_ALL_USER_REQUEST		1								//�߳��������
#define SE_HIGH_SCORE_LIST_REQUEST		2								//��������б�
#define SE_HIGH_SCORE_REQUEST			3								//�������÷���
#define SE_HIGH_SCORE_GET_REQUEST		4								//�����ȡ����

//////////////////////////////////////////////////////////////////////////////////

#pragma pack()

#endif