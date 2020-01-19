
#include "RedisClient.h"

CRedisClient::CRedisClient()
{

}


CRedisClient::~CRedisClient()
{

}


void CRedisClient::Keepalive() {
	bool bRet = Ping();
	if (!bRet) {
		RedisConnect();
	} 
}


void CRedisClient::FreeReply(redisReply* reply)
{
	if(NULL != reply)
	{
		freeReplyObject(reply);
		reply = NULL;
	}
}
void CRedisClient::SetDebugString( const char *str) 
{
	//CacheLog_DEBUG(LOG_DEBUG, "Redis Cmd error. %s", str);
}

void CRedisClient::SetDebugString( const VDATA& vData ) 
{
	string szEr = "";
	for( size_t i=0;i<vData.size();i++ )
		szEr += vData[i];

	//CacheLog_DEBUG(LOG_DEBUG, "Redis Cmd error. %s", szEr.c_str() );
}

void CRedisClient::SetDebugMessage( const char* fmt, ...)
{
	char szBuf[128] = { 0 };
	va_list va;
	va_start(va, fmt);
	vsnprintf(szBuf, sizeof(szBuf), fmt, va);
	va_end(va);
	SetDebugString( szBuf);
}


void CRedisClient::SetErrString( const char *str) 
{
	//CacheLog_ERROR(LOG_ERROR, "Redis Cmd error. %s", str);
}

void CRedisClient::SetErrString( const VDATA& vData ) 
{
	string szEr = "";
	for( size_t i=0;i<vData.size();i++ )
		szEr += vData[i];

	//CacheLog_ERROR(LOG_ERROR, "Redis Cmd error. %s", szEr.c_str() );
}

void CRedisClient::SetErrMessage( const char* fmt, ...)
{
    char szBuf[128] = { 0 };
    va_list va;
    va_start(va, fmt);
    vsnprintf(szBuf, sizeof(szBuf), fmt, va);
    va_end(va);
    SetErrString( szBuf);
}

redisReply *CRedisClient::command( const char* cmd)
{
    redisReply *reply = static_cast<redisReply *>(redisCommand(getCtx(), cmd));
    return reply;
}

bool CRedisClient::command_bool( const char *cmd, ...) {
    bool bRet = false;
    va_list args;
    va_start(args, cmd);
    redisReply *reply = static_cast<redisReply *>(redisvCommand(getCtx(), cmd, args));
    va_end(args);


    if (CheckReply(reply)) {
        if (REDIS_REPLY_STATUS==reply->type) {
            bRet = true;
        } else {
            bRet = (reply->integer == 1) ? true : false;
        }
    } else {
		if (reply &&reply->type == REDIS_REPLY_ERROR)
			SetErrString( reply->str );
		else
			SetDebugString( cmd);
    }

    FreeReply(reply);
 
    return bRet;
}

bool CRedisClient::command_status( const char* cmd, ...) {
    bool bRet = false;
 
    va_list args;
    va_start(args, cmd);
    redisReply *reply = static_cast<redisReply *>(redisvCommand( getCtx(), cmd, args));
    va_end(args);


    if (CheckReply(reply)) {
        bRet = true;
    } else {
		if (reply &&reply->type == REDIS_REPLY_ERROR)
			SetErrString( reply->str );
		else
			SetDebugString( cmd);
    }

    FreeReply(reply);
 
    return bRet;
}

bool CRedisClient::command_integer( TINT &retval, const char* cmd, ...) {
    bool bRet = false;
 
    va_list args;
    va_start(args, cmd);
    redisReply *reply = static_cast<redisReply *>(redisvCommand(getCtx(), cmd, args));
    va_end(args);
    if (CheckReply(reply)) {
        retval = reply->integer;
        bRet = true;
    } else {
		if (reply &&reply->type == REDIS_REPLY_ERROR)
			SetErrString( reply->str );
		else
			SetDebugString( cmd);
    }

    FreeReply(reply);
 
    return bRet;
}

bool CRedisClient::command_string( string &data, const char* cmd, ...) {
    bool bRet = false;
 
    va_list args;
    va_start(args, cmd);
    redisReply *reply = static_cast<redisReply *>(redisvCommand(getCtx(), cmd, args));
    va_end(args);
    if (CheckReply(reply)) {
        data.assign(reply->str, reply->len);
        bRet = true;
    } else {
		if (reply &&reply->type == REDIS_REPLY_ERROR)
			SetErrString( reply->str );
		else
			SetDebugString( cmd);
    }

    FreeReply(reply);
 
    return bRet;
}



bool CRedisClient::command_list( VALUES &vValue, const char* cmd, ...) {
    bool bRet = false;
 
    va_list args;
    va_start(args, cmd);
    redisReply *reply = static_cast<redisReply *>(redisvCommand(getCtx(), cmd, args));
    va_end(args);
    if (CheckReply(reply)) {
        for (size_t i = 0; i<reply->elements; i++) {
            vValue.push_back(string(reply->element[i]->str, reply->element[i]->len));
        }
        bRet  = true;
    } else {
		if (reply &&reply->type == REDIS_REPLY_ERROR)
			SetErrString( reply->str );
		else
			SetDebugString( cmd);
    }

    FreeReply(reply);
 
    return bRet;
}

bool CRedisClient::command_array(  DATAITEM& array,  const char* cmd, ...){
    bool bRet = false;
 
    va_list args;
    va_start(args, cmd);
    redisReply *reply = static_cast<redisReply *>(redisvCommand(getCtx(), cmd, args));
    va_end(args);
    if (CheckReply(reply)) {
        for (size_t i = 0; i<reply->elements; i++) {
            DataItem item;
            item.type = reply->element[i]->type;
            item.str.assign(reply->element[i]->str, reply->element[i]->len);
            array.push_back(item);
        }
        bRet  = true;
    } else {
		if (reply &&reply->type == REDIS_REPLY_ERROR)
			SetErrString( reply->str );
		else
			SetDebugString( cmd);
    }

    FreeReply(reply);
    
    return bRet;
}

bool CRedisClient::commandargv_bool( const VDATA& vData) {
    bool bRet = false;
    vector<const char *> argv( vData.size() );
    vector<size_t> argvlen( vData.size() );
    unsigned int j = 0;
    for ( VDATA::const_iterator i = vData.begin(); i != vData.end(); ++i, ++j ) {
        argv[j] = i->c_str(), argvlen[j] = i->size();
    }

    redisReply *reply = static_cast<redisReply *>(redisCommandArgv(getCtx(), argv.size(), &(argv[0]), &(argvlen[0])));
    if (CheckReply(reply)) {
        bRet = (reply->integer==1)?true:false;
    } else {
		if (reply &&reply->type == REDIS_REPLY_ERROR)
			SetErrString( reply->str );
		else
			SetDebugString( vData );
    }

    FreeReply(reply);
    

    return bRet;
}

bool CRedisClient::commandargv_status( const VDATA& vData) {
    bool bRet = false;
 
    vector<const char *> argv( vData.size() );
    vector<size_t> argvlen( vData.size() );
    unsigned int j = 0;
    for ( VDATA::const_iterator i = vData.begin(); i != vData.end(); ++i, ++j ) {
        argv[j] = i->c_str(), argvlen[j] = i->size();
    }

    redisReply *reply = static_cast<redisReply *>(redisCommandArgv(getCtx(), argv.size(), &(argv[0]), &(argvlen[0])));
    if (CheckReply(reply)) {
        bRet = true;
    } else {
		if (reply &&reply->type == REDIS_REPLY_ERROR)
			SetErrString( reply->str );
		else
			SetDebugString( vData );
    }

    FreeReply(reply);
    

    return bRet;
}

bool CRedisClient::commandargv_array( const VDATA& vDataIn, DATAITEM& array){
    bool bRet = false;
 
    vector<const char*> argv( vDataIn.size() );
    vector<size_t> argvlen( vDataIn.size() );
    unsigned int j = 0;
    for ( VDATA::const_iterator i = vDataIn.begin(); i != vDataIn.end(); ++i, ++j ) {
        argv[j] = i->c_str(), argvlen[j] = i->size();
    }

    redisReply *reply = static_cast<redisReply *>(redisCommandArgv(getCtx(), argv.size(), &(argv[0]), &(argvlen[0])));
    if (CheckReply(reply)) {
        for (size_t i = 0; i<reply->elements; i++) {
            DataItem item;
            item.type = reply->element[i]->type;
            item.str.assign(reply->element[i]->str, reply->element[i]->len);
            //item.str = (NULL == reply->element[i]->str) ? string("") : reply->element[i]->str;
            array.push_back(item);
        }
        bRet  = true;
    } else {
		if (reply &&reply->type == REDIS_REPLY_ERROR)
			SetErrString( reply->str );
		else
			SetDebugString( vDataIn );
    }

    FreeReply(reply);
    
    return bRet;
}

bool CRedisClient::commandargv_array( const VDATA& vDataIn, VALUES& array){
    bool bRet = false;
 
    vector<const char*> argv( vDataIn.size() );
    vector<size_t> argvlen( vDataIn.size() );
    unsigned int j = 0;
    for ( VDATA::const_iterator i = vDataIn.begin(); i != vDataIn.end(); ++i, ++j ) {
        argv[j] = i->c_str(), argvlen[j] = i->size();
    }

    redisReply *reply = static_cast<redisReply *>(redisCommandArgv(getCtx(), argv.size(), &(argv[0]), &(argvlen[0])));
    if (CheckReply(reply)) {
        for (size_t i = 0; i<reply->elements; i++) {
            //string str = (NULL == reply->element[i]->str) ? string("") : reply->element[i]->str;
            string str(reply->element[i]->str, reply->element[i]->len);
            array.push_back(str);
        }
        bRet  = true;
    } else {
		if (reply &&reply->type == REDIS_REPLY_ERROR)
			SetErrString( reply->str );
		else
			SetDebugString( vDataIn );
    }

    FreeReply(reply);
    
    return bRet;
}

bool CRedisClient::commandargv_integer( const VDATA& vDataIn, TINT& retval){
    bool bRet = false;
 
    vector<const char*> argv( vDataIn.size() );
    vector<size_t> argvlen( vDataIn.size() );
    unsigned int j = 0;
    for ( VDATA::const_iterator iter = vDataIn.begin(); iter != vDataIn.end(); ++iter, ++j ) {
        argv[j] = iter->c_str(), argvlen[j] = iter->size();
    }

    redisReply *reply = static_cast<redisReply *>(redisCommandArgv(getCtx(), argv.size(), &(argv[0]), &(argvlen[0])));
    if (CheckReply(reply)) {
        retval = reply->integer;
        bRet  = true;
    } else {
		if (reply &&reply->type == REDIS_REPLY_ERROR)
			SetErrString( reply->str );
		else
			SetDebugString( vDataIn );
    }

    FreeReply(reply);
    
    return bRet;
}

void CRedisClient::quit(){

}


bool CRedisClient::echo( const string& str, string &value)
{
	if (0==str.length()) {
		return false;
	}

	return command_string( value, "echo %s", str.c_str());
}