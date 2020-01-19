
#include "RedisClient.h"


bool CRedisClient::zadd( const KEY& key,   const VALUES& vValues, TINT& count){
    VDATA vCmdData;
    vCmdData.push_back("ZADD");
    vCmdData.push_back(key);
    addparam(vCmdData, vValues);
    
    return commandargv_integer( vCmdData, count);
}

bool CRedisClient::zscrad(     const string& key, TINT& count){
    if (0==key.length()) {
        return false;
    }
    
    return command_integer( count, "ZSCRAD %s", key.c_str());
}

bool CRedisClient::zincrby(  const string& key, const double &increment, const string& member, string& value ) {
    if (0==key.length()) {
        return false;
    }
    
    return command_string( value, "ZINCRBY %s %f %s", key.c_str(), increment, member.c_str());
}

bool CRedisClient::zrange(  const string& key, int start, int end, VALUES& vValues, bool withscore) {
    if (0==key.length()) {
        return false;
    }
    
    if (withscore) {
        return command_list( vValues, "ZRANGE %s %d %d WITHSCORES", key.c_str(), start, end );
    }
    return command_list( vValues, "ZRANGE %s %d %d", key.c_str(), start, end);
}

bool CRedisClient::zrank(  const string& key, const string& member, TINT &rank) {
    if (0==key.length()) {
        return false;
    }
    
    return command_integer( rank, "ZRANK %s %s", key.c_str(), member.c_str());
}

bool CRedisClient::zrem(        const KEY& key, const VALUES& vmembers, TINT &count) {
    VDATA vCmdData;
    vCmdData.push_back("ZREM");
    vCmdData.push_back(key);
    addparam(vCmdData, vmembers);
    
    return commandargv_integer( vCmdData, count);
}

bool CRedisClient::zrangebyscore(  const string& key, string& start, string& end,VALUES& vValues, bool withscore)
{
	if (0==key.length()) {
		return false;
	}

	if (withscore) {
		return command_list( vValues, "zrangebyscore %s %s %s WITHSCORES", key.c_str(), start.c_str(), end.c_str());
	}
	return command_list( vValues, "zrangebyscore %s %s %s", key.c_str(), start.c_str(), end.c_str());
}

bool CRedisClient::zremrangebyrank(  const string& key, int start, int stop, TINT& count) {
    if (0==key.length()) {
        return false;
    }
    
    return command_integer( count, "ZREMRANGEBYRANK %s %d %d", key.c_str(), start, stop);
}

bool CRedisClient::zrevrange(  const string& key, int start, int end, VALUES& vValues, bool withscore) {
    if (0==key.length()) {
        return false;
    }
    if (withscore) {
        return command_list( vValues, "ZREVRANGE %s %d %d WITHSCORES", key.c_str(), start, end);
    }
    return command_list( vValues, "ZREVRANGE %s %d %d", key.c_str(), start, end);
}

 bool CRedisClient::zrevrank(  const string& key, const string &member, TINT& rank){
     if (0==key.length()) {
         return false;
     }
     
     return command_integer( rank, "ZREVRANK %s %s", key.c_str(), member.c_str());
 }

 bool CRedisClient::zscore(  const string& key, const string &member, string& score ){
     if (0==key.length()) {
         return false;
     }
     
     return command_string( score, "ZSCORE %s %s", key.c_str(), member.c_str());
 }


