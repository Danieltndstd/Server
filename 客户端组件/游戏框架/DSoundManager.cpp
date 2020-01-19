#include "StdAfx.h"
#include "WaveFile.h"
#include "DSoundManager.h"
#include "GameFrameEngine.h"
#include "IniConfig.h"
#include <conio.h>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/erase.hpp>
#include <boost/algorithm/string/find.hpp>

/*
* 线程锁
*/
class ThreadLock
{
public:
	ThreadLock() { InitializeCriticalSection(&_handle); }

	~ThreadLock() { DeleteCriticalSection(&_handle); }

	void Lock() { EnterCriticalSection(&_handle); }

	void UnLock() { LeaveCriticalSection(&_handle); }

	class AutoLock
	{
	public:
		AutoLock(ThreadLock& lock) : _lock(lock) { _lock.Lock();}

		~AutoLock() { _lock.UnLock(); }

	private:
		ThreadLock& _lock;
	};

private:
	CRITICAL_SECTION	_handle;
};

ThreadLock s_lock;

/*DSound链接库*/
#pragma comment(lib, "./Other/lib/fmodex_vc.lib")

/*错误检测*/
void ERRCHECK(FMOD_RESULT result)
{
	if(result != FMOD_OK)
	{
		CString errString;
		WCHAR pDst[1024];

		auto iLen = strlen(FMOD_ErrorString(result));
		auto iWLen = MultiByteToWideChar(CP_ACP, 0, FMOD_ErrorString(result), iLen, nullptr, 0);
		iWLen = MultiByteToWideChar(CP_ACP, 0, FMOD_ErrorString(result), iLen, pDst, iWLen);

		pDst[iWLen] = 0;
		errString.Format(TEXT("FMOD error! (%d) %s\n"), result, pDst);

		if(result == FMOD_ERR_INVALID_HANDLE)
		{
			errString += TEXT("请在OnInitGameEngine方法中调用InitializeDSound方法，一般都可以解决问题！\n");
		}

		AfxMessageBox(errString);
	}
}

/*单例对象*/
CDSoundManager* CDSoundManager::GetHinstance()
{
	static CDSoundManager manager;
	return &manager;
}

/*初始化声音对象*/
BOOL CDSoundManager::InitializeDSound(HWND hWnd)
{
	HRESULT hr;

	/*初始化FMOD引擎*/
	if(FAILED(hr = InitializeFmod()))
	{
		CGameFrameEngine::m_bDXSound = false;
		return FALSE;
	}

	/*读取Ini配置*/
	if (FAILED(hr = ReadSoundIni()))
		return FALSE;

	return TRUE;
}

/*写入Ini*/
void CDSoundManager::WriteSoundIni()
{
	/*获取游戏名称*/
	auto pEngine = CGameFrameEngine::GetInstance();

	/*INI对象*/
	IniConfig iniFile;
	iniFile.ReadIni(std::string("Games/SoundSetting.ini"));

	/*写入INI*/
	char szFileName[MAX_PATH] = { 0 };
	if (GetModuleFileNameA(nullptr, szFileName, MAX_PATH))
	{
		//生成路径名
		auto exeName = std::string(szFileName);

		//查找路径尾部
		boost::erase_head(exeName, exeName.find_last_of("\\") + 1);

		//删除exe后缀
		boost::erase_first(exeName, ".exe");

		auto soundSwitch = exeName + ".SoundSwitch";		//音效开关
		auto musicSwitch = exeName + ".MusicSwitch";		//音乐开关
		auto volume = exeName + ".Volume";						//音量

		iniFile.PutString(soundSwitch, boost::lexical_cast<std::string>(m_bSound));
		iniFile.PutString(musicSwitch, boost::lexical_cast<std::string>(m_bMusic));
		iniFile.PutString(volume, boost::lexical_cast<std::string>(m_dwVolume));

		iniFile.WriteIni(std::string("Games/SoundSetting.ini"));
	}
}

/*播放音乐*/
BOOL CDSoundManager::PlayMusic(LPTSTR resourceName)
{
	/*如果不播放音乐直接返回TRUE*/
 	if(m_bMusic == false)
 		return TRUE;

	/*通过FMOD加载音乐*/
	if(ReadSoundByFmod(resourceName))
	{
		FMOD_RESULT		result;
		bool						isPaused;

		/*停止之前的背景音乐*/
		if (m_mapChannels.find(m_pszCurrentMusic) != m_mapChannels.end())
		{
			if (FMOD_OK != (result = m_mapChannels[m_pszCurrentMusic]->getPaused(&isPaused)))
				return FALSE;

			if (FALSE == isPaused)
			{
				// 停止当前音乐
				result = m_mapChannels[m_pszCurrentMusic]->setPaused(true);
				ERRCHECK(result);
			}
		}

		if(FMOD_OK != (result = m_mapChannels[resourceName]->getPaused(&isPaused)))
			return FALSE;

		if(TRUE == isPaused)
		{
			result = m_mapChannels[resourceName]->setMode(FMOD_LOOP_NORMAL);
			ERRCHECK(result);
			result = m_mapChannels[resourceName]->setPaused(false);
			ERRCHECK(result);
		}

		m_pSystem->update();

		/*当前音乐*/
		_tcscpy(m_pszCurrentMusic, resourceName);

		return TRUE;
	}

	return FALSE;
}

/*播放音效*/
BOOL CDSoundManager::PlaySound(LPTSTR resourceName)
{
	/*如果不播放音效直接返回TRUE*/
	if(m_bSound == false)
		return TRUE;

	/*通过FMOD加载音乐*/
	if(ReadSoundByFmod(resourceName))
	{
		FMOD_RESULT		result;

		m_mapChannels[resourceName]->stop();

		result = m_pSystem->playSound(FMOD_CHANNEL_FREE, m_mapSounds[resourceName], true, &m_mapChannels[resourceName]);
		ERRCHECK(result);
		result = m_mapChannels[resourceName]->setChannelGroup(m_pChannelGroup);
		ERRCHECK(result);
		result = m_mapChannels[resourceName]->setPaused(false);
		ERRCHECK(result);

		m_pSystem->update();

		return TRUE;
	}

	return FALSE;
}

/*停止全部播放*/
BOOL CDSoundManager::Stop()
{
	/*全部停止*/

	FMOD_MODE mode;

	auto itt = m_mapChannels.begin();
	for (; itt != m_mapChannels.end(); ++itt)
	{
		itt->second->getMode(&mode);

		if((mode & FMOD_LOOP_NORMAL) == FMOD_LOOP_NORMAL)
		{
			itt->second->setPaused(true);
		}
		else
		{
			itt->second->stop();
		}
	}

	return TRUE;
}

/*停止单曲播放*/
BOOL CDSoundManager::Stop(LPTSTR resourceName)
{
	/*寻找该声音资源是否已经载入*/
	if(m_mapChannels.count(resourceName))
	{
		FMOD_RESULT result;
		FMOD_MODE	mode;

		/*尝试停止播放*/
		result = m_mapChannels[resourceName]->getMode(&mode);
		ERRCHECK(result);

		if((FMOD_LOOP_NORMAL & mode) == FMOD_LOOP_NORMAL)
			result = m_mapChannels[resourceName]->setPaused(true);
		else
			result = m_mapChannels[resourceName]->stop();
		ERRCHECK(result);

		return TRUE;
	}

	return TRUE;
}

/*刷新*/
BOOL CDSoundManager::Flush()
{
	/*重新设置音量、音效、背景音乐*/
	if(!m_bSound)
		Stop();

	if(m_bMusic)
		PlayMusic(m_pszCurrentMusic);
	else
		Stop(m_pszCurrentMusic);

	SetVolume();

	return TRUE;
}

CDSoundManager::~CDSoundManager(void)
{
}

CDSoundManager::CDSoundManager(void)
{
	/*初始化当前音乐*/
	_tcscpy(m_pszCurrentMusic, TEXT("BACK_MUSIC"));

	/*默认40音量*/
	m_dwVolume = 100;

	/*变量初始化*/
	m_bMusic = true;
	m_bSound = true;
}

HRESULT CDSoundManager::InitializeFmod()
{
	/*变量声明*/
	CString				warningString;
	FMOD_RESULT			result;
	FMOD_SPEAKERMODE	speakermode;
	FMOD_CAPS			caps;
	unsigned int		version;
	int					numdrivers;
	char				name[256];

	/*创建一个系统实例并初始化*/
	result = FMOD::System_Create(&m_pSystem);
	ERRCHECK(result);

	result = m_pSystem->getVersion(&version);
	ERRCHECK(result);

	if(version < FMOD_VERSION)
	{
		warningString.Format(TEXT("Error!  You are using an old version of FMOD %08x.  This program requires %08x\n"), version, FMOD_VERSION);
		AfxMessageBox(warningString);
		return -1;
	}

	result = m_pSystem->getNumDrivers(&numdrivers);
	ERRCHECK(result);

	if(numdrivers == 0)
	{
		result = m_pSystem->setOutput(FMOD_OUTPUTTYPE_NOSOUND);
		ERRCHECK(result);
	}
	else
	{
		result = m_pSystem->getDriverCaps(0, &caps, nullptr, &speakermode);
		ERRCHECK(result);

		result = m_pSystem->setSpeakerMode(speakermode);
		ERRCHECK(result);

		if(caps & FMOD_CAPS_HARDWARE_EMULATED)
		{
			result = m_pSystem->setDSPBufferSize(1024, 10);
			ERRCHECK(result);
		}

		result = m_pSystem->getDriverInfo(0, name, 256, nullptr);
		ERRCHECK(result);

		if(strstr(name, "SigmaTel"))
		{
			result = m_pSystem->setSoftwareFormat(48000, FMOD_SOUND_FORMAT_PCMFLOAT, 0, 0, FMOD_DSP_RESAMPLER_LINEAR);
			ERRCHECK(result);
		}
	}

	result = m_pSystem->init(100, FMOD_INIT_NORMAL, nullptr);
	if(result == FMOD_ERR_OUTPUT_CREATEBUFFER)
	{
		result = m_pSystem->setSpeakerMode(FMOD_SPEAKERMODE_STEREO);
		ERRCHECK(result);
		result = m_pSystem->init(100, FMOD_INIT_NORMAL, nullptr);
		ERRCHECK(result);
	}

	result = m_pSystem->createChannelGroup("SoundGroup", &m_pChannelGroup);
	ERRCHECK(result);

	return 0L;
}

HRESULT CDSoundManager::ReadSoundIni()
{
	/*获取游戏名称*/
	auto pEngine = CGameFrameEngine::GetInstance();

	/*读入INI文件*/
	IniConfig iniFile;
	iniFile.ReadIni(std::string("Games/SoundSetting.ini"));

	/*检测文件完整性*/
	{
		char szFileName[MAX_PATH] = { 0 };
		if (GetModuleFileNameA(nullptr, szFileName, MAX_PATH))
		{
			//生成路径名
			auto exeName = std::string(szFileName);

			//查找路径尾部
			boost::erase_head(exeName, exeName.find_last_of("\\") + 1);

			//删除exe后缀
			boost::erase_first(exeName, ".exe");

			auto soundSwitch = exeName + ".SoundSwitch";		//音效开关
			auto musicSwitch = exeName + ".MusicSwitch";		//音乐开关
			auto volume = exeName + ".Volume";						//音量

			if (iniFile.Count(exeName) == 0)	//未找到
			{
				iniFile.PutString(soundSwitch, boost::lexical_cast<std::string>(true));
				iniFile.PutString(musicSwitch, boost::lexical_cast<std::string>(true));
				iniFile.PutString(volume, boost::lexical_cast<std::string>(100));

				iniFile.WriteIni(std::string("Games/SoundSetting.ini"));
			}
			else
			{
				m_bSound = iniFile.GetValue(soundSwitch, true);
				m_bMusic = iniFile.GetValue(musicSwitch, true);
				m_dwVolume = iniFile.GetValue(volume, 100);
			}
		}
	}

	return 0L;
}

BOOL CDSoundManager::ReadSoundByFmod(LPTSTR resourceName)
{
	/*已经载入音乐*/
	if(m_mapSounds.count(resourceName) != 0)
		return TRUE;

	/*声明变量*/
	FMOD::Sound*	pSound;
	FMOD::Channel*	pChannel;
	FMOD_RESULT		result;
	char			pszResourceName[MAX_PATH] = "\0";

	/*转换格式*/
#ifdef _UNICODE
	{
		auto nlength = wcslen(resourceName);
		auto nbytes = WideCharToMultiByte(0, 0, resourceName, nlength, nullptr, 0, nullptr, nullptr);
		nbytes = WideCharToMultiByte(0, 0, resourceName, nlength, pszResourceName, nbytes, nullptr, nullptr);
	}
#endif

	/*加载声音*/
	result = m_pSystem->createSound(pszResourceName, FMOD_2D, 0, &pSound);
	if(result != FMOD_OK)
	{
		FMOD_CREATESOUNDEXINFO	exinfo;
		HRSRC					hResInfo;
		HGLOBAL					hResData;
		DWORD					dwSize;
		VOID*					pvRes;

		/*文件夹中不存在该文件，试从内存中加载*/
		if(nullptr == (hResInfo = FindResource(nullptr, resourceName, L"WAVE")))
		{
			if(nullptr == (hResInfo = FindResource(nullptr, resourceName, L"WAV")))
				return FALSE;
		}

		if(nullptr == (hResData = LoadResource(GetModuleHandle(nullptr), hResInfo)))
			return FALSE;

		if(0 == (dwSize = SizeofResource(GetModuleHandle(nullptr), hResInfo)))
			return FALSE;

		if(nullptr == (pvRes = LockResource(hResData)))
			return FALSE;

		ZeroMemory(&exinfo, sizeof(FMOD_CREATESOUNDEXINFO));
		exinfo.cbsize = sizeof(FMOD_CREATESOUNDEXINFO);
		exinfo.length = dwSize;

		result = m_pSystem->createSound((const char*)pvRes, FMOD_HARDWARE | FMOD_OPENMEMORY, &exinfo, &pSound);
		ERRCHECK(result);
		result = m_pSystem->playSound(FMOD_CHANNEL_FREE, pSound, true, &pChannel);
		ERRCHECK(result);
		result = pChannel->setChannelGroup(m_pChannelGroup);
		ERRCHECK(result);

		/*插入map*/
		m_mapSounds[resourceName] = pSound;
		m_mapChannels[resourceName] = pChannel;
	}

	return TRUE;
}

BOOL CDSoundManager::ReleaseFmod()
{
	/*停止播放*/
	Stop();

	/*释放声音对象*/
	MAPSOUNDS::iterator it = m_mapSounds.begin();
	for (; it != m_mapSounds.end(); ++it)
	{
		it->second->release();
		it->second = nullptr;
	}

	/*退出声音管理组*/
	m_pChannelGroup->release();

	/*清空map*/
	m_mapSounds.clear();

	/*释放资源（dll中工作不正常？）*/

	FMOD_RESULT result;
	result = m_pSystem->close();
	ERRCHECK(result);
	result = m_pSystem->release();
	ERRCHECK(result);

	return TRUE;
}

/*调整全体音量*/
BOOL CDSoundManager::SetVolume()
{
	/*调节音量*/
	FMOD_RESULT result;

	result = m_pChannelGroup->setVolume((m_dwVolume / 100.0f));
	ERRCHECK(result);

	m_pSystem->update();

	return TRUE;
}

/*调整单曲音量*/
HRESULT CDSoundManager::SetVolume(LPTSTR resourceName)
{
	/*寻找该声音资源是否已经载入*/
	if(m_mapChannels.count(resourceName))
	{
		/*尝试调节音量*/
		return m_mapChannels[resourceName]->setVolume((m_dwVolume / 100.0f));
	}

	return 0L;
}