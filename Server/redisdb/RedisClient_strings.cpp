

#include "hiredis/hiredis.h"
#include "RedisClient.h"

bool CRedisClient::psetex(    const string& key,  int milliseconds, const string& value) {
    
    return command_bool( "PSETEX %s %d %s", key.c_str(), milliseconds, value.c_str());
}

bool CRedisClient::append(    const string& key,  const string& value) {
    VDATA vCmdData;
    vCmdData.push_back("APPEND");
    vCmdData.push_back(key);
    vCmdData.push_back(value);
    
    return commandargv_status( vCmdData);
}

bool CRedisClient::set(    const string& key,  const string& value) {
    VDATA vCmdData;
    vCmdData.push_back("SET");
    vCmdData.push_back(key);
    vCmdData.push_back(value);
    
    return commandargv_status( vCmdData);
}

bool CRedisClient::set( const string& key, const char *value, int len, int second) {
    
    if (0==second) {
        return command_bool( "set %s %b", key.c_str(), value, len);
    } else {
        return command_bool( "set %s %b EX %d", key.c_str(), value, len, second);
    }
}

bool CRedisClient::setbit( const string& key,  int offset, TINT newbitValue, TINT oldbitValue) {
    
    return command_integer( oldbitValue, "SETBIT %s %d %lld", key.c_str(), offset, newbitValue);
}

bool CRedisClient::get( const string& key,  string& value) {
    
    return command_string( value, "GET %s", key.c_str());
}

bool CRedisClient::getbit(  const string& key, int& offset, int& bit ) {
    
    TINT intval = 0;
    bool bRet = command_integer( intval, "GETBIT %s %d", key.c_str(), offset);
    bit = (int)intval;
    return bRet;
}

bool CRedisClient::getrange(const string& key,  int start, int end, string& out) {
    
    return command_string( out, "GETRANGE %s %d %d", key.c_str(), start, end);
}

bool CRedisClient::getset( const string& key,  const string& newValue, string& oldValue) {
    
    return command_string( oldValue, "GETSET %s %s", key.c_str(), newValue.c_str());
}

bool CRedisClient::mget( const KEYS &  keys, DATAITEM& vDdata) {
    bool bRet = false;

    
    DataItem item;

    KEYS::const_iterator iter_key = keys.begin();
    for (;iter_key!=keys.end();++iter_key) {

        const string &key = *iter_key;
        if (key.length()>0) {
            bool ret = command_string( item.str, "GET %s", key.c_str());
            if (!ret) {
                item.type = REDIS_REPLY_NIL;
                item.str  = "";
            } else {
                item.type = REDIS_REPLY_STRING;
                bRet = true;
            }
            vDdata.push_back(item);
        }
    }
    return bRet;
}

bool CRedisClient::mset( const VDATA& vData) {

    VDATA::const_iterator iter_data = vData.begin();
    for (; iter_data != vData.end();) {
        const string &key = (*iter_data++);
        const string &value = (*iter_data++);

        command_status( "SET %s %s", key.c_str(), value.c_str());
    }
    return true;
}

bool CRedisClient::setex(    const string& key,  int seconds, const string& value) {
    VDATA vCmdData;

    vCmdData.push_back("SETEX");
    vCmdData.push_back(key);
    vCmdData.push_back(toString(seconds));
    vCmdData.push_back(value);
    
    return commandargv_bool( vCmdData);
}

bool CRedisClient::setnx(  const string& key,  const string& value) {
    VDATA vCmdData;
    vCmdData.push_back("SETNX");
    vCmdData.push_back(key);
    vCmdData.push_back(value);
    
    return commandargv_bool( vCmdData);
}

bool CRedisClient::msetnx(  const VDATA& vData) {
	VDATA vCmdData;
	vCmdData.push_back("MSETNX");
	addparam(vCmdData, vData);

	return commandargv_status( vCmdData);
}

bool CRedisClient::setrange(const string& key,  int offset, const string& value, int& length) {
    TINT intval = 0;
    
    bool bRet = command_integer( intval, "setrange %s %d %s", key.c_str(), offset, value.c_str());
    length = (int)intval;
    return bRet;
}

bool CRedisClient::strlen(const string& key, int& length) {
    TINT intval = 0;
    
    bool bRet = command_integer( intval, "STRLEN %s", key.c_str());
    length = (int)intval;
    return bRet;
}

bool CRedisClient::incr(   const string& key, TINT& result) {
    
    return command_integer( result, "INCR %s", key.c_str());
}

bool CRedisClient::incrby( const string& key, int by, TINT& result) {
    
    return command_integer( result, "INCRBY %s %d", key.c_str(), by);
}

bool CRedisClient::bitcount(  const string& key, int& count, int start, int end) {
    TINT intval = 0;
    bool bRet = false;
    
    if ( (start!=0)||(end!=0) ) {
        bRet = command_integer( intval, "bitcount %s %d %d", key.c_str(), start, end);
    } else {
        bRet =  command_integer( intval, "bitcount %s", key.c_str());
    }
    count = (int)intval;
    return bRet;
}

//// 这个实现有问题
//bool CRedisClient::bitop( const BITOP operation, const string& destkey, const KEYS& keys, int& lenght) {
//    static const char *op_cmd[4]= {"AND","OR","XOR","NOT"};
//    VDATA vCmdData;
//    TINT intval = 0;
//    vCmdData.push_back("bitop");
//    vCmdData.push_back(op_cmd[operation]);
//    vCmdData.push_back(destkey);
//    addparam(vCmdData, keys);
//    
//    bool bRet = commandargv_integer( vCmdData, intval);
//    lenght = (int)intval;
//    return bRet;
//}

bool CRedisClient::bitpos( const string& key, int bit, TINT& pos, int start, int end) {
    
    if ( (start!=0)||(end!=0) ) {
        return command_integer( pos, "BITPOS %s %d %d %d", key.c_str(), bit, start, end);
    } 
    return command_integer( pos, "BITPOS %s %d", key.c_str(), bit);
}

bool CRedisClient::decr(   const string& key, TINT& result) {
    
    return command_integer(result,"decr %s", key.c_str());
}

bool CRedisClient::decrby( const string& key, int by, TINT& result) {
    
    return command_integer( result, "decrby %s %d", key.c_str(), by);
}


















