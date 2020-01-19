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
	static RedPacket * createRedPacket(DWORD sendUserID,DWORD sendUserNum,DWORD sendScore,BYTE redType,const TCHAR *sendContent,tagRedInfoArray * redInfoArray,BYTE &result);//创建

	void Release();//消除对象

	DWORD getRedPacket();//获取红包，从vector最后获取，获取一个删除一个

	DWORD getSurplusRedNum();//剩余红包数量

	bool isEmpty();//红包是否还有剩余

	TCHAR * getRedContent();

	void printRed();//打印现有的红包

	void addRecvRedRecord(int UserID); //增加抢红包记录

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
	bool init(DWORD sendUserID,DWORD sendUserNum,DWORD sendScore,BYTE redType,const TCHAR *sendContent,tagRedInfoArray * redInfoArray,BYTE &result);//初始化对象

	bool calculateRed(BYTE &result);//计算每个红包的数值存入redWareHouse

private:
	DWORD dwSendUserID;			//发红包玩家ID
	DWORD dwSendUserNum;		//红包总人数
	DWORD dwSendScore;			//红包金额
	BYTE cbRedType;				//红包类型 

	DWORD dwResidueScore;				//剩余金额
	time_t sendTime;			//发送时间
	
	
	TCHAR szSendContent[128];	//红包附加内容

	vector<double> m_redWareHouse;	//红包仓库

	vector<int> m_RecvRedRecord; //抢红包记录

	tagRedInfoArray * m_RedInfoArray;
};


#endif