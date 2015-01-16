
#include "DSoundDevice.h"


//
DSoundBuffer::DSoundBuffer(DSoundDevice* pDevice)
			:	m_pDevice(pDevice),
				m_pDirectSoundBuffer(NULL)
{
	m_nFragmentIndex			= 0;


	DSBUFFERDESC	BufferDesc;
	ZeroMemory(&BufferDesc, sizeof(DSBUFFERDESC));
	BufferDesc.dwSize			= sizeof(DSBUFFERDESC);
	BufferDesc.dwFlags			= DSBCAPS_PRIMARYBUFFER;
	BufferDesc.dwBufferBytes	= 0;
	BufferDesc.dwReserved		= 0;
	BufferDesc.lpwfxFormat		= NULL;
	BufferDesc.guid3DAlgorithm	= GUID_NULL;

	HRESULT hResult = m_pDevice->GetDirectSound()->CreateSoundBuffer(&BufferDesc, &m_pDirectSoundBuffer, NULL);
	if(FAILED(hResult)){ return ; }
}

DSoundBuffer::~DSoundBuffer( )
{
	std::map<int, IDirectSoundBuffer*>::iterator i = m_FragmentBuffers.begin();
	for( ; i != m_FragmentBuffers.end(); i ++)
	{ 
		i->second->Stop();
		i->second->Release(); 
	}
	m_FragmentBuffers.clear();

	if(m_pDirectSoundBuffer)
	{
		m_pDirectSoundBuffer->Release( );
		m_pDirectSoundBuffer = NULL;
	}
	m_pDevice	= NULL;
}

bool	DSoundBuffer::Init(AudioFormat* pFormat)
{
	if(!m_pDirectSoundBuffer){ return false; }
	m_AudioFormat				= *pFormat;

	WAVEFORMATEX	WaveFormat;
	ZeroMemory(&WaveFormat, sizeof(WAVEFORMATEX));
	WaveFormat.cbSize			= sizeof(WAVEFORMATEX);
	WaveFormat.wFormatTag		= WAVE_FORMAT_PCM;
	WaveFormat.nChannels		= (WORD)pFormat->m_lChannels;
	WaveFormat.nSamplesPerSec	= (DWORD)pFormat->m_lSamplesPerSec;
	WaveFormat.wBitsPerSample	= (WORD)pFormat->m_lBitsPerSample;
	WaveFormat.nBlockAlign		= (WORD)(WaveFormat.wBitsPerSample / 8 * WaveFormat.nChannels);
	WaveFormat.nAvgBytesPerSec	= (DWORD)(WaveFormat.nSamplesPerSec * WaveFormat.nBlockAlign);

	HRESULT hResult = m_pDirectSoundBuffer->SetFormat(&WaveFormat);
	if(FAILED(hResult)){ return false; }

	//
	return true;
}

int		DSoundBuffer::CreateFragmentBuffer(void* pBuffer, int nLength)
{
	WAVEFORMATEX	WaveFormat;
	HRESULT hResult = m_pDirectSoundBuffer->GetFormat(&WaveFormat, sizeof(WAVEFORMATEX), NULL);
	if(FAILED(hResult)){ return 0; }

	DSBUFFERDESC	BufferDesc;
	ZeroMemory(&BufferDesc, sizeof(DSBUFFERDESC));
	BufferDesc.dwSize			= sizeof(DSBUFFERDESC);
	BufferDesc.dwFlags			= DSBCAPS_GLOBALFOCUS| DSBCAPS_CTRLVOLUME| DSBCAPS_CTRLFREQUENCY;
	BufferDesc.dwBufferBytes	= nLength;
	BufferDesc.dwReserved		= 0;
	BufferDesc.lpwfxFormat		= &WaveFormat;
	BufferDesc.guid3DAlgorithm	= GUID_NULL;

	IDirectSoundBuffer* pTempDirectSoundBuffer = NULL;
	hResult = m_pDevice->GetDirectSound()->CreateSoundBuffer(&BufferDesc, &pTempDirectSoundBuffer, NULL);
	if(FAILED(hResult)){ return 0; }

	void*	pTempBuffer = NULL;
	DWORD	nTempLength	= 0;
	hResult = pTempDirectSoundBuffer->Lock(0, nLength, &pTempBuffer, &nTempLength, NULL, NULL, 0);
	if(SUCCEEDED(hResult))
	{
		memcpy(pTempBuffer, pBuffer, nLength);
		pTempDirectSoundBuffer->Unlock(pTempBuffer, nTempLength, NULL, 0);
	}

	//
	m_FragmentBuffers[++m_nFragmentIndex] = pTempDirectSoundBuffer;
	return m_nFragmentIndex;
}

bool	DSoundBuffer::DestroyFragmentBuffer(int nIndex)
{
	std::map<int, IDirectSoundBuffer*>::iterator i = m_FragmentBuffers.find(nIndex);
	if( i == m_FragmentBuffers.end() ){ return false; }
	
	// 正在播放就停下来
	DWORD nStatus = 0;
	i->second->GetStatus(&nStatus);
	if( nStatus & DSBSTATUS_PLAYING )
	{ i->second->Stop( ); }

	i->second->Release( );
	i->second = NULL;
	m_FragmentBuffers.erase(i);
	return true;
}

bool	DSoundBuffer::IsPlaying(int nIndex)
{
	std::map<int, IDirectSoundBuffer*>::iterator i = m_FragmentBuffers.find(nIndex);
	if( i == m_FragmentBuffers.end() ){ return false; }

	DWORD nStatus = 0;
	i->second->GetStatus(&nStatus);
	if( nStatus & DSBSTATUS_PLAYING )
	{ return true; }

	return false;
}

void	DSoundBuffer::Play(int nIndex)
{
	std::map<int, IDirectSoundBuffer*>::iterator i = m_FragmentBuffers.find(nIndex);
	if( i == m_FragmentBuffers.end() ){ return ; }

	HRESULT hResult = i->second->Play(0, 0, 0);
	if(FAILED(hResult)){ return; }

	i->second->SetVolume(0);
}

void	DSoundBuffer::Stop(int nIndex)
{
	std::map<int, IDirectSoundBuffer*>::iterator i = m_FragmentBuffers.find(nIndex);
	if( i == m_FragmentBuffers.end() ){ return ; }

	HRESULT hResult = i->second->Stop( );
	if(FAILED(hResult)){ return; }
}

void	DSoundBuffer::Volume(int nIndex, int vol)
{
	std::map<int, IDirectSoundBuffer*>::iterator i = m_FragmentBuffers.find(nIndex);
	if( i == m_FragmentBuffers.end() ){ return ; }

	HRESULT hResult = i->second->SetVolume(vol);
	if(FAILED(hResult)){ return; }
}

int		DSoundBuffer::Volume(int nIndex)
{
	std::map<int, IDirectSoundBuffer*>::iterator i = m_FragmentBuffers.find(nIndex);
	if( i == m_FragmentBuffers.end() ){ return 0; }

	long vol = 0;
	HRESULT hResult = i->second->GetVolume(&vol);
	if(FAILED(hResult)){ return 0; }
	return vol;
}

//
DSoundDevice::DSoundDevice(void)
{
	m_pDirectSound	= NULL;
}

DSoundDevice::~DSoundDevice(void)
{
	Release( );
}

//
void	DSoundDevice::Release( )
{
	if( m_pDirectSound )
	{
		m_pDirectSound->Release( );
		m_pDirectSound = NULL;
	}
}

//
bool	DSoundDevice::Initialize(HWND hWnd)
{
	HRESULT	hResult	= CoCreateInstance(CLSID_DirectSound, NULL, CLSCTX_INPROC_SERVER, IID_IDirectSound, (LPVOID*)&m_pDirectSound);
	if( FAILED(hResult) ){ return false; }
	
	hResult = m_pDirectSound->Initialize(NULL);
	if( FAILED(hResult) ){ return false; }

	hResult = m_pDirectSound->SetCooperativeLevel(hWnd, DSSCL_PRIORITY);
	if( FAILED(hResult) ){ return false; }

	//
	return true;
}

//
AudioBuffer*	DSoundDevice::CreateAudioBuffer( )
{
	return new DSoundBuffer(this);
}

//
void			DSoundDevice::DestroyAudioBuffer(AudioBuffer*& pAudioBuffer)
{
	delete pAudioBuffer;
	pAudioBuffer = NULL;
}