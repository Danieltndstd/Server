#ifndef __RED_PACKET_H__
#define	__RED_PACKET_H__

#include <iostream>
#include <vector>
#include <stdlib.h>
#include <string.h>
#include <time.h>
using namespace std;

class RedPacket
{

public:
	static RedPacket * createRedPacket(DWORD sendUserID,DWORD sendUserNum,DWORD sendScore,BYTE redType,const TCHAR *sendContent,tagRedInfoArray * redInfoArray,BYTE &result);//����

	void Release();//��������

	DWORD getRedPacket();//��ȡ�������vector����ȡ����ȡһ��ɾ��һ��

	DWORD getSurplusRedNum();//ʣ��������

	bool isEmpty();//����Ƿ���ʣ��

	TCHAR * getRedContent();

	void printRed();//��ӡ���еĺ��

	void addRecvRedRecord(int UserID); //�����������¼

public:

	vector<int> & getRecvRedRecord()
	{
		return m_RecvRedRecord;
	}

	DWORD getSendUserID()
	{
		return dwSendUserID;
	}

	time_t getSendTime()
	{
		return sendTime;
	}

	DWORD getResidueScore()
	{
		return dwResidueScore;
	}
protected:
	RedPacket(){};
	~RedPacket(){};
	bool init(DWORD sendUserID,DWORD sendUserNum,DWORD sendScore,BYTE redType,const TCHAR *sendContent,tagRedInfoArray * redInfoArray,BYTE &result);//��ʼ������

	bool calculateRed(BYTE &result);//����ÿ���������ֵ����redWareHouse

private:
	DWORD dwSendUserID;			//��������ID
	DWORD dwSendUserNum;		//���������
	DWORD dwSendScore;			//������
	BYTE cbRedType;				//������� 

	DWORD dwResidueScore;				//ʣ����
	time_t sendTime;			//����ʱ��
	
	
	TCHAR szSendContent[128];	//�����������

	vector<double> m_redWareHouse;	//����ֿ�

	vector<int> m_RecvRedRecord; //�������¼

	tagRedInfoArray * m_RedInfoArray;
};


#endif