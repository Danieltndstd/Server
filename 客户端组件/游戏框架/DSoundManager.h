#pragma once

#include <map>
#include <dsound.h>
#include <boost/noncopyable.hpp>

/*声音缓冲区map*/
typedef std::map<CString, FMOD::Sound*> MAPSOUNDS;
typedef std::map<CString, FMOD::Channel*> MAPCHANNELS;

class CDSoundManager : public boost::noncopyable
{
public:
	/*单例*/
	static CDSoundManager* GetHinstance();

	/*初始化声音对象*/
	BOOL InitializeDSound(HWND hWnd);
	/*写入Ini*/
	void	WriteSoundIni();
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
	/*释放FMOD资源*/
	BOOL ReleaseFmod();
	
	
	/*析构函数-释放所有的声音资源*/
	~CDSoundManager(void);

	/*音乐开关*/
	__declspec(property(get = GetMusicEffect, put = SetMusicEffect)) bool music_effect;
	bool GetMusicEffect(void) const { return m_bMusic; }
	void SetMusicEffect(bool val){ m_bMusic = val; }

	/*音效开关*/
	__declspec(property(get = GetSoundEffect, put = SetSoundEffect)) bool sound_ffect;
	bool GetSoundEffect(void) const { return m_bSound; }
	void SetSoundEffect(bool val){ m_bSound = val; }

	/*音量赋值*/
	__declspec(property(get = GetVolume, put = SetVolume)) DWORD volume_value;
	DWORD GetVolume(void) const { return m_dwVolume; }
	void SetVolume(DWORD val){ m_dwVolume = val; }

private:
	/*构造函数-私有*/
	CDSoundManager(void);

	/*计算音量*/
	DWORD TransformVolume();
	/*调整全部音量*/
	BOOL SetVolume();
	/*调整单曲音量*/
	HRESULT SetVolume(LPTSTR resourceName);

	/*初始化FMOD引擎*/
	HRESULT InitializeFmod();
	/*读取Ini配置文件*/
	HRESULT ReadSoundIni();
	/*通过FMOD加载DSound无法播放的音乐*/
	BOOL ReadSoundByFmod(LPTSTR resourceName);

private:
	MAPSOUNDS			m_mapSounds;				//FMOD声音
	MAPCHANNELS			m_mapChannels;				//FMOD管理声音

	bool				m_bMusic;					//背景音乐播放
	bool				m_bSound;					//音效
	DWORD				m_dwVolume;					//音量

	FMOD::System*		m_pSystem;					//引擎系统
	FMOD::ChannelGroup*	m_pChannelGroup;			//声音管理组
	TCHAR				m_pszCurrentMusic[MAX_PATH];//当前音乐
};
