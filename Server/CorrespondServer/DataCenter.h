#pragma once
#include "Singleton.h"

#include <map>
#include <vector>
#include <list>

using namespace  std;


//map<int,int>& testMap = CDataCenter::Instance()->m_testMap;
//testMap[100]=100;


class CDataCenter  :public Singleton<CDataCenter>
{
public:
	map<int,int> m_testMap;
	


};

