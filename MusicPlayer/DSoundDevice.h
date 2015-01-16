#pragma once

//
#include <map>

//
#include <dxsdk/dsound.h>
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dsound.lib")

//
#include "AudioDevice.h"

//
class DSoundDevice;

//
class DSoundBuffer : public AudioBuffer
{
public:
	DSoundBuffer(DSoundDevice* pDevice);
	virtual ~DSoundBuffer( );

private:
	DSoundDevice*			m_pDevice;
	IDirectSoundBuffer*		m_pDirectSoundBuffer;
	
	AudioFormat				m_AudioFormat;
	int						m_nFragmentIndex;

	std::map<int, IDirectSoundBuffer*>	m_FragmentBuffers;

public:
	virtual bool	Init(AudioFormat* pFormat);
	virtual int		CreateFragmentBuffer(void* pBuffer, int nLength);
	virtual bool	DestroyFragmentBuffer(int nIndex);

public:
	virtual bool	IsPlaying(int nIndex);
	virtual void	Play(int nIndex);
	virtual void	Stop(int nIndex);

	virtual void	Volume(int nIndex, int vol);
	virtual int		Volume(int nIndex);
};


//
class DSoundDevice : public AudioDevice
{
public:
	DSoundDevice(void);
	virtual ~DSoundDevice(void);

private:
	IDirectSound*		m_pDirectSound;

public:
	IDirectSound*	GetDirectSound( ){ return m_pDirectSound; }

public:
	virtual void	Release( );
	virtual bool	Initialize(HWND hWnd);

public:
	virtual AudioBuffer*	CreateAudioBuffer( );
	virtual void			DestroyAudioBuffer(AudioBuffer*& pAudioBuffer);
};
