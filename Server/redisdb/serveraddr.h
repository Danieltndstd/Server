#ifndef ServerAddr_h__
#define ServerAddr_h__

#pragma once

#include "BaseType.h"
#define MAX_IP_STR_LEN      16

// 服务器的IP及port
struct ServerAddr
{
    ServerAddr()
    {
        clean();
    }
    ServerAddr(const char* szServerIP, unsigned int nPort)
    {
        set(szServerIP, nPort);
    }
    void	set(const char* szServerIP, unsigned int nPort)
    {
        strncpy(IP, szServerIP, MAX_IP_STR_LEN);
        IP[MAX_IP_STR_LEN - 1] = '\0';
        Port = nPort;
    }

    void	clean()
    {
        memset(IP, 0, sizeof(IP));
        Port = 0;
    }

    int	size() const {
        return (sizeof(IP) + sizeof(Port) + sizeof(ServerPort));
    }

    ServerAddr& operator=(const ServerAddr& rhs)
    {
        if (this != &rhs){
            strncpy(IP, rhs.IP, MAX_IP_STR_LEN);
            IP[MAX_IP_STR_LEN - 1] = '\0';
            Port = rhs.Port;
        }
        return *this;
    }

	DWORD tol() const
	{
		return  ntohl( inet_addr(IP)); 
	}


    char	IP[MAX_IP_STR_LEN];
    unsigned short	Port;
	unsigned short	ServerPort;

};

inline bool operator < (const ServerAddr& lhs, const ServerAddr& rhs)
{
    int nRet = strcmp(&lhs.IP[0], &rhs.IP[0]);
    if (nRet < 0)
        return true;
    if (nRet > 0)
        return false;

    return lhs.Port < lhs.Port;
}
inline bool operator == (const ServerAddr& lhs, const ServerAddr& rhs)
{
    int nRet = strcmp(&lhs.IP[0], &rhs.IP[0]);
    if (nRet != 0)
        return false;

    return (lhs.Port == rhs.Port);
}

// login服务器验证后，玩家登陆到game的密匙等
struct LoginGameData
{
    LoginGameData(){
        nToken = 0;
    }
    int		nToken;
};
inline bool operator == (const LoginGameData& lhs, const LoginGameData& rhs){
    return (lhs.nToken == rhs.nToken);
}


#endif