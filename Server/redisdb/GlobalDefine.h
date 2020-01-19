//----------------------------------------------------------------------------------------------------------------
//	π´”√∂®“ÂŒƒº�?
//
//----------------------------------------------------------------------------------------------------------------

#ifndef __GLOBALDEFINE_H__
#define __GLOBALDEFINE_H__

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <math.h>
#include <map>
#include <list>
#include <algorithm>
#include <set>
#include <queue>
# include <deque>
# include <stack>
#include <algorithm>
#include <ostream>
#include <fstream>
#include <sstream>
#include <iosfwd>
#include <string.h>
# include <iostream>
#include <signal.h>
//#include <slist>
#include <fcntl.h>
#include <ctype.h> 
#include <limits>
#ifdef WIN32
#include <unordered_map>
#else
#include <map>
#endif

#include <sys/stat.h>
#include <sys/types.h>

#include <time.h>

#ifndef WIN32
#include <unistd.h>
#include<unistd.h>

#include <stdarg.h>
#include <malloc.h>
#include <iconv.h>
#include <errno.h>
# include <pthread.h>
//# include <execinfo.h>
# include <unistd.h>  
#endif

using namespace std; //STL�������ռ�

#define  DEFAULT_HEARTBEAT_TIME   1000*30    //ms-->30s

#define	PI					3.141592654f
#define	PI2					(PI*2)
#define	PI_HALF				(PI*0.5f)
#define	RAD					(PI/180.f)
#define	ANG_TO_RAD(x)		((x)*RAD)
#define	RAD_TO_ANG(x)		((x)*(1/RAD))
#define  isDigital(x)    (((x)>='0') && ((x)<='9'))

#define RANDOM_RANGE_F(range) (rand()/(double)(RAND_MAX/range))

#define MAX_NAME_LEN		256
#define MAX_LOGIN_KEY_LEN    128

//’À∫≈
#define MAX_BUFLEN_ACCOUNT		(64+1)
//√‹¬�?
#define MAX_BUFLEN_PASSWORD		(64+1)
// Macµÿ÷�?
#define MAX_BUFLEN_MACADDR		(20)

// ÕÊº“objid∆ º
#define PLAYER_OBJ_ID_BASE         10000

#define INIT_TOTAL_CALN_NUM				10000

#ifdef WIN32
	#ifndef WIN32_LEAN_AND_MEAN
		#define WIN32_LEAN_AND_MEAN
	#endif
	#include "wtypes.h"
#else
	#include <pthread.h> 
#endif


struct localDatetime
{
	int year;
	int month;
	int day;
	int hour;
	int minute;
	int second;
	localDatetime(){ year=month=day=hour=minute=second=0;}
	localDatetime(int y,int mon,int d,int h,int min,int s)
		:year(y),month(mon),day(d),hour(h),minute(min),second(s){}
	bool SetDatetimeString(const char* datetimeStr)
	{return sscanf(datetimeStr, "%d/%d/%d",&year, &month, &day)!=-1;}
	std::string GetDatetimeString()
	{char szTime[64] = {0};_snprintf(szTime,sizeof(szTime),"%d/%d/%d-%d:%d:%d",year,month,day,hour,minute,second);return szTime;}
};


#endif // __GLOBALDEFINE_H__

