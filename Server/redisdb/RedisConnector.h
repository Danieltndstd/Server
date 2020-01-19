
#ifndef _CRedis_POOL_H_
#define _CRedis_POOL_H_

#include "BaseType.h"
//#include "LogSystem.h"
#include "Lock.h"
#include "hiredis/hiredis.h"
#include "RedisConnParam.h"

enum ENUM_REDIS_CONN_STATUS
{
	REDIS_CONN_INIT = 1,		//Redis连接初始状态
	REDIS_CONN_RUNNING,			//Redis连接运行中
	REDIS_CONN_CLOSE,			//Redis连接断开
};

class RedisConnector
{
public:
    RedisConnector():m_enumConn(REDIS_CONN_INIT),mCtx(NULL)
    {
    }

    ~RedisConnector()
    {
    }

    void Init( KRedisConnParam &connParam )
    {
		m_connParam = connParam;
    }

	bool DisConnectDB()
	{
		if (mCtx != NULL)
		{
			redisFree(mCtx);
			mCtx = NULL;
		}

		m_enumConn = REDIS_CONN_CLOSE;
		return true;
	}

	bool ConnectDB()
	{
		bool bRet = false;
		struct timeval timeoutVal;
		timeoutVal.tv_sec  = m_connParam.GetTimeOut();
		timeoutVal.tv_usec = 0;

		mCtx = redisConnectWithTimeout(m_connParam.GetHost(), m_connParam.GetServerPort(), timeoutVal);
		if (NULL == mCtx || mCtx->err) 
		{
			if (NULL!=mCtx) 
			{
				redisFree(mCtx);
				mCtx = NULL;
			} 
		}
		else
		{  
			bRet = true;
			m_enumConn = REDIS_CONN_RUNNING;
		}

		return bRet;
	}

	bool AuthDB()
	{
		std::string szAuth = "AUTH ";
		szAuth.append(m_connParam.GetPassword());
		redisReply *reply = static_cast<redisReply *>(redisCommand(mCtx, szAuth.c_str()));
		bool bRet = (NULL != reply);
		freeReplyObject(reply);
		return bRet;
	}

	bool Publish()
	{
		std::string szChannel = "pub_sub:wh:message ";
		std::string szMessage = "good123456";
		std::string szCommand = "publish ";
		szCommand += szChannel + szMessage;
		redisReply *reply = static_cast<redisReply *>(redisCommand(mCtx,szCommand.c_str()));
		bool bRet = (NULL != reply);
		freeReplyObject(reply);
		return bRet;
	}

    bool RedisConnect()
    {
        DisConnectDB();

		return ConnectDB();
    }

	bool CheckReply(const redisReply *reply){
		if(NULL==reply) 
		{
			switch(mCtx->err)
			{
			case REDIS_ERR_PROTOCOL:
			case REDIS_ERR_IO:
			case REDIS_ERR_EOF:
			case REDIS_ERR_OTHER:
				m_enumConn = REDIS_CONN_CLOSE;
				//CacheLog_ERROR(LOG_ERROR, "m_enumConn = REDIS_CONN_CLOSE");
				break;
			default:
				break;
			}

			return false;
		}

		switch(reply->type)
		{
			case REDIS_REPLY_STRING:
				return true;
									
			case REDIS_REPLY_ARRAY:
				return true;
								   
			case REDIS_REPLY_INTEGER:
				return true;
									 
			case REDIS_REPLY_NIL:
				return false;
								 
			case REDIS_REPLY_STATUS:
#ifdef WIN32
				return (stricmp(reply->str,"OK") == 0)?true:false;				
#else 
				return (strcasecmp(reply->str,"OK") == 0)?true:false;	
#endif
			case REDIS_REPLY_ERROR:
				{
					//CacheLog_ERROR(LOG_ERROR, "ExecuteRedisCommand error. %s", reply->str);
					return false;
				}
								   
			default:
				return false;
		}

		return false;
	}

	void FreeReply(const redisReply *reply){
		if (NULL!=reply) {
			freeReplyObject((void*)reply);
		}
	}
    bool Ping() const
    {
        redisReply *reply = static_cast<redisReply *>(redisCommand(mCtx, "PING"));
        bool bRet = (NULL != reply);
        freeReplyObject(reply);
        return bRet;
    }

	ENUM_REDIS_CONN_STATUS GetConnStatus() {	return m_enumConn; }
    redisContext  *getCtx() const     { return mCtx; }

private:
    // redis connector context
    redisContext *mCtx;
	ENUM_REDIS_CONN_STATUS		m_enumConn;				//Redis连接状态
	KRedisConnParam m_connParam;
};


#endif
