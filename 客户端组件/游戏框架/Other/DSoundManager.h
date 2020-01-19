#pragma once

#include <map>
#include <dsound.h>
#include <boost/noncopyable.hpp>

/*����������map*/
typedef std::map<CString, LPDIRECTSOUNDBUFFER8> MAPBUFFERS;

class CDSoundManager : public boost::noncopyable
{
public:
	/*����*/
	static CDSoundManager* GetHinstance();

	/*��ʼ����������*/
	BOOL InitializeDSound(HWND hWnd);
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
	
	/*��������-�ͷ����е�������Դ*/
	~CDSoundManager(void);

	/*���ֿ���*/
	__declspec(property(put = SetMusicEffect)) bool music_effect;
	void SetMusicEffect(bool val){ m_bMusic = val; }

	/*��Ч����*/
	__declspec(property(put = SetSoundEffect)) bool sound_ffect;
	void SetSoundEffect(bool val){ m_bSound = val; }

	/*������ֵ*/
	__declspec(property(put = SetVolumeVal)) DWORD volume_value;
	void SetVolumeVal(DWORD val){ m_dwVolume = val; }

private:
	/*���캯��-˽��*/
	CDSoundManager(void);

	/*��ȡwav������Դ������һ���ӻ���������*/
	BOOL ReadSoundInBuffer(LPTSTR resourceName);
	/*��������*/
	DWORD TransformVolume();
	/*����ȫ������*/
	BOOL SetVolume();
	/*������������*/
	HRESULT SetVolume(LPTSTR resourceName);

private:
	LPDIRECTSOUND8	m_pSound8;		//�豸����
	MAPBUFFERS		m_mapBuffers;	//������map

	bool			m_bMusic;		//�������ֲ���
	bool			m_bSound;		//��Ч
	DWORD			m_dwVolume;		//����
};
