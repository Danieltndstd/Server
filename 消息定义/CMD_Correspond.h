#ifndef CMD_CORRESPOND_HEAD_FILE
#define CMD_CORRESPOND_HEAD_FILE

#pragma pack(1)

//////////////////////////////////////////////////////////////////////////////////
//ע������

#define MDM_CS_REGISTER				1									//����ע��

//����ע��
#define SUB_CS_C_REGISTER_PLAZA		100									//ע��㳡
#define SUB_CS_C_REGISTER_SERVER	101									//ע�᷿��
#define SUB_CS_C_REGISTER_MATCH		102									//ע�����

//ע����
#define SUB_CS_S_REGISTER_FAILURE	200									//ע��ʧ��

//////////////////////////////////////////////////////////////////////////////////

//ע��㳡
struct CMD_CS_C_RegisterPlaza
{
	TCHAR							szServerAddr[32];					//�����ַ
	TCHAR							szServerName[LEN_SERVER];			//��������
};

//ע����Ϸ
struct CMD_CS_C_RegisterServer
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
	BYTE							cbRoomType;							//���伶��
	TCHAR							szServerAddr[32];					//�����ַ
	TCHAR							szServerName[LEN_SERVER];			//��������

	//��������
	SCORE							lRestrictScore;						//���ƻ���
	SCORE							lMinTableScore;						//��ͻ���
	SCORE							lMinEnterScore;						//��ͻ���
	SCORE							lMaxEnterScore;						//��߻���
	SCORE							lMinEnterTiLi;						//�������


	WORD							wCardBoxIndex;						//��������
};

//ע�����
struct CMD_CS_C_RegisterMatch
{
	/*
	WORD				wOrderID;						//��������
	WORD				wMatchTab;						//������ǩ
	WORD				wMatchType;						//��������
	DWORD				dwJoinUserCountUpper;			//����������		
	TCHAR				szMatchReward[32];				//�ھ���Ʒ
	TCHAR				szMatchName[80];				//����ȫ��
	*/
	//CMD_GR_MatchDateTime		DateTime;				//����ʱ��
	//tagMatchRequireInfo			Require;					//��������
	//tagMatchFeeInfo				Fee;						//��������
	CMD_CS_C_RegisterServer		RegServer;
	//tagMatchReviveInfo			ReviveInfo;					//��������
	//tagMatchTagout				Tagout;						//��������
};

//ע��ʧ��
struct CMD_CS_S_RegisterFailure
{
	LONG							lErrorCode;							//�������
	TCHAR							szDescribeString[128];				//������Ϣ
};

//////////////////////////////////////////////////////////////////////////////////
//������Ϣ

#define MDM_CS_SERVICE_INFO			2									//������Ϣ

//������Ϣ
#define SUB_CS_C_SERVER_ONLINE		1									//��������
#define SUB_CS_C_SERVER_MODIFY		2									//�����޸�

//��������
#define SUB_CS_C_FRIEND_REQUEST		10									//��������
#define SUB_CS_C_AGREE_REQUEST		11									//����ͬ������
#define SUB_CS_C_DEL_FRIEND			12									//ɾ������
#define SUB_CS_C_REFUSE				13									//�ܾ�����


#define SUB_CS_S_FRIEND_REQUEST		20									//��������
#define SUB_CS_S_AGREE_REQUEST		21									//��������ͬ��
#define SUB_CS_S_DEL_FRIEND			22									//ɾ������
#define SUB_CS_S_REFUSE				23									//�ܾ�����

//�㳡����
#define SUB_CS_S_PLAZA_INFO			100									//�㳡��Ϣ
#define SUB_CS_S_PLAZA_INSERT		101									//�㳡�б�
#define SUB_CS_S_PLAZA_REMOVE		103									//�㳡ɾ��
#define SUB_CS_S_PLAZA_FINISH		104									//�㳡���

//��������
#define SUB_CS_S_SERVER_INFO		110									//������Ϣ
#define SUB_CS_S_SERVER_ONLINE		111									//��������
#define SUB_CS_S_SERVER_INSERT		112									//�����б�
#define SUB_CS_S_SERVER_MODIFY		113									//�����޸�
#define SUB_CS_S_SERVER_REMOVE		114									//����ɾ��
#define SUB_CS_S_SERVER_FINISH		115									//�������

#define SUB_CS_S_MATCH_INSERT		116									//�����б�
//////////////////////////////////////////////////////////////////////////////////
//��������
struct CMD_CS_C_ServerModify
{
	WORD							wSortID;							//��������
	WORD							wKindID;							//��������
	WORD							wNodeID;							//�ڵ�����
	WORD							wServerPort;						//����˿�
	DWORD							dwOnLineCount;						//��������
	DWORD							dwFullCount;						//��Ա����
	TCHAR							szServerAddr[32];					//�����ַ
	TCHAR							szServerName[LEN_SERVER];			//��������
};

//��������
struct CMD_CS_C_ServerOnLine
{
	DWORD							dwOnLineCount;						//��������
};

//�û���ȡ���а�
struct CMD_CS_C_UserRankGet
{
	DWORD							dwSocketID;							//
};

//////////////////////////////////////////////////////////////////////////////////

//�㳡ɾ��
struct CMD_CS_S_PlazaRemove
{
	DWORD							dwPlazaID;							//�㳡��ʶ
};

//��������
struct CMD_CS_S_ServerOnLine
{
	WORD							wServerID;							//�����ʶ
	DWORD							dwOnLineCount;						//��������
};

//�����޸�
struct CMD_CS_S_ServerModify
{
	WORD							wSortID;							//��������
	WORD							wKindID;							//��������
	WORD							wNodeID;							//�ڵ�����
	WORD							wServerID;							//�����ʶ
	WORD							wServerPort;						//����˿�
	DWORD							dwOnLineCount;						//��������
	DWORD							dwFullCount;						//��Ա����
	TCHAR							szServerAddr[32];					//�����ַ
	TCHAR							szServerName[LEN_SERVER];			//��������
};

//����ɾ��
struct CMD_CS_S_ServerRemove
{
	WORD							wServerID;							//�����ʶ
};

//�����������
struct CMD_CS_S_UserRankGroup
{
	DWORD							dwSocketID;							// �����û�
	int								iGroup;								// ����
	TCHAR							szName[64];							// ��������
	int								iYear;								// ��
	int								iMon;								// ��
	int								iDay;								// ��
};


//������������µ���
struct CMD_CS_S_UserRank
{
	DWORD							dwSocketID;							// �����û�
	int								iGroup;								// ��Ӧ����
	TCHAR							szNickName[LEN_NICKNAME];			// �ǳ�
	LONGLONG						lScore;								// ����
};
//////////////////////////////////////////////////////////////////////////////////
//�û�����

#define MDM_CS_USER_COLLECT			3									//�û�����

//�û�״̬
#define SUB_CS_C_USER_ENTER			1									//�û�����
#define SUB_CS_C_USER_LEAVE			2									//�û��뿪
#define SUB_CS_C_USER_FINISH		3									//�û����

#define SUB_CS_C_USER_BLOCK			4									//�û�����

//�û�״̬
#define SUB_CS_S_COLLECT_REQUEST	100									//��������

#define SUB_CS_S_USER_BLOCK			101									//�û�������Ϣ

//////////////////////////////////////////////////////////////////////////////////

//�û�����
struct CMD_CS_C_UserEnter
{
	//�û���Ϣ
	DWORD							dwUserID;							//�û���ʶ
	DWORD							dwGameID;							//��Ϸ��ʶ
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�

	//������Ϣ
	BYTE							cbGender;							//�û��Ա�
	BYTE							cbMemberOrder;						//��Ա�ȼ�
	BYTE							cbMasterOrder;						//����ȼ�
};

//�û��뿪
struct CMD_CS_C_UserLeave
{
	DWORD							dwUserID;							//�û���ʶ
};

struct CMD_CS_C_UserBlock
{
	DWORD							dwSocket;							//�û�socket
	DWORD							dwUserID;							//�û���ʶ
	WORD							wCurServerID;						//��ǰ�����ʶ
	WORD							wKindID;							//�����ʶ
	WORD							wServerID;							//��������
	TCHAR							szErrorMsg[128];					//��������
};

struct CMD_CS_S_UserBlock
{
	DWORD							dwSocket;							//�û�socket
	DWORD							dwUserID;							//�û���ʶ
	WORD							wCurServerID;						//��ǰ�����ʶ
	WORD							wServerID;							//�����ʶ
	TCHAR							szAddr[32];							//�����ַ
	WORD							wPort;								//��������
	TCHAR							szErrorMsg[128];					//��������
};

//////////////////////////////////////////////////////////////////////////////////
//Զ�̷���

#define MDM_CS_REMOTE_SERVICE		4									//Զ�̷���

//���ҷ���
#define SUB_CS_C_SEARCH_DATABASE	1									//���ݲ���
#define SUB_CS_C_SEARCH_CORRESPOND	2									//Э������
#define SUB_CS_C_SEND_RED			3									//Э�����
#define SUB_CS_C_GRAP_RED			4									//Э�������
#define SUB_CS_C_INVITE_FRIEND		5									//�������

//�洢
#define SUB_CS_SAVE_REDINFO		6									//�洢�������

#define SUB_CS_C_APPLY_RED			7									//������


//���ҽ��
#define SUB_CS_S_SEARCH_DATABASE	100									//���ݲ���
#define SUB_CS_S_SEARCH_CORRESPOND	101									//Э������
#define SUB_CS_S_SEND_RED			102									//Э�����
#define SUB_CS_S_GRAP_RED			103									//Э�������
#define SUB_CS_S_INVITE_FRIEND		104									//�������
#define SUB_CS_S_PUSH_RED			105									//���ͺ��
#define SUB_CS_S_DELETE_RED			106									//ɾ�����

#define SUB_CS_S_USER_CLEAR			200									//�ߵ����

//�ߵ����
struct CMD_CS_S_ClearUser
{
	DWORD							dwServerID;							//����ID
	DWORD							dwUserID;							//UserID
	DWORD							dwLimitTime;						//��ʱ
};

//////////////////////////////////////////////////////////////////////////////////

//Э������
struct CMD_CS_C_SearchCorrespond
{
	//���Ӳ���
	DWORD							dwSocketID;							//�����ʶ
	DWORD							dwClientAddr;						//���ӵ�ַ

	//��������
	DWORD							dwGameID;							//��Ϸ��ʶ
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�
};

//Э������
struct CMD_CS_S_SearchCorrespond
{
	//������Ϣ
	DWORD							dwSocketID;							//�����ʶ
	DWORD							dwClientAddr;						//���ӵ�ַ

	//������Ϣ
	WORD							wUserCount;							//�û���Ŀ
	tagUserRemoteInfo				UserRemoteInfo[16];					//�û���Ϣ
};

//Э�����
struct CMD_CS_C_SendRed
{
	DWORD dwSocketID;			//�����ʶ
	DWORD dwSendUserID;			//��������ID
	DWORD dwSendUserNum;		//���������
	DWORD dwSendScore;			//������
	BYTE cbRedType;			//�������
	TCHAR szSendContent[128];	//�����������
};

//Э��������
struct CMD_CS_S_SendRed
{
	DWORD dwSocketID;			//�����ʶ
	BYTE  cbSuccess;			//�Ƿ�ɹ�
	DWORD dwSendUserID;			//��������ID
	DWORD dwSendUserNum;		//���������
	DWORD dwSendScore;			//������
	BYTE  cbRedType;			//�������
	TCHAR szSendContent[128];	//�����������
};

//�����Ϣ��map�洢��
struct CMD_MB_MAP_SENDRED
{
	DWORD dwSendUserID;			//��������ID
	DWORD dwSendUserNum;		//���������
	DWORD dwSendScore;			//������
	DWORD dwPerScore;			//ÿ���˿���ȡ�Ľ��
	DWORD dwReceiveCount;		//����ȡ����
	TCHAR szSendContent[128];	//�����������
};

//Э�������
struct CMD_CS_C_GrapRed
{
	DWORD dwSocketID;		//�����ʶ
	DWORD dwSendUserID;		//��������ID
	DWORD dwGrapUserID;		//��������ID
	DWORD dwGrapRedID;		//���ĺ��ID
};

//Э����������
struct CMD_CS_S_GrapRed
{
	DWORD dwSocketID;		//�����ʶ
	BYTE  cbSuccess;		//�Ƿ�ɹ�
	DWORD dwSendUserID;		//��������ID
	DWORD dwGrapUserID;		//��������ID
	DWORD dwRecvScore;		//�����Ľ��
	TCHAR szSendContent[128];	//�����������
	DWORD dwGrapRedID;		//���ĺ��ID
};

//Э��������
struct CMD_CS_C_ApplyRed
{
	DWORD dwSocketID;			//�����˵������ʶ
	DWORD dwUserID;				//�����û�ID
};

//Э��������
struct CMD_CS_S_ApplyRed
{
	DWORD dwSocketID;			//�����˵������ʶ
	BYTE  cbSuccess;			//�Ƿ�ɹ�
	DWORD dwUserID;				//�����û�ID
};

//ɾ�����
struct CMD_CS_S_DeleteRed
{
	DWORD dwSendUserID;			//�������ID
	DWORD dwResidueScore;		//���ʣ����
};

//////////////////////////////////////////////////////////////////////////////////
//�������

#define MDM_CS_MANAGER_SERVICE		5									//�������

#define SUB_CS_C_SYSTEM_MESSAGE 	1									//ϵͳ��Ϣ
#define SUB_CS_C_PROPERTY_TRUMPET 	2									//������Ϣ
#define SUB_CS_C_USER_CHAT			6									//�������

#define SUB_CS_S_SYSTEM_MESSAGE 	100									//ϵͳ��Ϣ
#define SUB_CS_S_PROPERTY_TRUMPET  	200									//������Ϣ
#define SUB_CS_S_GLAD_MESSAGE	  	300									//ϲ����Ϣ
#define SUB_CS_S_HIGH_SCORE_LIST	400									//��߷�����Ϣ�б�
#define SUB_CS_S_HIGH_SCORE_LIST_R	401									//��߷�����Ϣ�б�����
#define SUB_CS_S_HIGH_SCORE			500									//������߷�����Ϣ
#define SUB_CS_S_HIGH_SCORE_GET		600									//��ȡ�߷�����Ϣ
#define SUB_CS_S_USER_CHAT			700									//�������

//��������
struct CMD_CS_S_SendTrumpet
{
	WORD                           wPropertyIndex;                      //�������� 
	DWORD                          dwSendUserID;                         //�û� I D
	DWORD                          TrumpetColor;                        //������ɫ
	TCHAR                          szSendNickName[32];				    //����ǳ� 
	TCHAR                          szTrumpetContent[TRUMPET_MAX_CHAR];  //��������
};

//����ϲ��
struct CMD_CS_S_SendGladMsg
{
	TCHAR							szContent[TRUMPET_MAX_CHAR];		//����
	TCHAR							szNickName[LEN_NICKNAME];			//�ǳ�
	TCHAR							szNum[128];							//����
	DWORD							colText;							//������ɫ
	DWORD							colName;							//�ǳ���ɫ
	DWORD							colNum;								//������ɫ
};

//��������


//��������
#define MDM_CS_MATCH_SERVICE       6


//////////////////////////////////////////////////////////////////////////////////

#pragma pack()

#endif