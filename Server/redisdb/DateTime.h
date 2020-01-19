#ifndef __DATETIME_H__
#define __DATETIME_H__

#include "BaseType.h"
#include <time.h>

#if defined(WIN32)
# include <sys/timeb.h>
#else
# include <sys/time.h>
#endif
#include <stdio.h>
#include <string>

class DateTime
{
public:
	DateTime()
	{
#ifndef WIN32
		struct timezone tz;
		gettimeofday(&m_tv, &tz);
#else
		time(&m_time);
		SYSTEMTIME sTime;
		GetSystemTime(&sTime);
		m_milliSecond = sTime.wMilliseconds;
#endif
	}
	DateTime( LONGLONG second, int milliSecond )
	{
#ifndef WIN32
		m_tv.tv_sec = second;
		m_tv.tv_usec = milliSecond * 1000;
#else
		m_time = second;
		m_milliSecond = milliSecond;
#endif
	}

	//绝对时间：
	INT GetYear()
	{
#ifndef WIN32
		struct tm stm;
		localtime_r(&m_tv.tv_sec, &stm);
		struct tm * ptm = &stm;
#else
		struct tm *ptm = localtime(&m_time);
#endif
		return 1900 + ptm->tm_year;
	}

	INT GetMonth()
	{
#ifndef WIN32
		struct tm stm;
		localtime_r(&m_tv.tv_sec, &stm);
		struct tm * ptm = &stm;
#else
		struct tm *ptm = localtime(&m_time);
#endif
		return ptm->tm_mon+1;
	}
	INT GetDay()
	{
#ifndef WIN32
		struct tm stm;
		localtime_r(&m_tv.tv_sec, &stm);
		struct tm * ptm = &stm;
#else
		struct tm *ptm = localtime(&m_time);
#endif
		return ptm->tm_mday;
	}
	INT GetYDay()
	{
#ifndef WIN32
		struct tm stm;
		localtime_r(&m_tv.tv_sec, &stm);
		struct tm * ptm = &stm;
#else
		struct tm *ptm = localtime(&m_time);
#endif
		return ptm->tm_yday;
	}
	// 0-6 Sunday-Saturday
	INT GetWeekDay()
	{
#ifndef WIN32
		struct tm stm;
		localtime_r(&m_tv.tv_sec, &stm);
		struct tm * ptm = &stm;
#else
		struct tm *ptm = localtime(&m_time);
#endif
		return ptm->tm_wday;
	}
	INT GetHour()
	{
#ifndef WIN32
		struct tm stm;
		localtime_r(&m_tv.tv_sec, &stm);
		struct tm * ptm = &stm;
#else
		struct tm *ptm = localtime(&m_time);
#endif
		return ptm->tm_hour;
	}
	INT GetMinute()
	{
#ifndef WIN32
		struct tm stm;
		localtime_r(&m_tv.tv_sec, &stm);
		struct tm * ptm = &stm;
#else
		struct tm *ptm = localtime(&m_time);
#endif
		return ptm->tm_min;

	}
	INT GetSecond()
	{
#ifndef WIN32
		struct tm stm;
		localtime_r(&m_tv.tv_sec, &stm);
		struct tm * ptm = &stm;
#else
		//struct tm *ptm = localtime(&m_time);
		struct tm stm;
		localtime_s(&stm, &m_time);
		struct tm * ptm = &stm;

#endif
		return ptm->tm_sec;
	}
	INT GetMilliSecond()
	{
#ifndef WIN32
		return m_tv.tv_usec/1000;
#else
		return m_milliSecond;
#endif
	}

	//1970 至今的秒数
	LONGLONG GetBigSecond()
	{
#ifndef WIN32
		return m_tv.tv_sec;
#else
		return m_time;
#endif
	}
    //1970 至今的毫秒数
	LONGLONG GetBigMs()
	{
#ifndef WIN32
		return (LONGLONG)m_tv.tv_sec*1000+ (LONGLONG)m_tv.tv_usec/1000;
#else
		return (LONGLONG)m_time*1000 + (LONGLONG)m_milliSecond;
#endif
	}

public:
    enum   { day_seconds=24*3600,hour_seconds=3600,min_seconds=60 };
    static bool  IsLeapYear(int year)
	{
		if((year % 4) == 0)
		{
			if((year % 100) == 0 && (year % 400) != 0)
			{
				return false;
			}
			else
			{
				return true;
			}
		}
		else
		{
			return false;
		}
	}
    static bool  IsValidDatetime(const localDatetime& ldt)
	{
		if(ldt.month < 1 || ldt.month > 12) return false;
		int days = DateTime::GetMonthDays(ldt.year, ldt.month);
		if(ldt.day < 1 || ldt.day > days) return false;
		if(ldt.hour < 0 || ldt.hour > 24) return false;
		if(ldt.minute < 0 || ldt.minute > 60) return false;
		if(ldt.second < 0 || ldt.second > 60) return false;
		return true;
	}
    static int   GetMonthDays(int year, int month)
	{
		static int leapDays[] = {31,29,31,30,31,30,31,31,30,31,30,31};
		static int nonleapDays[] = {31,28,31,30,31,30,31,31,30,31,30,31};
		if(DateTime::IsLeapYear(year))
		{
			return leapDays[month-1];
		}
		else
		{
			return nonleapDays[month-1];
		}
	}
    static time_t GetYearFirstTime(int year)
	{
		struct tm stm = {0,0,0,1,0,year-1900,0,0,0};
		return mktime(&stm);
	}
    static time_t GetYearFirstTimeFast(int year)
	{
#define year_count 200
		const int startYear = 1971;
		const int endYear = startYear + year_count;

		static time_t yearSecs[year_count];
		volatile static bool bInit = false;

		if(year<startYear || year>=endYear)
		{
			return DateTime::GetYearFirstTime(year);
		}
		else
		{
			if(bInit)
			{
				return yearSecs[year-startYear];
			}
			else
			{
				if(bInit)
				{
					return yearSecs[year-startYear];
				}
				else
				{
					for(int i=0; i<year_count; i++)
					{
						yearSecs[i] = DateTime::GetYearFirstTime(startYear+i);
					}
					bInit = true;
					return yearSecs[year-startYear];
				}
			}
		}
	}

    bool SetLocalTime(const localDatetime& ldt)
	{
		if(!DateTime::IsValidDatetime(ldt)) return false;
#ifndef WIN32   
		long &m_secs = m_tv.tv_sec;
#else
		time_t &m_secs = m_time;
#endif

		m_secs = DateTime::GetYearFirstTimeFast(ldt.year);
		if(m_secs < 0) m_secs = 0;

		for(int i=1; i<ldt.month; i++)
		{
			int days = DateTime::GetMonthDays(ldt.year, i);
			m_secs += days * day_seconds;
		}

		m_secs += (ldt.day-1) * day_seconds;
		m_secs += ldt.hour * hour_seconds;
		m_secs += ldt.minute * min_seconds;
		m_secs += ldt.second;

#ifndef WIN32   
		m_tv.tv_usec = m_tv.tv_sec * 1000;
#else
		m_milliSecond = m_time * 100;
#endif

		return true;
	}
	localDatetime GetLocalTime() const
	{
		enum { max_year_secs = 366*day_seconds };
		static const localDatetime emptyDatetime(0,0,0,0,0,0);
#ifndef WIN32   
		const long &m_secs = m_tv.tv_sec;
#else
		const time_t &m_secs = m_time;
#endif
		if(m_secs < 0) return emptyDatetime;

		int ys = (int)(m_secs/max_year_secs);
		localDatetime ldt(ys+1970,1,1,0,0,0);

		time_t secs = DateTime::GetYearFirstTimeFast(ldt.year);
		if(secs < 0) return emptyDatetime;

		while(true)
		{
			time_t secs2 = DateTime::GetYearFirstTimeFast(ldt.year+1);
			if(secs2 < 0) return emptyDatetime;

			if(m_secs < secs2) break;
			ldt.year += 1;
			secs = secs2;
		}

		time_t delta = m_secs - secs;
		for(; ldt.month<=12; ldt.month++)
		{
			int days = DateTime::GetMonthDays(ldt.year, ldt.month);
			if(delta >= days*day_seconds)
			{
				delta -= days*day_seconds;
			}
			else
			{
				for(; ldt.day<=days; ldt.day++)
				{
					if(delta >= day_seconds)
					{
						delta -= day_seconds;
					}
					else
					{
						ldt.hour = (int)(delta/hour_seconds);
						ldt.minute = (int)((delta%hour_seconds)/min_seconds);
						ldt.second = (int)((delta%hour_seconds)%min_seconds);
						return ldt;
					}
				}
			}
		}

		// 不可达
		return emptyDatetime;
	}

    DateTime& AddSecond(int secs)
	{
#ifndef WIN32   
		long &m_secs = m_tv.tv_sec;
#else
		time_t &m_secs = m_time;
#endif

		m_secs += secs;

		return *this;
	}
    DateTime& AddDay(int days)
	{
#ifndef WIN32   
		long &m_secs = m_tv.tv_sec;
#else
		time_t &m_secs = m_time;
#endif
		m_secs += days * day_seconds;
		return *this;
	}

	DateTime& operator=(const DateTime& other)
	{
		if(this == &other) return *this;

#ifndef WIN32
		m_tv.tv_sec = other.m_tv.tv_sec;
		m_tv.tv_usec = other.m_tv.tv_usec;
#else
		m_time = other.m_time;
		m_milliSecond = other.m_milliSecond;
#endif
		return *this;
	}
    bool operator==(const DateTime& other) const
	{
#ifndef WIN32
		return m_tv.tv_sec == other.m_tv.tv_sec;
#else
		return m_time == other.m_time;
#endif
	}
    bool operator!=(const DateTime& other) const
	{
#ifndef WIN32
		return m_tv.tv_sec != other.m_tv.tv_sec;
#else
		return m_time != other.m_time;
#endif
	}
    bool operator >(const DateTime& other) const
	{
#ifndef WIN32
		return m_tv.tv_sec > other.m_tv.tv_sec;
#else
		return m_time > other.m_time;
#endif
	}
    bool operator <(const DateTime& other) const
	{
#ifndef WIN32
		return m_tv.tv_sec < other.m_tv.tv_sec;
#else
		return m_time < other.m_time;
#endif
	}
    bool operator>=(const DateTime& other) const
	{
#ifndef WIN32
		return m_tv.tv_sec >= other.m_tv.tv_sec;
#else
		return m_time >= other.m_time;
#endif
	}
    bool operator<=(const DateTime& other) const
	{
#ifndef WIN32
		return m_tv.tv_sec <= other.m_tv.tv_sec;
#else
		return m_time <= other.m_time;
#endif
	}
private:

#ifndef WIN32
	struct timeval m_tv;
#else
	time_t		m_time;
	int			m_milliSecond;
#endif
};

#endif // __DATETIME_H__ 
