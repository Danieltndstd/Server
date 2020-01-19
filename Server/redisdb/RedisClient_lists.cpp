
 
#include "RedisClient.h"

bool CRedisClient::lindex(    const string& key, TINT index, VALUE& value){
    if (0==key.length()) {
        return false;
    }
    
    return command_string( value, "LINDEX %s %lld", key.c_str(), index);
}

bool CRedisClient::linsert(    const string& key, const LMODEL mod, const string& pivot, const string& value, TINT& retval){
    static const char *lmodel[2]= {"BEFORE","AFTER"};
    if (0==key.length()) {
        return false;
    }
    
    return command_integer( retval, "LINSERT %s %s %s %s", key.c_str(), lmodel[mod], pivot.c_str(), value.c_str());
}

bool CRedisClient::llen(    const string& key, TINT& retval){
    if (0==key.length()) {
        return false;
    }
    
    return command_integer( retval, "LLEN %s", key.c_str());
}

bool CRedisClient::lpop(    const string& key, string& value){
    if (0==key.length()) {
        return false;
    }
    
    return command_string( value, "LPOP %s", key.c_str());
}

bool CRedisClient::lpush(    const string& key, const VALUES& vValue, TINT& retval){
    if (0==key.length()) {
        return false;
    }
	VDATA vCmdData;
    vCmdData.push_back("LPUSH");
    vCmdData.push_back(key);
    addparam(vCmdData, vValue);
    
    return commandargv_integer( vCmdData, retval);
}
bool CRedisClient::lpushx(const string& key, const string& value, TINT& retval){
	if (0==key.length()) {
		return false;
	}
	VDATA vCmdData;
	vCmdData.push_back("LPUSH");
	vCmdData.push_back(key);
	vCmdData.push_back(value);
    return commandargv_integer( vCmdData, retval);
}


bool CRedisClient::lrange(    const string& key, TINT start, TINT end, DATAITEM& array){
    if (0==key.length()) {
        return false;
    }
    
	return command_array( array, "LRANGE %s %lld %lld", key.c_str(), start, end);
}

bool CRedisClient::lrem( const string& key, int count, const string& value, TINT num){
    if (0==key.length()) {
        return false;
    }
    
    return command_integer( num, "LREM %s %d %s", key.c_str(), count, value.c_str());
}

bool CRedisClient::lset( const string& key, int index, const string& value){
    if (0==key.length()) {
        return false;
    }
    
    return command_status( "LSET %s %d %s", key.c_str(), index, value.c_str());
}

bool CRedisClient::ltrim( const string& key, int start, int end){
    if (0==key.length()) {
        return false;
    }
    
    return command_status( "LTRIM %s %d %d", key.c_str(), start, end);
}

bool CRedisClient::rpop(    const string& key, string& value){
    if (0==key.length()) {
        return false;
    }
    
    return command_string( value, "RPOP %s", key.c_str());
}

bool CRedisClient::rpoplpush(    const string& key_src, const string& key_dest, string& value){
    if ((0 == key_src.length()) || (0==key_dest.length())) {
        return false;
    }
    
    return command_string( value, "RPOPLPUSH %s %s", key_src.c_str(), key_dest.c_str());
}

bool CRedisClient::rpush(    const string& key, const VALUES& vValue, TINT& length){
    if (0==key.length()) {
        return false;
    }
	VDATA vCmdData;
    vCmdData.push_back("RPUSH");
    vCmdData.push_back(key);
    addparam(vCmdData, vValue);
    
    return commandargv_integer( vCmdData, length);
}

bool CRedisClient::rpushx(   const string& key, const string& value, TINT& length){
    if (0==key.length()) {
        return false;
    }
    
    return command_integer( length, "RPUSHX %s %s", key.c_str(), value.c_str());
}

