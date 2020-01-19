inline void EnableMemLeakCheck()
{
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
}
#ifdef _DEBUG
#define new   new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

#include "StdAfx.h"
#include "Afxinet.h"
#include "TCPNetworkEngine.h"
#include "TraceServiceManager.h"

//////////////////////////////////////////////////////////////////////////
//�궨��

//ϵ������
#define DEAD_QUOTIETY				0									//����ϵ��
#define DANGER_QUOTIETY				1									//Σ��ϵ��
#define SAFETY_QUOTIETY				200									//��ȫϵ��

//��������
#define ASYNCHRONISM_SEND_DATA		1									//���ͱ�ʶ
#define ASYNCHRONISM_SEND_BATCH		2									//Ⱥ�巢��
#define ASYNCHRONISM_SHUT_DOWN		3									//��ȫ�ر�
#define ASYNCHRONISM_ALLOW_BATCH	4									//����Ⱥ��
#define ASYNCHRONISM_CLOSE_SOCKET	5									//�ر�����
#define ASYNCHRONISM_DETECT_SOCKET	6									//�������

//��������
#define SOCKET_INDEX(dwSocketID)	LOWORD(dwSocketID)					//λ������
#define SOCKET_ROUNTID(dwSocketID)	HIWORD(dwSocketID)					//ѭ������

//////////////////////////////////////////////////////////////////////////
//�ṹ����

//��������
struct tagSendDataRequest
{
	WORD							wIndex;								//��������
	WORD							wRountID;							//ѭ������
	WORD							wMainCmdID;							//��������
	WORD							wSubCmdID;							//��������
	WORD							wDataSize;							//���ݴ�С
	BYTE							cbSendBuffer[SOCKET_TCP_PACKET];		//���ͻ���
};

//Ⱥ������
struct tagBatchSendRequest
{
	WORD							wMainCmdID;							//��������
	WORD							wSubCmdID;							//��������
	WORD							wDataSize;							//���ݴ�С
	BYTE                            cbBatchMask;                        //��������
	BYTE							cbSendBuffer[SOCKET_TCP_PACKET];		//���ͻ���
};

//����Ⱥ��
struct tagAllowBatchSend
{
	WORD							wIndex;								//��������
	WORD							wRountID;							//ѭ������
	BYTE                            cbBatchMask;                        //��������
	BYTE							cbAllowBatch;						//�����־
};

//�ر�����
struct tagCloseSocket
{
	WORD							wIndex;								//��������
	WORD							wRountID;							//ѭ������
};

//��ȫ�ر�
struct tagShutDownSocket
{
	WORD							wIndex;								//��������
	WORD							wRountID;							//ѭ������
};

//////////////////////////////////////////////////////////////////////////

//���캯��
COverLapped::COverLapped(enOperationType OperationType) : m_OperationType(OperationType)
{
	//���ñ���
	ZeroMemory(&m_WSABuffer,sizeof(m_WSABuffer));
	ZeroMemory(&m_OverLapped,sizeof(m_OverLapped));

	return;
}

//��������
COverLapped::~COverLapped()
{
}

//////////////////////////////////////////////////////////////////////////

//���캯��
COverLappedSend::COverLappedSend() : COverLapped(enOperationType_Send)
{
	m_WSABuffer.len=0;
	m_WSABuffer.buf=(char *)m_cbBuffer;
}

//��������
COverLappedSend::~COverLappedSend()
{
}

//////////////////////////////////////////////////////////////////////////

//���캯��
COverLappedRecv::COverLappedRecv() : COverLapped(enOperationType_Recv)
{
	m_WSABuffer.len=0;
	m_WSABuffer.buf=NULL;
}

//��������
COverLappedRecv::~COverLappedRecv()
{
}

//////////////////////////////////////////////////////////////////////////

//���캯��
CTCPNetworkItem::CTCPNetworkItem(WORD wIndex, ITCPNetworkItemSink * pITCPNetworkItemSink) 
	: m_wIndex(wIndex), m_pITCPNetworkItemSink(pITCPNetworkItemSink)
{
	//��������
	m_dwClientIP=0L;
	m_dwActiveTime=0L;

	//���ı���
	m_wRountID=1;
	m_wSurvivalTime=0;
	m_wSurvivalTime=DEAD_QUOTIETY;
	m_hSocketHandle=INVALID_SOCKET;

	//״̬����
	m_bSendIng=false;
	m_bRecvIng=false;
	m_bShutDown=false;
	m_bAllowBatch=false;
	m_bBatchMask=0xFF;

	//���ձ���
	m_wRecvSize=0;
	ZeroMemory(m_cbRecvBuf,sizeof(m_cbRecvBuf));

	//��������
	m_dwSendTickCount=0L;
	m_dwRecvTickCount=0L;
	m_dwSendPacketCount=0L;
	m_dwRecvPacketCount=0L;

	//��������
	m_cbSendRound=0;
	m_cbRecvRound=0;
	m_dwSendXorKey=0;
	m_dwRecvXorKey=0;
	m_ConnectType = CHECKING;
	return;
}

//��������
CTCPNetworkItem::~CTCPNetworkItem()
{
	//ɾ������
	for (INT_PTR i=0;i<m_OverLappedSendBuffer.GetCount();i++)
	{
		delete m_OverLappedSendBuffer[i];
	}

	//ɾ���
	for (INT_PTR i=0;i<m_OverLappedSendActive.GetCount();i++)
	{
		delete m_OverLappedSendActive[i];
	}

	//ɾ������
	m_OverLappedSendBuffer.RemoveAll();
	m_OverLappedSendActive.RemoveAll();

	return;
}

//�󶨶���
DWORD CTCPNetworkItem::Attach(SOCKET hSocket, DWORD dwClientIP)
{
	//Ч�����
	ASSERT(dwClientIP!=0);
	ASSERT(hSocket!=INVALID_SOCKET);

	//Ч��״̬
	ASSERT(m_bRecvIng==false);
	ASSERT(m_bSendIng==false);
	ASSERT(m_hSocketHandle==INVALID_SOCKET);

	//״̬����
	m_bSendIng=false;
	m_bRecvIng=false;
	m_bShutDown=false;
	m_bAllowBatch=false;
	m_bBatchMask=0xFF;

	//���ñ���
	m_dwClientIP=dwClientIP;
	m_hSocketHandle=hSocket;
	m_wSurvivalTime=SAFETY_QUOTIETY;
	m_dwActiveTime=(DWORD)time(NULL);
	m_dwRecvTickCount=GetTickCount();

	//����֪ͨ
	m_pITCPNetworkItemSink->OnEventSocketBind(this);

	return GetIdentifierID();
}

//�ָ�����
DWORD CTCPNetworkItem::ResumeData()
{
	//Ч��״̬
	ASSERT(m_hSocketHandle==INVALID_SOCKET);

	//��������
	m_dwClientIP=0L;
	m_dwActiveTime=0L;

	//���ı���
	m_wSurvivalTime=0;
	m_hSocketHandle=INVALID_SOCKET;
	m_ConnectType = CHECKING;
	m_wRountID=__max(1,m_wRountID+1);

	//״̬����
	m_bSendIng=false;
	m_bRecvIng=false;
	m_bShutDown=false;
	m_bAllowBatch=false;

	//���ձ���
	m_wRecvSize=0;
	ZeroMemory(m_cbRecvBuf,sizeof(m_cbRecvBuf));

	//��������
	m_dwSendTickCount=0L;
	m_dwRecvTickCount=0L;
	m_dwSendPacketCount=0L;
	m_dwRecvPacketCount=0L;

	//��������
	m_cbSendRound=0;
	m_cbRecvRound=0;
	m_dwSendXorKey=0;
	m_dwRecvXorKey=0;

	//�ص�����
	m_OverLappedSendBuffer.Append(m_OverLappedSendActive);
	m_OverLappedSendActive.RemoveAll();

	return GetIdentifierID();
}

//���ͺ���
bool CTCPNetworkItem::SendData(WORD wMainCmdID, WORD wSubCmdID, WORD wRountID)
{
	//�����ж�
	if (SendVerdict(wRountID)==false) return false;

	if ( m_ConnectType == WEBSOCKET ){
		char cbRawData[SOCKET_TCP_BUFFER] = { 0 };
		char cbSendData[SOCKET_TCP_BUFFER] = { 0 };
		TCP_Head * pHead = (TCP_Head*)cbRawData;
		//��������
		pHead->TCPInfo.cbCheckCode = 1;
		pHead->TCPInfo.cbDataKind = 1;

		pHead->CommandInfo.wSubCmdID = wSubCmdID;
		pHead->CommandInfo.wMainCmdID = wMainCmdID;
		
		int lenght = 8 ;
		pHead->TCPInfo.wPacketSize = lenght;
		int encodesize = zl::net::ws::encodeFrame(zl::net::ws::WsFrameType::WS_BINARY_FRAME, cbRawData, lenght, cbSendData, SOCKET_TCP_BUFFER);
		if (encodesize > SOCKET_TCP_BUFFER) encodesize = SOCKET_TCP_BUFFER;
		//TCHAR szBuffer2[128];
		//_sntprintf(szBuffer2, CountArray(szBuffer2), TEXT("SendData lenght:%d,wDataSize:%d,encodesize:%d"), lenght, wDataSize, encodesize);
		//g_TraceServiceManager.TraceString(szBuffer2, TraceLevel_Exception);
		WS_SendRawData(cbSendData, encodesize);
	}
	else{
		//��ȡ����
		WORD wPacketSize = sizeof(TCP_Head);
		COverLappedSend * pOverLappedSend = GetSendOverLapped(wPacketSize);

		//�ر��ж�
		if (pOverLappedSend == NULL)
		{
			CloseSocket(wRountID);
			return false;
		}

		//��������
		WORD wSourceLen = (WORD)pOverLappedSend->m_WSABuffer.len;
		TCP_Head * pHead = (TCP_Head *)(pOverLappedSend->m_cbBuffer + wSourceLen);

		//��������
		pHead->CommandInfo.wSubCmdID = wSubCmdID;
		pHead->CommandInfo.wMainCmdID = wMainCmdID;

		//��������
		WORD wEncryptLen = EncryptBuffer(pOverLappedSend->m_cbBuffer + wSourceLen, wPacketSize, sizeof(pOverLappedSend->m_cbBuffer) - wSourceLen);
		pOverLappedSend->m_WSABuffer.len += wEncryptLen;

		//��������
		if (m_bSendIng == false)
		{
			//��������
			DWORD dwThancferred = 0;
			INT nResultCode = WSASend(m_hSocketHandle, &pOverLappedSend->m_WSABuffer, 1, &dwThancferred, 0, &pOverLappedSend->m_OverLapped, NULL);

			//�������
			if ((nResultCode == SOCKET_ERROR) && (WSAGetLastError() != WSA_IO_PENDING))
			{
				CloseSocket(m_wRountID);
				return false;
			}

			//���ñ���
			m_bSendIng = true;
		}
	}

	return true;
}

//���ͺ���
bool CTCPNetworkItem::SendData(VOID * pData, WORD wDataSize, WORD wMainCmdID, WORD wSubCmdID, WORD wRountID)
{
	//Ч�����
	ASSERT(wDataSize<=SOCKET_TCP_PACKET);

	//�����ж�
	if (wDataSize>SOCKET_TCP_PACKET) return false;
	if (SendVerdict(wRountID)==false) return false;

	if (m_ConnectType == WEBSOCKET){
		
		char cbRawData[SOCKET_TCP_BUFFER] = {0};
		char cbSendData[SOCKET_TCP_BUFFER] = { 0 };
		TCP_Head * pHead = (TCP_Head*)cbRawData;
		//��������
		pHead->TCPInfo.cbCheckCode = 1;
		pHead->TCPInfo.cbDataKind = 1;
		
		pHead->CommandInfo.wSubCmdID = wSubCmdID;
		pHead->CommandInfo.wMainCmdID = wMainCmdID;
		//��������
 		if (wDataSize > 0)
 		{
 			ASSERT(pData != NULL);
 			CopyMemory(pHead + 1, pData, wDataSize);
 		}
		int lenght = 8+ wDataSize;
		pHead->TCPInfo.wPacketSize = lenght;
		int encodesize = zl::net::ws::encodeFrame(zl::net::ws::WsFrameType::WS_BINARY_FRAME, cbRawData, lenght, cbSendData, SOCKET_TCP_BUFFER);
		if (encodesize > SOCKET_TCP_BUFFER) encodesize = SOCKET_TCP_BUFFER;
		//TCHAR szBuffer2[128];
		//_sntprintf(szBuffer2, CountArray(szBuffer2), TEXT("SendData lenght:%d,wDataSize:%d,encodesize:%d"), lenght, wDataSize, encodesize);
		//g_TraceServiceManager.TraceString(szBuffer2, TraceLevel_Exception);
		WS_SendRawData(cbSendData, encodesize);
		
		
		
// 		Json::Value root;
// 		root["mainID"] = wMainCmdID;
// 		root["subID"] = wSubCmdID;
// 		std::string data = root.toStyledString();
// 		std::string sendData = data;
// 		char cbSendData[SOCKET_TCP_BUFFER];
// 		int encodesize = zl::net::ws::encodeFrame(zl::net::ws::WsFrameType::WS_TEXT_FRAME, sendData.c_str(), sendData.size(), cbSendData, SOCKET_TCP_BUFFER);
// 		WS_SendRawData(cbSendData, encodesize);

	}
	else{
		//��ȡ����
		WORD wPacketSize = sizeof(TCP_Head) + wDataSize;
		COverLappedSend * pOverLappedSend = GetSendOverLapped(wPacketSize);

		//�ر��ж�
		if (pOverLappedSend == NULL)
		{
			CloseSocket(wRountID);
			return false;
		}

		//��������
		WORD wSourceLen = (WORD)pOverLappedSend->m_WSABuffer.len;
		TCP_Head * pHead = (TCP_Head *)(pOverLappedSend->m_cbBuffer + wSourceLen);

		//���ñ���
		pHead->CommandInfo.wSubCmdID = wSubCmdID;
		pHead->CommandInfo.wMainCmdID = wMainCmdID;

		//��������
		if (wDataSize > 0)
		{
			ASSERT(pData != NULL);
			CopyMemory(pHead + 1, pData, wDataSize);
		}

		//��������
		WORD wEncryptLen = EncryptBuffer(pOverLappedSend->m_cbBuffer + wSourceLen, wPacketSize, sizeof(pOverLappedSend->m_cbBuffer) - wSourceLen);
		pOverLappedSend->m_WSABuffer.len += wEncryptLen;

		//��������
		if (m_bSendIng == false)
		{
			//��������
			DWORD dwThancferred = 0;
			INT nResultCode = WSASend(m_hSocketHandle, &pOverLappedSend->m_WSABuffer, 1, &dwThancferred, 0, &pOverLappedSend->m_OverLapped, NULL);

			//�������
			if ((nResultCode == SOCKET_ERROR) && (WSAGetLastError() != WSA_IO_PENDING))
			{
				CloseSocket(m_wRountID);
				return false;
			}

			//���ñ���
			m_bSendIng = true;
		}
	}

	return true;
}

bool CTCPNetworkItem::WS_SendRawData(const char* data, int len) {
	//Ѱ�ҷ��ͽṹ
	COverLappedSend * pOverLappedSend = GetSendOverLapped(0);
	ASSERT(pOverLappedSend != NULL);

	if (pOverLappedSend == NULL) {
		return false;
	}

	ZeroMemory(pOverLappedSend->m_cbBuffer, sizeof(pOverLappedSend->m_cbBuffer));
	for (int i = 0; i < len; i++) {
		pOverLappedSend->m_cbBuffer[i] = data[i];
	}

	pOverLappedSend->m_WSABuffer.len = len;

	//��������
	if (m_bSendIng == false)
	{
		//��������
		DWORD dwThancferred = 0;
		INT nResultCode = WSASend(m_hSocketHandle, &pOverLappedSend->m_WSABuffer, 1, &dwThancferred, 0, &pOverLappedSend->m_OverLapped, NULL);

		//�������
		if ((nResultCode == SOCKET_ERROR) && (WSAGetLastError() != WSA_IO_PENDING))
		{
			CloseSocket(m_wRountID);
			return false;
		}
		//g_TraceServiceManager.TraceString(TEXT("����WebSocket��Ϣ�ɹ�"), TraceLevel_Exception);
		//���ñ���
		m_bSendIng = true;
	}

	return true;
}

//Ͷ�ݽ���
bool CTCPNetworkItem::RecvData()
{
	//Ч�����
	ASSERT(m_bRecvIng==false);
	ASSERT(m_hSocketHandle!=INVALID_SOCKET);

	//��������
	DWORD dwThancferred=0,dwFlags=0;
	INT nResultCode=WSARecv(m_hSocketHandle,&m_OverLappedRecv.m_WSABuffer,1,&dwThancferred,&dwFlags,&m_OverLappedRecv.m_OverLapped,NULL);

	//�������
	if ((nResultCode==SOCKET_ERROR)&&(WSAGetLastError()!=WSA_IO_PENDING))
	{
		CloseSocket(m_wRountID);
		return false;
	}

	//���ñ���
	m_bRecvIng=true;

	return true;
}

//�ر�����
bool CTCPNetworkItem::CloseSocket(WORD wRountID)
{
	//״̬�ж�
	if (m_wRountID!=wRountID) return false;

	//�ر�����
	if (m_hSocketHandle!=INVALID_SOCKET)
	{
		closesocket(m_hSocketHandle);
		m_hSocketHandle=INVALID_SOCKET;
	}

	//�жϹر�
	if ((m_bRecvIng==false)&&(m_bSendIng==false))
	{
		OnCloseCompleted();
	}

	return true;
}

//���ùر�
bool CTCPNetworkItem::ShutDownSocket(WORD wRountID)
{
	//״̬�ж�
	if (m_hSocketHandle==INVALID_SOCKET) return false;
	if ((m_wRountID!=wRountID)||(m_bShutDown==true)) return false;

	//���ñ���
	m_wRecvSize=0;
	m_bShutDown=true;

	//��������
	//SendData(MDM_KN_COMMAND,SUB_KN_SHUT_DOWN_SOCKET,m_wRountID);

	return true;
}

//����Ⱥ��
bool CTCPNetworkItem::AllowBatchSend(WORD wRountID, bool bAllowBatch,BYTE cbBatchMask)
{
	//״̬�ж�
	if (m_wRountID!=wRountID) return false;
	if (m_hSocketHandle==INVALID_SOCKET) return false;

	//���ñ���
	m_bAllowBatch=bAllowBatch;

	m_bBatchMask=cbBatchMask;

	return true;
}

//�������
bool CTCPNetworkItem::OnSendCompleted(COverLappedSend * pOverLappedSend, DWORD dwThancferred)
{
	//Ч�����
	ASSERT(m_bSendIng==true);
	ASSERT(m_OverLappedSendActive.GetCount()>0);
	ASSERT(pOverLappedSend==m_OverLappedSendActive[0]);

	//�ͷŽṹ
	m_OverLappedSendActive.RemoveAt(0);
	m_OverLappedSendBuffer.Add(pOverLappedSend);

	//���ñ���
	m_bSendIng=false;

	//�жϹر�
	if (m_hSocketHandle==INVALID_SOCKET)
	{
		CloseSocket(m_wRountID);
		return true;
	}

	//���ñ���
	if (dwThancferred!=0)
	{
		m_wSurvivalTime=SAFETY_QUOTIETY;
		m_dwSendTickCount=GetTickCount();
	}

	//��������
	if (m_OverLappedSendActive.GetCount()>0)
	{
		//��ȡ����
		pOverLappedSend=m_OverLappedSendActive[0];

		//��������
		DWORD dwThancferred=0;
		INT nResultCode=WSASend(m_hSocketHandle,&pOverLappedSend->m_WSABuffer,1,&dwThancferred,0,&pOverLappedSend->m_OverLapped,NULL);

		//�������
		if ((nResultCode==SOCKET_ERROR)&&(WSAGetLastError()!=WSA_IO_PENDING))
		{
			CloseSocket(m_wRountID);
			return false;
		}

		//���ñ���
		m_bSendIng=true;
	}

	return true;
}

//�������
bool CTCPNetworkItem::OnRecvCompleted(COverLappedRecv * pOverLappedRecv, DWORD dwThancferred)
{
	//Ч������
	ASSERT(m_bRecvIng==true);

	//���ñ���
	m_bRecvIng=false;

	//�жϹر�
	if (m_hSocketHandle==INVALID_SOCKET)
	{
		CloseSocket(m_wRountID);
		return true;
	}

	//��������
	INT nResultCode=recv(m_hSocketHandle,(char *)m_cbRecvBuf+m_wRecvSize,sizeof(m_cbRecvBuf)-m_wRecvSize,0);

	//�ر��ж�
	if (nResultCode<=0)
	{
		CloseSocket(m_wRountID);
		return true;
	}

	//�ж��ж�
	if (m_bShutDown==true) return true;

	//���ñ���
	m_wRecvSize+=nResultCode;
	m_wSurvivalTime=SAFETY_QUOTIETY;
	m_dwRecvTickCount=GetTickCount();

	if (m_ConnectType == CHECKING) {
		//const char szCRLF[] = "\r\n\r\n";
		const char szGet[] = "GET";
		const char szWebSocketKey[] = "Sec-WebSocket-Key";

		char* recvChar = (char*)m_cbRecvBuf;
		const char* pos = std::search(recvChar, recvChar + m_wRecvSize, szGet, szGet + 3);
		if (pos == recvChar + m_wRecvSize) { //�Ҳ���websocket
			m_ConnectType = WINSOCKET;
			//return RecvData();
		}
		else {
			pos = std::search(recvChar, recvChar + m_wRecvSize, szWebSocketKey, szWebSocketKey + 17);
			if ( pos == recvChar + m_wRecvSize) {
				m_ConnectType = WINSOCKET;
				//return RecvData();
			}
			m_ConnectType = HANDSHAKING;
		}

	}

	if ( m_ConnectType == HANDSHAKING ) { //websocket client - server��������
		char* recvChar = (char*)m_cbRecvBuf;
		zl::net::ByteBuffer handshakeBuffer;
		handshakeBuffer.write(recvChar,m_wRecvSize);
		bool success = WS_HandShake(&handshakeBuffer);

		if ( success == true ){
			m_ConnectType = WEBSOCKET;
			m_wRecvSize = 0;
			//g_TraceServiceManager.TraceString(TEXT("websocket�������"), TraceLevel_Exception);
			return RecvData(); 
		}
		else {
			CloseSocket(m_wRountID);
			return false;
		}
	}

	if ( m_ConnectType == WEBSOCKET ){
		if ( WS_HandleWebSocketRecv() == false ){
			CloseSocket(m_wRountID);
			return false;
		}
		return RecvData();
	}

	if ( m_ConnectType == WINSOCKET ) {
		//�������
		BYTE cbBuffer[SOCKET_TCP_BUFFER];
		TCP_Head * pHead = (TCP_Head *)m_cbRecvBuf;

		//��������
		try
		{
			while (m_wRecvSize >= sizeof(TCP_Head))
			{
				//Ч������
				WORD wPacketSize = pHead->TCPInfo.wPacketSize;

				//�����ж�
				if (wPacketSize > SOCKET_TCP_BUFFER) throw TEXT("���ݰ�����̫��");
				if (wPacketSize < sizeof(TCP_Head))
				{
					TCHAR szBuffer[512];
					_sntprintf(szBuffer, CountArray(szBuffer), TEXT("���ݰ�����̫�� %d,%d,%d,%d,%d,%d,%d,%d"), m_cbRecvBuf[0],
						m_cbRecvBuf[1],
						m_cbRecvBuf[2],
						m_cbRecvBuf[3],
						m_cbRecvBuf[4],
						m_cbRecvBuf[5],
						m_cbRecvBuf[6],
						m_cbRecvBuf[7]
					);
					g_TraceServiceManager.TraceString(szBuffer, TraceLevel_Exception);

					_sntprintf(szBuffer, CountArray(szBuffer), TEXT("���� %d, �汾 %d, Ч���� %d"), pHead->TCPInfo.wPacketSize,
						pHead->TCPInfo.cbDataKind, pHead->TCPInfo.cbCheckCode);

					g_TraceServiceManager.TraceString(szBuffer, TraceLevel_Exception);
					throw TEXT("���ݰ�����̫��");
				}
				if (pHead->TCPInfo.cbDataKind != DK_MAPPED && pHead->TCPInfo.cbDataKind != 0x05) {
					CString aa;
					aa.Format(TEXT("0x%x"), pHead->TCPInfo.cbDataKind);
					g_TraceServiceManager.TraceString(aa, TraceLevel_Exception);
					throw TEXT("���ݰ��汾��ƥ��");
				}
				//����ж�
				if (m_wRecvSize < wPacketSize) break;

				//��ȡ����
				CopyMemory(cbBuffer, m_cbRecvBuf, wPacketSize);
				WORD wRealySize = CrevasseBuffer(cbBuffer, wPacketSize);

				//���ñ���
				m_dwRecvPacketCount++;

				//��������
				LPVOID pData = cbBuffer + sizeof(TCP_Head);
				WORD wDataSize = wRealySize - sizeof(TCP_Head);
				TCP_Command Command = ((TCP_Head *)cbBuffer)->CommandInfo;

				//��Ϣ����
				if (Command.wMainCmdID != MDM_KN_COMMAND)	m_pITCPNetworkItemSink->OnEventSocketRead(Command, pData, wDataSize, this);

				//TCHAR szBuffer2[128];
				//_sntprintf(szBuffer2, CountArray(szBuffer2), TEXT("WinSocket wPacketSize: %d,m_wRecvSize:%d"), wPacketSize, m_wRecvSize);
				//g_TraceServiceManager.TraceString(szBuffer2, TraceLevel_Exception);
				//ɾ������
				m_wRecvSize -= wPacketSize;
				if (m_wRecvSize > 0) MoveMemory(m_cbRecvBuf, m_cbRecvBuf + wPacketSize, m_wRecvSize);
			}
		}
		catch (LPCTSTR pszMessage)
		{
			//������Ϣ
			TCHAR szString[512] = TEXT("");
			_sntprintf(szString, CountArray(szString), TEXT("SocketEngine Index=%ld��RountID=%ld��OnRecvCompleted ������%s���쳣"), m_wIndex, m_wRountID, pszMessage);
			g_TraceServiceManager.TraceString(szString, TraceLevel_Exception);

			//�ر�����
			CloseSocket(m_wRountID);

			return false;
		}
		catch (...)
		{
			//������Ϣ
			TCHAR szString[512] = TEXT("");
			_sntprintf(szString, CountArray(szString), TEXT("SocketEngine Index=%ld��RountID=%ld��OnRecvCompleted �������Ƿ����쳣"), m_wIndex, m_wRountID);
			g_TraceServiceManager.TraceString(szString, TraceLevel_Exception);

			//�ر�����
			CloseSocket(m_wRountID);

			return false;
		}
	}

	return RecvData();
}

//�ر����
bool CTCPNetworkItem::OnCloseCompleted()
{
	//Ч��״̬
	ASSERT(m_hSocketHandle==INVALID_SOCKET);
	ASSERT((m_bSendIng==false)&&(m_bRecvIng==false));

	//�ر��¼�
	m_pITCPNetworkItemSink->OnEventSocketShut(this);

	//�ָ�����
	ResumeData();

	return true;
}

//websocket����
bool CTCPNetworkItem::WS_HandShake(zl::net::ByteBuffer* ByteBuffer)
{
	zl::net::HttpContext context;
	if ( !context.parseRequest(ByteBuffer) || context.request().method() != zl::net::HttpGet ) {
		std::string msg = "HTTP/1.1 400 Bad Request\r\n\r\n";
		WS_SendRawData(msg.c_str(),msg.size());
		return false;
	}

	ASSERT(context.gotAll());
	zl::net::HttpRequest& req = context.request();
	std::string query = req.query();
	if ( query.empty() == false ) {
		query = query.substr(1);
	}

	std::string key = req.getHeader(zl::net::ws::kSecWebSocketKeyHeader);
	//û�д���IP
// 	std::string ip = req.getHeader("X-Forwarded-For");
// 	if ( ip.empty() == false ){
// 		m_dwClientIP = inet_addr(ip.c_str());
// 		m_pITCPNetworkItemSink->OnEventSocketBind(this);
// 	}

	std::string answer = zl::net::ws::makeHandshakeResponse(key.c_str(), req.getHeader(zl::net::ws::kSecWebSocketProtocolHeader));
	WS_SendRawData(answer.c_str(), answer.size());
	m_dwRecvPacketCount++;

	return true;
}

//websocket����
bool CTCPNetworkItem::WS_HandleWebSocketRecv() 
{
	using namespace zl::net::ws;

	if ( m_wRecvSize > SOCKET_TCP_BUFFER ) {
		return false;
	}
	try {
		while ( m_wRecvSize > 0 )
		{
			char outbuf[SOCKET_TCP_BUFFER] = {0};
			int outlen = 0;
			int frameSize = 0;
			WsFrameType type = decodeFrame((char*)m_cbRecvBuf, m_wRecvSize,outbuf,&outlen,&frameSize);

			
<<<<<<< .mine
			TCHAR szBuffer2[512];
			_sntprintf(szBuffer2, CountArray(szBuffer2), TEXT("outbuf: %s,outlen:%d,frameSize:%d"), outbuf, outlen, frameSize);
			g_TraceServiceManager.TraceString(szBuffer2, TraceLevel_Exception);

||||||| .r68

			//_sntprintf(szBuffer2, CountArray(szBuffer2), TEXT("outbuf: %s,outlen:%d,frameSize:%d"), outbuf, outlen, frameSize);
			//g_TraceServiceManager.TraceString(szBuffer2, TraceLevel_Exception);

=======
			//TCHAR szBuffer2[512];
			//_sntprintf(szBuffer2, CountArray(szBuffer2), TEXT("m_wRecvSize:%d,outlen:%d,frameSize:%d"), m_wRecvSize, outlen, frameSize);
			//g_TraceServiceManager.TraceString(szBuffer2, TraceLevel_Exception);

>>>>>>> .r71
			switch ( type ) {
// 			case WS_INCOMPLETE_TEXT_FRAME:
// 			case WS_INCOMPLETE_BINARY_FRAME:
// 			case WS_INCOMPLETE_FRAME: {
// 				return true;
// 			}
			case WS_TEXT_FRAME:
			case WS_BINARY_FRAME: {
				//�������
				BYTE cbBuffer[SOCKET_TCP_BUFFER] = { 0 };
				TCP_Head * pHead = (TCP_Head *)outbuf;

				//��������
				while (outlen > sizeof(TCP_Head))
				{

					
					//_sntprintf(szBuffer2, CountArray(szBuffer2), TEXT("m_cbRecvBuf: %s,m_wRecvSize:%d"), m_cbRecvBuf, m_wRecvSize);
					//g_TraceServiceManager.TraceString(szBuffer2, TraceLevel_Exception);
					//for (int i = 0; i < m_wRecvSize; ++i)
					//{
					//	_sntprintf(szBuffer2, CountArray(szBuffer2), TEXT("m_cbRecvBuf[%d]:%d"), i, m_cbRecvBuf[i]);
					//	g_TraceServiceManager.TraceString(szBuffer2, TraceLevel_Exception);
					//}

					//Ч������
					WORD wPacketSize = pHead->TCPInfo.wPacketSize;

					//_sntprintf(szBuffer2, CountArray(szBuffer2), TEXT("m_wRecvSize:%d,outlen:%d,frameSize:%d,wPacketSize:%d"), m_wRecvSize, outlen, frameSize,wPacketSize);
					//g_TraceServiceManager.TraceString(szBuffer2, TraceLevel_Exception);

					//�����ж�
					if (wPacketSize > SOCKET_TCP_BUFFER) throw TEXT("���ݰ�����̫��");
					// 						if (wPacketSize < sizeof(TCP_Head))
					// 						{
					// 							TCHAR szBuffer[512];
					// 							_sntprintf(szBuffer, CountArray(szBuffer), TEXT("���ݰ�����̫�� %d,%d,%d,%d,%d,%d,%d,%d"), outbuf[0],
					// 								outbuf[1],
					// 								outbuf[2],
					// 								outbuf[3],
					// 								outbuf[4],
					// 								outbuf[5],
					// 								outbuf[6],
					// 								outbuf[7]
					// 							);
					// 							g_TraceServiceManager.TraceString(szBuffer, TraceLevel_Exception);
					// 
					// 							_sntprintf(szBuffer, CountArray(szBuffer), TEXT("���� %d, �汾 %d, Ч���� %d"), pHead->TCPInfo.wPacketSize,
					// 								pHead->TCPInfo.cbDataKind, pHead->TCPInfo.cbCheckCode);
					// 
					// 							g_TraceServiceManager.TraceString(szBuffer, TraceLevel_Exception);
					// 							throw TEXT("���ݰ�����̫��");
					// 						}
					// 						if (pHead->TCPInfo.cbDataKind != DK_MAPPED && pHead->TCPInfo.cbDataKind != 0x05) {
					// 							CString aa;
					// 							aa.Format(TEXT("0x%x"), pHead->TCPInfo.cbDataKind);
					// 							g_TraceServiceManager.TraceString(aa, TraceLevel_Exception);
					// 							throw TEXT("���ݰ��汾��ƥ��");
					// 						}
					//����ж�
					//if (m_wRecvSize < wPacketSize) break;

					//��ȡ����
					CopyMemory(cbBuffer, outbuf, wPacketSize);
					//WORD wRealySize = CrevasseBuffer(cbBuffer, wPacketSize);
					WORD wRealySize = wPacketSize;

					//���ñ���
					m_dwRecvPacketCount++;

					TCHAR szBuffer3[128];

					//��������
					LPVOID pData = cbBuffer + sizeof(TCP_Head);
					WORD wDataSize = wRealySize - sizeof(TCP_Head);
					TCP_Command Command = ((TCP_Head *)cbBuffer)->CommandInfo;
					_sntprintf(szBuffer3, CountArray(szBuffer3),TEXT("M_Cmd:%d,S_Cmd:%d"), Command.wMainCmdID,Command.wSubCmdID);
					g_TraceServiceManager.TraceString(szBuffer3, TraceLevel_Exception);

					//��Ϣ����
					if (Command.wMainCmdID != MDM_KN_COMMAND)	m_pITCPNetworkItemSink->OnEventSocketRead(Command, pData, wDataSize, this);
					else {
						SendData(MDM_KN_COMMAND, SUB_KN_DETECT_SOCKET, m_wRountID);
					}

					//ɾ������
<<<<<<< .mine
					outlen -= wPacketSize;
 					if (outlen > 0) MoveMemory(outbuf, outbuf + frameSize, outlen);
||||||| .r68
					m_wRecvSize -= frameSize;
					if (m_wRecvSize > 0) MoveMemory(outbuf, outbuf + frameSize, m_wRecvSize);
=======
					outlen -= wPacketSize;
					if (outlen > 0) MoveMemory(outbuf, outbuf + wPacketSize, outlen);
>>>>>>> .r71
				}
				
				break;
			}
			case WS_PING_FRAME:
			case WS_PONG_FRAME:
				break;
			case WS_CLOSE_FRAME:
				return true;
			default:
				return true;

			}
			m_wRecvSize -= frameSize;
			MoveMemory(m_cbRecvBuf,m_cbRecvBuf + frameSize, m_wRecvSize);
		}
	}
	catch (...) {
		return false;
	}

	return true;
}

//��������
WORD CTCPNetworkItem::EncryptBuffer(BYTE pcbDataBuffer[], WORD wDataSize, WORD wBufferSize)
{
	WORD i = 0;
	//Ч�����
	ASSERT(wDataSize >= sizeof(TCP_Head));
	ASSERT(wDataSize <= (sizeof(TCP_Head) + SOCKET_TCP_BUFFER));
	ASSERT(wBufferSize >= (wDataSize + 2*sizeof(DWORD)));

	//��д��Ϣͷ
	TCP_Head * pHead = (TCP_Head *)pcbDataBuffer;
	pHead->TCPInfo.wPacketSize = wDataSize;
	pHead->TCPInfo.cbDataKind = DK_MAPPED;


	BYTE checkCode = 0;

	for (WORD i = sizeof(TCP_Info); i < wDataSize; i++)
	{
		checkCode += pcbDataBuffer[i];
		pcbDataBuffer[i] = MapSendByte(pcbDataBuffer[i]);
	}
	pHead->TCPInfo.cbCheckCode = ~checkCode+1;

	//���ñ���
	m_dwSendPacketCount++;

	return wDataSize;
}

//��������
WORD CTCPNetworkItem::CrevasseBuffer(BYTE pcbDataBuffer[], WORD wDataSize)
{
	WORD i = 0;
	//Ч�����
	ASSERT(wDataSize >= sizeof(TCP_Head));
	ASSERT(((TCP_Head *)pcbDataBuffer)->TCPInfo.wPacketSize == wDataSize);

	//Ч�������ֽ�ӳ��
	TCP_Head * pHead = (TCP_Head *)pcbDataBuffer;
	for (i = sizeof(TCP_Info); i < wDataSize; i++)
	{
		pcbDataBuffer[i] = MapRecvByte(pcbDataBuffer[i]);
	}

	return wDataSize;
}

//���ӳ��
WORD CTCPNetworkItem::SeedRandMap(WORD wSeed)
{
  DWORD dwHold = wSeed;
  return (WORD)((dwHold = dwHold * 241103L + 2533101L) >> 16);
}

//ӳ�䷢������
BYTE CTCPNetworkItem::MapSendByte(BYTE const cbData)
{
	BYTE cbMap;
	cbMap = g_SendByteMap[cbData];
	return cbMap;
}

//ӳ���������
BYTE CTCPNetworkItem::MapRecvByte(BYTE const cbData)
{
	BYTE cbMap;
	cbMap = g_RecvByteMap[cbData];
	return cbMap;
}

//�����ж�
bool CTCPNetworkItem::SendVerdict(WORD wRountID)
{
	if ((m_wRountID!=wRountID)||(m_bShutDown==true)) return false;
	if ((m_hSocketHandle==INVALID_SOCKET)||(m_dwRecvPacketCount==0)) return false;

	return true;
}

//��ȡ���ͽṹ
COverLappedSend * CTCPNetworkItem::GetSendOverLapped(WORD wPacketSize)
{
	//�����ж�
	if (m_OverLappedSendActive.GetCount()>1 && wPacketSize > 0)
	{
		INT_PTR nActiveCount=m_OverLappedSendActive.GetCount();
		COverLappedSend * pOverLappedSend=m_OverLappedSendActive[nActiveCount-1];
		if (sizeof(pOverLappedSend->m_cbBuffer)>=(pOverLappedSend->m_WSABuffer.len+wPacketSize+sizeof(DWORD)*2)) return pOverLappedSend;
	}

	//���ж���
	if (m_OverLappedSendBuffer.GetCount()>0)
	{
		//��ȡ����
		INT_PTR nFreeCount=m_OverLappedSendBuffer.GetCount();
		COverLappedSend * pOverLappedSend=m_OverLappedSendBuffer[nFreeCount-1];

		//���ñ���
		pOverLappedSend->m_WSABuffer.len=0;
		m_OverLappedSendActive.Add(pOverLappedSend);
		m_OverLappedSendBuffer.RemoveAt(nFreeCount-1);

		return pOverLappedSend;
	}

	try
	{
		//��������
		COverLappedSend * pOverLappedSend=new COverLappedSend;
		ASSERT(pOverLappedSend!=NULL);

		//���ñ���
		pOverLappedSend->m_WSABuffer.len=0;
		m_OverLappedSendActive.Add(pOverLappedSend);

		return pOverLappedSend;
	}
	catch (...) { ASSERT(FALSE); }

	return NULL;
}

//////////////////////////////////////////////////////////////////////////

//���캯��
CTCPNetworkThreadReadWrite::CTCPNetworkThreadReadWrite()
{
	m_hCompletionPort=NULL;
}

//��������
CTCPNetworkThreadReadWrite::~CTCPNetworkThreadReadWrite()
{
}

//���ú���
bool CTCPNetworkThreadReadWrite::InitThread(HANDLE hCompletionPort)
{
	ASSERT(hCompletionPort!=NULL);
	m_hCompletionPort=hCompletionPort;
	return true;
}

//���к���
bool CTCPNetworkThreadReadWrite::OnEventThreadRun()
{
	//Ч�����
	ASSERT(m_hCompletionPort!=NULL);

	//��������
	DWORD dwThancferred=0;					
	OVERLAPPED * pOverLapped=NULL;
	CTCPNetworkItem * pTCPNetworkItem=NULL;

	//��ɶ˿�
	BOOL bSuccess=GetQueuedCompletionStatus(m_hCompletionPort,&dwThancferred,(PULONG_PTR)&pTCPNetworkItem,&pOverLapped,INFINITE);
	if ((bSuccess==FALSE)&&(GetLastError()!=ERROR_NETNAME_DELETED)) return false;

	//�˳��ж�
	if ((pTCPNetworkItem==NULL)&&(pOverLapped==NULL)) return false;

	//��������
	COverLapped * pSocketLapped=CONTAINING_RECORD(pOverLapped,COverLapped,m_OverLapped);

	//��������
	switch (pSocketLapped->GetOperationType())
	{
	case enOperationType_Send:	//���ݷ���
		{
			CWHDataLocker ThreadLock(pTCPNetworkItem->GetCriticalSection());
			pTCPNetworkItem->OnSendCompleted((COverLappedSend *)pSocketLapped,dwThancferred);
			break;
		}
	case enOperationType_Recv:	//���ݶ�ȡ
		{
			CWHDataLocker ThreadLock(pTCPNetworkItem->GetCriticalSection());
			pTCPNetworkItem->OnRecvCompleted((COverLappedRecv *)pSocketLapped,dwThancferred);
			break;
		}
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////

//���캯��
CTCPNetworkThreadAccept::CTCPNetworkThreadAccept()
{
	m_hCompletionPort=NULL;
	m_pTCPNetworkEngine=NULL;
	m_hListenSocket=INVALID_SOCKET;
}

//��������
CTCPNetworkThreadAccept::~CTCPNetworkThreadAccept()
{
}

//���ú���
bool CTCPNetworkThreadAccept::InitThread(HANDLE hCompletionPort, SOCKET hListenSocket, CTCPNetworkEngine * pNetworkEngine)
{
	//Ч�����
	ASSERT(pNetworkEngine!=NULL);
	ASSERT(hCompletionPort!=NULL);
	ASSERT(hListenSocket!=INVALID_SOCKET);

	//���ñ���
	m_hListenSocket=hListenSocket;
	m_hCompletionPort=hCompletionPort;
	m_pTCPNetworkEngine=pNetworkEngine;

	return true;
}

//���к���
bool CTCPNetworkThreadAccept::OnEventThreadRun()
{
	//Ч�����
	ASSERT(m_hCompletionPort!=NULL);
	ASSERT(m_pTCPNetworkEngine!=NULL);

	//��������
	SOCKET hConnectSocket=INVALID_SOCKET;
	CTCPNetworkItem * pTCPNetworkItem=NULL;

	try
	{
		//��������
		SOCKADDR_IN	SocketAddr;
		INT nBufferSize=sizeof(SocketAddr);
		hConnectSocket=WSAAccept(m_hListenSocket,(SOCKADDR *)&SocketAddr,&nBufferSize,NULL,NULL);

		//�˳��ж�
		if (hConnectSocket==INVALID_SOCKET) return false;

		//��ȡ����
		pTCPNetworkItem=m_pTCPNetworkEngine->ActiveNetworkItem();

		//ʧ���ж�
		if (pTCPNetworkItem==NULL)
		{
			ASSERT(FALSE);
			throw TEXT("�������Ӷ���ʧ��");
		}

		//��������
		CWHDataLocker ThreadLock(pTCPNetworkItem->GetCriticalSection());

		//�󶨶���
		pTCPNetworkItem->Attach(hConnectSocket,SocketAddr.sin_addr.S_un.S_addr);
		CreateIoCompletionPort((HANDLE)hConnectSocket,m_hCompletionPort,(ULONG_PTR)pTCPNetworkItem,0);

		//�������
		pTCPNetworkItem->RecvData();
	}
	catch (...)
	{
		//�������
		ASSERT(pTCPNetworkItem==NULL);

		//�ر�����
		if (hConnectSocket!=INVALID_SOCKET)
		{
			closesocket(hConnectSocket);
		}
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////

//���캯��
CTCPNetworkThreadDetect::CTCPNetworkThreadDetect()
{
	m_dwPileTime=0L;
	m_dwDetectTime=10000L;
	m_pTCPNetworkEngine=NULL;
}

//��������
CTCPNetworkThreadDetect::~CTCPNetworkThreadDetect()
{
}

//���ú���
bool CTCPNetworkThreadDetect::InitThread(CTCPNetworkEngine * pNetworkEngine, DWORD dwDetectTime)
{
	//Ч�����
	ASSERT(pNetworkEngine!=NULL);

	//���ñ���
	m_dwPileTime=0L;
	m_dwDetectTime=dwDetectTime;
	m_pTCPNetworkEngine=pNetworkEngine;

	return true;
}

//���к���
bool CTCPNetworkThreadDetect::OnEventThreadRun()
{
	//Ч�����
	ASSERT(m_pTCPNetworkEngine!=NULL);

	//���ü��
	Sleep(200);
	m_dwPileTime+=200L;

	//�������
	if (m_dwPileTime>=m_dwDetectTime)
	{
		m_dwPileTime=0L;
		m_pTCPNetworkEngine->DetectSocket();
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////

//���캯��
CTCPNetworkEngine::CTCPNetworkEngine()
{
	//��֤����
	m_bValidate=false;
	m_bNormalRun=true;

	//��������
	m_bService=false;
	ZeroMemory(m_cbBuffer,sizeof(m_cbBuffer));

	//���ñ���
	m_wMaxConnect=0;
	m_wServicePort=0;
	m_dwDetectTime=10000L;

	//�ں˱���
	m_hCompletionPort=NULL;
	m_hServerSocket=INVALID_SOCKET;
	m_pITCPNetworkEngineEvent=NULL;

	return;
}

//��������
CTCPNetworkEngine::~CTCPNetworkEngine()
{
}

//�ӿڲ�ѯ
VOID * CTCPNetworkEngine::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IServiceModule,Guid,dwQueryVer);
	QUERYINTERFACE(ITCPNetworkEngine,Guid,dwQueryVer);
	QUERYINTERFACE(IAsynchronismEngineSink,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITCPNetworkEngine,Guid,dwQueryVer);
	return NULL;
}

//��������
bool CTCPNetworkEngine::StartService()
{
	//״̬Ч��
	ASSERT(m_bService==false);
	if (m_bService==true) return false;

	//Ч�����
	ASSERT((m_wMaxConnect!=0)&&(m_wServicePort!=0));
	if ((m_wMaxConnect==0)||(m_wServicePort==0)) return false;

	//ϵͳ��Ϣ
	SYSTEM_INFO SystemInfo;
	GetSystemInfo(&SystemInfo);
	DWORD dwThreadCount=SystemInfo.dwNumberOfProcessors;

	//��ɶ˿�
	m_hCompletionPort=CreateIoCompletionPort(INVALID_HANDLE_VALUE,NULL,NULL,SystemInfo.dwNumberOfProcessors);
	if (m_hCompletionPort==NULL) return false;

	//��������
	SOCKADDR_IN SocketAddr;
	ZeroMemory(&SocketAddr,sizeof(SocketAddr));

	//��������
	SocketAddr.sin_family=AF_INET;
	SocketAddr.sin_addr.s_addr=INADDR_ANY;
	SocketAddr.sin_port=htons(m_wServicePort);
	m_hServerSocket=WSASocket(AF_INET,SOCK_STREAM,IPPROTO_TCP,NULL,0,WSA_FLAG_OVERLAPPED);

	//�����ж�
	if (m_hServerSocket==INVALID_SOCKET) 
	{
		LPCTSTR pszString=TEXT("ϵͳ��Դ������� TCP/IP Э��û�а�װ����������ʧ��");
		g_TraceServiceManager.TraceString(pszString,TraceLevel_Exception);
		return false;
	}

	//������
	if (bind(m_hServerSocket,(SOCKADDR*)&SocketAddr,sizeof(SocketAddr))==SOCKET_ERROR)
	{
		LPCTSTR pszString=TEXT("����󶨷���������������ʧ��");
		g_TraceServiceManager.TraceString(pszString,TraceLevel_Exception);
		return false;
	}

	//�����˿�
	if (listen(m_hServerSocket,200)==SOCKET_ERROR)
	{
		TCHAR szString[512]=TEXT("");
		_sntprintf(szString,CountArray(szString),TEXT("�˿�������������ռ�ã����� %ld �˿�ʧ��"),m_wServicePort);
		g_TraceServiceManager.TraceString(szString,TraceLevel_Exception);
		return false;
	}

	//�첽����
	IUnknownEx * pIUnknownEx=QUERY_ME_INTERFACE(IUnknownEx);
	if (m_AsynchronismEngine.SetAsynchronismSink(pIUnknownEx)==false)
	{
		ASSERT(FALSE);
		return false;
	}

	//��ҳ��֤
	WebAttestation();

	//��������
	if (m_AsynchronismEngine.StartService()==false)
	{
		ASSERT(FALSE);
		return false;
	}

	//��д�߳�
	for (DWORD i=0;i<dwThreadCount;i++)
	{
		CTCPNetworkThreadReadWrite * pNetworkRSThread=new CTCPNetworkThreadReadWrite();
		if (pNetworkRSThread->InitThread(m_hCompletionPort)==false) return false;
		m_SocketRWThreadArray.Add(pNetworkRSThread);
	}

	//Ӧ���߳�
	if (m_SocketAcceptThread.InitThread(m_hCompletionPort,m_hServerSocket,this)==false) return false;

	//��д�߳�
	for (DWORD i = 0; i<dwThreadCount; i++)
	{
		CTCPNetworkThreadReadWrite * pNetworkRSThread=m_SocketRWThreadArray[i];
		ASSERT(pNetworkRSThread!=NULL);
		if (pNetworkRSThread->StartThread()==false) return false;
	}

	//����߳�
	m_SocketDetectThread.InitThread(this,m_dwDetectTime);
	if (m_SocketDetectThread.StartThread()==false) return false;

	//Ӧ���߳�
	if (m_SocketAcceptThread.StartThread()==false) return false;

	//���ñ���
	m_bService=true;

	return true;
}

//ֹͣ����
bool CTCPNetworkEngine::ConcludeService()
{
	//���ñ���
	m_bService=false;

	//����߳�
	m_SocketDetectThread.ConcludeThread(INFINITE);

	//Ӧ���߳�
	if (m_hServerSocket!=INVALID_SOCKET)
	{
		closesocket(m_hServerSocket);
		m_hServerSocket=INVALID_SOCKET;
	}
	m_SocketAcceptThread.ConcludeThread(INFINITE);

	//�첽����
	m_AsynchronismEngine.ConcludeService();

	//��д�߳�
	INT_PTR nCount=m_SocketRWThreadArray.GetCount();
	if (m_hCompletionPort!=NULL)
	{
		for (INT_PTR i=0;i<nCount;i++) PostQueuedCompletionStatus(m_hCompletionPort,0,NULL,NULL);
	}
	for (INT_PTR i=0;i<nCount;i++)
	{
		CTCPNetworkThreadReadWrite * pSocketThread=m_SocketRWThreadArray[i];
		ASSERT(pSocketThread!=NULL);
		pSocketThread->ConcludeThread(INFINITE);
		SafeDelete(pSocketThread);
	}
	m_SocketRWThreadArray.RemoveAll();

	//��ɶ˿�
	if (m_hCompletionPort!=NULL)
	{
		CloseHandle(m_hCompletionPort);
		m_hCompletionPort=NULL;
	}

	//�ر�����
	CTCPNetworkItem * pTCPNetworkItem=NULL;
	for (INT_PTR i=0;i<m_NetworkItemActive.GetCount();i++)
	{
		pTCPNetworkItem=m_NetworkItemActive[i];
		pTCPNetworkItem->CloseSocket(pTCPNetworkItem->GetRountID());
		pTCPNetworkItem->ResumeData();
	}

	//��������
	m_NetworkItemBuffer.Append(m_NetworkItemActive);
	m_NetworkItemActive.RemoveAll();
	m_TempNetworkItemArray.RemoveAll();

	return true;
}

//���ò���
bool CTCPNetworkEngine::SetServiceParameter(WORD wServicePort, WORD wMaxConnect, LPCTSTR  pszCompilation)
{
	//״̬Ч��
	ASSERT(m_bService==false);
	if (m_bService==true) return false;

	//���ñ���
	ASSERT(wServicePort!=0);
	m_wMaxConnect=wMaxConnect;
	m_wServicePort=wServicePort;

	return true;
}

//���ö˿�
WORD CTCPNetworkEngine::GetServicePort()
{
	return m_wServicePort;
}

//��ǰ�˿�
WORD CTCPNetworkEngine::GetCurrentPort()
{
	return m_wServicePort;
}

//���ýӿ�
bool CTCPNetworkEngine::SetTCPNetworkEngineEvent(IUnknownEx * pIUnknownEx)
{
	//״̬Ч��
	ASSERT(m_bService==false);
	if (m_bService==true) return false;

	//��ѯ�ӿ�
	m_pITCPNetworkEngineEvent=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITCPNetworkEngineEvent);

	//�����ж�
	if (m_pITCPNetworkEngineEvent==NULL)
	{
		ASSERT(FALSE);
		return false;
	}

    return true;
}

//���ͺ���
bool CTCPNetworkEngine::SendData(DWORD dwSocketID, WORD wMainCmdID, WORD wSubCmdID)
{
	//��������
	CWHDataLocker ThreadLock(m_BufferLocked);
	tagSendDataRequest * pSendDataRequest=(tagSendDataRequest *)m_cbBuffer;
	
	//��������
	pSendDataRequest->wDataSize=0;
	pSendDataRequest->wSubCmdID=wSubCmdID;
	pSendDataRequest->wMainCmdID=wMainCmdID;
	pSendDataRequest->wIndex=SOCKET_INDEX(dwSocketID);
	pSendDataRequest->wRountID=SOCKET_ROUNTID(dwSocketID);

	//��������
	WORD wSendSize=sizeof(tagSendDataRequest)-sizeof(pSendDataRequest->cbSendBuffer);
	return m_AsynchronismEngine.PostAsynchronismData(ASYNCHRONISM_SEND_DATA,m_cbBuffer,wSendSize);
}

//���ͺ���
bool CTCPNetworkEngine::SendData(DWORD dwSocketID, WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//Ч������
	ASSERT((wDataSize+sizeof(TCP_Head))<=SOCKET_TCP_PACKET);
	if ((wDataSize+sizeof(TCP_Head))>SOCKET_TCP_PACKET) return false;

	//��������
	CWHDataLocker ThreadLock(m_BufferLocked);
	tagSendDataRequest * pSendDataRequest=(tagSendDataRequest *)m_cbBuffer;

	//��������
	pSendDataRequest->wDataSize=wDataSize;
	pSendDataRequest->wSubCmdID=wSubCmdID;
	pSendDataRequest->wMainCmdID=wMainCmdID;
	pSendDataRequest->wIndex=SOCKET_INDEX(dwSocketID);
	pSendDataRequest->wRountID=SOCKET_ROUNTID(dwSocketID);
	if (wDataSize>0)
	{
		ASSERT(pData!=NULL);
		CopyMemory(pSendDataRequest->cbSendBuffer,pData,wDataSize);
	}

	//��������
	WORD wSendSize=sizeof(tagSendDataRequest)-sizeof(pSendDataRequest->cbSendBuffer)+wDataSize;
	return m_AsynchronismEngine.PostAsynchronismData(ASYNCHRONISM_SEND_DATA,m_cbBuffer,wSendSize);
}

//��������
bool CTCPNetworkEngine::SendDataBatch(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize,BYTE cbBatchMask)
{
	//Ч������
	ASSERT((wDataSize+sizeof(TCP_Head))<=SOCKET_TCP_PACKET);
	if ((wDataSize+sizeof(TCP_Head))>SOCKET_TCP_PACKET) return false;

	//��������
	CWHDataLocker ThreadLock(m_BufferLocked);
	tagBatchSendRequest * pBatchSendRequest=(tagBatchSendRequest *)m_cbBuffer;

	//��������
	pBatchSendRequest->wMainCmdID=wMainCmdID;
	pBatchSendRequest->wSubCmdID=wSubCmdID;
	pBatchSendRequest->wDataSize=wDataSize;
	pBatchSendRequest->cbBatchMask=cbBatchMask;
	if (wDataSize>0)
	{
		ASSERT(pData!=NULL);
		CopyMemory(pBatchSendRequest->cbSendBuffer,pData,wDataSize);
	}

	//��������
	WORD wSendSize=sizeof(tagBatchSendRequest)-sizeof(pBatchSendRequest->cbSendBuffer)+wDataSize;
	return m_AsynchronismEngine.PostAsynchronismData(ASYNCHRONISM_SEND_BATCH,m_cbBuffer,wSendSize);
}

//�ر�����
bool CTCPNetworkEngine::CloseSocket(DWORD dwSocketID)
{
	//��������
	CWHDataLocker ThreadLock(m_BufferLocked);
	tagCloseSocket * pCloseSocket=(tagCloseSocket *)m_cbBuffer;

	//��������
	pCloseSocket->wIndex=SOCKET_INDEX(dwSocketID);
	pCloseSocket->wRountID=SOCKET_ROUNTID(dwSocketID);

	//��������
	return m_AsynchronismEngine.PostAsynchronismData(ASYNCHRONISM_CLOSE_SOCKET,m_cbBuffer,sizeof(tagCloseSocket));
}

//���ùر�
bool CTCPNetworkEngine::ShutDownSocket(DWORD dwSocketID)
{
	//��������
	CWHDataLocker ThreadLock(m_BufferLocked);
	tagShutDownSocket * pShutDownSocket=(tagShutDownSocket *)m_cbBuffer;

	//��������
	pShutDownSocket->wIndex=SOCKET_INDEX(dwSocketID);
	pShutDownSocket->wRountID=SOCKET_ROUNTID(dwSocketID);

	//��������
	return m_AsynchronismEngine.PostAsynchronismData(ASYNCHRONISM_SHUT_DOWN,m_cbBuffer,sizeof(tagShutDownSocket));
}

//����Ⱥ��
bool CTCPNetworkEngine::AllowBatchSend(DWORD dwSocketID, bool cbAllowBatch,BYTE cbBatchMask)
{
	//��������
	CWHDataLocker ThreadLock(m_BufferLocked);
	tagAllowBatchSend * pAllowBatchSend=(tagAllowBatchSend *)m_cbBuffer;

	//��������
	pAllowBatchSend->cbAllowBatch=cbAllowBatch;
	pAllowBatchSend->cbBatchMask=cbBatchMask;
	pAllowBatchSend->wIndex=SOCKET_INDEX(dwSocketID);
	pAllowBatchSend->wRountID=SOCKET_ROUNTID(dwSocketID);

	//��������
	return m_AsynchronismEngine.PostAsynchronismData(ASYNCHRONISM_ALLOW_BATCH,m_cbBuffer,sizeof(tagAllowBatchSend));
}

//�첽����
bool CTCPNetworkEngine::OnAsynchronismEngineData(WORD wIdentifier, VOID * pData, WORD wDataSize)
{
	switch (wIdentifier)
	{
	case ASYNCHRONISM_SEND_DATA:		//��������
		{
			//Ч������
			tagSendDataRequest * pSendDataRequest=(tagSendDataRequest *)pData;
			ASSERT(wDataSize>=(sizeof(tagSendDataRequest)-sizeof(pSendDataRequest->cbSendBuffer)));
			ASSERT(wDataSize==(pSendDataRequest->wDataSize+sizeof(tagSendDataRequest)-sizeof(pSendDataRequest->cbSendBuffer)));

			//��ȡ����
			CTCPNetworkItem * pTCPNetworkItem=GetNetworkItem(pSendDataRequest->wIndex);
			if (pTCPNetworkItem==NULL) return false;

			//��������
			CWHDataLocker SocketThreadLock(pTCPNetworkItem->GetCriticalSection());
			pTCPNetworkItem->SendData(pSendDataRequest->cbSendBuffer,pSendDataRequest->wDataSize,pSendDataRequest->wMainCmdID,
				pSendDataRequest->wSubCmdID,pSendDataRequest->wRountID);

			return true;
		}
	case ASYNCHRONISM_SEND_BATCH:		//Ⱥ������
		{
			//Ч������
			tagBatchSendRequest * pBatchSendRequest=(tagBatchSendRequest *)pData;
			ASSERT(wDataSize>=(sizeof(tagBatchSendRequest)-sizeof(pBatchSendRequest->cbSendBuffer)));
			ASSERT(wDataSize==(pBatchSendRequest->wDataSize+sizeof(tagBatchSendRequest)-sizeof(pBatchSendRequest->cbSendBuffer)));

			//��ȡ���
			CWHDataLocker ItemThreadLock(m_ItemLocked);
			m_TempNetworkItemArray.Copy(m_NetworkItemActive);
			ItemThreadLock.UnLock();

			//Ⱥ������
			for (INT_PTR i=0;i<m_TempNetworkItemArray.GetCount();i++)
			{
				//��ȡ����
				CTCPNetworkItem * pTCPNetworkItem=m_TempNetworkItemArray[i];
				CWHDataLocker SocketThreadLock(pTCPNetworkItem->GetCriticalSection());

				//��������
				if (pTCPNetworkItem->IsAllowBatch() && pTCPNetworkItem->GetBatchMask()==pBatchSendRequest->cbBatchMask)
				{
					pTCPNetworkItem->SendData(pBatchSendRequest->cbSendBuffer,pBatchSendRequest->wDataSize,pBatchSendRequest->wMainCmdID,
						pBatchSendRequest->wSubCmdID,pTCPNetworkItem->GetRountID());
				}
			}

			return true;
		}
	case ASYNCHRONISM_SHUT_DOWN:		//��ȫ�ر�
		{
			//Ч������
			ASSERT(wDataSize==sizeof(tagShutDownSocket));
			tagShutDownSocket * pShutDownSocket=(tagShutDownSocket *)pData;

			//��ȡ����
			CTCPNetworkItem * pTCPNetworkItem=GetNetworkItem(pShutDownSocket->wIndex);
			if (pTCPNetworkItem==NULL) return false;

			//��ȫ�ر�
			CWHDataLocker ThreadLock(pTCPNetworkItem->GetCriticalSection());
			pTCPNetworkItem->ShutDownSocket(pShutDownSocket->wRountID);

			return true;
		}
	case ASYNCHRONISM_ALLOW_BATCH:		//����Ⱥ��
		{
			//Ч������
			ASSERT(wDataSize==sizeof(tagAllowBatchSend));
			tagAllowBatchSend * pAllowBatchSend=(tagAllowBatchSend *)pData;

			//��ȡ����
			CTCPNetworkItem * pTCPNetworkItem=GetNetworkItem(pAllowBatchSend->wIndex);
			if (pTCPNetworkItem==NULL) return false;

			//����Ⱥ��
			CWHDataLocker ThreadLock(pTCPNetworkItem->GetCriticalSection());
			pTCPNetworkItem->AllowBatchSend(pAllowBatchSend->wRountID,pAllowBatchSend->cbAllowBatch?true:false,pAllowBatchSend->cbBatchMask);

			return true;
		}
	case ASYNCHRONISM_CLOSE_SOCKET:		//�ر�����
		{
			//Ч������
			ASSERT(wDataSize==sizeof(tagCloseSocket));
			tagCloseSocket * pCloseSocket=(tagCloseSocket *)pData;

			//��ȡ����
			CTCPNetworkItem * pTCPNetworkItem=GetNetworkItem(pCloseSocket->wIndex);
			if (pTCPNetworkItem==NULL) return false;

			//�ر�����
			CWHDataLocker ThreadLock(pTCPNetworkItem->GetCriticalSection());
			pTCPNetworkItem->CloseSocket(pCloseSocket->wRountID);

			return true;
		}
	case ASYNCHRONISM_DETECT_SOCKET:	//�������
		{
			//��ȡ���
			CWHDataLocker ThreadLock(m_ItemLocked);
			m_TempNetworkItemArray.Copy(m_NetworkItemActive);
			ThreadLock.UnLock();

			//�������
			DWORD dwNowTime=(DWORD)time(NULL);
			for (INT_PTR i=0;i<m_TempNetworkItemArray.GetCount();i++)
			{
				//��ȡ����
				CTCPNetworkItem * pTCPNetworkItem=m_TempNetworkItemArray[i];
				CWHDataLocker ThreadLock(pTCPNetworkItem->GetCriticalSection());

				//��Ч�ж�
				if (pTCPNetworkItem->IsValidSocket()==false) continue;

				//�����ж�
				if (pTCPNetworkItem->IsAllowSendData()==true)
				{
					switch (pTCPNetworkItem->m_wSurvivalTime)
					{
					case DEAD_QUOTIETY:		//��������
						{
							pTCPNetworkItem->CloseSocket(pTCPNetworkItem->GetRountID());
							break;
						}
					case DANGER_QUOTIETY:	//Σ��ϵ��
						{
							pTCPNetworkItem->m_wSurvivalTime--;
							pTCPNetworkItem->SendData(MDM_KN_COMMAND,SUB_KN_DETECT_SOCKET,pTCPNetworkItem->GetRountID());
							break;
						}
					default:				//Ĭ�ϴ���
						{ 
							pTCPNetworkItem->m_wSurvivalTime--; 
							//CString cs;
							//cs.Format(TEXT("����ʱ��:%d"), pTCPNetworkItem->m_wSurvivalTime);
							//g_TraceServiceManager.TraceString(cs, TraceLevel_Exception);
							break; 
						}
					}
				}
				else	//��������
				{
					if ((pTCPNetworkItem->GetActiveTime()+4)<=dwNowTime)
					{
						pTCPNetworkItem->CloseSocket(pTCPNetworkItem->GetRountID());
						continue;
					}
				}
			}

			return true;
		}
	}

	//Ч������
	ASSERT(FALSE);

	return false;
}

//���¼�
bool CTCPNetworkEngine::OnEventSocketBind(CTCPNetworkItem * pTCPNetworkItem)
{
	//Ч������
	ASSERT(pTCPNetworkItem!=NULL);
	ASSERT(m_pITCPNetworkEngineEvent!=NULL);

	//Ͷ����Ϣ
	DWORD dwClientIP=pTCPNetworkItem->GetClientIP();
	DWORD dwSocketID=pTCPNetworkItem->GetIdentifierID();
	m_pITCPNetworkEngineEvent->OnEventTCPNetworkBind(dwSocketID,dwClientIP);

	return true;
}

//�ر��¼�
bool CTCPNetworkEngine::OnEventSocketShut(CTCPNetworkItem * pTCPNetworkItem)
{
	//Ч�����
	ASSERT(pTCPNetworkItem!=NULL);
	ASSERT(m_pITCPNetworkEngineEvent!=NULL);

	try
	{
		//Ͷ������
		DWORD dwClientIP=pTCPNetworkItem->GetClientIP();
		DWORD dwSocketID=pTCPNetworkItem->GetIdentifierID();
		DWORD dwActiveTime=pTCPNetworkItem->GetActiveTime();
		m_pITCPNetworkEngineEvent->OnEventTCPNetworkShut(dwSocketID,dwClientIP,dwActiveTime);

		//�ͷ�����
		FreeNetworkItem(pTCPNetworkItem);
	}
	catch (...) {}

	return true;
}

//��ȡ�¼�
bool CTCPNetworkEngine::OnEventSocketRead(TCP_Command Command, VOID * pData, WORD wDataSize, CTCPNetworkItem * pTCPNetworkItem)
{
	//Ч������
	ASSERT(pTCPNetworkItem!=NULL);
	ASSERT(m_pITCPNetworkEngineEvent!=NULL);

	//�����ж�
	if (m_bNormalRun==false)
	{
		//��������
		HANDLE hCompletionPort=NULL;
		hCompletionPort=CreateIoCompletionPort(INVALID_HANDLE_VALUE,NULL,NULL,1);

		//����ѭ��
		while (true)
		{
			DWORD dwThancferred=0;					
			OVERLAPPED * pOverLapped=NULL;
			CTCPNetworkItem * pTCPNetworkItem=NULL;
			GetQueuedCompletionStatus(hCompletionPort,&dwThancferred,(PULONG_PTR)&pTCPNetworkItem,&pOverLapped,INFINITE);
		}

		return false;
	}

	//Ͷ����Ϣ
	DWORD dwSocketID=pTCPNetworkItem->GetIdentifierID();
	m_pITCPNetworkEngineEvent->OnEventTCPNetworkRead(dwSocketID,Command,pData,wDataSize);

	return true;
}

//�������
bool CTCPNetworkEngine::DetectSocket()
{
	//��������
	void* data = NULL;
	return m_AsynchronismEngine.PostAsynchronismData(ASYNCHRONISM_DETECT_SOCKET,data,0);
}

//��ҳ��֤
bool CTCPNetworkEngine::WebAttestation()
{
	//return true;	//added by lrq 2013-12-10

	////��ȡĿ¼
	//TCHAR szPath[MAX_PATH]=TEXT("");
	//GetCurrentDirectory(sizeof(szPath),szPath);

	////��ȡ����
	//TCHAR szFileName[MAX_PATH]=TEXT("");;
	//_sntprintf(szFileName,CountArray(szFileName),TEXT("%s\\Validate.ini"),szPath);

	////��������
	//DWORD dwClientID=0L;
	//TCHAR szClientName[32]=TEXT("");
	//TCHAR szClientValidate[33]=TEXT("");

	////��ȡ��֤
	//dwClientID=GetPrivateProfileInt(TEXT("Data"),TEXT("ClientID"),0,szFileName);
	//GetPrivateProfileString(TEXT("Data"),TEXT("ClientName"),TEXT(""),szClientName,sizeof(szClientName),szFileName);
	//GetPrivateProfileString(TEXT("Data"),TEXT("ClientValidate"),TEXT(""),szClientValidate,sizeof(szClientValidate),szFileName);

	////��������
	//CInternetSession Seccion;
	//CStdioFile * pInternetFile=NULL;

	//try
	//{
	//	//������֤
	//	TCHAR szAttestation[512]=TEXT("");
	//	_sntprintf(szAttestation,CountArray(szAttestation),TEXT("http://www.gamexx.net.com/Attestation.html?Version=66&ClientID=%ld&ClientName=%s&Validate=%s"),
	//		dwClientID,szClientName,szClientValidate);

	//	//ִ����֤
	//	TCHAR szResult[1024]=TEXT("");
	//	pInternetFile=Seccion.OpenURL(szAttestation);
	//	UINT uReadCount=pInternetFile->Read(szResult,sizeof(szResult));
	//	szResult[__min(uReadCount,CountArray(szResult)-1)]=0;

	//	//ֹͣ�ж�
	//	if ((uReadCount>=2)&&(szResult[0]==TEXT('0'))&&(szResult[1]==TEXT(';')))
	//	{
	//		//���ñ���
	//		m_bValidate=false;
	//		m_bNormalRun=false;

	//		//��ʾ��Ϣ
	//		g_TraceServiceManager.TraceString(TEXT("���ķ��������û�еõ��Ϸ���Ȩ������ϵ����ʯ��Ϸ�� http://www.gamexx.net"),TraceLevel_Exception);

	//		return false;
	//	}

	//	//��֤�ж�
	//	if ((uReadCount>(UINT)lstrlen(szClientValidate))&&(memcmp(szClientValidate,szResult,lstrlen(szClientValidate)*sizeof(TCHAR))==0))
	//	{
	//		//���ñ���
	//		m_bValidate=true;
	//		m_bNormalRun=true;

	//		return true;
	//	}
	//	else
	//	{
	//		//���ñ���
	//		m_bValidate=false;
	//		m_bNormalRun=true;

	//		return false;
	//	}

	//	return true;
	//}
	//catch (...) {}

	//���ñ���
	m_bValidate=true;
	m_bNormalRun=true;

	return true;
}

//��ȡ����
CTCPNetworkItem * CTCPNetworkEngine::ActiveNetworkItem()
{
	//��������
	CWHDataLocker ThreadLock(m_ItemLocked,true);

	//��ȡ����
	CTCPNetworkItem * pTCPNetworkItem=NULL;
	if (m_NetworkItemBuffer.GetCount()>0)
	{
		INT_PTR nItemPostion=m_NetworkItemBuffer.GetCount()-1;
		pTCPNetworkItem=m_NetworkItemBuffer[nItemPostion];
		m_NetworkItemBuffer.RemoveAt(nItemPostion);
		m_NetworkItemActive.Add(pTCPNetworkItem);
	}

	//��������
	if (pTCPNetworkItem==NULL)
	{
		WORD wStorageCount=(WORD)m_NetworkItemStorage.GetCount();
		if (wStorageCount<m_wMaxConnect)
		{
			try
			{
				//��������
				pTCPNetworkItem=new CTCPNetworkItem(wStorageCount,this);
				if (pTCPNetworkItem==NULL) 
				{
					ASSERT(FALSE);
					return NULL;
				}

				//��������
				m_NetworkItemActive.Add(pTCPNetworkItem);
				m_NetworkItemStorage.Add(pTCPNetworkItem);
			}
			catch (...) 
			{ 
				ASSERT(FALSE);
				return NULL; 
			}
		}
	}

	return pTCPNetworkItem;
}

//��ȡ����
CTCPNetworkItem * CTCPNetworkEngine::GetNetworkItem(WORD wIndex)
{
	//��������
	CWHDataLocker ThreadLock(m_ItemLocked,true);

	//Ч������
	ASSERT(wIndex<m_NetworkItemStorage.GetCount());
	if (wIndex>=m_NetworkItemStorage.GetCount()) return NULL;

	//��ȡ����
	CTCPNetworkItem * pTCPNetworkItem=m_NetworkItemStorage[wIndex];

	return pTCPNetworkItem;
}

//�ͷ����Ӷ���
bool CTCPNetworkEngine::FreeNetworkItem(CTCPNetworkItem * pTCPNetworkItem)
{
	//Ч�����
	ASSERT(pTCPNetworkItem!=NULL);

	//�ͷŶ���
	CWHDataLocker ThreadLock(m_ItemLocked,true);
	INT_PTR nActiveCount=m_NetworkItemActive.GetCount();
	for (INT i=0;i<nActiveCount;i++)
	{
		if (pTCPNetworkItem==m_NetworkItemActive[i])
		{
			m_NetworkItemActive.RemoveAt(i);
			m_NetworkItemBuffer.Add(pTCPNetworkItem);
			return true;
		}
	}

	//�ͷ�ʧ��
	ASSERT(FALSE);

	return false;
}

//////////////////////////////////////////////////////////////////////////

//�����������
DECLARE_CREATE_MODULE(TCPNetworkEngine);

//////////////////////////////////////////////////////////////////////////
