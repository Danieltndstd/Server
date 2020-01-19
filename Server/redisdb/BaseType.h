/*
	文件	：BaseType.h
	简介	：基本数据类型定义

	最后修改日期	修改者		修改内容
	---------------------------------------------------------------------------------------------

	---------------------------------------------------------------------------------------------
*/

#ifndef __BASETYPE_H__
#define __BASETYPE_H__

#pragma warning (disable: 4996)
#pragma warning (disable: 4244)   //这个以后可以适当放开，以检查类型转换的loss of data的问题。

#ifdef FD_SETSIZE
#undef FD_SETSIZE
#endif
#define FD_SETSIZE (2048)

#ifndef __x86_64__
#define __x86_64__
#endif

typedef char				CHAR;
typedef unsigned char		UCHAR;
typedef unsigned char       BYTE;
typedef BYTE*				LPBYTE;

typedef short				SHORT;
typedef unsigned short		USHORT;

typedef int                 INT;
typedef unsigned int        UINT;

typedef long				LONG;
typedef long long			LONGLONG;
typedef unsigned long		ULONG;
typedef unsigned long       u_long;

#ifndef WORD
typedef unsigned short		WORD;
#endif
#ifndef DWORD
typedef unsigned long		DWORD;
#endif

typedef float               FLOAT;
typedef double              DOUBLE;

typedef void               *LPVOID;

#ifdef __x86_64__
    typedef long long TINT;
    typedef double  TFLOAT;
#else
    typedef int TINT;
    typedef float  TFLOAT;
#endif

#ifndef VOID
# define VOID void
#endif

#ifndef NULL
#define NULL    0
#endif

#ifndef FALSE
#define FALSE               0
#endif

#ifndef TRUE
#define TRUE                1
#endif

#ifndef MAX
#define MAX(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef MIN
#define MIN(a,b)            (((a) < (b)) ? (a) : (b))
#endif

#define     DS_INVALID_ID  (-1)

#ifndef SAFE_DELETE
#define SAFE_DELETE(p) { if(p) { delete (p); (p) = NULL; } }
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p); (p) = NULL; } }
#endif

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p) { if(p) { p->release(); (p) = NULL; }}
#endif

#ifndef SAFE_RELEASE_OC
#define SAFE_RELEASE_OC(p)  if(p != nil) { [p release]; p = nil;}
#endif

#ifdef WIN32
#define  ERRORNO WSAGetLastError()
#else
#define ERRORNO	errno
#endif

// 类型定义与非法值定义
typedef int Guid_t;
typedef int ID_t;
typedef int MsgID_t;
typedef int skill_id;
typedef int effect_id;
typedef int impact_id;
typedef int card_id;
typedef int zone_index;


typedef USHORT PacketId_t;

#define invalid_guid         (-1)
#define invalid_obj_id       (-1)
#define invalid_msg_id       (-1)
#define invalid_skill_id     (-1)
#define invalid_effect_id    (-1)
#define invalid_impact_id    (-1)
#define invalid_card_id      (-1)
#define invalid_scene_id     (-1)
#define invalid_item_id      (-1)
#define invalid_zone_id		 (-1)
#define invalid_id			(-1)

const int SLEEP_TIME_MICRO_SECOND_PER_TICK = 50;

const int BUFF_SIZE_64K		= 64 * 1024;
const int BUFF_SIZE_128K	= 128 * 1024;
const int BUFF_SIZE_512K	= 512 * 1024;
const int BUFF_SIZE_8M		= 8 * 1024 * 1024;
const int BUFF_SIZE_16M		= 16 * 1024 * 1024;
const int BUFF_SIZE_12M		= 12 * 1024 * 1024;
const int BUFF_SIZE_24M		= 24 * 1024 * 1024;
// socket ，句柄 定义

#if defined (WIN32)
	#ifndef WIN32_LEAN_AND_MEAN
	#define WIN32_LEAN_AND_MEAN
	#endif // 防止windows.h里包含winsock.h
#define _WINSOCKAPI_
#include <wtypes.h>
#include <WinSock2.h>
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#endif

#if defined (WIN32)

typedef HANDLE DS_HANDLE;
typedef SOCKET DS_SOCKET;

# define DS_INVALID_HANDLE INVALID_HANDLE_VALUE

#else

typedef int DS_HANDLE;
typedef DS_HANDLE DS_SOCKET;

#ifdef INVALID_HANDLE_VALUE
#undef INVALID_HANDLE_VALUE
#endif
#define INVALID_HANDLE_VALUE (-1)


# define DS_INVALID_HANDLE (-1)
# define SOCKET_ERROR (-1)

#ifdef INVALID_SOCKET
#undef INVALID_SOCKET
#endif
#define INVALID_SOCKET (DS_SOCKET)(~0)


#endif /* WIN32 */


#ifndef WIN32
# ifndef _snprintf
#  define _snprintf snprintf
# endif
# ifndef strnicmp
#  define strnicmp strncasecmp
# endif
#endif

#ifndef MAX_PATH
#define MAX_PATH  256
#endif

//////////////////////////////////////////////////////////////////////////
// 定义与寄存器等长的数值类型
template <size_t n> struct __regtype__{typedef int int_r; typedef unsigned int uint_r; };
template <> struct __regtype__<8>     {typedef long long int_r;typedef unsigned long long uint_r;};
typedef __regtype__<sizeof(void*)>::int_r  int_r;
typedef __regtype__<sizeof(void*)>::uint_r uint_r;

#ifdef _CHK_MEM_LEAK_
	#ifdef __cplusplus
	extern "C"
	{
	#endif

		void* calloc_k(size_t nmemb, size_t size);
		void* malloc_k(size_t size);
		void  free_k(void *ptr);
		void* realloc_k(void *ptr, size_t size);

	#ifdef __cplusplus
	}
	#endif

	#ifdef __cplusplus

	#if defined(WIN32) || defined(WINDOWS)

	#include <new>

	void* operator new (size_t size);
	void operator delete (void* ptr);
	//void *operator new[] (size_t size);
	void operator delete[] (void* ptr);
	//void* operator new(size_t, void *_Where);
	_Ret_bytecap_(_Size) void *__CRTDECL operator new[](size_t _Size) _THROW1(std::bad_alloc);

	#else

	#include <new>

	void* operator new(size_t) throw (std::bad_alloc);
	void* operator new [] (size_t) throw (std::bad_alloc);
	void operator delete(void*) throw ();
	void operator delete [] (void*) throw ();
	//void* operator new(size_t, void*) throw ();

	#endif

	#endif

#else
	#if defined(WIN32) || defined(WINDOWS)
		#include <new.h>
		#include <malloc.h>
	#else
		//#include <new>
		#include <memory.h>
	#endif

	#define calloc_k calloc
	#define malloc_k malloc
	#define free_k free
	#define realloc_k realloc

#endif

#ifndef WIN32
# ifndef _snprintf
#  define _snprintf snprintf
# endif
# ifndef strnicmp
#  define strnicmp strncasecmp
# endif
#define PACKED  __attribute__ ((aligned(8)))
#else
#define PACKED
#endif

#include "GlobalDefine.h"
#endif // __BASETYPE_H__
