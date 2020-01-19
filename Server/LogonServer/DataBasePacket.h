#ifndef DATA_BASE_PACKET_HEAD_FILE
#define DATA_BASE_PACKET_HEAD_FILE

#pragma pack(1)

//////////////////////////////////////////////////////////////////////////////////
//�������ݰ�

//��¼����
#define	DBR_GP_LOGON_GAMEID			1									//I D ��¼
#define	DBR_GP_LOGON_ACCOUNTS		2									//�ʺŵ�¼
#define DBR_GP_REGISTER_ACCOUNTS	3									//ע���ʺ�
#define DBR_GP_COLLECTION			4									//�ղ��б�
#define DBR_GP_LOGON_ThirdParty		5									//������¼
#define DBR_GP_REGISTER_THIRD		6									//����ע��
#define DBR_GP_LOGON_Third			7									//������¼
#define DBR_GP_LOGON_OUT			8									//�ʺŵǳ�
#define DBR_GP_WEB_REGISTER_ACCOUNTS	9								//Webע��

//�˺ŷ���
#define DBR_GP_MODIFY_MACHINE		10									//�޸Ļ���
#define DBR_GP_MODIFY_LOGON_PASS	11									//�޸ĵ�¼����
#define DBR_GP_MODIFY_INSURE_PASS	12									//�޸�����
#define DBR_GP_MODIFY_UNDER_WRITE	13									//�޸�ǩ��
#define DBR_GP_MODIFY_NICKNAME		14									//�޸��ǳ�
#define DBR_GP_MODIFY_INDIVIDUAL	15									//�޸�����
#define DBR_GP_BIND_VISITOR_ACCOUNT 16									//�ο�ת��
#define DBR_GP_PW_RECOVERY_ID		17									//���֤ID�һ�����
#define DBR_GP_PW_RECOVERY_MOBILE	18									//�ֻ��һ�����
#define DBR_GP_PW_RECOVERY			19									//�һ�����
#define DBR_GP_BIND_MOBILE			20									//���ֻ�
#define DBR_GP_GET_HONOR			21									//����ƺ�
#define DBR_GP_SET_HONOR			22									//���óƺ�

//ͷ������
#define DBR_GP_GET_AVATAR_INFO		23									//��ȡͷ��
#define DBR_GP_GET_IMAGE_INFO		24									//��ȡ����
#define DBR_GP_MODIFY_AVATAR		25									//�޸�ͷ��
#define DBR_GP_MODIFY_IMAGE			26									//�޸�����

//��������
#define DBR_GP_USER_SAVE_SCORE		30									//����Ԫ��
#define DBR_GP_USER_TAKE_SCORE		31									//��ȡԪ��
#define DBR_GP_USER_TRANSFER_SCORE	32									//ת��Ԫ��
#define DBR_GP_USER_CHANGE_SCORE	33									//�ı�Ԫ��

//��ѯ����
#define DBR_GP_QUERY_INDIVIDUAL		40									//��ѯ����
#define DBR_GP_QUERY_INSURE_INFO	41									//��ѯ����
#define DBR_GP_QUERY_USER_INFO	    42									//��ѯ�û�

//ϵͳ����
#define DBR_GP_LOAD_GAME_LIST		50									//�����б�
#define DBR_GP_ONLINE_COUNT_INFO	51									//������Ϣ
#define DBR_GP_BROADCASE_MSG		52									//�㲥��Ϣ
#define	DBR_GP_BROADCAST_TYPHON		53									//������Ϣ
#define DBR_GP_GET_RANK				54									//���а�����
#define DBR_GP_CONTROL_ROOM_STATUS	55									//�򿪷���

#define DBR_GP_TODAY_IS_SIGNIN      59								     //�����Ƿ��Ѿ�ǩ��
#define DBR_GP_GET_SIGNIN			60									//��ȡǩ��
#define DBR_GP_SET_SIGNIN			61									//ǩ��
#define DBR_GP_USER_DAILYGET		62									//�ͱ���ȡ
#define DBR_GP_REQUEST_MAIL			63									//�����ʼ�
#define DBR_GP_READ_MAIL			64									//��ȡ�ʼ�
#define DBR_GP_DELETE_MAIL			65									//ɾ���ʼ�

// ת��
#define DBR_GP_GETDAILINFO			66									// ��ȡת����Ϣ
#define DBR_GP_ROTATEDAILAWARD		67									// ת��ת�̻�ý���


#define DBR_GP_GET_STORE			70									//��ȡ�̳ǵ���
#define DBR_GP_GET_AUCTION			71									//�����̳�
#define	DBR_GP_BUY_STORE_ITEM		72									//�����̳ǵ���
#define DBR_GP_BUY_AUCTION_ITEM		73									//������������
#define	DBR_GP_ONSALE_AUCTION_ITEM	74									//�ϼ�/�¼���������
#define DBR_GP_USE_ITEM				75									//ʹ�õ���
#define DBR_GP_GET_PACK				76									//������ȡ
#define DBR_GP_GET_STOREINFO		77									//��ȡ�̳ǵ�����Ϣ����ʵʱ��棩
#define DBR_GP_GET_ITEMINFO			78									//��ȡ������Ϣ
#define DBR_GP_GET_SELF_AUCTION		79									//�����̳�
#define DBR_GP_OFFSALE_AUCTION_ITEM 80
#define DBR_GP_EXTRACT_SCORE		81
#define DBR_GP_RESHELF_AUCTION_ITEM 82									//�����ϼ�
#define DBR_GP_GRANT_ITEM			83									//�������

//���ѷ���
#define DBR_GP_REQUEST_FRIEND		84									//�������
#define DBR_GP_REQUEST_REPLY		85									//�������루ͬ��/�ܾ���
#define DBR_GP_REQUEST_REPLYS		86									//�������루ȫ��ͬ��/�ܾ���
#define DBR_GP_DELETE_FRIEND		87									//ɾ������
#define DBR_GP_GET_FRIEND			88									//��ȡ����
#define DBR_GP_GET_REQUEST			89									//��ȡ����
#define DBR_GP_SEARCH_USER			90									//�����û�

#define DBR_GP_GET_FAMILY			91									//��ȡ����
#define DBR_GP_SET_FAMILY			92									//�������
#define DBR_GP_QUIT_FAMILY			93									//�˳�����
#define DBR_GP_SEND_RED				94									//���������
#define DBR_GP_GARP_RED				95									//�����
#define DBR_GP_BUY_POWER			96									//ˢ������
#define DBR_GP_GET_EXP				97									//��ȡ����
#define DBR_GP_GET_SYSEXP			98									//��ȡ����ϵͳ
#define DBR_GP_GET_TASK				99									//��ȡ����
#define DBR_GP_REFRESH_TASK			100									//ˢ������
#define DBR_GP_TAKE_TASK			101									//��ȡ������
#define DBR_GP_GET_USERACHIEVE		102									//��ȡ�ɾ�����
#define DBR_GP_GET_USERWAGES		103									//��ȡ�ɾ͹���
#define DBR_GP_INSERT_USERTASK		104									//�����������
#define DBR_GP_INSERT_USERACHIEVE	105									//������ҳɾ�
#define DBR_GP_INSERT_USERFAME		106									//�����������
#define DBR_GP_GRANT_GOODS			107									//������������
#define DBR_GP_ALTER_REMARK			108									//�޸ĺ��ѱ�ע
#define DBR_GP_MATCH_DESCRIPTION	109									//��ȡ��������
#define DBR_GP_MATCH_RULE			110									//��ȡ��������
#define DBR_GP_MATCH_REWARD			111									//��ȡ��������
#define DBR_GP_RECOMMENDFRIEND		112									//��ȡ�Ƽ�����
#define DBR_GP_GET_USERFAME			113									//��ȡ��������

#define DBR_GP_BUY_VIP				114									//����VIP
#define DBR_GP_MODIFY_SECOND_PASS	115									//���ö�������
#define DBR_GP_GET_RED_INFO			116									//��ȡ�����������
#define DBR_GP_SET_LEAVE_MESSAGE    117									//�������а�����
#define DBR_GP_CHECK_SECOND_PASS	118									//

//�۱���
#define DBR_GP_GET_USER_GEM			119									//��ȡ��ʯ����
#define DBR_GP_GET_USER_ORNAMENT    120									//��ȡ��Ʒ����
#define DBR_GP_BURNISH_GEM				121								//��ĥ��ʯ
#define DBR_GP_CARRY_GEM				122								//Я����ʯ
#define DBR_GP_INPUT_GEM				123								//��ֲ��ʯ
#define	DBR_GP_WEAR_ORNAMENT			124								//������Ʒ
#define DBR_GP_GET_GEM_FRIENDS			125								//��ȡ��ʯ����

#define DBR_GP_RETRIEVE_PASSWORD        126								//�һ����루������֤�˺ţ���֤�ֻ���

#define DBR_GP_GET_PERSONAL_INFO		127								//��ȡ������Ϣ����Ϸ����Ϣ���ǳƣ��ȼ���vip��ǩ�����ƺţ����ܣ������ȼ�����ͨ���ȼ������ֶ��ȼ�....��

#define DBR_GP_RECOMMENDFRIEND2			130									//��ȡ�Ƽ�����
#define DBR_GP_FEEDBACK_SYSTEM			131								//������Ϣϵͳ
#define DBR_GP_PUSH_RED					132								//��ȡ���ڵĺ��
#define DBR_GP_GET_DELETE_RED			133								//ɾ�����

#define DBR_GP_RELOAD_SCORE				135								//czgˢ�»���

//���������Ϣ
#define DBR_GP_GET_TIME_RANK_INFO		136								//���䶨ʱ����������Ϣ
#define DBR_GP_GET_ROOM_SUMMARIZE		137								//��ȡ�������
#define DBR_GP_GET_USER_RESULTS			138								//��ȡ�û���ս��

#define DBR_GP_GET_GAME_RECORDS         139								//��ȡ�û���Ϸ��¼
#define DBR_GP_GET_MATCH_APPLY_STATUS   140								//��ȡ��������״̬

#define DBR_GP_INPUT_INVITE_CODE		141								//���������루���ձ������룩
#define DBR_GP_GET_TODATY_SENDRED       142								//��ȡ���췢�ĺ��
#define DBR_GP_GET_RECEIVE_RED_INFO		143								//��ȡ���������

#define DBR_GP_GET_INVITE_CODE_REWARD_INFO		144							//���������뽱������
#define DBR_GP_OBTAIN_INVITE_CODE_REWARD		145							//��ȡ�����뽱��
#define DBR_GP_GET_FEEDBACK_REWARD_STATUE		146							//��ȡĳһ�����������������״̬
#define DBR_GP_GET_USER_INVITE_CODE_INFO		147							//��ȡ�û���������ϸ��Ϣ

#define DBR_GP_LOAD_SPREAD_INFO 148											//��ȡ�ƹ㽱������
#define DBR_GP_DISTANCE_REWARD_TIME 149										//�����´ν���ʱ��
#define DBR_GP_CALCULATION_SPREAD_REWARD 150								//�����ƹ㽱��

#define DBR_GP_CACHE_RANK 151												//�������а�

#define DBR_GP_GET_USER_PERFECT_INFORMATION   152							//��ȡ�û�������Ϣ
#define DBR_GP_SET_USER_PERFECT_INFORMATION   153							//�����û�������Ϣ

#define DBR_GP_IS_SPREAD_REGISTER	  154										//�ƹ�ע��
#define DBR_GP_BIND_USER_SPREAD		155											//���û��ƹ�
#define DBR_GP_IS_BIND_SPREAD		156											//�Ƿ��Ѿ����ƹ�
#define DBR_GP_IS_ALREADY_MODIFY_NICKNAME 157								//�Ƿ��Ѿ��޸Ĺ��ǳ�
#define DBR_GP_IS_TODAY_FIRST_TIME_LOGIN 158								//�Ƿ�����һ�ε�¼
#define DBR_GP_CLIENT_LOGIN_SUCCESS 159								//�ͻ��˵�¼���

#define DBR_GP_KILL_LOCK	160										//ɾ������
#define DBR_GR_LOAD_SCROLL_MESSAGE		161							//��ȡ������Ϣ
#define DBR_GR_CLEAR_SCROLL_MESSAGE     162							//���������¼


#define DBR_GR_CHICKBINDBANK			165							// ��ѯ�����п�
#define DBR_GR_BINDBANK					166							// �����п�
#define DBR_GR_CHICKBINDZFB				167							// ��ѯ��֧����
#define DBR_GR_BINDZFB					168							// ��֧����

#define DBR_GR_REQUEST_MAIL				170									//�����ʼ�
#define DBR_GR_READ_DELETE_MAIL			171									//��ȡɾ���ʼ�

//////////////////////////////////////////////////////////////////////////////////

//�ͻ��������Ϣ
struct DBR_GP_ClientInfo
{
	unsigned long                       dwKindID;                   //���ͱ�ʶ
	unsigned long						dwVersion;					//�汾
};

//ID ��¼
struct DBR_GP_LogonGameID
{
	//��¼��Ϣ
	DWORD							dwGameID;							//�û� I D
	TCHAR							szPassword[LEN_MD5];				//��¼����
	BYTE							cbNeeValidateMBCard;				//�ܱ�У��

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������

	//������Ϣ
	LPVOID							pBindParameter;						//�󶨲���
};

//�ʺŵ�¼
struct DBR_GP_LogonAccounts
{
	//��¼��Ϣ
	TCHAR							szPassword[LEN_MD5];				//��¼����
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	BYTE							cbNeeValidateMBCard;				//�ܱ�У��

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������

	//������Ϣ
	LPVOID							pBindParameter;						//�󶨲���
};

//�ʺŵ�¼
struct DBR_GP_LogonThird
{
	//��¼��Ϣ
	DWORD							dwUserID;							//�û�ID
	BYTE							cbNeeValidateMBCard;				//�ܱ�У��

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������

	//������Ϣ
	LPVOID							pBindParameter;						//�󶨲���
};

//�˺�ע��
struct DBR_GP_LogonOut
{
	DWORD							dwUserID;							//�û�ID
	DWORD							dwInOutIndex;						//����
};

struct DBR_GP_LogonThirdParty
{
	//��¼��Ϣ
	TCHAR							szPassword[LEN_MD5];				//��¼����
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
};

//Web �ʺ�ע��
struct DBR_GP_Web_RegisterAccounts
{
	//ע����Ϣ
	BYTE							cbGender;							//�û��Ա�
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	TCHAR							szNickName[LEN_ACCOUNTS];			//�û��ǳ�
	
	TCHAR							szLogonPass[LEN_MD5];				//��¼����
	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	//������Ϣ
	LPVOID							pBindParameter;						//�󶨲���
};

//�ʺ�ע��
struct DBR_GP_RegisterAccounts
{
	//ע����Ϣ
	WORD							wFaceID;							//ͷ���ʶ
	BYTE							cbGender;							//�û��Ա�
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	TCHAR							szNickName[LEN_ACCOUNTS];			//�û��ǳ�
	TCHAR							szSpreader[LEN_ACCOUNTS];			//�Ƽ��ʺ�
	TCHAR							szPassPortID[LEN_PASS_PORT_ID];		//֤������
	TCHAR							szCompellation[LEN_COMPELLATION];	//��ʵ����

	//�������
	TCHAR							szLogonPass[LEN_MD5];				//��¼����
	TCHAR							szInsurePass[LEN_MD5];				//��������

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������

	//������Ϣ
	LPVOID							pBindParameter;						//�󶨲���
};

//�������ʺ�ע��
struct DBR_GP_RegisterThird
{
	//ע����Ϣ
	WORD							wFaceID;							//ͷ���ʶ
	BYTE							cbGender;							//�û��Ա�
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	TCHAR							szNickName[LEN_ACCOUNTS];			//�û��ǳ�
	TCHAR							szSpreader[LEN_ACCOUNTS];			//�Ƽ��ʺ�
	TCHAR							szPassPortID[LEN_PASS_PORT_ID];		//֤������
	TCHAR							szCompellation[LEN_COMPELLATION];	//��ʵ����

	//�������
	TCHAR							szLogonPass[LEN_MD5];				//��¼����
	TCHAR							szInsurePass[LEN_MD5];				//��������

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������

	//������Ϣ
	LPVOID							pBindParameter;						//�󶨲���

	//�������˺�
	TCHAR							szThirdParty[LEN_ACCOUNTS];			//��������¼�ʺ�
};

//��¼���������ݱ���Ϣ
struct DBR_GP_Insert_Accounts
{
	TCHAR							szThirdAccounts[LEN_ACCOUNTS];			//�����ʺ�
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	TCHAR							szLogonPass[LEN_MD5];				//��¼����
};

//�޸Ļ���
struct DBR_GP_ModifyMachine
{
	BYTE							cbBind;								//�󶨱�־
	DWORD							dwUserID;							//�û���ʶ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szPassword[LEN_PASSWORD];			//�û�����
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//�޸�����
struct DBR_GP_ModifyLogonPass
{
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szAccounts[LEN_ACCOUNTS];			//�û��˺�
	TCHAR							szScrPassword[LEN_PASSWORD];		//�û�����
	TCHAR							szDesPassword[LEN_PASSWORD];		//�û�����	
};

//�޸�����
struct DBR_GP_ModifyInsurePass
{
	DWORD							dwUserID;							//�û� I D
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szDesPassword[LEN_PASSWORD];		//�û�����
	TCHAR							szScrPassword[LEN_PASSWORD];		//�û�����
};

//�ֻ��һ�����
struct DBR_GP_PWRecoveryMobile
{
	TCHAR							szAccounts[LEN_ACCOUNTS];			// �û��˺�
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	// Ԥ���ֻ���
};

//���ֻ�
struct DBR_GP_BindMobile
{
	DWORD							dwUserID;							// �û�ID
	TCHAR							szMobliePhone[LEN_MOBILE_PHONE];	// ���ֻ���
	TCHAR							szPassword[LEN_PASSWORD];			//�û�����
};

//�޸�����
struct DBR_GP_ModifyIndividual
{
	//��֤����
	DWORD							dwUserID;							//�û� I D
	TCHAR							szCompellation[LEN_COMPELLATION];	//��ʵ����
	TCHAR							szPassportID[LEN_PASS_PORT_ID];		//���֤ I D
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//Ԥ���ֻ���
	TCHAR							szPassword[LEN_MD5];				//����
};

//����Ԫ��
struct DBR_GP_UserSaveScore
{
	DWORD							dwUserID;							//�û� I D
	SCORE							lSaveScore;							//����Ԫ��
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//ȡ��Ԫ��
struct DBR_GP_UserTakeScore
{
	DWORD							dwUserID;							//�û� I D
	SCORE							lTakeScore;							//��ȡԪ��
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szPassword[LEN_PASSWORD];			//��������
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//ת��Ԫ��
struct DBR_GP_UserTransferScore
{
	DWORD							dwUserID;							//�û� I D
	DWORD							dwClientAddr;						//���ӵ�ַ
	BYTE                            cbByNickName;                       //�ǳ�����
	SCORE							lTransferScore;						//ת��Ԫ��
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�
	TCHAR							szPassword[LEN_PASSWORD];			//��������
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//��ѯ����
struct DBR_GP_QueryInsureInfo
{
	DWORD							dwUserID;							//�û� I D
	DWORD							dwClientAddr;						//���ӵ�ַ
};

//�ı�Ԫ��
struct DBR_GP_CHANGE_SCORE
{
	DWORD							dwUserID;							//�û� I D
	SCORE							changeScore;						//�û����
	SCORE							changeInsureScore;					//���н��
};

//�û�����
struct DBO_GP_UserTransferUserInfo
{
	DWORD							dwGameID;							//�û� I D
	TCHAR							szNickName[LEN_NICKNAME];			//�ʺ��ǳ�
};

//�ƺ���Ϣ
struct DBO_GP_HonorInfo
{
	DWORD							dwHonorID;							//�ƺ�ID
	DWORD							dwHonorType;						//�ƺ�����
};

//ͷ��/������Ϣ
struct DBO_GP_AvatarInfo
{
	BYTE							cbAvatarType;						//ͷ��/����
	DWORD							dwAvatarID;							//ID
	DWORD							dwRemainTime;						//ʣ��ʱ�䣨��С��λ���֣�
};

//������Ϣ
struct DBR_GP_OnLineCountInfo
{
	WORD							wKindCount;							//������Ŀ
	DWORD							dwOnLineCountSum;					//��������
	tagOnLineInfoKind				OnLineCountKind[MAX_KIND];			//��������
};

//�㲥��Ϣ
struct DBR_GP_OnBroadcastMsg
{
	TCHAR							szNickName[32];
	TCHAR							szKindName[32];
	TCHAR							szScore[32];
};

//�ο�ת��
struct DBR_GP_BindVisitorAccount
{
	DWORD							dwUserID;
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�
	TCHAR							szMachineID[LEN_MACHINE_ID];			//����ID
	TCHAR							szPassword[LEN_PASSWORD];			//�û�����
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�ֻ���
};

// ��ȡת��
struct CMD_GP_GetDailInfo
{
	DWORD							dwUserID;
	DWORD							dwClientAddr;						//���ӵ�ַ
};

// ת��ת����ȡ����
struct CMD_GP_RotateDailAward
{
	DWORD							dwUserID;							// �û�ID
	DWORD							dwDailType;							// ת������
	DWORD							dwClientAddr;						//���ӵ�ַ
};

// ��ѯ�����п�
struct CMD_GP_ChickBindBank
{
	DWORD							dwClientAddr;						//���ӵ�ַ
	DWORD							wSubCommandID;						//��ϢID
	DWORD							dwUserID;							// ��ѯ�û�
};

// �����п�
struct CMD_GP_BindBankCard
{
	DWORD							dwClientAddr;						//���ӵ�ַ
	DWORD							wSubCommandID;						//��ϢID
	DWORD							dwUserID;							// ��ѯ�û�
	TCHAR							szBankName[32];							// ��������
	TCHAR							szBankCardNum[32];						// ���п���
	TCHAR							szAccountHolder[32];					// ������
};

// ��ѯ��֧����
struct CMD_GP_ChickBindZFB
{
	DWORD							dwClientAddr;						//���ӵ�ַ
	DWORD							wSubCommandID;						//��ϢID
	DWORD							dwUserID;								// ��ѯ�û�
};

// ��֧����
struct CMD_GP_BindZFB
{
	DWORD							dwClientAddr;						//���ӵ�ַ
	DWORD							wSubCommandID;						//��ϢID
	DWORD							dwUserID;							// ��ѯ�û�
	TCHAR							szZFBName[32];							// ֧�����˺�
	TCHAR							szNickName[32];							// �û�����
};

// �����ʼ�
struct CMD_GP_RequestMail
{
	DWORD							dwClientAddr;						//���ӵ�ַ
	DWORD							dwUserID;							// ��ѯ�û�
};

// ��ȡɾ���ʼ�
struct CMD_GP_ReadDeleteMail
{
	DWORD							dwClientAddr;						//���ӵ�ַ
	DWORD							dwUserID;							// ��ѯ�û�

	DWORD							dwOpenType;							// ������ʶ 0 ��ȡ 1 ɾ��
	DWORD							dwMailIndex;						// �ʼ���ʶ
};

//////////////////////////////////////////////////////////////////////////////////
//�����Ϣ

//��¼���
#define DBO_GP_LOGON_SUCCESS		100									//��¼�ɹ�
#define DBO_GP_LOGON_FAILURE		101									//��¼ʧ��
#define DBR_GP_VALIDATE_MBCARD		102									//��¼ʧ��
#define DBO_GP_THIRD_REGISTER		103									//����ע��
#define DBO_GP_THIRD_LOGON			104									//������¼
#define DBO_GP_WEB_LOGON_SUCCESS	105									//Web��¼�ɹ�

//�������
#define DBO_GP_USER_INDIVIDUAL		110									//�û�����

//��������
#define DBO_GP_USER_INSURE_INFO		120									//��������
#define DBO_GP_USER_INSURE_SUCCESS	121									//���гɹ�
#define DBO_GP_USER_INSURE_FAILURE	122									//����ʧ��
#define DBO_GP_USER_INSURE_USER_INFO   123								//�û�����

//�б���
#define DBO_GP_GAME_TYPE_ITEM		130									//������Ϣ
#define DBO_GP_GAME_KIND_ITEM		131									//������Ϣ
#define DBO_GP_GAME_NODE_ITEM		132									//�ڵ���Ϣ
#define DBO_GP_GAME_PAGE_ITEM		133									//������Ϣ
#define DBO_GP_GAME_LIST_RESULT		134									//���ؽ��
#define DBO_GP_GAME_COLLECTION		135								    //�ղ���Ϣ
#define DBO_GP_BROADCAST_MSG		139									//�㲥��Ϣ
#define DBO_GP_BROADCAST_TYPHON		145									//������Ϣ

//�û�����
#define DBO_GP_GET_HONOR			150									//�ƺ���Ϣ
#define DBO_GP_AVATAR_INFO			151									//ͷ��
#define DBO_GP_IMAGE_INFO			152									//����

//������
#define DBO_GP_OPERATE_RESULT		500									//�������

//�鿴�û�ǩ��
#define DBO_GP_GET_SIGNIN			700									//����û�ǩ��
#define DBO_GP_SET_SIGNIN			701									//�����û�ǩ��
#define DBO_GP_TODAY_IS_SIGNIN      702									//��ȡ�����Ƿ�ǩ��

#define DBO_GP_GET_DAIL_INFO		705									//��ȡת����Ϣ
#define DBO_GP_ROTATEDAILAWARD		706									// �û��齱��Ϣ

#define DBR_GP_CHICKBINDBANK		710									// �����п�
#define DBR_GP_CHICKBINDZFB			712									// ��֧����

#define DBR_GP_REQUESTMAIL			713									// �����ʼ�
#define DBR_GP_READDELEETETAIL		714									// ��ȡɾ���ʼ�

#define DBO_GP_USER_DAILYGET		200									//�ͱ����
#define DBO_GP_MAIL					201									//�ʼ�����
#define DBO_GP_MAILDETAIL			202									//�ʼ���ϸ

#define DBO_GP_GET_PACK				204									//�������
#define DBO_GP_GET_STORE			205									//��ͨ�����̳�
#define DBO_GP_GET_AUCTION			206									//�����̳�
#define DBO_GP_GET_SELF_AUCTION		207									//�����̳�

//���ѷ���
#define DBO_GP_GET_FRIEND			210									//��ȡ����
#define DBO_GP_GET_REQUEST			211									//��������
#define DBO_GP_SEARCH_USER			212									//��Ѱ�û�
#define DBO_GP_REQUEST_NOTICE		213									//����֪ͨ�Է�
#define DBO_GP_AGREE_REQUEST		214									//ͬ������֪ͨ�Է�
#define DBO_GP_DEL_FRIEND_NOTICE	215									//ɾ������֪ͨ
#define DBO_GP_REFUSE_NOTICE		216									//�ܾ�����֪ͨ�Է�

#define DBO_GP_GET_RANK_GOLD		220									//�������
#define DBO_GP_GET_RANK_ACHIEVE		221									//�ɾ�����
#define DBO_GP_GET_RANK_MASTER		222									//��ʦ������
#define DBO_GP_LOBBY_SENDRED		223									//���������
#define DBO_GP_GRAPRED				224									//��������
#define DBO_GP_BUY_POWER			225									//ˢ������
#define DBO_GP_GETSTOREINFO			226									//�̳���Ϣ��ʵʱ���
#define DBO_GP_GETSTOREINFO_SCORE	198									//�̳���Ϣ��ʵʱ��棩
#define DBO_GP_GETSTOREINFO_JEWEL	199									//�̳���Ϣ��ʵʱ��棩
#define DBO_GP_GETITEMINFO			227									//����������Ϣ
#define DBO_GP_GETUSEREXP			228									//��ȡ������Ϣ
#define DBO_GP_GETUSERTASK			229									//��ȡ�û�����
#define DBO_GP_GETACHIEVEINFO		230									//����ɾ���Ϣ
#define DBO_GP_GETSPECIFICACHIEVE	231									//����ɾ���Ϣ	
#define DBO_GP_GRANT_POWER			232									//�����������
#define DBO_GP_DEL_FRIEND			233									//ɾ������
#define DBO_GP_GET_MATCHDES			234									//��ȡ��������
#define DBO_GP_GET_MATCHRULE		235									//��ȡ��������
#define DBO_GP_GET_MATCHSOLUTION	236									//��ȡ�����������
#define DBO_GP_GET_MATCHREWARD		237									//��ȡ��������
#define DBO_GP_GET_MATCHINFOEND		238									//��ȡ������Ϣ����
#define DBO_GP_RECOMMENDFRIEND		239									//�Ƽ�����
#define DBO_GP_GETFAMEINFO			240									//����������Ϣ
#define DBO_GP_GETSPECIFICFAME		241									//����������Ϣ	
#define DBO_GP_RECOMMENDFRIEND2		242									//�Ƽ�����

#define DBO_GP_GET_RANK				243									//���а�
#define DBO_GP_SET_LEAVE_MESSAGE    244									//�������а�����
#define DBO_GP_GRANT_SCORE			245									//���ͺ��ѽ��
#define DBO_GP_GET_RED_INFO			246									//��ȡ�����������

#define DBO_GP_GET_USER_GEM			247									//��ȡ��ʯ����
#define DBO_GP_GET_USER_ORNAMENT    248									//��ȡ��Ʒ����
#define DBO_GP_BURNISH_GEM				249								//��ĥ��ʯ
#define DBO_GP_CARRY_GEM				250								//Я����ʯ
#define DBO_GP_INPUT_GEM				251									//��ֲ��ʯ
#define	DBO_GP_WEAR_ORNAMENT			252									//������Ʒ
#define DBO_GP_GET_GEM_FRIENDS			253								//��ȡ��ʯ����

#define DBO_GP_RETRIEVE_PASSWORD        254								//�һ����루������֤�˺ţ���֤�ֻ���
#define DBO_GP_GET_PERSONAL_INFO		255								//��ȡ������Ϣ����Ϸ����Ϣ���ǳƣ��ȼ���vip��ǩ�����ƺţ����ܣ������ȼ�����ͨ���ȼ������ֶ��ȼ�....��

			
#define DBO_GP_UPDATE_ATTRIBUTE			256								//czg��������
#define DBO_GP_PUSH_RED				257									//��ȡ���ڵĺ��

#define DBO_GP_MODIFY_AVATAR		258									//�޸�ͷ��
#define DBO_GP_MODIFY_IMAGE			259									//�޸�����
#define DBO_GP_MODIFY_HONOR			260									//�޸ĳƺ�

#define DBO_GP_RELOAD_SCORE		261									//���»���


#define DBO_GP_UPDATE_ITEM			262									//����һ������
#define DBO_GP_DELETE_ITEM			263									//ɾ��һ������

//���������Ϣ
#define DBO_GP_GET_TIME_RANK_INFO	264									//��ȡ��ʱ������ϸ��Ϣ
#define DBO_GP_GET_ROOM_SUMMARIZE	265									//��ȡ�������	
#define DBO_GP_GET_USER_RESULTS		266									//��ȡ�û�ս��

#define DBO_GP_GET_GAME_RECORDS		267									//��ȡ�û���Ϸ��¼

#define DBO_GP_GET_MATCH_APPLY_STATUS	277								//��ȡ��������״̬
#define DBO_GP_GET_TODATY_SENDRED		278								//��ȡ����ķ��͵ĺ��
#define DBO_GP_GET_RECEIVE_RED_INFO		279								//��ȡ���������

#define DBO_GP_GET_USER_INVITE_CODE_INFO     280							//��ȡ�û���������Ϣ
#define DBO_GP_GET_USER_INVITE_CODE_REWARD_INFO     281					//��ȡ�����뽱����Ϣ
#define DBO_GP_GET_FEEDBACK_REWARD_STATUS     282						//��ȡ����������״̬

#define DBO_GR_LOAD_SPREAD_INFO		283									//��ȡ�ƹ㽱������

#define DBO_GP_GET_USER_PERFECT_INFORMATION   284							//��ȡ�û�������Ϣ

#define DBO_GP_IS_SPREAD_REGISTER				285									//�Ƿ��ƹ�ע��
#define DBO_GP_IS_BIND_SPREAD					286									//�Ƿ��Ѿ����ƹ�
#define DBO_GP_IS_ALREADY_MODIFY_NICKNAME					287									//�Ƿ��Ѿ��޸Ĺ��ǳ�
#define DBO_GP_BIND_USER_SPREAD								288									//���û��ƹ�
#define DBO_GP_IS_TODAY_FIRST_TIME_LOGIN					289								//�Ƿ��ǽ����һ�ε�¼
#define DBO_GP_IS_LOST_CONNECTION_LOGIN					290								//�Ƿ��ǵ������µ�¼
#define DBO_GR_LOAD_SCROLL_MESSAGE					291								//��ȡ������Ϣ

//��¼�ɹ�
struct DBO_GP_LogonSuccess
{
	//��������
	DWORD							wFaceID;							//ͷ���ʶ
	DWORD							dwUserID;							//�û���ʶ
	DWORD							dwGameID;							//��Ϸ��ʶ
	DWORD							dwGroupID;							//���ű�ʶ
	DWORD							dwCustomID;							//�Զ�����
	DWORD							dwUserMedal;						//�û�����
	DWORD							dwExperience;						//������ֵ
	DWORD							dwLoveLiness;						//�û�����
	TCHAR							szPassword[LEN_MD5];				//��¼����
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�
	TCHAR							szGroupName[LEN_GROUP_NAME];		//��������

	//��չ��Ϣ
	TCHAR							szCompellation[16];				//��ʵ����
	TCHAR							szQQ[16];								//QQ
	TCHAR							szEMail[32];							//�����ʼ�
	TCHAR							szSeatPhone[32];					//�̶��绰
	TCHAR							szMobilePhone[16];				//�ֻ�����
	TCHAR							szDwellingPlace[128];			//��ϵ��ַ
	TCHAR							szUserNote[256];					//�û���ע

	//�û��ɼ�
	SCORE							lUserScore;							//�û���Ϸ��
	SCORE							lUserInsure;							//�û�����

	//�û�����
	BYTE							cbGender;							//�û��Ա�
	BYTE							cbMoorMachine;						//��������
	TCHAR							szUnderWrite[LEN_UNDER_WRITE];		//����ǩ��

	//��Ա����
	BYTE							cbMemberOrder;						//��Ա�ȼ�
	SYSTEMTIME						MemberOverDate;						//����ʱ��
	SYSTEMTIME						LastLogonDate;						//�����¼

	TCHAR							szLastClientAddr[16];
	TCHAR							szClientAddr[16];
	//������Ϣ
	TCHAR							szDescribeString[128];				//������Ϣ
};

//��������¼�ɹ�
struct DBO_GP_ThirdPartySuccess
{
	//��¼��Ϣ
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	TCHAR							szPassword[LEN_MD5];				//��¼����
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//��¼ʧ��
struct DBR_GP_ValidateMBCard
{
	UINT							uMBCardID;						//��������
};

//ͷ����Ϣ
struct DBO_GP_UserFaceInfo
{
	WORD							wFaceID;							//ͷ���ʶ
	DWORD							dwCustomID;							//�Զ�����
};

//ͷ����Ϣ
struct DBO_GP_UserAvatarInfo
{
	WORD							wAvatarID;
	SYSTEMTIME						sysLimit;
};

//������Ϣ
struct DBO_GP_UserImageInfo
{
	WORD							wImageID;
	SYSTEMTIME						sysLimit;
};

//��������
struct DBO_GP_UserIndividual
{
	//�û���Ϣ
	TCHAR							szCompellation[LEN_COMPELLATION];	//��ʵ����
	TCHAR							szPassportID[LEN_PASS_PORT_ID];		//���֤
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//Ԥ���ֻ���
	TCHAR							szEmail[LEN_EMAIL];					//Ԥ������
};

//��������
struct DBO_GP_UserInsureInfo
{
	DWORD							dwUserID;							//�û� I D
	WORD							wRevenueTake;						//˰�ձ���
	WORD							wRevenueTransfer;					//˰�ձ���
	WORD							wServerID;							//�����ʶ
	SCORE							lUserScore;							//�û�Ԫ��
	SCORE							lUserInsure;						//����Ԫ��
	SCORE							lTransferPrerequisite;				//ת������
};

//���гɹ�
struct DBO_GP_UserInsureSuccess
{
	DWORD							dwUserID;							//�û� I D
	SCORE							lSourceScore;						//ԭ��Ԫ��
	SCORE							lSourceInsure;						//ԭ��Ԫ��
	SCORE							lInsureRevenue;						//����˰��
	SCORE							lVariationScore;					//Ԫ���仯
	SCORE							lVariationInsure;					//���б仯
	TCHAR							szDescribeString[128];				//������Ϣ
};

//���������eg:���롢��;��������Ϣ��
struct DBO_GP_OperateResult
{	
	WORD							wSubCommandID;						//��Ϣ��;
	DWORD							dwResultCode;						//��������
	TCHAR							szDescribeString[128];				//������Ϣ
};

//�ʼ�����
struct DBO_GP_Mail
{
	BYTE							cbMailType;					//�ʼ����ͣ�0ϵͳ ��0����
	BYTE							cbOutDate;					//�Ƿ����
	BYTE							cbRead;						//�Ƿ��Ѷ�
	WORD							wLimitType;					//�ʼ�����
	DWORD							dwMailID;					//�ʼ�ID
	BYTE							cbContentType;				//��������
	BYTE							cbRewardCount;				//��������
	DWORD							dwSendTimeDiff;				//ʱ����

	BYTE							cbTitleSize;
	BYTE							cbNickNameSize;
	BYTE							cbSendTimeSize;

	/*TCHAR							szTitle[LEN_NICKNAME];		// �ʼ�����
	TCHAR							szNickName[LEN_NICKNAME];	// �ʼ�������
	TCHAR							szSendTime[25];				// ����ʱ��*/

	char							szExtra[0];
};

//�ʼ�����
struct DBO_GP_MailDetail
{		
	DWORD							dwItemID;					//��������ID ��6��
	WORD							wItemCount;					//��������
	DWORD							dwSecondItemID;
	WORD							wSecondItemCount;
	DWORD							dwThirdItemID;
	WORD							wThirdItemCount;
	DWORD							dwFourItemID;
	WORD							wFourItemCount;
	DWORD							dwFiveItemID;
	WORD							wFiveItemCount;
	DWORD							dwSixItemID;
	WORD							wSixItemCount;
	WORD							wPower;						
	WORD							wLoveLiness;

	TCHAR							szContent[80];				// ��������*/
};
//////////////////////////////////////////////////////////////////////////////////

//��Ϸ����
struct DBO_GP_GameType
{
	WORD							wJoinID;							//�ҽ�����
	WORD							wSortID;							//��������
	WORD							wTypeID;							//��������
	TCHAR							szTypeName[LEN_TYPE];				//��������
	
};

//��Ϸ����
struct DBO_GP_GameKind
{
	WORD							wTypeID;							//��������
	WORD							wJoinID;							//�ҽ�����
	WORD							wSortID;							//��������
	WORD							wKindID;							//��������
	WORD							wGameID;							//ģ������
	TCHAR							szKindName[LEN_KIND];				//��Ϸ����
	TCHAR							szProcessName[LEN_PROCESS];			//��������
	TCHAR							szDownLoadUrl[100];		//���ص�ַ
};

//��Ϸ�ڵ�
struct DBO_GP_GameNode
{
	WORD							wKindID;							//��������
	WORD							wJoinID;							//�ҽ�����
	WORD							wSortID;							//��������
	WORD							wNodeID;							//�ڵ�����
	TCHAR							szNodeName[LEN_NODE];				//�ڵ�����
};

//��������
struct DBO_GP_GamePage
{
	WORD							wKindID;							//��������
	WORD							wNodeID;							//�ڵ�����
	WORD							wSortID;							//��������
	WORD							wPageID;							//��������
	WORD							wOperateType;						//��������
	TCHAR							szDisplayName[LEN_PAGE];			//��ʾ����
};

//�ղ�����
struct DBO_GP_GameCollection
{
	WORD							wUserID; //�û�����
	WORD							wSortID;//��������
	TCHAR							szDisplayName[LEN_KIND]; //��Ϸ����
	WORD							wResult;
	WORD							wFlag;
	WORD							wCount;
	tagGameCollection				Collection[MAX_KIND];
};

struct DBO_GP_BroadcastMsg
{
	tagBroadcastMsg					ArrayBroadcastMsg[10];
	WORD							wCount;
};

struct DBO_GP_BroadcastTyphon
{
	BYTE							cbType;								
	BYTE							cbSuccess;
	DWORD							dwUserID;
	TCHAR							szNickName[LEN_NICKNAME];
	TCHAR							szContent[128];
};

//���ؽ��
struct DBO_GP_GameListResult
{
	BYTE							cbSuccess;							//�ɹ���־
};

#define SIGNCOUNT 256
#define SIGNLEN	  10

//�ͱ����
struct DBO_GP_UserDailyGet
{
	BYTE							cbSuccess;
	TCHAR							szMessage[128];
};


// �û���Ϣ
struct DBO_GP_UserInfo
{
	DWORD						dwUserID;					// ID
	DWORD						wFaceID;					// ͷ��ID
	DWORD						wLobbyLV;					// �����ȼ�
	BYTE						cbOnLine;					// �Ƿ�����
	TCHAR						szNickName[LEN_NICKNAME];	// �ǳ�
};

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//�ֻ����ݰ�

//��¼����
#define	DBR_MB_LOGON_GAMEID			500									//I D ��¼
#define	DBR_MB_LOGON_ACCOUNTS		501									//�ʺŵ�¼
#define DBR_MB_REGISTER_ACCOUNTS	502									//ע���ʺ�
#define DBR_MB_REGISTER_MOBILE		503									//�ֻ�ע��
#define DBR_MB_VISITOR_ACCOUNTS		504									//�ο͵�¼

//��¼���
#define DBO_MB_LOGON_SUCCESS		600									//��¼�ɹ�
#define DBO_MB_LOGON_FAILURE		601									//��¼ʧ��

//��¼����
#define	DBR_MB_LOGON_GAMEID_BCode			550									//I D ��¼
#define	DBR_MB_LOGON_ACCOUNTS_BCode			551									//�ʺŵ�¼
#define DBR_MB_REGISTER_ACCOUNTS_BCode		552									//ע���ʺ�
#define DBR_MB_REGISTER_MOBILE_BCode		553									//�ֻ�ע��
#define DBR_MB_VISITOR_ACCOUNTS_BCode		554									//�ο͵�¼

//��¼���
#define DBO_MB_LOGON_SUCCESS_BCode			650									//��¼�ɹ�
#define DBO_MB_LOGON_FAILURE_BCode			651									//��¼ʧ��

//////////////////////////////////////////////////////////////////////////////////

//ID ��¼
struct DBR_MB_LogonGameID
{
	//��¼��Ϣ
	DWORD							dwGameID;							//�û� I D
	TCHAR							szPassword[LEN_MD5];				//��¼����

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�绰����

	//������Ϣ
	LPVOID							pBindParameter;						//�󶨲���
};

//�ʺŵ�¼
struct DBR_MB_LogonAccounts
{
	//��¼��Ϣ
	TCHAR							szPassword[LEN_MD5];				//��¼����
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�绰����

	//������Ϣ
	LPVOID							pBindParameter;						//�󶨲���

	BYTE							cbDeviceType;                       //�豸����

	DWORD							dwInviteID;							// ������
	DWORD							dwSortID;							// ������

	DWORD							dwLogonType;						// ��¼���� 0 ��¼ 1 ע�� 2 ������
};

//�ʺ�ע��
struct DBR_MB_RegisterAccounts
{
	//ע����Ϣ
	WORD							wFaceID;							//ͷ���ʶ
	BYTE							cbGender;							//�û��Ա�
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	TCHAR							szNickName[LEN_ACCOUNTS];			//�û��ǳ�

	//�������
	TCHAR							szLogonPass[LEN_MD5];				//��¼����
	TCHAR							szInsurePass[LEN_MD5];				//��������

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�绰����

	//������Ϣ
	LPVOID							pBindParameter;						//�󶨲���
};

//�ʺ�ע��
struct DBR_MB_RegisterAccounts_BCode
{
	//ע����Ϣ
	WORD							wFaceID;							//ͷ���ʶ
	BYTE							cbGender;							//�û��Ա�
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	TCHAR							szNickName[LEN_ACCOUNTS];			//�û��ǳ�

	//�������
	TCHAR							szLogonPass[LEN_MD5];				//��¼����
	TCHAR							szInsurePass[LEN_MD5];				//��������

	//������Ϣ
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�绰����

	//������Ϣ
	LPVOID							pBindParameter;						//�󶨲���

	DWORD							dwSpreaderID;							//�ƹ���

};

//�ֻ�ע��
struct DBR_MB_RegisterMobile
{
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szMobile[LEN_MOBILE_PHONE];			//�ֻ�����
	TCHAR							szLogonPass[LEN_PASSWORD];			//��¼����
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
	LPVOID							pBindParameter;						//�󶨲���
};

//�ο��ʺ�
struct DBR_MB_VisitorAccounts
{
	DWORD							dwClientAddr;						//���ӵ�ַ
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	TCHAR							szLogonPass[LEN_MD5];				//��¼����

	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
	TCHAR							szSpreader[LEN_NICKNAME];			//�ƹ�Ա�˺�

	LPVOID							pBindParameter;						//�󶨲���

	DWORD							dwInviteID;							// ������
	DWORD							dwSortID;							// ������

	DWORD							dwLogonType;						// ��¼���� 0 ��¼ 1 ע�� 2 ������
};

//��¼�ɹ�
struct DBO_MB_LogonSuccess
{
	//�û�����
	DWORD							wFaceID;							//ͷ���ʶ
	BYTE							cbGender;							//�û��Ա�
	DWORD							dwUserID;							//�û� I D
	DWORD							dwGameID;							//��Ϸ I D
	DWORD							dwExperience;						//������ֵ
	DWORD							dwVipGrowthValue;					//VIP�ɳ�ֵ
	DWORD							dwLoveLiness;						//�û�����
	BYTE							cbMemberOrder;						//��Ա�ȼ�

	//�û��ɼ�
	DWORD							dwUserMedal;						//�û�����
	SCORE							lUserScore;							//�û�Ԫ��
	SCORE							lUserInsure;						//�û�����

	//��չ��Ϣ
	DWORD							dwInoutIndex;						//��¼����
	DWORD							dwHonorID;							//����ID
	DWORD							wAvatarID;							//����ID
	WORD							wPackGridCount;						//����������
	WORD							wMarketGridCount;					//�г�������

	LONG							lPower;								//�û�����
	LONG							lTimeDiff;							//ʱ���֣�
	LONG							lJewel;								//�û���ʯ
	SCORE							lRevenue;							//�û�˰��

	SCORE							lLobbyLV;							//��������

	SCORE							lMasterScore;						//��ʦ��
	SCORE							lHonorScore;						//ս����
	SCORE							lRaceScore;							//������
	SCORE							lAchieveScore;						//�ɾͷ�

	BYTE							cbVisitorClient;					//�οͱ�ʶ 0�� 1��
	BYTE							cbSecondPass;						//���������ʶ��0û�У�1��

	//������Ϣ
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	TCHAR							szUnderWrite[LEN_UNDER_WRITE];		//����ǩ��
};

//��¼�ɹ�
struct DBO_MB_LogonSuccess_BCode
{
	//�û�����
	DWORD							wFaceID;							//ͷ���ʶ
	BYTE							cbGender;							//�û��Ա�
	DWORD							dwUserID;							//�û� I D
	DWORD							dwGameID;							//��Ϸ I D
	DWORD							dwExperience;						//������ֵ
	BYTE							cbMemberOrder;						//��Ա�ȼ�

	//�û��ɼ�
	DWORD							dwUserMedal;						//�û�����
	SCORE							lUserScore;							//�û�Ԫ��
	SCORE							lUserInsure;						//�û�����

	//��չ��Ϣ
	DWORD							dwInoutIndex;						//��¼����

	SCORE							lRevenue;							//�û�˰��
	SCORE							lLobbyLV;							//��������

	BYTE							cbVisitorClient;					//�οͱ�ʶ 0�� 1��

	//������Ϣ
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	TCHAR							szUnderWrite[LEN_UNDER_WRITE];		//����ǩ��

	DWORD							dwServerID;							//����ID��0���Ƕ�������
};



//////////////////////////////////////////////////////////////////////////////////

//��ȡ������Ϣ����Ϸ����Ϣ���ǳƣ��ȼ���vip��ǩ�����ƺţ�����....��
struct DBO_GP_GetPersonalInfo
{
	DWORD							dwUserID;							//�û�ID
	DWORD							dwFaceID;							//ͷ�� I D
	BYTE							cbVIP;								// VIP
	DWORD							dwHonorID;							//�ƺ�ID

	DWORD							dwLobbyLv;							//�����ȼ�
	DWORD							dwAchieveScore;						//�ɾ�
	DWORD							dwRaceScore;						//������
	DWORD							dwZhanGong;							//ս��
	DWORD							dwMasterScore;						//��ʦ��
	DWORD							dwAvatarID;							//����ID

	TCHAR							szUnderWrite[LEN_UNDER_WRITE];		//����ǩ��
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�
};

//��ʱ����
struct DBO_GP_GetTimeRankInfo
{
	WORD						wRankUpper;					//��ʼ����
	WORD						wRankLower;					//��������
	DWORD						dwRewardType;				//����
	DWORD						dwRewardCount;				//��������
};

//�������
struct DBO_GP_GetRoomSummarize
{
	WORD						wMinPower;				//�볡��������
	DWORD						dwMinEnterScore;		//�볡����
	DWORD						dwObtainBlood;			//���Ѫ��

	DWORD						dwGameBase;				//��Ϸ����

	TCHAR						szRoomName[LEN_SERVER];	//��������
	TCHAR						szBoxIntroduce[128];    //�Ծֱ�������
};

//��ȡ�û�ս��
struct DBO_GP_GetUserResults
{
	DWORD						dwMasterScore;			//��ʦ��
	DWORD						dwMedal;				//�ڱ�
	DWORD						dwZhanGong;				//ս��
	DWORD						dwExp;					//����
	DWORD						dwBastRanking;			//�������
	DWORD						dwFirstRankCount;		//��õ�һ������
};

//��ȡ�û���Ϸ��¼����ʦ�֣�ս�������Σ�ʤ�ʣ��ܾ��飩
struct DBO_GP_GetUserGameRecords
{
	DWORD						dwUserlevel;			//�û��ȼ�
	BYTE						cbUserVIP;				//vip
	DWORD						dwMasterScore;			//��ʦ��
	DWORD						dwZhanGong;				//ս��
	DWORD						dwGamesNumber;			//��Ϸ����
	DWORD						dwWinProportion;		//ʤ������
	DWORD						dwExp;					//�ܾ���

	TCHAR						szNickName[LEN_NICKNAME];	//�ǳ�
};

//��ȡ��������״̬
struct DBO_GP_GetMatchApplyStatus
{
	DWORD dwMatchID;              //����ID
	BYTE cbMatchStatus;			//״̬
};

//���췢�ĺ��
struct DBO_GP_GetTodaySendRed
{
	DWORD dwRedID;				//���ID
	DWORD dwSendScore;			//���͵Ľ��
	TCHAR szSendRedTime[32];		//�����ʱ��
};

//��ȡ���������
struct DBO_GP_GetReceiveRedInfo
{
	DWORD   dwReceiveScore;				//��������
	TCHAR	szReceiveTime[32];		//�����ʱ��
	TCHAR	szNickName[LEN_NICKNAME];	//�ǳ�
};

//��ȡ��������Ϣ
struct DBO_GP_GetInviteCodeInfo
{
	DWORD   dwInviteSuccessNumber;				//����ɹ�����
	BYTE	cbIsReceiveInvite;					//�Ƿ��Ѿ���������
	TCHAR	szInviteCode[LEN_INVITE_CODE];	//������
};

//�����뽱����Ϣ
struct DBO_GP_InviteCodeRewardInfo
{
	DWORD dwID;							//�����ļ���ID
	DWORD dwFeedbackRewardUserID;         //�����������û�ID��0û����Ҫ��ȡ�Ľ���
};

//�����뽱����¼
struct DBO_GP_InviteCodeRewardRecord
{
	LONGLONG llFinishRewardRecord;    //��ɵĽ�����¼
	LONGLONG llReceiveRewardRecord;	 //�Ѿ���ȡ�Ľ�����¼
};

//�������������״̬
struct DBO_GP_GetFeedbackRewardStatus
{
	DWORD dwFeedbackRewardUserID;     //�����������û�ID��0û����Ҫ��ȡ�Ľ���
};

//�û��ƹ㽱����¼
struct DBR_GP_UserSpreadReward
{
	DWORD dwUserID;				//�û�ID
	WORD wFirestLevelSpread;	//һ���ƹ�
	WORD wScondLevelSpread;		//�����ƹ�
	WORD wThreeLevelSpread;		//�����ƹ�
	DWORD dwFirestRewardScore;	//һ���������
	DWORD dwScondRewradScore;	//�����������
	DWORD dwThreeRewradScore;	//�����������
	DWORD dwSpreadRewardCount;  //�ƹ㽱�������ڼ���
	DWORD dwUserRevenue;		//�û����ڵĳ�ˮ��
};

//��ȡǩ�����
struct DBO_GP_GetSignIn
{
	WORD wSignInNum;		//�Ѿ�ǩ������
	BYTE cbTodayIsSinIn;    //�����Ƿ��Ѿ�ǩ�� 0û�� 1�Ѿ�ǩ��
	DWORD	dwResultCode;						//��������
	TCHAR	szDescribeString[128];				//������Ϣ
};

//��ȡת����Ϣ
struct DBO_GP_GetDailInfo
{
	SCORE					dwSilverConsume;				//��������
	SCORE					dwGoldConsume;					//�ƽ�����
	SCORE					dwJewelConsume;					//��ʯ����

	SCORE					dwDrawScore;					//�齱����
	SCORE					dwAddScore;						//��ע����

	SCORE					dwDailInfo[3][8];				// ת����Ϣ
};

//��ȡת����Ϣ
struct DBO_GP_RotateDailAwardInfo
{
	DWORD					dwResultCode;						//��������
	TCHAR					szDescribeString[128];				//������Ϣ

	DWORD					dwUserID;						// �û���Ϣ
	DWORD					dwDailType;						// ��������

	DWORD					dwDailAward;					// ��������
	DWORD					dwAwardID;						// ����ID
	DWORD					dwConsumeSum;					// ��������

	SCORE					dwSilverConsume;				//��������
	SCORE					dwGoldConsume;					//�ƽ�����
	SCORE					dwJewelConsume;					//��ʯ����

	SCORE					dwDrawScore;					//�齱����
	SCORE					dwAddScore;						//��ע����

	SCORE					dwDailInfo[3][8];				// ת����Ϣ
};

// �����п���Ϣ
struct DBO_GP_BindBankInfo
{
	WORD							wSubCommandID;						//��Ϣ��;
	DWORD							dwResultCode;						//��������
	TCHAR							szDescribeString[128];				//������Ϣ

	TCHAR							szBankName[32];						// ��������
	TCHAR							szBankCardNum[32];					// ���п���
	TCHAR							szAccountHolder[32];				// ������
};

// ��֧������Ϣ
struct DBO_GP_BindZFBInfo
{
	WORD							wSubCommandID;						//��Ϣ��;
	DWORD							dwResultCode;						//��������
	TCHAR							szDescribeString[128];				//������Ϣ

	TCHAR							szZFBName[32];						// ֧�����˺�
	TCHAR							szNickName[32];						// �û�����
};

// �ʼ���Ϣ
struct RequestMail
{
	DWORD							dwMailIndex;						// �ʼ���ʶ
	DWORD							dwStartUsing;						// �ʼ���ȡ
	TCHAR							szMailContent[128];					// �ʼ�����
};
// �����ʼ�
struct DBO_GP_RequestMailToal
{
	DWORD							dwUserID;							// �û�ID
	DWORD							dwMailCount;						// �ʼ�����
	TCHAR							szDescribeString[128];				// ������Ϣ
	RequestMail						stRequestMail[15];					// �ʼ�����
};

// ��ȡɾ���ʼ�
struct DBO_GP_ReadDeleteMailToal
{
	DWORD							dwUserID;							// �û�ID
	DWORD							dwOpenType;							// ������ʶ 0 ��ȡ 1 ɾ��
	DWORD							dwResultCode;						// ��������
	TCHAR							szDescribeString[128];				// ������Ϣ
	DWORD							dwMailCount;						// �ʼ�����
	RequestMail						stRequestMail[15];					// �ʼ�����
};

//��ȡ�û���ϸ��Ϣ
struct DBO_GP_GetUserPerfectInformation
{
	TCHAR szCompellation[LEN_COMPELLATION];	//�û�����
	TCHAR szIdentifyCode[50];	//QQ�Ż��ֻ�����
};

//�û��ƹ㽱����¼
struct DBO_GP_UserSpreadReward
{
	DWORD dwUserID;					//�û�ID

	WORD wFirestLevelReward;	//һ����������
	WORD wSecondLevelReward;	//������������
	WORD wThreeLevelReward;		//������������

	DWORD dwFirestRewardScore;	//һ���������
	DWORD dwSecondRewradScore;	//�����������
	DWORD dwThreeRewradScore;	//�����������

	DWORD dwUserRevenue;				//�û���ˮ
	DWORD dwSecondLevelUserRevenueSum;	//�����û���ˮ����
	DWORD dwThreeLevelUserRevenueSum;	//�����û���ˮ

	DWORD dwSecondLevelUserNum;		//�����û���
	DWORD dwThreeLevelUserNum;		//�����û���
		
	DWORD dwSecondLevelUserReachNum;  //�����û������
	DWORD dwThreeLevelUserReachNum;   //�����û������

	DWORD dwCurrentRewardScore;		//��ǰ�Ľ�����
	DWORD dwSpreadRewardCount;	    //�ƹ㽱�����ڼ���
};

//��ȡ�ƹ㽱��ʱ��
struct  DBR_GP_LastSpreadRewardTime
{
	DWORD dwLastSpreadRewardCount;		//���һ�ν�������
	SYSTEMTIME stLastSpreadRewardTime;	//���һ���콱ʱ��
};

//���㽱��
struct DBO_GP_CalculateUserReward
{
	DWORD dwUserID;									//�û�ID
	DWORD dwLastSpreadRewardCount;						//���һ���ƹ㽱��������
	enCalculateSpreadRewardOperationType dwOperation;  //��������
};

//�û���ˮ��Ϣ
struct DBO_GP_UserRevenueInfo
{
	DWORD dwUserID;
	DWORD dwUserRevenue;
};

//�Ƿ����ƹ�ע��
struct DBO_GP_IsSpreadRegister
{
	BYTE cbIsSpreadRegister;  //0�����ƹ�ע�ᣬ1��
	DWORD dwRewardScore;	  //�����Ľ������
};

//������Ϣ
struct DBO_GR_ScrollMessage
{
	WORD wLength;					//�ַ�������
	TCHAR szScrollMessage[256];		//����
};

struct DBR_GP_ControlRoomStauts
{
	WORD wServerID;
	BYTE cbIsOpen;
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
		enumAttr_Email		=20,		//�ʼ�
	};

	WORD wType;
	SCORE wScore;
};


#pragma pack()
#endif