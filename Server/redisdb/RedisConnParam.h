
#ifndef _RedisConnParam__H_
#define _RedisConnParam__H_

#include "BaseType.h"
#include "serveraddr.h"
class KRedisConnParam
{
public:
	enum { MYSQL_DEFAULT_PORT=3306 };
 
	/// KRedisConnParam
	KRedisConnParam():m_serverPort(MYSQL_DEFAULT_PORT)
	{
	}

	KRedisConnParam(const KRedisConnParam& o)
		: m_serverPort(o.m_serverPort)
		, m_host(o.m_host)
		, m_nTimeout(5)
	{
	}

	~KRedisConnParam()
	{
	}

	KRedisConnParam& operator=(const KRedisConnParam& o)
	{
		if(this == &o) return *this;
		m_serverPort = o.m_serverPort;
		m_host = o.m_host;
		m_password = o.m_password;
		return *this;
	}

	void SetHost(const char* host)
	{
		m_host = host;
	}

	void SetServerPort(unsigned int serverPort)
	{
		m_serverPort = serverPort;
	}


	void SetTimeOut(unsigned int nTimeOut)
	{
		m_nTimeout = nTimeOut;
	}

	void SetPassword(const char* password)
	{
		m_password = password;
	}

	unsigned int GetServerPort() const
	{
		return m_serverPort;
	}

	const char* GetHost() const
	{
		return m_host.c_str();
	}

	const unsigned int GetTimeOut() const
	{
		return m_nTimeout;
	}

	const char* GetPassword() const
	{
		return m_password.c_str();
	}

private:
	unsigned int m_serverPort;
	string m_host;
	string m_password;
	unsigned int  m_nTimeout;       // connect timeout second};
};

#endif