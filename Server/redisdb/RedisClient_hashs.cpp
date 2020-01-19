

#include "RedisClient.h"

bool CRedisClient::hdel(    const string& key, const string& field, TINT& count) {
    
    return command_integer( count, "HDEL %s %s %s", key.c_str(), field.c_str());
}

bool CRedisClient::hdel(    const string& key, const KEYS& vfiled, TINT& count) {
    VDATA vCmdData;
    vCmdData.push_back("HDEL");
    vCmdData.push_back(key);
    addparam(vCmdData, vfiled);
    
    return commandargv_integer( vCmdData, count);
}

bool CRedisClient::hexists(   const string& key, const string& field){
    
    return command_bool("HEXISTS %s %s", key.c_str(), field.c_str());
}

bool CRedisClient::hget(    const string& key, const string& field, string& value) {
    
    return command_string( value, "HGET %s %s", key.c_str(), field.c_str());
}

bool  CRedisClient::hgetall(    const string& key, DATAITEM& array){
    
    return command_array( array, "HGETALL %s", key.c_str());
}

bool CRedisClient::hincrby(  const string& key, const string& field, TINT increment, TINT& num ) {
    
    return command_integer( num, "HINCRBY %s %s %lld", key.c_str(),field.c_str(), increment);
}

bool CRedisClient::hincrbyfloat(  const string& key, const string& field, float increment, float& value ) {
    
    bool bRet = false;

    redisReply *reply = static_cast<redisReply *>(redisCommand(getCtx(), "HINCRBYFLOAT %s %s %f", key.c_str(), field.c_str(), increment));
    if (CheckReply(reply)) {
        value = atof(reply->str);
        bRet = true;
    }

    FreeReply(reply);
    
    return bRet;
}

bool CRedisClient::hkeys(  const string& key, KEYS& keys){
    
    return command_list( keys, "HKEYS %s", key.c_str());
}

bool CRedisClient::hlen(  const string& key, TINT& count){
    
    return command_integer( count, "HLEN %s", key.c_str());
}

bool CRedisClient::hmget(    const string& key, const KEYS& field, DATAITEM& array){
    VDATA vCmdData;
    vCmdData.push_back("HMGET");
    vCmdData.push_back(key);
    addparam(vCmdData, field);
    
    return commandargv_array( vCmdData, array);
}

bool CRedisClient::hmset(    const string& key, const VDATA& vData){
    VDATA vCmdData;
    vCmdData.push_back("HMSET");
    vCmdData.push_back(key);
    addparam(vCmdData, vData);
    
    return commandargv_status( vCmdData);
}

bool CRedisClient::hset(    const string& key, const string& field, const string& value, TINT& retval){
    
    return command_integer( retval, "HSET %s %s %s", key.c_str(), field.c_str(), value.c_str());
}

bool CRedisClient::hsetnx(    const string& key, const string& field, const string& value){
    
    return command_bool( "HSETNX %s %s %s", key.c_str(), field.c_str(), value.c_str());
}

bool CRedisClient::hvals(  const string& key, VALUES& values) {
    
    return command_list( values, "HVALS %s", key.c_str());
}


