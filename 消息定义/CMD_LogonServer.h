#ifndef CMD_LONGON_HEAD_FILE
#define CMD_LONGON_HEAD_FILE

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

//////////////////////////////////////////////////////////////////////////////////
//��¼����

#define MDM_GP_LOGON				1									//�㳡��¼

//��¼ģʽ
#define SUB_GP_LOGON_GAMEID				1								//I D ��¼
#define SUB_GP_LOGON_ACCOUNTS			2								//�ʺŵ�¼
#define SUB_GP_REGISTER_ACCOUNTS		3								//ע���ʺ�
#define SUB_GP_GET_VALIDATE_CODE		4								//��ȡ��֤��

#define SUB_GP_REGISTER_ACCOUNTS_MB		5								//ע���ʺ�(����ֻ��� ����֤��)
#define SUB_GP_LOGON_THIRDPARTY			6								//������¼
#define SUB_GP_REGISTER_ACCOUNTS_THIRD	7								//������ע��
#define SUB_GP_LOGON_91SDK				8								//91��¼

#define SUB_GP_WEB_REGISTER_ACCOUNTS	9								// WEB ��Ϣ��¼

#define SUB_GP_LOGON_THIRD				14								//

//��¼���
#define SUB_GP_LOGON_SUCCESS		100									//��¼�ɹ�
#define SUB_GP_LOGON_FAILURE		101									//��¼ʧ��
#define SUB_GP_LOGON_FINISH			102									//��¼���
#define SUB_GP_VALIDATE_MBCARD      103                                 //��¼ʧ��
#define SUB_GP_VALIDATE_CODE		104                                 //��֤��
#define SUB_GP_WEB_LOGON_FINISH		105									//��¼���

//������ʾ
#define SUB_GP_UPDATE_NOTIFY		200									//������ʾ

//////////////////////////////////////////////////////////////////////////////////
//

#define MB_VALIDATE_FLAGS           0x01                                //Ч���ܱ�
#define LOW_VER_VALIDATE_FLAGS      0x02                                //Ч��Ͱ汾

//I D ��¼
struct CMD_GP_LogonGameID
{
	//ϵͳ��Ϣ
	DWORD							dwPlazaVersion;						//�㳡�汾
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������

	//��¼��Ϣ
	DWORD							dwGameID;							//��Ϸ I D
	TCHAR							szPassword[LEN_MD5];				//��¼����
	BYTE							cbValidateFlags;			        //У���ʶ
};

//�ʺŵ�¼
struct CMD_GP_LogonAccounts
{
	//ϵͳ��Ϣ
	DWORD							dwPlazaVersion;						//�㳡�汾
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������

	//��¼��Ϣ
	TCHAR							szPassword[LEN_MD5];				//��¼����
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	BYTE							cbValidateFlags;			        //У���ʶ
};

//������¼
struct CMD_GP_LogonThird
{
	//ϵͳ��Ϣ
	DWORD							dwPlazaVersion;						//�㳡�汾
	DWORD							dwUserID;

	BYTE							cbValidateFlags;			        //У���ʶ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������

};

//������¼
struct CMD_GP_LogonThirdParty
{
	TCHAR							szPassword[LEN_MD5];				//��¼����
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
};

//91��¼
struct CMD_GP_Logon91Sdk
{
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
};

//ע���ʺ�
struct CMD_GP_RegisterAccounts
{
	//ϵͳ��Ϣ
	DWORD							dwPlazaVersion;						//�㳡�汾
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������

	//�������
	TCHAR							szLogonPass[LEN_MD5];				//��¼����
	TCHAR							szInsurePass[LEN_MD5];				//��������

	//ע����Ϣ
	WORD							wFaceID;							//ͷ���ʶ
	BYTE							cbGender;							//�û��Ա�
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�
	TCHAR							szSpreader[LEN_ACCOUNTS];			//�Ƽ��ʺ�
	TCHAR							szPassPortID[LEN_PASS_PORT_ID];		//֤������
	TCHAR							szCompellation[LEN_COMPELLATION];	//��ʵ����
	BYTE							cbValidateFlags;			        //У���ʶ
	TCHAR							szValidateCode[LEN_VALIDATE_CODE];	//ͼ����֤��
};

//������ע���ʺ�
struct CMD_GP_RegisterThird
{
	//ϵͳ��Ϣ
	DWORD							dwPlazaVersion;						//�㳡�汾
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������

	//�������
	TCHAR							szLogonPass[LEN_MD5];				//��¼����
	TCHAR							szInsurePass[LEN_MD5];				//��������

	//ע����Ϣ
	WORD							wFaceID;							//ͷ���ʶ
	BYTE							cbGender;							//�û��Ա�
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�
	TCHAR							szSpreader[LEN_ACCOUNTS];			//�Ƽ��ʺ�
	TCHAR							szPassPortID[LEN_PASS_PORT_ID];		//֤������
	TCHAR							szCompellation[LEN_COMPELLATION];	//��ʵ����
	BYTE							cbValidateFlags;			        //У���ʶ
	TCHAR							szValidateCode[LEN_VALIDATE_CODE];	//ͼ����֤��

	//��������Ϣ
	TCHAR							szThirdParty[LEN_ACCOUNTS];			//�������ʺ�
};

//��¼�ɹ�
struct CMD_GP_LogonSuccess
{
	//��������
	WORD							wFaceID;							//ͷ���ʶ
	DWORD							dwUserID;							//�û� I D
	DWORD							dwGameID;							//��Ϸ I D
	DWORD							dwGroupID;							//���ű�ʶ
	DWORD							dwCustomID;							//�Զ���ʶ
	DWORD							dwUserMedal;						//�û�����
	DWORD							dwExperience;						//������ֵ
	DWORD							dwLoveLiness;						//�û�����

	//�û��ɼ�
	SCORE							lUserScore;							//�û����
	SCORE							lUserInsure;						//�û�����

	//������Ϣ
	BYTE							cbMoorMachine;						//��������
	BYTE                            cbShowServerStatus;                 //��ʾ������״̬
	BYTE							cbGender;							//�û��Ա�
	
	//��չ��Ϣ
	//��չ��Ϣ
	wchar_t							szCompellation[LEN_COMPELLATION];				//��ʵ����
	wchar_t							szQQ[LEN_QQ];								//QQ
	wchar_t							szEMail[LEN_EMAIL];							//�����ʼ�
	wchar_t							szSeatPhone[LEN_SEAT_PHONE];					//�̶��绰
	wchar_t							szMobilePhone[LEN_MOBILE_PHONE];				//�ֻ�����
	wchar_t							szDwellingPlace[LEN_DWELLING_PLACE];			//��ϵ��ַ
	wchar_t							szUserNote[LEN_USER_NOTE];					//�û���ע

	//�û���Ϣ
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	TCHAR							szNickName[LEN_ACCOUNTS];			//�û��ǳ�
	TCHAR							szGroupName[LEN_GROUP_NAME];		//��������
};

//��������¼�ɹ�
struct CMD_GP_ThirdPartySuccess
{
	//��¼��Ϣ
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	TCHAR							szPassword[LEN_MD5];				//��¼����
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//��¼ʧ��
struct CMD_GP_LogonFailure
{
	LONG							lResultCode;						//�������
	TCHAR							szDescribeString[128];				//������Ϣ
};

//��½���
struct CMD_GP_LogonFinish
{
	WORD							wIntermitTime;						//�ж�ʱ��
	WORD							wOnLineCountTime;					//����ʱ��
};

//��¼ʧ��
struct CMD_GP_ValidateMBCard
{
	UINT							uMBCardID;						//��������
};

//������ʾ
struct CMD_GP_UpdateNotify
{
	BYTE							cbMustUpdate;						//ǿ������
	BYTE							cbAdviceUpdate;						//��������
	DWORD							dwCurrentVersion;					//��ǰ�汾
};

struct CMD_GP_Get_RoomInfo
{
	unsigned int                           dwUserID;
	unsigned int                           dwRoomNum;
	unsigned int                           dwServerID[32];
};

struct CMD_GP_RoomInfo
{
	unsigned int dwServerID;                                              //�����ʶ
	long long llCellScore;                                                //��ע
	long long llMinEnterScore;                                            //��С�������
	long long llMaxEnterScore;                                            //���������
};

struct CMD_GP_ScoreInfo
{
	unsigned int dwWinCount;                                              //ʤ����Ŀ
	unsigned int dwLostCount;                                             //�ܾ���Ŀ
};

//////////////////////////////////////////////////////////////////////////////////
//Я����Ϣ CMD_GP_LogonSuccess

#define DTP_GP_GROUP_INFO			1									//������Ϣ
#define DTP_GP_MEMBER_INFO			2									//��Ա��Ϣ
#define	DTP_GP_UNDER_WRITE			3									//����ǩ��
#define DTP_GP_STATION_URL			4									//��ҳ��Ϣ
#define DTP_GP_ADDRESS_INFO			5									//��ַ��Ϣ

//������Ϣ
struct DTP_GP_GroupInfo
{
	DWORD							dwGroupID;							//��������
	TCHAR							szGroupName[LEN_GROUP_NAME];		//��������
};

//��Ա��Ϣ
struct DTP_GP_MemberInfo
{
	BYTE							cbMemberOrder;						//��Ա�ȼ�
	SYSTEMTIME						MemberOverDate;						//����ʱ��
};

//��ַ��Ϣ
struct DTP_GP_AddressInfo
{
	SYSTEMTIME						LastLogonDate;
	TCHAR							szLastClientAddr[32];
	TCHAR							szClientAddr[32];
	TCHAR							szMsgInfo[128];
};

//////////////////////////////////////////////////////////////////////////////////
//�б�����

#define MDM_GP_SERVER_LIST			2									//�б���Ϣ

//��ȡ����
#define SUB_GP_GET_LIST				1									//��ȡ�б�
#define SUB_GP_GET_SERVER			2									//��ȡ����
#define SUB_GP_GET_ONLINE			3									//��ȡ����
#define SUB_GP_GET_COLLECTION		4									//��ȡ�ղ�
#define SUB_GP_GET_ITEMSTORE		6									//�����̳�
#define SUB_GP_GET_MATCH			7									//��ȡ����

//�б���Ϣ
#define SUB_GP_LIST_TYPE			100									//�����б�
#define SUB_GP_LIST_KIND			101									//�����б�
#define SUB_GP_LIST_NODE			102									//�ڵ��б�
#define SUB_GP_LIST_PAGE			103									//�����б�
#define SUB_GP_LIST_SERVER			104									//�����б�
#define SUB_GP_LIST_COLLECTION 		105									//�ղ��б�
#define SUB_GP_LIST_ITEMSTORE		107									//�����̳�

//�����Ϣ
#define SUB_GP_LIST_FINISH			200									//�������
#define SUB_GP_SERVER_FINISH		201									//�������


//������Ϣ
#define SUB_GR_KINE_ONLINE			300									//��������
#define SUB_GR_SERVER_ONLINE		301									//��������

//////////////////////////////////////////////////////////////////////////////////

//��ȡ����
struct CMD_GP_GetOnline
{
	WORD							wServerCount;						//������Ŀ
	WORD							wOnLineServerID[MAX_SERVER];		//�����ʶ
};

//��������
struct CMD_GP_KindOnline
{
	WORD							wKindCount;							//������Ŀ
	tagOnLineInfoKind				OnLineInfoKind[MAX_KIND];			//��������
};

//��������
struct CMD_GP_ServerOnline
{
	WORD							wServerCount;						//������Ŀ
	tagOnLineInfoServer				OnLineInfoServer[MAX_SERVER];		//��������
};

//�����̳�
struct CMD_GP_ItemStore
{
	WORD						wCount;
	//tagItemStore				ItemStore[128];
};

//��������
struct CMD_GP_UserOnline
{
	DWORD							wRoomOnline;
	DWORD							wMatchOnline;
	DWORD							wActiveOnline;
};

#define COLLECTION_QUERY 0
#define COLLECTION_INSERT 1
#define COLLECTION_MODIFY 2
#define COLLECTION_DELETE 3

//��Ϸ�ղ�
struct CMD_GP_Collection
{
	WORD						wFlag;
	WORD						wUserID;
	WORD						wSortID;
	TCHAR						DisplayName[LEN_KIND];
	WORD						wCollectionCount;
	tagGameCollection			Collection[MAX_KIND];
};

//////////////////////////////////////////////////////////////////////////////////
//��������
#define MDM_GP_USER_SERVICE			3									//�û�����

//�˺ŷ���
#define SUB_GP_CESHI				99									//������Ϣ
#define SUB_GP_MODIFY_MACHINE		100									//�޸Ļ���
#define SUB_GP_MODIFY_LOGON_PASS	101									//�޸����루ԭʼ���룬�����룩
#define SUB_GP_MODIFY_INSURE_PASS	102									//�޸�����
#define SUB_GP_MODIFY_UNDER_WRITE	103									//�޸�ǩ��
#define SUB_GP_MODIFY_NICKNAME		104									//�޸��ǳ�
#define SUB_GP_PW_RECOVERY_ID		105									//ͨ�����֤�һ�����
#define SUB_GP_PW_RECOVERY_MOBILE	106									//ͨ���ֻ��һ�����
#define SUB_GP_PW_RECOVERY			107									//�һ����루�����룩
#define SUB_MB_GET_VARIFICATE_CODE	108									//��ȡ��֤��
#define SUB_GP_MODIFY_EMAIL			109									//�޸İ�����
#define SUB_GP_MODIFY_SECOND_PASS   110									//�޸Ķ�������
#define SUB_GP_CHECK_SECOND_PASS	111									//��ʵ��������
#define SUB_GP_RETRIEVE_PASSWORD    112									//���һ�����(������֤�˺ţ���֤�ֻ�����)

//���һ����뷵����Ϣ
#define SUB_GP_VERIFY_ACCOUNT			113									//��֤�˺�
#define SUB_GP_VERIFY_MOBILE_PHONE		114									//��֤�ֻ�
#define SUB_GP_VERIFY_VARIFICATE_CODE	115									//�����֤����޸�����
//////////////////////////////////////////////////////////////////////////

//�޸�ͷ��
#define SUB_GP_GET_AVATAR_INFO		200									//ͷ����Ϣ
#define SUB_GP_GET_IMAGE_INFO		201									//������Ϣ
#define SUB_GP_MODIFY_AVATAR		202									//�޸�ͷ��
#define SUB_GP_MODIFY_IMAGE			203									//�޸�����

//��������
#define SUB_GP_USER_INDIVIDUAL		301									//��������
#define	SUB_GP_QUERY_INDIVIDUAL		302								//��ѯ��Ϣ
#define SUB_GP_MODIFY_INDIVIDUAL	303									//�޸�����
#define SUB_GP_BIND_VISITOR_ACCOUNTS 304									//�ο�ת��
#define SUB_GP_CHANGE_MOBILE		305									//��������ֻ�
#define SUB_GP_BIND_MOBILE_PHONE	306									//���ֻ�
#define SUB_GP_GET_HONOR			307									//����ƺ�
#define SUB_GP_SET_HONOR			308									//���óƺ�
#define SUB_GP_GET_PERSONAL_INFO    309									//��ȡ������Ϣ����Ϸ����Ϣ���ǳƣ��ȼ���vip��ǩ�����ƺţ����ܣ������ȼ�����ͨ���ȼ������ֶ��ȼ�....��

//client->server
#define SUB_GP_FEEDBACK_SYSTEM		310									//������Ϣ

//server->client
#define SUB_GP_UPDATE_ATTRIBUTE		314									//czg��������

//���з���
#define SUB_GP_USER_SAVE_SCORE		400									//������
#define SUB_GP_USER_TAKE_SCORE		401									//ȡ�����
#define SUB_GP_USER_TRANSFER_SCORE	402									//ת�˲���
#define SUB_GP_USER_INSURE_INFO		403									//��������
#define SUB_GP_QUERY_INSURE_INFO	404									//��ѯ����
#define SUB_GP_USER_INSURE_SUCCESS	405									//���гɹ�
#define SUB_GP_USER_INSURE_FAILURE	406									//����ʧ��
#define SUB_GP_QUERY_USER_INFO_REQUEST	407								//��ѯ�û�
#define SUB_GP_QUERY_USER_INFO_RESULT	408								//�û���Ϣ

//�������
#define SUB_GP_OPERATE_SUCCESS		900									//�����ɹ�
#define SUB_GP_OPERATE_FAILURE		901									//����ʧ��
#define SUB_MB_VARIFICATE_RESULT	902									//��ȡ��֤����

//�ͻ���-->������
#define SUB_GP_GET_USER_SIGNIN		500									//��ȡǩ��
#define SUB_GP_SET_USER_SIGNIN		501									//ǩ��
#define SUB_GP_TODAY_IS_SIGNIN      502									//�����Ƿ�ǩ��

// ת��
#define SUB_GP_GET_DIAL_INFO		510									// ��ȡת����Ϣ
#define SUB_GP_DAIL_INFO_RESULT		511									// ת����Ϣ����
#define SUB_GP_ROTATE_DAIL_AWAED	512									// ת��ת����ȡ����
#define SUB_GP_RPTATEDAILAWARD_RESULT 513								// ת��ת���콱����
#define SUB_GP_GET_DIAL_RESULT		520									// ��ȡǩ������
#define SUB_GP_SET_DIAL_RESULT		521									// ����ǩ������
#define SUB_GP_CHICK_BANKCARD		530									// ��ѯ�����п�
#define SUB_GP_BIND_BANKCARD		531									// �����п�
#define SUB_GP_CHICK_BIND_ZFB		532									// ��ѯ��֧����
#define SUB_GP_BIND_ZFB				533									// ��֧����

#define SUB_GP_BANKCARDINFO			534									// �����п���Ϣ����
#define SUB_GP_ZFBBINDINFO			535									// ��֧������Ϣ����

//�㲥����
#define SUB_GP_BROADCAST_MSG		600									//�㲥��Ϣ
#define	SUB_GP_BROADCAST_TYPHON		601									//��С����

#define SUB_GP_USER_DAILYGET		602									//�ͱ���ȡ
#define SUB_GP_REQUEST_MAIL			603									//�����ʼ�
#define SUB_GP_READ_MAIL			604									//��ȡ�ʼ�
#define SUB_GP_DELETE_MAIL			605									//ɾ���ʼ�

#define SUB_GP_REQUEST_MAIL_RESULT				608									//�����ʼ�����
#define SUB_GP_READ_DELETE_MAIL_RESULT			609									//��ȡɾ���ʼ�����

#define SUB_GP_GET_STOCK_JEWEL		598									//ʵʱ���
#define SUB_GP_GET_STOCK_SCORE		606									//ʵʱ���
#define SUB_GP_GET_STORE			607									//��ͨ�̳�
#define SUB_GP_GET_AUCTION			608									//�����̳�
#define SUB_GP_BUY_STORE_ITEM		609									//�����̳ǵ���
#define SUB_GP_BUY_AUCTION_ITEM		610									//������������
#define SUB_GP_ONSALE_AUCTION_ITEM	611									//�ϼ���������
#define SUB_GP_OFFSALE_AUCTION_ITEM	612									//�¼���������
#define SUB_GP_USE_ITEM				613									//ʹ�õ���
#define SUB_GP_GRANT_ITEM			614										//�������
#define SUB_GP_GET_PACK				615									//������ȡ
#define SUB_GP_GET_RANK				616									//��ȡ����
#define SUB_GP_GET_ITEMINFO			617									//��ȡ������Ϣ
#define SUB_GP_GET_SELF_AUCTION		618									//��ȡ��������
#define SUB_GP_EXTRACT_SCORE		619									//��ȡ��������
#define SUB_GP_RESHELF_AUCTION_ITEM 599									//�����ϼ�

//���ѷ���
#define SUB_GP_REQUEST_FRIEND		620									//�������
#define SUB_GP_REQUEST_REPLY		621									//�������루�ܾ�/ͬ�⣩
#define SUB_GP_DELETE_FRIEND		622									//ɾ������
#define SUB_GP_UPDATE_FRIEND		623									//ˢ�º���
#define SUB_GP_GET_REQUEST			624									//��ȡ����
#define SUB_GP_SEARCH_USER			625									//�����û�
#define SUB_GP_REQUEST_NOTICE		626									//����֪ͨ

#define SUB_GP_SEND_RED				634									//���������(�ͻ���)
#define SUB_GP_LOBBY_SERVER_SENDRED		635									//���������������ˣ�
#define SUB_GP_RECEIVE_RED              636									//�����
#define SUB_GP_RECEIVE_RED_RESULT       637									//��������
#define SUB_GP_BUY_POWER				638									//��������
#define SUB_GP_GET_EXP					640									//��ȡ����
#define SUB_GP_GET_TASK					641									//��ȡ����
#define SUB_GP_REFRESH_TASK				642									//ˢ������
#define SUB_GP_TAKE_TASK				643									//��ȡ������
#define SUB_GP_GET_USERACHIEVE			644									//��ȡ�ɾ�����
#define SUB_GP_GET_USERWAGES			645									//��ȡ�ɾ͹���
#define SUB_GP_GET_SPECIFICRACHIEVE		646									//��ȡ����ɾ�
#define SUB_GP_SEND_POWER				647									//�������Ͷ���
#define SUB_GP_ALTER_REMARK				648									//�޸ĺ��ѱ�ע
#define SUB_GP_RECOMMENDFRIEND			649									//��ȡ�Ƽ�����
#define SUB_GP_GET_USERFAME				650									//��ȡ��������
#define SUB_GP_GET_SPECIFICFAME			651									//��ȡ��������

#define SUB_GP_USER_CHAT				652									//�������
#define SUB_GP_USER_EXPRESSION			653									//��ұ���
#define SUB_GP_BUY_VIP					654									//����VIP

#define SUB_GP_RECOMMENDFRIEND2			655									//��ȡ�Ƽ�����
#define SUB_GP_SET_LEAVE_MESSAGE		656									//��������
#define SUB_GP_INVITE_FRIEND			657									//���������Ϸ
#define SUB_GP_GRANT_SCORE				658									//���ͽ��

#define SUB_GP_GET_USER_GEM				670									//��ȡ��ʯ����
#define SUB_GP_GET_USER_ORNAMENT		671									//��ȡ��Ʒ����
#define SUB_GP_BURNISH_GEM				672									//��ĥ��ʯ
#define SUB_GP_CARRY_GEM				673									//Я����ʯ
#define SUB_GP_INPUT_GEM				674									//��ֲ��ʯ
#define	SUB_GP_WEAR_ORNAMENT			675									//������Ʒ
#define SUB_GP_GET_GEM_PRIZES			676									//�ջ�Ʒ
#define SUB_GP_GET_GEM_FRIENDS			677									//��ȡ��ʯ����
#define SUB_GP_PUSH_RED					678									//���ͺ��
#define SUB_GP_RELOAD_SCORE			680									//ˢ�»���	

//���������Ϣ
#define SUB_GP_GET_TIME_RANK_INFO		679									//��ȡ��ʱ����������Ϣ
#define SUB_GP_GET_ROOM_SUMMARIZE		681									//��ȡ�������
#define SUB_GP_GET_USER_RESULTS			682									//��ȡ�û�ս��

#define SUB_GP_UPDATE_ITEM				685
#define SUB_GP_DELETE_ITEM				686

#define SUB_GP_GET_GAME_RECORDS			687									//��ȡ�û���Ϸ��¼
#define SUB_GP_INPUT_INVITE_CODE		688									//����������
#define SUB_GP_GET_TODATY_SENDRED	    689									//��ȡ���췢�ĺ��
#define SUB_GP_GET_RECEIVE_RED_INFO	    690									//��ȡ���������

//������
#define SUB_GP_GET_USER_INVITE_CODE_INFO	691								//��ȡ�û���������ϸ��Ϣ
#define SUB_GP_GET_USER_INVITE_CODE_REWARD_INFO	692							//��ȡ�û������뽱����Ϣ
#define SUB_GP_OBTAIN_INVITE_CODE_REWARD	693								//��ȡ�����뽱��
#define SUB_GP_GET_FEEDBACK_REWARD_STATUE	694								//��ȡ����������״̬

#define SUB_GP_GET_USER_PERFECT_INFORMATION	    695							//��ȡ�û�������Ϣ
#define SUB_GP_SET_USER_PERFECT_INFORMATION     696							//�����û�������Ϣ

#define SUB_GP_IS_SPREAD_REGISTER     697									//�Ƿ����ƹ�ע��
#define SUB_GP_BIND_USER_SPREAD		  698									//���û��ƹ�
#define SUB_GP_IS_BIND_SPREAD		  699									//�Ƿ��Ѿ����ƹ�
#define SUB_GP_IS_ALREADY_MODIFY_NICKNAME 700								//�Ƿ��Ѿ��޸Ĺ��ǳ�
#define SUB_GP_IS_TODAY_FIRST_TIME_LOGIN 701								//�Ƿ��ǽ����һ�ε�¼
#define SUB_GP_IS_LOST_CONNECTION_LOGIN  702							        //�Ƿ��ǵ������µ�¼
#define SUB_GP_SCROLL_MESSAGE				703						        //������Ϣ



#define SUB_GP_FORCE_CLOSE				1000								//ǿ�ƶϿ��ͻ���

#define SUB_GP_CLIENT_LOGIN_SUCCESS		2000								//�ͻ��˵�¼���
#define SUB_GP_CLIENT_EXIT_ROOM			2001								//�ͻ����˳�����

//////////////////////////////////////////////////////////////////////////////////

//��ʵ��������
struct CMD_GP_CheckSecondPass
{
	DWORD								dwUserID;
	TCHAR								szPassword[LEN_MD5];				//��������
};


// �����̳ǵ���
struct CMD_GP_BuyStoreItem
{
	DWORD							dwUserID;							// �û�ID
	DWORD							wItemID;							// ����ID
	WORD							wItemCount;							// ��������
	BYTE							cbSaleType;							// ������
};

// ������������
struct CMD_GP_BuyAuctionItem
{
	DWORD							dwUserID;							// �����û�ID
	DWORD							dwAuctionID;							// ����ID
	WORD							wItemCount;							// ��������
	BYTE							cbSaleType;							// ������
};

// �ϼ���������
struct CMD_GP_OnSaleAuctionItem
{
	DWORD							dwUserID;							// �����û�ID
	DWORD							dwItemPrice;						// ���߼۸�
	DWORD							dwPacketID;							// ����ID
	WORD							wItemCount;							// ��������
};

//�����ϼ�
struct CMD_GP_ReShelfAuctionItem
{
	DWORD							dwUserID;							// �����û�ID
	DWORD							dwItemPrice;						// ���߼۸�
	WORD							wItemCount;							// ��������
	DWORD							dwAuctionID;						// ����ID
};

//�¼���������
struct CMD_GP_OffSaleAuctionItem
{
	DWORD							dwUserID;							// �����û�ID
	DWORD							dwAuctionID;							// ����ID
};

//��ѯ������Ϣ
struct CMD_GP_QueryItemInfo
{
	DWORD							dwUserID;							// �û�ID
	DWORD							dwItemID;							// ����ID
};

//��ȡ�����̳�
struct CMD_GP_GetAuction
{
	DWORD							dwUserID;
	BYTE							cbIndex;							//��ҳ���� ��1��ʼ
};


//��ȡ��������


//����ˢ������
struct CMD_GP_PackInfoRequest
{
	DWORD						dwUserID;
	DWORD						dwItemID;		//����ID
	BYTE							IsAutoUse;
};


//ʹ�õ���
struct CMD_GP_UseItem
{
	WORD							wCount;							
	DWORD							dwPackID;						//����ID
	DWORD							dwUserID;						//�û�ID
	DWORD							dwFriendID;						//����ID
};


struct CMD_GP_BroadcastTyphon
{
	BYTE							cbType;								//3���������з��� 4С���ȵ�ǰ����
	BYTE							cbSuccess;							//1 �ɹ� 0 ʧ��
	DWORD							dwUserID;
	TCHAR							szNickName[LEN_NICKNAME];
	TCHAR							szContent[128];
};

//��ȡ�ʼ�
struct CMD_GP_ReadMail
{
	DWORD							dwUserID;
	DWORD							dwMailID;		//0ȫ�� 1��һ
	BYTE							cbPowerMail;	//0���� 1������
	BYTE							cbMailType;		//0ϵͳ 1����
};

//ɾ���ʼ�
struct CMD_GP_DeleteMail
{
	DWORD							dwUserID;
	DWORD							dwMailID;		//0ȫ�� 1��һ
	BYTE							cbPowerMail;	//0���� 1������
	BYTE							cbMailType;		//0ϵͳ 1����
};

//�㲥��Ϣ
struct CMD_GP_BroadcastMsg
{
	tagBroadcastMsg				ArrayBroadcastMsg[10];
	WORD						wCount;
};

//�ͱ����
struct CMD_GP_DailyGetResult
{
	BYTE						cbSuccess;
	TCHAR						szMessage[128];
};

// �������
struct CMD_GP_OperateResult
{
	BYTE						cbStringLength;
	WORD						wSubCommandID;
	TCHAR						szDescribe[128];
};

#define SIGNCOUNT 256
#define SIGNLEN	  10

//�ı�Ԫ��
struct CMD_GP_CHANGE_SCORE
{
	DWORD							dwUserID;							//�û� I D
	SCORE							changeScore;						//�û����
	SCORE							changeInsureScore;					//���н��
};

//�޸�����
struct CMD_GP_ModifyLogonPass
{
	TCHAR							szAccounts[LEN_ACCOUNTS];			//�û��˺�
	TCHAR							szScrPassword[LEN_PASSWORD];		//�û�����
	TCHAR							szDesPassword[LEN_PASSWORD];		//�û�����
};

//�޸�����
struct CMD_GP_ModifyInsurePass
{
	DWORD							dwUserID;							//�û� I D
	TCHAR							szDesPassword[LEN_PASSWORD];		//�û�����
	TCHAR							szScrPassword[LEN_PASSWORD];		//�û�����
};

//���ö�������
struct CMD_GP_ModifySecondPass
{

	DWORD							dwUserID;							//�û�ID
	DWORD							operationType;						//�������ͣ�1�����ö������룬2���޸Ķ�������
	TCHAR							szSecondPassword[LEN_PASSWORD];		//��������
	TCHAR							szLogonPassword[LEN_PASSWORD];		//��¼����
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�ֻ�����
	TCHAR							szVerificationCode[LEN_VARIFICATE_CODE]; // ��֤��

};

//�޸�ǩ��
struct CMD_GP_ModifyUnderWrite
{
	DWORD							dwUserID;							//�û� I D
	TCHAR							szUnderWrite[LEN_UNDER_WRITE];		//����ǩ��
};

//�޸��ǳ�
struct CMD_GP_ModifyNickName
{
	DWORD							dwUserID;							//�û� ID
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�
};

//�޸�����
struct CMD_GP_ModifyPass
{
	TCHAR							szAccounts[LEN_ACCOUNTS];			//�û��˺�
	TCHAR							szPassword[LEN_PASSWORD];			//�û�����
};

//////////////////////////////////////////////////////////////////////////////////

//�޸�ͷ��
struct CMD_GP_ModifyAvatar
{
	DWORD							dwUserID;							//�û� I D
	DWORD							dwAvatarID;							//ͷ�� I D
};

//�޸�����
struct CMD_GP_ModifyImage
{
	DWORD							dwUserID;							//�û� I D
	DWORD							dwImageID;							//���� I D
};

//��ȡͷ��/����
struct CMD_GP_GetAvatarInfo
{
	DWORD							dwUserID;
	BYTE							cbAvatarType;
};

//�ο�ת��
struct CMD_GP_BindVisitorAccount
{
	DWORD							dwUserID;
	TCHAR							szMachineID[LEN_MACHINE_ID];			//����ID
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�
	TCHAR							szPassword[LEN_PASSWORD];			//�û�����
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�ֻ���
};

//�����ֻ�����
struct CMD_GP_ChangeMobilePhone
{
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�ֻ���
	TCHAR							szVerificationCode[LEN_VARIFICATE_CODE]; //��֤��
};

//���ֻ�����
struct CMD_GP_BindMobilePhone
{
	DWORD							dwUserID;
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�ֻ���
	TCHAR							szVerificationCode[LEN_VARIFICATE_CODE]; //��֤��
	TCHAR							szPassword[LEN_PASSWORD];			//�û�����
};

//����������
struct CMD_GP_BindEmail	
{
	DWORD							dwUserID;							
	TCHAR							szEmailAddress[LEN_EMAIL];			//�������ַ
	TCHAR							szPassword[LEN_PASSWORD];			//��¼������������
};

//���óƺ�
struct CMD_GP_SetHonor
{
	DWORD							dwUserID;
	DWORD							dwHonorID;
};

//////////////////////////////////////////////////////////////////////////////////

//�󶨻���
struct CMD_GP_ModifyMachine
{
	BYTE							cbBind;								//�󶨱�־
	DWORD							dwUserID;							//�û���ʶ
	TCHAR							szPassword[LEN_PASSWORD];			//�û�����
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//////////////////////////////////////////////////////////////////////////////////

//��������
struct CMD_GP_UserIndividual
{
	TCHAR							szCompellation[LEN_COMPELLATION];	//��ʵ����
	TCHAR							szPassportID[LEN_PASS_PORT_ID];		//���֤ I D
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//Ԥ���ֻ���
};

//�޸�����
struct CMD_GP_ModifyIndividual
{
	DWORD							dwUserID;									//�û� I D
	TCHAR							szCompellation[LEN_COMPELLATION];			//��ʵ����
	TCHAR							szPassportID[LEN_PASS_PORT_ID];				//���֤ I D
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];			//Ԥ���ֻ���
	TCHAR							szPassword[LEN_MD5];						//����
	TCHAR							szVerificationCode[LEN_VARIFICATE_CODE];	//��֤��
};

//////////////////////////////////////////////////////////////////////////////////
//Я����Ϣ CMD_GP_UserIndividual

#define DTP_GP_UI_NICKNAME			1									//�û��ǳ�
#define DTP_GP_UI_USER_NOTE			2									//�û�˵��
#define DTP_GP_UI_UNDER_WRITE		3									//����ǩ��
#define DTP_GP_UI_QQ				4									//Q Q ����
#define DTP_GP_UI_EMAIL				5									//�����ʼ�
#define DTP_GP_UI_SEAT_PHONE		6									//�̶��绰
#define DTP_GP_UI_MOBILE_PHONE		7									//�ƶ��绰
#define DTP_GP_UI_COMPELLATION		8									//��ʵ����
#define DTP_GP_UI_DWELLING_PLACE	9									//��ϵ��ַ

//////////////////////////////////////////////////////////////////////////////////

//��������
struct CMD_GP_UserInsureInfo
{
	WORD							wRevenueTake;						//˰�ձ���
	WORD							wRevenueTransfer;					//˰�ձ���
	WORD							wServerID;							//�����ʶ
	SCORE							lUserScore;							//�û����
	SCORE							lUserInsure;						//���н��
	SCORE							lTransferPrerequisite;				//ת������
};

//������
struct CMD_GP_UserSaveScore
{
	DWORD							dwUserID;							//�û� I D
	SCORE							lSaveScore;							//������
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//��ȡ���
struct CMD_GP_UserTakeScore
{
	DWORD							dwUserID;							//�û� I D
	SCORE							lTakeScore;							//��ȡ���
	TCHAR							szPassword[LEN_MD5];				//��������
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//ת�˽��
struct CMD_GP_UserTransferScore
{
	DWORD							dwUserID;							//�û� I D
	BYTE                            cbByNickName;                       //�ǳ�����
	SCORE							lTransferScore;						//ת�˽��
	TCHAR							szPassword[LEN_MD5];				//��������
	TCHAR							szNickName[LEN_NICKNAME];			//Ŀ���û�
	TCHAR							szMachineID[LEN_MACHINE_ID];		//��������
};

//���гɹ�
struct CMD_GP_UserInsureSuccess
{
	DWORD							dwUserID;							//�û� I D
	SCORE							lUserScore;							//�û����
	SCORE							lUserInsure;						//���н��
	TCHAR							szDescribeString[128];				//������Ϣ
};

//����ʧ��
struct CMD_GP_UserInsureFailure
{
	LONG							lResultCode;						//�������
	TCHAR							szDescribeString[128];				//������Ϣ
};

//��ȡ���
struct CMD_GP_UserTakeResult
{
	DWORD							dwUserID;							//�û� I D
	SCORE							lUserScore;							//�û����
	SCORE							lUserInsure;						//���н��
};

//��ѯ�û�
struct CMD_GP_QueryUserInfoRequest
{
	BYTE                            cbByNickName;                       //�ǳ�����
	TCHAR							szNickName[LEN_NICKNAME];			//Ŀ���û�
};

//�û���Ϣ
struct CMD_GP_UserTransferUserInfo
{
	DWORD							dwTargetGameID;						//Ŀ���û�
	TCHAR							szNickName[LEN_NICKNAME];			//Ŀ���û�
};
//////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////
//Զ�̷���
#define MDM_GP_REMOTE_SERVICE		4									//Զ�̷���

//���ҷ���
#define SUB_GP_C_SEARCH_DATABASE	100									//���ݲ���
#define SUB_GP_C_SEARCH_CORRESPOND	101									//Э������

//���ҷ���
#define SUB_GP_S_SEARCH_DATABASE	200									//���ݲ���
#define SUB_GP_S_SEARCH_CORRESPOND	201									//Э������

//////////////////////////////////////////////////////////////////////////////////

//Э������
struct CMD_GP_C_SearchCorrespond
{
	DWORD							dwGameID;							//��Ϸ��ʶ
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�
};

//Э������
struct CMD_GP_S_SearchCorrespond
{
	WORD							wUserCount;							//�û���Ŀ
	tagUserRemoteInfo				UserRemoteInfo[16];					//�û���Ϣ
};

/************************************************************************/
/*									���ζ���                                              */
/************************************************************************/
//��¼����
#define MDM_MB_LOGON				100									//�㳡��¼
#define MDM_MB_LOGON_BCode			102									//�㳡��¼

//��¼ģʽ
#define SUB_MB_LOGON_GAMEID			1									//I D ��¼
#define SUB_MB_LOGON_ACCOUNTS		2									//�ʺŵ�¼
#define SUB_MB_REGISTER_ACCOUNTS	3									//ע���ʺ�
#define SUB_MB_VISITOR_ACCOUNTS		4									//�ο͵�¼
#define SUB_MB_REGISTER_MOBILE		6									//ע���ʺ�(�ֻ�����ע��)

//��¼���
#define SUB_MB_LOGON_SUCCESS		100									//��¼�ɹ�
#define SUB_MB_LOGON_FAILURE		101									//��¼ʧ��
#define SUB_MB_LOGON_FINISH			102									//��¼���
#define SUB_MB_IS_LOST_CONNECTION_LOGIN 103								//�Ƿ��������


#define SUB_MB_UPDATE_NOTIFY		200									//������ʾ

//////////////////////////////////////////////////////////////////////////
//�б�����
#define MDM_MB_SERVER_LIST		200									//�б���Ϣ

//�б���
#define SUB_MB_LIST_KIND				101								//�����б�
#define SUB_MB_LIST_SERVER			104									//�����б�
#define SUB_MB_LIST_MATCH			105									//�����б�
#define SUB_MB_LIST_FINISH			200									//�б����
#define SUB_MB_LIST_MATCH_FINISH	201									//�б����

#define SUB_MB_KINE_ONLINE			300									//��������

//������Ϣ����
#define	MDM_MB_MATCH_INFO		300									//������Ϣ

#define MATCH_MB_INFO					100									//������Ϣ���ܣ�
#define	MATCH_MB_FEE					101									//����������
#define	MATCH_MB_FEE_AND_REQUIR			102									//���������ѣ��Լ�����	
#define	MATCH_MB_DESCRIPTION			103									//��������
#define	MATCH_MB_RULE					104									//��������
#define	MATCH_MB_SOLUTION				105									//�����������
#define	MATCH_MB_REWARD					106									//��������
#define	MATCH_MB_END					107									//��ȡ������Ϣ����
#define MATCH_MB_APPLY_STATUS			108									//��������״̬



//////////////////////////////////////////////////////////////////////////////////

//I D ��¼
struct CMD_MB_LogonGameID
{
	//ϵͳ��Ϣ
	DWORD							dwPlazaVersion;						//�㳡�汾
	BYTE                            cbDeviceType;                       //�豸����

	//��¼��Ϣ
	DWORD							dwGameID;							//��Ϸ I D
	TCHAR							szPassword[LEN_MD5];				//��¼����

	//������Ϣ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//������ʶ
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�绰����
};

//�ʺŵ�¼
struct CMD_MB_LogonAccounts
{
	//ϵͳ��Ϣ
	DWORD							dwPlazaVersion;						//�㳡�汾
	BYTE							cbDeviceType;                       //�豸����

	//��¼��Ϣ
	TCHAR							szPassword[LEN_MD5];				//��¼����
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�

	//������Ϣ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//������ʶ
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�绰����
};

//ע���ʺ�
struct CMD_MB_RegisterAccounts
{
	//ϵͳ��Ϣ
	BYTE                            cbDeviceType;                       //�豸����

	//�������
	TCHAR							szLogonPass[LEN_MD5];				//��¼����
	TCHAR							szInsurePass[LEN_MD5];				//��������

	//ע����Ϣ
	WORD							wFaceID;							//ͷ���ʶ
	BYTE							cbGender;							//�û��Ա�
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�

	//������Ϣ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//������ʶ
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�绰����
};

//ע���ʺ�
struct CMD_MB_RegisterAccounts_BCode
{
	//ϵͳ��Ϣ
	BYTE                            cbDeviceType;                       //�豸����

	//�������
	TCHAR							szLogonPass[LEN_MD5];				//��¼����
	TCHAR							szInsurePass[LEN_MD5];				//��������

	//ע����Ϣ
	WORD							wFaceID;							//ͷ���ʶ
	BYTE							cbGender;							//�û��Ա�
	TCHAR							szAccounts[LEN_ACCOUNTS];			//��¼�ʺ�
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�

	//������Ϣ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//������ʶ
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�绰����

	DWORD							dwSpreaderID;							//�ƹ���
};

//�ο͵�¼
struct CMD_MB_LogonVisitor
{
	//ϵͳ��Ϣ
	DWORD							dwPlazaVersion;						//�㳡�汾
	BYTE							cbDeviceType;                       //�豸����

	//������Ϣ
	TCHAR							szMachineID[LEN_MACHINE_ID];		//������ʶ
	TCHAR							szSpreader[LEN_NICKNAME];			//�ƹ�Ա�˺�
};

//��ȡ��֤��
struct CMD_MB_GetVarificateCode
{
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];	//�ֻ�����
	WORD							wSubCommandID;						//��;
};

//��֤����
struct CMD_MB_VarificateResult
{
	WORD							wCode;								//���
	WORD							wSubCommandID;						//��;
};

//�ֻ�ע��
struct CMD_MB_RegisterMobile
{
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];		//�ֻ�����
	TCHAR							szLogonPass[LEN_MD5];					//��¼����
	TCHAR							szMachineID[LEN_MACHINE_ID];			//������ʶ
	TCHAR							szVerificationCode[LEN_VARIFICATE_CODE];//��֤��
};

//�����һ�(���֤���ֻ���)
struct CMD_MB_PWRecoveryID
{
	TCHAR							szAccounts[LEN_ACCOUNTS];				// �û��˺�
	TCHAR							szPassPortID[LEN_PASS_PORT_ID];			// ���֤
};

//�����һ�(�ֻ���)
struct CMD_MB_PWRecoveryMobile
{
	TCHAR							szAccounts[LEN_ACCOUNTS];				// �û��˺�
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];		// Ԥ���ֻ���
	TCHAR							szVerificationCode[LEN_VARIFICATE_CODE];// ��֤��
};

//��¼ʧ��
struct CMD_MB_LogonFailure
{
	LONG							lResultCode;						//�������
	TCHAR							szDescribeString[128];				//������Ϣ
};

//������ʾ
struct CMD_MB_UpdateNotify
{
	BYTE							cbMustUpdate;						//ǿ������
	BYTE							cbAdviceUpdate;						//��������
	DWORD							dwCurrentVersion;					//��ǰ�汾
};

//������������ͻ���--����ˣ�
struct CMD_MB_LOBBY_SENDRED
{
	DWORD dwRedID;				//���ID
	DWORD dwSendUserID;			//��������ID
	DWORD dwSendUserNum;		//���������
	DWORD dwSendScore;			//������
	BYTE  cbRedType;			//�������
	TCHAR szSendContent[128];	//�����������
};


//�����
struct CMD_MB_GRAPRED
{
	DWORD dwSendUserID;		//��������ID
	DWORD dwGrapUserID;		//��������ID
	DWORD dwGrapRedID;		//���ĺ��ID
};

/*
//����������ݿ�洢��
struct CMD_MB_DB_GRAPRED
{
	DWORD dwSendUserID;		//��������ID
	DWORD dwGrapUserID;		//��������ID
	DWORD dwRecvScore;		//��������
	TCHAR szSendContent[128];	//�����������
};
*/

struct CMD_MB_BuyVip
{
	DWORD	dwUserID;			
	DWORD	PayAmount;			//��ֵ���
};

//�������
struct CMD_GP_InviteFriend
{
	DWORD							dwUserID;
	TCHAR							dwNickName[LEN_NICKNAME];	
	DWORD							dwFriendID;
	DWORD							dwRoomScore;						//�����볡
	WORD							wTableID;							//ָ������
	WORD							wServerPort;						//����˿�
	TCHAR							szServerAddr[LEN_SERVER];			//�����ַ
};

//������Ϣ
struct CMD_GP_LeaveMessage
{
	DWORD							dwUserID;							//�û�ID
	WORD							dwRankType;							//���а�����
	WORD							dwLeaveMassageID;					//��������ID
};

//��ʯ����
struct CMD_GP_GemPack
{
	DWORD							dwGemID;							//��ʯID
	BYTE							cbIsCarry;							//�Ƿ�Я��
	BYTE							cbIsInPut;							//�Ƿ����
	DWORD							dwWearOrnamentID;					//�Ѵ�����ƷID
};

//��Ʒ����
struct CMD_GP_OrnamentPack
{
	DWORD							dwOrnamentID;						//��ƷID
	BYTE							cbIsWear;							//�Ƿ񴩴�
};

//Я����ʯ
struct CMD_GP_CarryGem
{
	BYTE							cbCarry;							//1Я�� 0ж��
	DWORD							dwGemID;
	DWORD							dwUserID;
};

//���뱦ʯ
struct CMD_GP_InPutGem
{
	BYTE							cbInPut;							//1���� 0�ջ�
	DWORD							dwGemID;
	DWORD							dwUserID;
};

//��ĥ��ʯ
struct CMD_GP_BurnishGem
{
	DWORD							dwGemID;
	DWORD							dwUserID;
};

//������Ʒ
struct CMD_GP_WearOrnament
{
	BYTE							cbWear;								//1���� 0����
	DWORD							dwOrnamentID;
	DWORD							dwUserID;
	DWORD							dwGemID;
};

//�ջ�Ʒ
struct CMD_GP_GetGemPrize
{
	DWORD							dwPrizeType;						//0һ����ȡ 1��� 4��ʯ 16�ش�� 88�ն� 413095��ʯ
	DWORD							dwGemID;
	DWORD							dwUserID;
};

//�һ����루������֤�˺ţ���֤�ֻ����ܣ�
struct CMD_GP_RetrievePassword
{
	BYTE							cbOperationType;						//�������ͣ�1��֤�˺ţ�2��֤�ֻ���3�����֤��ͬʱ�޸�����
	TCHAR							szAccounts[LEN_ACCOUNTS];				//�û��˺�
	TCHAR							szMobilePhone[LEN_MOBILE_PHONE];		//�ֻ�����
	TCHAR							szNewLogonPassword[LEN_PASSWORD];			//�µ�¼����
	TCHAR							szVerificationCode[LEN_VARIFICATE_CODE];//��֤��
};

//��ȡ�û�ս��
struct CMD_GP_GetUserResults
{
	DWORD dwServerID;		
	DWORD dwUserID;			//�û�ID
};

//���������루���ձ������룩
struct CMD_GP_InputInviteCode
{
	DWORD dwUserID;										//�û�ID
	TCHAR dwInviteCode[LEN_INVITE_CODE];		//������
};

//��ȡ���췢�ĺ��
struct CMD_GP_GetTodaySendRed
{
	DWORD dwSendUserID;				//������û�ID
	DWORD dwStartRecord;			//��ʼ��¼
	DWORD dwGetNum;					//��ȡ����
};

//��ȡ���������
struct CMD_GP_GetReceiveRedInfo
{
	DWORD dwSendUserID;				//������û�ID
	DWORD dwRedID;					//���ID
};

//��ȡ�����뽱��
struct CMD_GP_ObtainInviteCodeReward
{
	DWORD dwID;						//����ID
	DWORD dwUserID;					//�û�ID
	DWORD dwFeedbackUserID;			//�������û�ID
};

//��ȡһ���������������״̬
struct CMD_GP_GetFeedbackRewardStatus
{
	DWORD dwID;						//����ID
	DWORD dwUserID;					//�û�ID
};

//�����û���ϸ��Ϣ
struct CMD_GP_SetUserPerfectInformation
{
	DWORD dwUserID;							//�û�ID
	TCHAR szCompellation[LEN_COMPELLATION];	//�û�����
	TCHAR szIdentifyCode[50];				//�ֻ�����
	TCHAR szVerificationCode[LEN_VARIFICATE_CODE]; // ��֤��
};

//���û��ƹ�
struct CMD_GP_BindUserSpread
{
	DWORD dwUserID;						//�û�ID
	DWORD dwSpreadID;					//�ƹ���
};


// ��ȡǩ��
struct CMD_GP_GetUserSignIn
{
	DWORD					dwUserID;				// ǩ���û�
};

// ǩ��
struct CMD_GP_SetUserSignIn
{
	DWORD					dwUserID;				// ǩ���û�
};



#pragma pack()

#endif

