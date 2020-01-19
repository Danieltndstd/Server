#pragma once

#include <map>
#include <dsound.h>
#include <boost/noncopyable.hpp>

/*声音缓冲区map*/
typedef std::map<CString, LPDIRECTSOUNDBUFFER8> MAPBUFFERS;

class CDSoundManager : public boost::noncopyable
{
public:
	/*单例*/
	static CDSoundManager* GetHinstance();

	/*初始化声音对象*/
	BOOL InitializeDSound(HWND hWnd);
	/*播放音乐*/
	BOOL PlayMusic(LPTSTR resourceName);
	/*播放音效*/
	BOOL PlaySound(LPTSTR resourceName);
	/*停止全部播放*/
	BOOL Stop();
	/*停止单曲播放*/
	BOOL Stop(LPTSTR resourceName);
	/*全体调整*/
	BOOL Flush();
	
	/*析构函数-释放所有的声音资源*/
	~CDSoundManager(void);

	/*音乐开关*/
	__declspec(property(put = SetMusicEffect)) bool music_effect;
	void SetMusicEffect(bool val){ m_bMusic = val; }

	/*音效开关*/
	__declspec(property(put = SetSoundEffect)) bool sound_ffect;
	void SetSoundEffect(bool val){ m_bSound = val; }

	/*音量赋值*/
	__declspec(property(put = SetVolumeVal)) DWORD volume_value;
	void SetVolumeVal(DWORD val){ m_dwVolume = val; }

private:
	/*构造函数-私有*/
	CDSoundManager(void);

	/*读取wav声音资源并创建一个从缓冲区储存*/
	BOOL ReadSoundInBuffer(LPTSTR resourceName);
	/*计算音量*/
	DWORD TransformVolume();
	/*调整全部音量*/
	BOOL SetVolume();
	/*调整单曲音量*/
	HRESULT SetVolume(LPTSTR resourceName);

private:
	LPDIRECTSOUND8	m_pSound8;		//设备对象
	MAPBUFFERS		m_mapBuffers;	//缓冲区map

	bool			m_bMusic;		//背景音乐播放
	bool			m_bSound;		//音效
	DWORD			m_dwVolume;		//音量
};
