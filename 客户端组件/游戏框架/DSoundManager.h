#pragma once

#include <map>
#include <dsound.h>
#include <boost/noncopyable.hpp>

/*����������map*/
typedef std::map<CString, FMOD::Sound*> MAPSOUNDS;
typedef std::map<CString, FMOD::Channel*> MAPCHANNELS;

class CDSoundManager : public boost::noncopyable
{
public:
	/*����*/
	static CDSoundManager* GetHinstance();

	/*��ʼ����������*/
	BOOL InitializeDSound(HWND hWnd);
	/*д��Ini*/
	void	WriteSoundIni();
	/*��������*/
	BOOL PlayMusic(LPTSTR resourceName);
	/*������Ч*/
	BOOL PlaySound(LPTSTR resourceName);
	/*ֹͣȫ������*/
	BOOL Stop();
	/*ֹͣ��������*/
	BOOL Stop(LPTSTR resourceName);
	/*ȫ�����*/
	BOOL Flush();
	/*�ͷ�FMOD��Դ*/
	BOOL ReleaseFmod();
	
	
	/*��������-�ͷ����е�������Դ*/
	~CDSoundManager(void);

	/*���ֿ���*/
	__declspec(property(get = GetMusicEffect, put = SetMusicEffect)) bool music_effect;
	bool GetMusicEffect(void) const { return m_bMusic; }
	void SetMusicEffect(bool val){ m_bMusic = val; }

	/*��Ч����*/
	__declspec(property(get = GetSoundEffect, put = SetSoundEffect)) bool sound_ffect;
	bool GetSoundEffect(void) const { return m_bSound; }
	void SetSoundEffect(bool val){ m_bSound = val; }

	/*������ֵ*/
	__declspec(property(get = GetVolume, put = SetVolume)) DWORD volume_value;
	DWORD GetVolume(void) const { return m_dwVolume; }
	void SetVolume(DWORD val){ m_dwVolume = val; }

private:
	/*���캯��-˽��*/
	CDSoundManager(void);

	/*��������*/
	DWORD TransformVolume();
	/*����ȫ������*/
	BOOL SetVolume();
	/*������������*/
	HRESULT SetVolume(LPTSTR resourceName);

	/*��ʼ��FMOD����*/
	HRESULT InitializeFmod();
	/*��ȡIni�����ļ�*/
	HRESULT ReadSoundIni();
	/*ͨ��FMOD����DSound�޷����ŵ�����*/
	BOOL ReadSoundByFmod(LPTSTR resourceName);

private:
	MAPSOUNDS			m_mapSounds;				//FMOD����
	MAPCHANNELS			m_mapChannels;				//FMOD��������

	bool				m_bMusic;					//�������ֲ���
	bool				m_bSound;					//��Ч
	DWORD				m_dwVolume;					//����

	FMOD::System*		m_pSystem;					//����ϵͳ
	FMOD::ChannelGroup*	m_pChannelGroup;			//����������
	TCHAR				m_pszCurrentMusic[MAX_PATH];//��ǰ����
};
