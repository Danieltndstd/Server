

#include "RedisClient.h"

bool  CRedisClient::del(    const string& key) {
    if (0==key.length()) {
        return false;
    }

    
    return command_bool( "DEL %s", key.c_str());
}

bool  CRedisClient::del(    const KEYS &  vkey, TINT& count) {
    count = 0;

    KEYS::const_iterator iter_key     = vkey.begin();
    for(;iter_key!=vkey.end();++iter_key) {
        const string &key     = (*iter_key);
        if (del( key)) {
            count++;
        }
    }
    return true;
}

bool CRedisClient::exists( const string& key) {
    if (0==key.length()) {
        return false;
    }
    
    return command_bool( "EXISTS %s", key.c_str());
}

bool CRedisClient::expire( const string& key, unsigned int second) {
    if (0==key.length()) {
        return false;
    }
    
    TINT ret = -1;
    if (!command_integer( ret, "EXPIRE %s %u", key.c_str(), second)) {
        return false;
    }

    if (1==ret) {
        return true;
    } else {
        SetErrMessage( "expire return %ld ", ret);
        return false;
    }
}

bool CRedisClient::expireat( const string& key, unsigned int timestamp) {
    if (0==key.length()) {
        return false;
    }
    
    return command_bool( "EXPIREAT %s %u", key.c_str(), timestamp);
}

bool CRedisClient::persist( const string& key) {
    if (0==key.length()) {
        return false;
    }
    
    return command_bool( "PERSIST %s %u", key.c_str());
}

bool CRedisClient::pexpire( const string& key, unsigned int milliseconds) {
    if (0==key.length()) {
        return false;
    }
    return command_bool( "PEXPIRE %s %u", key.c_str(), milliseconds);
}

bool CRedisClient::pexpireat( const string& key, unsigned int millisecondstimestamp) {
    if (0==key.length()) {
        return false;
    }
    
    return command_bool( "PEXPIREAT %s %u", key.c_str(), millisecondstimestamp);
}

bool CRedisClient::pttl( const string& key, TINT &milliseconds) {
    if (0==key.length()) {
        return false;
    }
    
    return command_integer( milliseconds, "PTTL %s", key.c_str());
}

bool CRedisClient::ttl( const string& key, TINT &seconds) {
    if (0==key.length()) {
        return false;
    }
    
    return command_integer( seconds, "TTL %s", key.c_str());
}

bool CRedisClient::randomkey( KEY& key){
    
    return command_string( key, "RANDOMKEY");
}




bool CRedisClient::sort( DATAITEM& array, const string& key, const char* by,
    LIMIT *limit /*= NULL*/, bool alpha /*= false*/, const FILEDS* get /*= NULL*/,
    const SORTODER order /*= ASC*/, const char* destination )
{
    static const char *sort_order[3] = { "ASC", "DESC" };
    if (0 == key.length()) {
        return false;
    }
       

    VDATA vCmdData;
    vCmdData.push_back("sort");
    vCmdData.push_back(key);
    if (NULL != by) {
        vCmdData.push_back("by");
        vCmdData.push_back(by);
    }

    if (NULL != limit) {
        vCmdData.push_back("LIMIT");
        vCmdData.push_back(toString(limit->offset));
        vCmdData.push_back(toString(limit->count));
    }
    if (alpha) {
        vCmdData.push_back("ALPHA");
    }

    if (NULL != get) {
        for (FILEDS::const_iterator iter = get->begin(); iter != get->end(); ++iter) {
            vCmdData.push_back("get");
            vCmdData.push_back(*iter);
        }
    }

    vCmdData.push_back(sort_order[order]);
    if (destination) {
        vCmdData.push_back(destination);
    }
    
    return commandargv_array( vCmdData, array);
}






