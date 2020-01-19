#pragma once
#include "Singleton.h"
#include "DataBasePacket.h"

#include <map>
#include <vector>
#include <list>

struct tagSameTableUsers
{
	DWORD		dwUserID;

};

using namespace  std;
class CDataCenter  :public Singleton<CDataCenter>
{
public:
	map<int,int> m_testMap;
	DWORD dwFinishedFristGameReward;

	DWORD dwAndroidIsAllowedSitDown;   //机器人是否允许坐下

	map<DWORD,DBO_GR_TipMessage> m_mTipMessage;  //弹窗消息
	map<DWORD,map<DWORD,WORD> > m_SameTableUsersMap;
};

