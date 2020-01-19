

#include "RedisClient.h"

bool CRedisClient::sadd(     const string& key, const VALUES& vValue, TINT& count){
    VDATA vCmdData;
    vCmdData.push_back("SADD");
    vCmdData.push_back(key);
    addparam(vCmdData, vValue);
    
    return commandargv_integer( vCmdData, count);
}

bool CRedisClient::scard(     const string& key, TINT& count){
    if (0==key.length()) {
        return false;
    }
    
    return command_integer( count, "SCARD %s", key.c_str());
}

bool CRedisClient::sdiff(     const KEYS& vkey, VALUES& sValue){
	size_t size = vkey.size();
    if (0 == size) {
        return false;
    }
    VALUES *setData = new VALUES[size];
    VALUES::iterator endpos;

    KEYS::const_iterator     iter_key = vkey.begin();
    int i=0;
    for (; iter_key!=vkey.end(); ++iter_key, ++i) {
        const string &key = *iter_key;

        if (!smembers( key, setData[i])) {
            delete [] setData;
            return false;
        }
    }

    size_t n=0;
    while(n++<size-1) {
        endpos = set_difference( setData[n].begin(), setData[n].end(), setData[n+1].begin(), setData[n+1].end() , sValue.begin());
        sValue.resize( endpos - sValue.begin());
    }
    delete [] setData;
    return true;
}

bool CRedisClient::sdiffstore(  const KEY& destinationkey,   const KEYS& vkey, TINT& count){
    VALUES sValue;
    if (!sdiff(vkey, sValue)) {
       return false;
    }
    return sadd( destinationkey, sValue, count);
}

bool CRedisClient::sinter( const KEYS& vkey, VALUES& sValue){
    size_t size = vkey.size();
    VALUES *setData = new VALUES[size];
    VALUES::iterator endpos;

    KEYS::const_iterator     iter_key = vkey.begin();
    int i=0;
    for (; iter_key!=vkey.end(); ++iter_key,  ++i) {
        const string &key = *iter_key;

        if (!smembers( key, setData[i])) {
            delete [] setData;
            return false;
        }
    }

    size_t n=0;
    while(n++<size-1){
        endpos = set_intersection( setData[n].begin(), setData[n].end(), setData[n+1].begin(), setData[n+1].end() , sValue.begin());
        sValue.resize( endpos - sValue.begin());
    }
    delete [] setData;

    return true;
}

bool CRedisClient::sinterstore(const KEY& destinationkey,  const KEYS& vkey, TINT& count){
    VALUES sValue;
    if (!sinter( vkey, sValue)) {
        return false;
    }
    return sadd(destinationkey, sValue, count);
}

bool CRedisClient::sismember(  const KEY& key,   const VALUE& member){
    if (0==key.length()) {
        return false;
    }
    return command_bool( "SISMEMBER %s %s", key.c_str(), member.c_str());
}

bool CRedisClient::smembers(  const KEY& key, VALUES& vValue){
    if (0==key.length()) {
        return false;
    }
    
    return command_list( vValue, "SMEMBERS %s", key.c_str());
}

bool CRedisClient::smove(  const KEY& srckey, const KEY& deskey,  const VALUE& member){
    if (0==srckey.length()) {
        return false;
    }
    
    return command_bool( "SMOVE %s", srckey.c_str(), deskey.c_str(), member.c_str());
}

bool CRedisClient::spop(  const KEY& key, VALUE& member){
    if (0==key.length()) {
        return false;
    }
    
    return command_string( member, "SPOP %s", key.c_str());
}

bool CRedisClient::srandmember(  const KEY& key, VALUES& members, int count){
    if (0==key.length()) {
        return false;
    }
    
    if (0==count) {
        return command_list( members, "SRANDMEMBER %s", key.c_str());
    }
    return command_list( members, "SRANDMEMBER %s %d", key.c_str(), count);
}

bool CRedisClient::srem(  const KEY& key, const VALUES& vmembers, TINT& count){
    if (0==key.length()) {
        return false;
    }
    
    VDATA vCmdData;
    vCmdData.push_back("SREM");
    vCmdData.push_back(key);
    addparam(vCmdData, vmembers);
    return commandargv_integer( vCmdData, count);
}

bool CRedisClient::sunion(     const KEYS& vkey, VALUES& sValue){
    size_t size = vkey.size();
    VALUES *setData = new VALUES[size];
    VALUES::iterator endpos;

    KEYS::const_iterator     iter_key = vkey.begin();
    int i=0;
    for (; iter_key!=vkey.end(); ++iter_key,++i) {
        const string &key = *iter_key;

        if (!smembers( key, setData[i])) {
            delete [] setData;
            return false;
        }
    }

    size_t n=0;
    while(n++<size-1) {
            endpos = set_union( setData[n].begin(), setData[n].end(), setData[n+1].begin(), setData[n+1].end() , sValue.begin());
        sValue.resize( endpos - sValue.begin());
    }
    delete [] setData;
    return true;
}

bool CRedisClient::sunionstore(  const KEY& deskey,  const KEYS& vkey, TINT& count){
    VALUES sValue;

    if (!sunion(vkey, sValue)) {
        return false;
    }
    return sadd( deskey, sValue, count);
}


