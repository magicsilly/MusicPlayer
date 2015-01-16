#pragma once

//
struct AudioFormat 
{
	long		m_lChannels;		//声道数
	long		m_lSamplesPerSec;	//采样频率
	long		m_lBitsPerSample;	//采样位数
	long		m_lAvgBytesPerSec;	//每秒字节
	long		m_lBlockAlign;		//每样字节
};

//
class AudioBuffer
{
public:
	AudioBuffer( ){ };
	virtual ~AudioBuffer( ){ };

public:
	virtual bool	Init(AudioFormat* pFormat) = 0;
	virtual int		CreateFragmentBuffer(void* pBuffer, int nLength) = 0;
	virtual bool	DestroyFragmentBuffer(int nIndex) = 0;

public:
	virtual bool	IsPlaying(int nIndex) = 0;
	virtual void	Play(int nIndex) = 0;
	virtual void	Stop(int nIndex) = 0;

	virtual void	Volume(int nIndex, int vol) = 0;
	virtual int		Volume( int nIndex ) = 0;
};


//
class AudioDevice
{
public:
	AudioDevice(void){ };
	virtual ~AudioDevice(void){ };

public:
	virtual void	Release( )	{ };
	virtual bool	Initialize(HWND hWnd){ return true; };

public:
	virtual AudioBuffer*	CreateAudioBuffer( ) = 0;
	virtual void			DestroyAudioBuffer(AudioBuffer*& pAudioBuffer) = 0;
};
