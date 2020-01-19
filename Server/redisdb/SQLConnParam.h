#ifndef _SQLConnParam__H_
#define _SQLConnParam__H_
#include "BaseType.h"
#include "serveraddr.h"
class KSQLConnParam
{
public:
	enum { MYSQL_DEFAULT_PORT=3306 };

public:


	/// KSQLConnParam
	KSQLConnParam():m_serverPort(MYSQL_DEFAULT_PORT)
	{
	}

	KSQLConnParam(const KSQLConnParam& o)
		: m_serverPort(o.m_serverPort)
		, m_host(o.m_host)
		, m_dbName(o.m_dbName)
		, m_userName(o.m_userName)
		, m_passwd(o.m_passwd)
		, m_charset(o.m_charset)
	{
	}

	~KSQLConnParam()
	{
	}

	KSQLConnParam& operator=(const KSQLConnParam& o)
	{
		if(this == &o) return *this;
		m_serverPort = o.m_serverPort;
		m_host = o.m_host;
		m_dbName = o.m_dbName;
		m_userName = o.m_userName;
		m_passwd = o.m_passwd;
		m_charset = o.m_charset;
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

	void SetDbName(const char* dbName)
	{
		m_dbName = dbName;
	}

	void SetUserName(const char* userName)
	{
		m_userName = userName;
	}

	void SetPassword(const char* passwd)
	{
		m_passwd = passwd;
	}

	void SetCharset(const char* charset)
	{
		m_charset = charset;
	}

	unsigned int GetServerPort() const
	{
		return m_serverPort;
	}

	const char* GetHost() const
	{
		return m_host.c_str();
	}

	const char* GetDbName() const
	{
		return m_dbName.c_str();
	}

	const char* GetUserName() const
	{
		return m_userName.c_str();
	}

	const char* GetPasswd() const
	{
		return m_passwd.c_str();
	}

	const char* GetCharset() const
	{
		return m_charset.c_str();
	}




private:
	unsigned int m_serverPort;
	string m_host;
	string m_dbName;
	string m_userName;
	string m_passwd;
	string m_charset;
};


#endif