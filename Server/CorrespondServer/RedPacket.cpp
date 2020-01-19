#include "RedPacket.h"
#include "Stdafx.h"

RedPacket* RedPacket::createRedPacket(DWORD sendUserID,DWORD sendUserNum,DWORD sendScore,BYTE redType,const TCHAR *sendContent,tagRedInfoArray * redInfoArray,BYTE &result)
{
	RedPacket * ret = new RedPacket();
	if(ret == NULL)
	{
		result = 2;
		return NULL;
	}

	if(ret->init(sendUserID,sendUserNum,sendScore,redType,sendContent,redInfoArray,result))
	{
		result = 1;
		return ret;
	}
	else
	{
		return NULL;
	}
}

void RedPacket::Release()
{
	delete this;
}

bool RedPacket::init(DWORD sendUserID,DWORD sendUserNum,DWORD sendScore,BYTE redType,const TCHAR *sendContent,tagRedInfoArray * redInfoArray,BYTE &result)
{

	if(redInfoArray == NULL)
	{
		result = 4;
		return false;
	}

	dwSendUserID = sendUserID;
	dwSendUserNum = sendUserNum;
	dwSendScore = sendScore;
	cbRedType = redType;

	time(&sendTime);

	m_RedInfoArray = redInfoArray;

	lstrcpyn(szSendContent,sendContent,CountArray(szSendContent));

// 	tagRedInfo * redInfo = nullptr;
// 
// 	for (int i = 0;i<m_RedInfoArray->wCount;i++)
// 	{
// 		if(cbRedType == m_RedInfoArray->RedInfo[i].cbRedGrantType)
// 		{
// 			redInfo = &m_RedInfoArray->RedInfo[i];
// 			break;
// 		}
// 	}
// 	if ( redInfo == NULL ) return false;
// 
// 	if(dwSendScore < redInfo->dwLowestAmount || dwSendScore < dwSendUserNum)
// 	{
// 		result = 3;
// 		return false;
// 	}
// 
// 	if(szSendContent == NULL)
// 	{
// 		
// 	}

	return calculateRed(result);
}

bool RedPacket::calculateRed(BYTE &result)
{
	//DWORD number, i, everage_money;
	//double total_money, money, min = 0.01;

	//number = dwSendUserNum;
	//total_money = dwSendScore;

	//srand(time(0));

	//for (i = 0; i < number - 1; i++)
	//{
	//	everage_money = ((total_money - (number - i) * min) / (number - i)) * 100;
	//	money = (1 + rand() % everage_money) / 100.0;

	//	m_redWareHouse.push_back(money);

	//	total_money = total_money - money;
	//}

	//m_redWareHouse.push_back(total_money);


	tagRedInfo * redInfo = nullptr;

	for (int i = 0;i<m_RedInfoArray->wCount;i++)
	{
		if(cbRedType == m_RedInfoArray->RedInfo[i].cbRedGrantType)
		{
			redInfo = &m_RedInfoArray->RedInfo[i];
			break;
		}
	}

	if(dwSendScore < redInfo->dwLowestAmount || dwSendScore < dwSendUserNum)	//判断红包金额是否达到下限,红包金额是否小于红包数量
	{
		result = 3;
		return false;
	}

	//if(dwSendUserNum < redInfo->dwUserLowest) //判断下限人数
	//{
	//	result = 5;
	//	return false;
	//}

	//抽成
	dwSendScore = dwSendScore - dwSendScore*((double)redInfo->cbRedRevenue / 100);

	dwResidueScore=dwSendScore;

	int number, i, everage_money;
	double total_money, money, min = 0.01;

	number = dwSendUserNum;
	total_money = dwSendScore;

	srand(time(0));

	for (i = 0; i < number - 1; i++)
	{
		everage_money = ((total_money - (number - i) * min) / (number - i)) * 100;
		money = (1 + rand() % everage_money) / 100.0;

		if(money < redInfo->dwLowestBase)   //判断是否过低
		{
			money = redInfo->dwLowestBase;
		}

		if(money > dwSendScore*(double)redInfo->cbHighestBasePercent)   //判断是否过高
		{
			money = dwSendScore*(double)redInfo->cbHighestBasePercent;
		}

		m_redWareHouse.push_back((int)money);

		total_money = total_money - (int)money;
	}

	m_redWareHouse.push_back(total_money);   //将剩下的放入

	return true;
}

DWORD RedPacket::getSurplusRedNum()
{
	return m_redWareHouse.size();
}

bool RedPacket::isEmpty()
{
	return m_redWareHouse.empty();
}

DWORD RedPacket::getRedPacket()
{
	if(!isEmpty())
	{
		DWORD score = m_redWareHouse.back();
		m_redWareHouse.pop_back();

		dwResidueScore -= score;

		return score;
	}
	else
	{
		return 0;
	}
}

TCHAR * RedPacket::getRedContent()
{
	return szSendContent;
}

void RedPacket::printRed()
{
	if(dwSendUserNum != m_redWareHouse.size())
	{
		printf("%d %d size error\n",dwSendUserNum,m_redWareHouse.size());
		return ;
	}

	for(DWORD i=0;i<dwSendUserNum;i++)
	{	
		printf("%d : %.2f\n",i,m_redWareHouse[i]);
	}
}

void RedPacket::addRecvRedRecord(int UserID)//增加抢红包记录
{
	m_RecvRedRecord.push_back(UserID);
}