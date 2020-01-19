

#ifndef _CRedis_CLIENT_H_
#define _CRedis_CLIENT_H_

#include "BaseType.h"
#include "RedisConnector.h"
//#include "LogSystem.h"

typedef string             KEY;
typedef string             VALUE;
typedef vector<KEY>        KEYS;
typedef KEYS               FILEDS;
typedef vector<VALUE>      VALUES;
typedef vector<string>     VDATA;


typedef enum _BIT_OP_{
	AND = 0,
	OR  = 1,
	XOR = 2,
	NOT = 3
}BITOP;

typedef struct _SORT_LIMIT_
{ 
	int offset; 
	int count; 
}LIMIT;

typedef enum _SORT_ORDER_{
	ASC = 0,
	DESC = 1
}SORTODER;


typedef enum _LIST_MODEL_{
	BEFORE = 0,
	AFTER  = 1
}LMODEL;

class RedisPool;
class CRedisClient;

typedef struct _DATA_ITEM_{
    int    type;
    string str;
    
    _DATA_ITEM_ & operator=(const _DATA_ITEM_ &data) {
        type = data.type;
        str  = data.str;
        return *this;
    }
}DataItem;


typedef vector<DataItem> DATAITEM;

template<class T>
string toString(const T &t) {
    ostringstream oss;
    oss << t;
    return oss.str();
}


class CRedisClient :public RedisConnector
{

public:
    CRedisClient();
    ~CRedisClient();

    void Keepalive();

    static void FreeReply(redisReply* reply);

public:
	
    //              Connection
    /* AUTH        */  /* nonsupport */
    /* ECHO        */  bool echo( const string& str, string &value);
    /* PING        */  /* nonsupport */
 
    /* QUIT        */  void quit();
    /* SELECT      */  /* nonsupport */

    //                 Commands operating on string values
    /* APPEND      */  bool append(  const string& key,  const string& value);
    /* BITCOUNT    */  bool bitcount(const string& key, int& count, int start=0, int end=0);
    /* BITOP       */  bool bitop(   const BITOP operation, const string& destkey, const KEYS& keys, int& lenght);
    /* BITPOS      */  bool bitpos(  const string& key, int bit, TINT& pos, int start=0, int end=0);
    /* DECR        */  bool decr(    const string& key, TINT& result);
    /* DECRBY      */  bool decrby(  const string& key, int by, TINT& result);
    /* GET         */  bool get(     const string& key, string& value);
    /* GETBIT      */  bool getbit(  const string& key, int& offset, int& bit);
    /* GETRANGE    */  bool getrange(const string& key, int start, int end, string& out);
    /* GETSET      */  bool getset(  const string& key, const string& newValue, string& oldValue);
    /* INCR        */  bool incr(    const string& key, TINT& result);
    /* INCRBY      */  bool incrby(  const string& key, int by, TINT& result);
    /* INCRBYFLOAT */  
    /* MGET        */  bool mget(    const KEYS &  keys, DATAITEM& vDdata);
    /* MSET        */  bool mset(    const VDATA& data);
    /* MSETNX      */  
    /* PSETEX      */  bool psetex(  const string& key,  int milliseconds, const string& value);
    /* SET         */  bool set(     const string& key,  const string& value);
    /* SET         */  bool set(     const string& key, const char *value, int len, int second=0);
    /* SETBIT      */  bool setbit(  const string& key,  int offset, TINT newbitValue, TINT oldbitValue);
    /* SETEX       */  bool setex(   const string& key,  int seconds, const string& value);
    /* SETNX       */  bool setnx(   const string& key,  const string& value);
	/* msetnx      */  bool msetnx(  const VDATA& vData);

    /* SETRANGE    */  bool setrange(const string& key,  int offset, const string& value, int& length);
    /* STRLEN      */  bool strlen(  const string& key, int& length);


    /* DEL          */  bool del(    const string& key);
                        bool del(      const KEYS &  vkey, TINT& count);
    /* DUMP         */
    /* EXISTS       */  bool exists( const string& key);
    /* EXPIRE       */  bool expire( const string& key, unsigned int second);
    /* EXPIREAT     */  bool expireat( const string& key, unsigned int timestamp);
    /* KEYS         */  
    /* MIGRATE      */  
    /* MOVE         */  
    /* OBJECT       */  
    /* PERSIST      */  bool persist( const string& key);
    /* PEXPIRE      */  bool pexpire( const string& key, unsigned int milliseconds);
    /* PEXPIREAT    */  bool pexpireat( const string& key, unsigned int millisecondstimestamp);
    /* PTTL         */  bool pttl( const string& key,  TINT &milliseconds);
    /* RANDOMKEY    */  bool randomkey(  KEY& key);
    /* RENAME       */  
    /* RENAMENX     */  
    /* RESTORE      */  
    /* SCAN         */  

    
    /* SORT         */  bool sort( DATAITEM& array, const string& key, const char* by = NULL,
        LIMIT *limit = NULL, bool alpha = false, const FILEDS* get = NULL, 
        const SORTODER order = ASC, const char* destination = NULL);

    /* TTL          */  bool ttl( const string& key, TINT& seconds);
    /* TYPE         */  


    /* HDEL         */  bool hdel(    const string& key, const string& field, TINT& num);
                        bool hdel(    const string& key, const KEYS& vfiled, TINT& num);
    /* HEXISTS      */  bool hexists(  const string& key, const string& field);
    /* HGET         */  bool hget(    const string& key, const string& field, string& value);
    /* HGETALL      */  bool hgetall( const string& key, DATAITEM& array);
    /* HINCRBY      */  bool hincrby( const string& key, const string& field, TINT increment ,TINT& value);
    /* HINCRBYFLOAT */  bool hincrbyfloat(  const string& key, const string& field, const float increment, float& value);
    /* HKEYS        */  bool hkeys(   const string& key, KEYS& keys);
    /* HLEN         */  bool hlen(    const string& key, TINT& count);
    /* HMGET        */  bool hmget(   const string& key, const KEYS& field, DATAITEM& array);
    /* HMSET        */  bool hmset(   const string& key, const VDATA& vData);
    /* HSCAN        */                                      
    /* HSET         */  bool hset(    const string& key, const string& field, const string& value, TINT& retval);
    /* HSETNX       */  bool hsetnx(  const string& key, const string& field, const string& value);
    /* HVALS        */  bool hvals(   const string& key, VALUES& values);

    /* BLPOP        */  
    /* BRPOP        */  
    /* BRPOPLPUSH   */  
    /* LINDEX       */  bool lindex(    const string& key, TINT index, VALUE& value);
    /* LINSERT      */  bool linsert(  const string& key, LMODEL mod, const string& pivot, const string& value, TINT& retval);
    /* LLEN         */  bool llen(     const string& key, TINT& len);
    /* LPOP         */  bool lpop(     const string& key, string& value);
    /* LPUSH        */  bool lpush(    const string& key, const VALUES& vValue, TINT& retval);
    /* LPUSHX       */  bool lpushx(   const string& key, const string& value, TINT& retval);
    /* LRANGE       */  bool lrange(   const string& key, TINT start, TINT end, DATAITEM& array);
    /* LREM         */  bool lrem(     const string& key,  int count, const string& value, TINT num);
    /* LSET         */  bool lset(     const string& key,  int index, const string& value);
    /* LTRIM        */  bool ltrim(    const string& key,  int start, int end);
    /* RPOP         */  bool rpop(     const string& key, string& value);
    /* RPOPLPUSH    */  bool rpoplpush(const string& key_src, const string& key_dest, string& value);
    /* RPUSH        */  bool rpush(    const string& key, const VALUES& vValue, TINT& length);
    /* RPUSHX       */  bool rpushx(   const string& key, const string& value, TINT& length);



    /* SADD         */  bool sadd(        const KEY& key, const VALUES& vValue, TINT& count);
    /* SCARD        */  bool scard( const KEY& key, TINT& count);
    /* SDIFF        */  bool sdiff(       const KEYS& vKkey, VALUES& vValue);
    /* SDIFFSTORE   */  bool sdiffstore(  const KEY& destinationkey,  const KEYS& vkey, TINT& count);
    /* SINTER       */  bool sinter(      const KEYS& vkey, VALUES& vValue);
    /* SINTERSTORE  */  bool sinterstore( const KEY& destinationkey,  const KEYS& vkey, TINT& count);
    /* SISMEMBER    */  bool sismember(   const KEY& key,   const VALUE& member);
    /* SMEMBERS     */  bool smembers(     const KEY& key,  VALUES& vValue);
    /* SMOVE        */  bool smove(       const KEY& srckey, const KEY& deskey,  const VALUE& member);
    /* SPOP         */  bool spop(        const KEY& key, VALUE& member);
    /* SRANDMEMBER  */  bool srandmember( const KEY& key, VALUES& vmember, int num=0);
    /* SREM         */  bool srem(        const KEY& key, const VALUES& vmembers, TINT& count);
    /* SSCAN        */  
    /* SUNION       */  bool sunion(      const KEYS& vkey, VALUES& vValue);
    /* SUNIONSTORE  */  bool sunionstore( const KEY& deskey,  const KEYS& vkey, TINT& count);

    /* ZADD             */  bool zadd(    const KEY& deskey,   const VALUES& vValues, TINT& count);
    /* ZCARD            */  bool zscrad(  const string& key, TINT& num);
    /* ZCOUNT           */
    /* ZINCRBY          */  bool zincrby( const string& key, const double &increment, const string& member, string& value );
    /* ZINTERSTORE      */  
    /* ZRANGE           */  bool zrange(  const string& key, int start, int end, VALUES& vValues, bool withscore=false);
    /* ZRANGEBYSCORE    */  bool zrangebyscore(  const string& key, string& start, string& stop,VALUES& vValues, bool withscore = false);
    /* ZRANK            */  bool zrank(   const string& key, const string& member, TINT &rank);
    /* ZREM             */  bool zrem(    const KEY& key, const VALUES& vmembers, TINT &num);
    /* ZREMRANGEBYRANK  */  bool zremrangebyrank(  const string& key, int start, int stop, TINT& num);
    /* ZREMRANGEBYSCORE */  
    /* ZREVRANGE        */  bool zrevrange(  const string& key, int start, int end, VALUES& vValues, bool withscore=false);
    /* ZREVRANGEBYSCORE */  
    /* ZREVRANK         */  bool zrevrank(  const string& key, const string &member, TINT& rank);
    /* ZSCAN            */  
    /* ZSCORE           */  bool zscore(  const string& key, const string &member, string& score);
    /* ZUNIONSTORE      */  

    /* PSUBSCRIBE   */
    /* PUBLISH      */
    /* PUBSUB       */
    /* PUNSUBSCRIBE */
    /* SUBSCRIBE    */
    /* UNSUBSCRIBE  */


    /* DISCARD  */
    /* EXEC     */
    /* MULTI    */
    /* UNWATCH  */
    /* WATCH    */


private:
    void addparam(VDATA& vDes, const VDATA& vSrc) {
        for (VDATA::const_iterator iter=vSrc.begin(); iter!=vSrc.end();++iter) {
            vDes.push_back(*iter);
        }
    }
    void SetDebugString( const char *str);
	void SetDebugString( const VDATA& vData);
	void SetDebugMessage( const char* fmt, ...);


    void SetErrString( const char *str);
    void SetErrString( const VDATA& vData);
    void SetErrMessage( const char* fmt, ...);

public:

    bool command_bool(                       const char* cmd, ...);
    bool command_status(                     const char* cmd, ...);
    bool command_integer( TINT &intval,   const char* cmd, ...);
    bool command_string(  string &data,      const char* cmd, ...);
    bool command_list(    VALUES &vValue,    const char* cmd, ...);
    bool command_array(   DATAITEM& array, const char* cmd, ...);
    redisReply *command( const char* cmd);
private:
    bool commandargv_bool(   const VDATA& vData);
    bool commandargv_status( const VDATA& vData);
    bool commandargv_array(  const VDATA& vDataIn, DATAITEM& array);
    bool commandargv_array(  const VDATA& vDataIn, VALUES& array);
    bool commandargv_integer(const VDATA& vDataIn, TINT& retval);

};



#endif





