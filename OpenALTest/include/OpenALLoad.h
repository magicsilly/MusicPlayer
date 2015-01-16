#ifndef	__OpenALLoad_H__
#define __OpenALLoad_H__

#pragma once

//
#include <windows.h>
#include <cassert>


//
#include <al\al.h>
#include <al\alc.h>


//
namespace	OpenAL
{
	//
	#ifndef ALchar
		#define ALchar char
	#endif

	#ifndef ALCchar
		#define ALCchar char
	#endif

	//
	typedef void           (ALAPIENTRY *LPALENABLE)( ALenum capability );
	typedef void           (ALAPIENTRY *LPALDISABLE)( ALenum capability ); 
	typedef ALboolean      (ALAPIENTRY *LPALISENABLED)( ALenum capability ); 
	typedef const ALchar*  (ALAPIENTRY *LPALGETSTRING)( ALenum param );
	typedef void           (ALAPIENTRY *LPALGETBOOLEANV)( ALenum param, ALboolean* data );
	typedef void           (ALAPIENTRY *LPALGETINTEGERV)( ALenum param, ALint* data );
	typedef void           (ALAPIENTRY *LPALGETFLOATV)( ALenum param, ALfloat* data );
	typedef void           (ALAPIENTRY *LPALGETDOUBLEV)( ALenum param, ALdouble* data );
	typedef ALboolean      (ALAPIENTRY *LPALGETBOOLEAN)( ALenum param );
	typedef ALint          (ALAPIENTRY *LPALGETINTEGER)( ALenum param );
	typedef ALfloat        (ALAPIENTRY *LPALGETFLOAT)( ALenum param );
	typedef ALdouble       (ALAPIENTRY *LPALGETDOUBLE)( ALenum param );
	typedef ALenum         (ALAPIENTRY *LPALGETERROR)( void );
	typedef ALboolean      (ALAPIENTRY *LPALISEXTENSIONPRESENT)(const ALchar* extname );
	typedef void*          (ALAPIENTRY *LPALGETPROCADDRESS)( const ALchar* fname );
	typedef ALenum         (ALAPIENTRY *LPALGETENUMVALUE)( const ALchar* ename );
	typedef void           (ALAPIENTRY *LPALLISTENERF)( ALenum param, ALfloat value );
	typedef void           (ALAPIENTRY *LPALLISTENER3F)( ALenum param, ALfloat value1, ALfloat value2, ALfloat value3 );
	typedef void           (ALAPIENTRY *LPALLISTENERFV)( ALenum param, const ALfloat* values );
	typedef void           (ALAPIENTRY *LPALLISTENERI)( ALenum param, ALint value );
	typedef void           (ALAPIENTRY *LPALGETLISTENERF)( ALenum param, ALfloat* value );
	typedef void           (ALAPIENTRY *LPALGETLISTENER3F)( ALenum param, ALfloat *value1, ALfloat *value2, ALfloat *value3 );
	typedef void           (ALAPIENTRY *LPALGETLISTENERFV)( ALenum param, ALfloat* values );
	typedef void           (ALAPIENTRY *LPALGETLISTENERI)( ALenum param, ALint* value );
	typedef void           (ALAPIENTRY *LPALGENSOURCES)( ALsizei n, ALuint* sources ); 
	typedef void           (ALAPIENTRY *LPALDELETESOURCES)( ALsizei n, const ALuint* sources );
	typedef ALboolean      (ALAPIENTRY *LPALISSOURCE)( ALuint sid ); 
	typedef void           (ALAPIENTRY *LPALSOURCEF)( ALuint sid, ALenum param, ALfloat value); 
	typedef void           (ALAPIENTRY *LPALSOURCE3F)( ALuint sid, ALenum param, ALfloat value1, ALfloat value2, ALfloat value3 );
	typedef void           (ALAPIENTRY *LPALSOURCEFV)( ALuint sid, ALenum param, const ALfloat* values );
	typedef void           (ALAPIENTRY *LPALSOURCEI)( ALuint sid, ALenum param, ALint value); 
	typedef void           (ALAPIENTRY *LPALGETSOURCEF)( ALuint sid, ALenum param, ALfloat* value );
	typedef void           (ALAPIENTRY *LPALGETSOURCE3F)( ALuint sid, ALenum param, ALfloat* value1, ALfloat* value2, ALfloat* value3);
	typedef void           (ALAPIENTRY *LPALGETSOURCEFV)( ALuint sid, ALenum param, ALfloat* values );
	typedef void           (ALAPIENTRY *LPALGETSOURCEI)( ALuint sid, ALenum param, ALint* value );
	typedef void           (ALAPIENTRY *LPALSOURCEPLAYV)( ALsizei ns, const ALuint *sids );
	typedef void           (ALAPIENTRY *LPALSOURCESTOPV)( ALsizei ns, const ALuint *sids );
	typedef void           (ALAPIENTRY *LPALSOURCEREWINDV)( ALsizei ns, const ALuint *sids );
	typedef void           (ALAPIENTRY *LPALSOURCEPAUSEV)( ALsizei ns, const ALuint *sids );
	typedef void           (ALAPIENTRY *LPALSOURCEPLAY)( ALuint sid );
	typedef void           (ALAPIENTRY *LPALSOURCESTOP)( ALuint sid );
	typedef void           (ALAPIENTRY *LPALSOURCEREWIND)( ALuint sid );
	typedef void           (ALAPIENTRY *LPALSOURCEPAUSE)( ALuint sid );
	typedef void           (ALAPIENTRY *LPALSOURCEQUEUEBUFFERS)(ALuint sid, ALsizei numEntries, const ALuint *bids );
	typedef void           (ALAPIENTRY *LPALSOURCEUNQUEUEBUFFERS)(ALuint sid, ALsizei numEntries, ALuint *bids );
	typedef void           (ALAPIENTRY *LPALGENBUFFERS)( ALsizei n, ALuint* buffers );
	typedef void           (ALAPIENTRY *LPALDELETEBUFFERS)( ALsizei n, const ALuint* buffers );
	typedef ALboolean      (ALAPIENTRY *LPALISBUFFER)( ALuint bid );
	typedef void           (ALAPIENTRY *LPALBUFFERDATA)( ALuint bid, ALenum format, const ALvoid* data, ALsizei size, ALsizei freq );
	typedef void           (ALAPIENTRY *LPALGETBUFFERF)( ALuint bid, ALenum param, ALfloat* value );
	typedef void           (ALAPIENTRY *LPALGETBUFFERI)( ALuint bid, ALenum param, ALint* value );
	typedef void           (ALAPIENTRY *LPALDOPPLERFACTOR)( ALfloat value );
	typedef void           (ALAPIENTRY *LPALDOPPLERVELOCITY)( ALfloat value );
	typedef void           (ALAPIENTRY *LPALDISTANCEMODEL)( ALenum distanceModel );

	typedef ALCcontext *   (ALCAPIENTRY *LPALCCREATECONTEXT) (ALCdevice *device, const ALCint *attrlist);
	typedef ALCboolean     (ALCAPIENTRY *LPALCMAKECONTEXTCURRENT)( ALCcontext *context );
	typedef void           (ALCAPIENTRY *LPALCPROCESSCONTEXT)( ALCcontext *context );
	typedef void           (ALCAPIENTRY *LPALCSUSPENDCONTEXT)( ALCcontext *context );
	typedef void           (ALCAPIENTRY *LPALCDESTROYCONTEXT)( ALCcontext *context );
	typedef ALCcontext *   (ALCAPIENTRY *LPALCGETCURRENTCONTEXT)( ALCvoid );
	typedef ALCdevice *    (ALCAPIENTRY *LPALCGETCONTEXTSDEVICE)( ALCcontext *context );
	typedef ALCdevice *    (ALCAPIENTRY *LPALCOPENDEVICE)( const ALCchar *devicename );
	typedef ALCboolean     (ALCAPIENTRY *LPALCCLOSEDEVICE)( ALCdevice *device );
	typedef ALCenum        (ALCAPIENTRY *LPALCGETERROR)( ALCdevice *device );
	typedef ALCboolean     (ALCAPIENTRY *LPALCISEXTENSIONPRESENT)( ALCdevice *device, const ALCchar *extname );
	typedef void *         (ALCAPIENTRY *LPALCGETPROCADDRESS)(ALCdevice *device, const ALCchar *funcname );
	typedef ALCenum        (ALCAPIENTRY *LPALCGETENUMVALUE)(ALCdevice *device, const ALCchar *enumname );
	typedef const ALCchar* (ALCAPIENTRY *LPALCGETSTRING)( ALCdevice *device, ALCenum param );
	typedef void           (ALCAPIENTRY *LPALCGETINTEGERV)( ALCdevice *device, ALCenum param, ALCsizei size, ALCint *dest );

	//
	// XRAM Extension function pointer variables and enum values
	typedef ALboolean		(__cdecl *LPEAXSETBUFFERMODE)(ALsizei n, ALuint *buffers, ALint value);
	typedef ALenum			(__cdecl *LPEAXGETBUFFERMODE)(ALuint buffer, ALint *value);

	//
	class Load
	{
	public:
		Load( char *szOALFullPathName = "openal32.dll" )
			: m_hOpenALDLL( NULL )
		{
			//
			assert( !ms_pfn ); ms_pfn = this;

			//
			if (szOALFullPathName)
			{ m_hOpenALDLL = LoadLibraryA(szOALFullPathName); }
			else
			{ m_hOpenALDLL = LoadLibraryA("openal32.dll"); }
			if (!m_hOpenALDLL){ return ; }

			// Get function pointers
			this->alEnable = (LPALENABLE)GetProcAddress(m_hOpenALDLL, "alEnable");
			if (this->alEnable == NULL)
			{
				OutputDebugStringA("Failed to retrieve 'alEnable' function address\n");
				return ;
			}
			this->alDisable = (LPALDISABLE)GetProcAddress(m_hOpenALDLL, "alDisable");
			if (this->alDisable == NULL)
			{
				OutputDebugStringA("Failed to retrieve 'alDisable' function address\n");
				return ;
			}
			this->alIsEnabled = (LPALISENABLED)GetProcAddress(m_hOpenALDLL, "alIsEnabled");
			if (this->alIsEnabled == NULL)
			{
				OutputDebugStringA("Failed to retrieve 'alIsEnabled' function address\n");
				return ;
			}
			this->alGetBoolean = (LPALGETBOOLEAN)GetProcAddress(m_hOpenALDLL, "alGetBoolean");
			if (this->alGetBoolean == NULL)
			{
				OutputDebugStringA("Failed to retrieve 'alGetBoolean' function address\n");
				return ;
			}
			this->alGetInteger = (LPALGETINTEGER)GetProcAddress(m_hOpenALDLL, "alGetInteger");
			if (this->alGetInteger == NULL)
			{
				OutputDebugStringA("Failed to retrieve 'alGetInteger' function address\n");
				return ;
			}
			this->alGetFloat = (LPALGETFLOAT)GetProcAddress(m_hOpenALDLL, "alGetFloat");
			if (this->alGetFloat == NULL)
			{
				OutputDebugStringA("Failed to retrieve 'alGetFloat' function address\n");
				return ;
			}
			this->alGetDouble = (LPALGETDOUBLE)GetProcAddress(m_hOpenALDLL, "alGetDouble");
			if (this->alGetDouble == NULL)
			{
				OutputDebugStringA("Failed to retrieve 'alGetDouble' function address\n");
				return ;
			}
			this->alGetBooleanv = (LPALGETBOOLEANV)GetProcAddress(m_hOpenALDLL, "alGetBooleanv");
			if (this->alGetBooleanv == NULL)
			{
				OutputDebugStringA("Failed to retrieve 'alGetBooleanv' function address\n");
				return ;
			}
			this->alGetIntegerv = (LPALGETINTEGERV)GetProcAddress(m_hOpenALDLL, "alGetIntegerv");
			if (this->alGetIntegerv == NULL)
			{
				OutputDebugStringA("Failed to retrieve 'alGetIntegerv' function address\n");
				return ;
			}
			this->alGetFloatv = (LPALGETFLOATV)GetProcAddress(m_hOpenALDLL, "alGetFloatv");
			if (this->alGetFloatv == NULL)
			{
				OutputDebugStringA("Failed to retrieve 'alGetFloatv' function address\n");
				return ;
			}
			this->alGetDoublev = (LPALGETDOUBLEV)GetProcAddress(m_hOpenALDLL, "alGetDoublev");
			if (this->alGetDoublev == NULL)
			{
				OutputDebugStringA("Failed to retrieve 'alGetDoublev' function address\n");
				return ;
			}
			this->alGetString = (LPALGETSTRING)GetProcAddress(m_hOpenALDLL, "alGetString");
			if (this->alGetString == NULL)
			{
				OutputDebugStringA("Failed to retrieve 'alGetString' function address\n");
				return ;
			}
			this->alGetError = (LPALGETERROR)GetProcAddress(m_hOpenALDLL, "alGetError");
			if (this->alGetError == NULL)
			{
				OutputDebugStringA("Failed to retrieve 'alGetError' function address\n");
				return ;
			}
			this->alIsExtensionPresent = (LPALISEXTENSIONPRESENT)GetProcAddress(m_hOpenALDLL, "alIsExtensionPresent");
			if (this->alIsExtensionPresent == NULL)
			{
				OutputDebugStringA("Failed to retrieve 'alIsExtensionPresent' function address\n");
				return ;
			}
			this->alGetProcAddress = (LPALGETPROCADDRESS)GetProcAddress(m_hOpenALDLL, "alGetProcAddress");
			if (this->alGetProcAddress == NULL)
			{
				OutputDebugStringA("Failed to retrieve 'alGetProcAddress' function address\n");
				return ;
			}
			this->alGetEnumValue = (LPALGETENUMVALUE)GetProcAddress(m_hOpenALDLL, "alGetEnumValue");
			if (this->alGetEnumValue == NULL)
			{
				OutputDebugStringA("Failed to retrieve 'alGetEnumValue' function address\n");
				return ;
			}
			this->alListeneri = (LPALLISTENERI)GetProcAddress(m_hOpenALDLL, "alListeneri");
			if (this->alListeneri == NULL)
			{
				OutputDebugStringA("Failed to retrieve 'alListeneri' function address\n");
				return ;
			}
			this->alListenerf = (LPALLISTENERF)GetProcAddress(m_hOpenALDLL, "alListenerf");
			if (this->alListenerf == NULL)
			{
				OutputDebugStringA("Failed to retrieve 'alListenerf' function address\n");
				return ;
			}
			this->alListener3f = (LPALLISTENER3F)GetProcAddress(m_hOpenALDLL, "alListener3f");
			if (this->alListener3f == NULL)
			{
				OutputDebugStringA("Failed to retrieve 'alListener3f' function address\n");
				return ;
			}
			this->alListenerfv = (LPALLISTENERFV)GetProcAddress(m_hOpenALDLL, "alListenerfv");
			if (this->alListenerfv == NULL)
			{
				OutputDebugStringA("Failed to retrieve 'alListenerfv' function address\n");
				return ;
			}
			this->alGetListeneri = (LPALGETLISTENERI)GetProcAddress(m_hOpenALDLL, "alGetListeneri");
			if (this->alGetListeneri == NULL)
			{
				OutputDebugStringA("Failed to retrieve 'alGetListeneri' function address\n");
				return ;
			}
			this->alGetListenerf =(LPALGETLISTENERF)GetProcAddress(m_hOpenALDLL, "alGetListenerf");
			if (this->alGetListenerf == NULL)
			{
				OutputDebugStringA("Failed to retrieve 'alGetListenerf' function address\n");
				return ;
			}
			this->alGetListener3f = (LPALGETLISTENER3F)GetProcAddress(m_hOpenALDLL, "alGetListener3f");
			if (this->alGetListener3f == NULL)
			{
				OutputDebugStringA("Failed to retrieve 'alGetListener3f' function address\n");
				return ;
			}
			this->alGetListenerfv = (LPALGETLISTENERFV)GetProcAddress(m_hOpenALDLL, "alGetListenerfv");
			if (this->alGetListenerfv == NULL)
			{
				OutputDebugStringA("Failed to retrieve 'alGetListenerfv' function address\n");
				return ;
			}
			this->alGenSources = (LPALGENSOURCES)GetProcAddress(m_hOpenALDLL, "alGenSources");
			if (this->alGenSources == NULL)
			{
				OutputDebugStringA("Failed to retrieve 'alGenSources' function address\n");
				return ;
			}
			this->alDeleteSources = (LPALDELETESOURCES)GetProcAddress(m_hOpenALDLL, "alDeleteSources");
			if (this->alDeleteSources == NULL)
			{
				OutputDebugStringA("Failed to retrieve 'alDeleteSources' function address\n");
				return ;
			}
			this->alIsSource = (LPALISSOURCE)GetProcAddress(m_hOpenALDLL, "alIsSource");
			if (this->alIsSource == NULL)
			{
				OutputDebugStringA("Failed to retrieve 'alIsSource' function address\n");
				return ;
			}
			this->alSourcei = (LPALSOURCEI)GetProcAddress(m_hOpenALDLL, "alSourcei");
			if (this->alSourcei == NULL)
			{
				OutputDebugStringA("Failed to retrieve 'alSourcei' function address\n");
				return ;
			}
			this->alSourcef = (LPALSOURCEF)GetProcAddress(m_hOpenALDLL, "alSourcef");
			if (this->alSourcef == NULL)
			{
				OutputDebugStringA("Failed to retrieve 'alSourcef' function address\n");
				return ;
			}
			this->alSource3f = (LPALSOURCE3F)GetProcAddress(m_hOpenALDLL, "alSource3f");
			if (this->alSource3f == NULL)
			{
				OutputDebugStringA("Failed to retrieve 'alSource3f' function address\n");
				return ;
			}
			this->alSourcefv = (LPALSOURCEFV)GetProcAddress(m_hOpenALDLL, "alSourcefv");
			if (this->alSourcefv == NULL)
			{
				OutputDebugStringA("Failed to retrieve 'alSourcefv' function address\n");
				return ;
			}
			this->alGetSourcei = (LPALGETSOURCEI)GetProcAddress(m_hOpenALDLL, "alGetSourcei");
			if (this->alGetSourcei == NULL)
			{
				OutputDebugStringA("Failed to retrieve 'alGetSourcei' function address\n");
				return ;
			}
			this->alGetSourcef = (LPALGETSOURCEF)GetProcAddress(m_hOpenALDLL, "alGetSourcef");
			if (this->alGetSourcef == NULL)
			{
				OutputDebugStringA("Failed to retrieve 'alGetSourcef' function address\n");
				return ;
			}
			this->alGetSourcefv = (LPALGETSOURCEFV)GetProcAddress(m_hOpenALDLL, "alGetSourcefv");
			if (this->alGetSourcefv == NULL)
			{
				OutputDebugStringA("Failed to retrieve 'alGetSourcefv' function address\n");
				return ;
			}
			this->alSourcePlayv = (LPALSOURCEPLAYV)GetProcAddress(m_hOpenALDLL, "alSourcePlayv");
			if (this->alSourcePlayv == NULL)
			{
				OutputDebugStringA("Failed to retrieve 'alSourcePlayv' function address\n");
				return ;
			}
			this->alSourceStopv = (LPALSOURCESTOPV)GetProcAddress(m_hOpenALDLL, "alSourceStopv");
			if (this->alSourceStopv == NULL)
			{
				OutputDebugStringA("Failed to retrieve 'alSourceStopv' function address\n");
				return ;
			}
			this->alSourcePlay = (LPALSOURCEPLAY)GetProcAddress(m_hOpenALDLL, "alSourcePlay");
			if (this->alSourcePlay == NULL)
			{
				OutputDebugStringA("Failed to retrieve 'alSourcePlay' function address\n");
				return ;
			}
			this->alSourcePause = (LPALSOURCEPAUSE)GetProcAddress(m_hOpenALDLL, "alSourcePause");
			if (this->alSourcePause == NULL)
			{
				OutputDebugStringA("Failed to retrieve 'alSourcePause' function address\n");
				return ;
			}
			this->alSourceStop = (LPALSOURCESTOP)GetProcAddress(m_hOpenALDLL, "alSourceStop");
			if (this->alSourceStop == NULL)
			{
				OutputDebugStringA("Failed to retrieve 'alSourceStop' function address\n");
				return ;
			}
			this->alGenBuffers = (LPALGENBUFFERS)GetProcAddress(m_hOpenALDLL, "alGenBuffers");
			if (this->alGenBuffers == NULL)
			{
				OutputDebugStringA("Failed to retrieve 'alGenBuffers' function address\n");
				return ;
			}
			this->alDeleteBuffers = (LPALDELETEBUFFERS)GetProcAddress(m_hOpenALDLL, "alDeleteBuffers");
			if (this->alDeleteBuffers == NULL)
			{
				OutputDebugStringA("Failed to retrieve 'alDeleteBuffers' function address\n");
				return ;
			}
			this->alIsBuffer = (LPALISBUFFER)GetProcAddress(m_hOpenALDLL, "alIsBuffer");
			if (this->alIsBuffer == NULL)
			{
				OutputDebugStringA("Failed to retrieve 'alIsBuffer' function address\n");
				return ;
			}
			this->alBufferData = (LPALBUFFERDATA)GetProcAddress(m_hOpenALDLL, "alBufferData");
			if (this->alBufferData == NULL)
			{
				OutputDebugStringA("Failed to retrieve 'alBufferData' function address\n");
				return ;
			}
			this->alGetBufferi = (LPALGETBUFFERI)GetProcAddress(m_hOpenALDLL, "alGetBufferi");
			if (this->alGetBufferi == NULL)
			{
				OutputDebugStringA("Failed to retrieve 'alGetBufferi' function address\n");
				return ;
			}
			this->alGetBufferf = (LPALGETBUFFERF)GetProcAddress(m_hOpenALDLL, "alGetBufferf");
			if (this->alGetBufferf == NULL)
			{
				OutputDebugStringA("Failed to retrieve 'alGetBufferf' function address\n");
				return ;
			}
			this->alSourceQueueBuffers = (LPALSOURCEQUEUEBUFFERS)GetProcAddress(m_hOpenALDLL, "alSourceQueueBuffers");
			if (this->alSourceQueueBuffers == NULL)
			{
				OutputDebugStringA("Failed to retrieve 'alSourceQueueBuffers' function address\n");
				return ;
			}
			this->alSourceUnqueueBuffers = (LPALSOURCEUNQUEUEBUFFERS)GetProcAddress(m_hOpenALDLL, "alSourceUnqueueBuffers");
			if (this->alSourceUnqueueBuffers == NULL)
			{
				OutputDebugStringA("Failed to retrieve 'alSourceUnqueueBuffers' function address\n");
				return ;
			}
			this->alDistanceModel = (LPALDISTANCEMODEL)GetProcAddress(m_hOpenALDLL, "alDistanceModel");
			if (this->alDistanceModel == NULL)
			{
				OutputDebugStringA("Failed to retrieve 'alDistanceModel' function address\n");
				return ;
			}
			this->alDopplerFactor = (LPALDOPPLERFACTOR)GetProcAddress(m_hOpenALDLL, "alDopplerFactor");
			if (this->alDopplerFactor == NULL)
			{
				OutputDebugStringA("Failed to retrieve 'alDopplerFactor' function address\n");
				return ;
			}
			this->alDopplerVelocity = (LPALDOPPLERVELOCITY)GetProcAddress(m_hOpenALDLL, "alDopplerVelocity");
			if (this->alDopplerVelocity == NULL)
			{
				OutputDebugStringA("Failed to retrieve 'alDopplerVelocity' function address\n");
				return ;
			}
			this->alcGetString = (LPALCGETSTRING)GetProcAddress(m_hOpenALDLL, "alcGetString");
			if (this->alcGetString == NULL)
			{
				OutputDebugStringA("Failed to retrieve 'alcGetString' function address\n");
				return ;
			}
			this->alcGetIntegerv = (LPALCGETINTEGERV)GetProcAddress(m_hOpenALDLL, "alcGetIntegerv");
			if (this->alcGetIntegerv == NULL)
			{
				OutputDebugStringA("Failed to retrieve 'alcGetIntegerv' function address\n");
				return ;
			}
			this->alcOpenDevice = (LPALCOPENDEVICE)GetProcAddress(m_hOpenALDLL, "alcOpenDevice");
			if (this->alcOpenDevice == NULL)
			{
				OutputDebugStringA("Failed to retrieve 'alcOpenDevice' function address\n");
				return ;
			}
			this->alcCloseDevice = (LPALCCLOSEDEVICE)GetProcAddress(m_hOpenALDLL, "alcCloseDevice");
			if (this->alcCloseDevice == NULL)
			{
				OutputDebugStringA("Failed to retrieve 'alcCloseDevice' function address\n");
				return ;
			}
			this->alcCreateContext = (LPALCCREATECONTEXT)GetProcAddress(m_hOpenALDLL, "alcCreateContext");
			if (this->alcCreateContext == NULL)
			{
				OutputDebugStringA("Failed to retrieve 'alcCreateContext' function address\n");
				return ;
			}
			this->alcMakeContextCurrent = (LPALCMAKECONTEXTCURRENT)GetProcAddress(m_hOpenALDLL, "alcMakeContextCurrent");
			if (this->alcMakeContextCurrent == NULL)
			{
				OutputDebugStringA("Failed to retrieve 'alcMakeContextCurrent' function address\n");
				return ;
			}
			this->alcProcessContext = (LPALCPROCESSCONTEXT)GetProcAddress(m_hOpenALDLL, "alcProcessContext");
			if (this->alcProcessContext == NULL)
			{
				OutputDebugStringA("Failed to retrieve 'alcProcessContext' function address\n");
				return ;
			}
			this->alcGetCurrentContext = (LPALCGETCURRENTCONTEXT)GetProcAddress(m_hOpenALDLL, "alcGetCurrentContext");
			if (this->alcGetCurrentContext == NULL)
			{
				OutputDebugStringA("Failed to retrieve 'alcGetCurrentContext' function address\n");
				return ;
			}
			this->alcGetContextsDevice = (LPALCGETCONTEXTSDEVICE)GetProcAddress(m_hOpenALDLL, "alcGetContextsDevice");
			if (this->alcGetContextsDevice == NULL)
			{
				OutputDebugStringA("Failed to retrieve 'alcGetContextsDevice' function address\n");
				return ;
			}
			this->alcSuspendContext = (LPALCSUSPENDCONTEXT)GetProcAddress(m_hOpenALDLL, "alcSuspendContext");
			if (this->alcSuspendContext == NULL)
			{
				OutputDebugStringA("Failed to retrieve 'alcSuspendContext' function address\n");
				return ;
			}
			this->alcDestroyContext = (LPALCDESTROYCONTEXT)GetProcAddress(m_hOpenALDLL, "alcDestroyContext");
			if (this->alcDestroyContext == NULL)
			{
				OutputDebugStringA("Failed to retrieve 'alcDestroyContext' function address\n");
				return ;
			}
			this->alcGetError = (LPALCGETERROR)GetProcAddress(m_hOpenALDLL, "alcGetError");
			if (this->alcGetError == NULL)
			{
				OutputDebugStringA("Failed to retrieve 'alcGetError' function address\n");
				return ;
			}
			this->alcIsExtensionPresent = (LPALCISEXTENSIONPRESENT)GetProcAddress(m_hOpenALDLL, "alcIsExtensionPresent");
			if (this->alcIsExtensionPresent == NULL)
			{
				OutputDebugStringA("Failed to retrieve 'alcIsExtensionPresent' function address\n");
				return ;
			}
			this->alcGetProcAddress = (LPALCGETPROCADDRESS)GetProcAddress(m_hOpenALDLL, "alcGetProcAddress");
			if (this->alcGetProcAddress == NULL)
			{
				OutputDebugStringA("Failed to retrieve 'alcGetProcAddress' function address\n");
				return ;
			}
			this->alcGetEnumValue = (LPALCGETENUMVALUE)GetProcAddress(m_hOpenALDLL, "alcGetEnumValue");
			if (this->alcGetEnumValue == NULL)
			{
				OutputDebugStringA("Failed to retrieve 'alcGetEnumValue' function address\n");
				return ;
			}

			// Effect objects
			alGenEffects = NULL;
			alDeleteEffects = NULL;
			alIsEffect = NULL;
			alEffecti = NULL;
			alEffectiv = NULL;
			alEffectf = NULL;
			alEffectfv = NULL;
			alGetEffecti = NULL;
			alGetEffectiv = NULL;
			alGetEffectf = NULL;
			alGetEffectfv = NULL;

			//Filter objects
			alGenFilters = NULL;
			alDeleteFilters = NULL;
			alIsFilter = NULL;
			alFilteri = NULL;
			alFilteriv = NULL;
			alFilterf = NULL;
			alFilterfv = NULL;
			alGetFilteri = NULL;
			alGetFilteriv = NULL;
			alGetFilterf = NULL;
			alGetFilterfv = NULL;

			// Auxiliary slot object
			alGenAuxiliaryEffectSlots = NULL;
			alDeleteAuxiliaryEffectSlots = NULL;
			alIsAuxiliaryEffectSlot = NULL;
			alAuxiliaryEffectSloti = NULL;
			alAuxiliaryEffectSlotiv = NULL;
			alAuxiliaryEffectSlotf = NULL;
			alAuxiliaryEffectSlotfv = NULL;
			alGetAuxiliaryEffectSloti = NULL;
			alGetAuxiliaryEffectSlotiv = NULL;
			alGetAuxiliaryEffectSlotf = NULL;
			alGetAuxiliaryEffectSlotfv = NULL;

			eaxSetBufferMode = NULL;
			eaxGetBufferMode = NULL;

			eXRAMSize = 0;
			eXRAMFree = 0;
			eXRAMAuto = 0;
			eXRAMHardware = 0;
			eXRAMAccessible = 0;

			return ;
		}
		~Load( )
		{
			if (m_hOpenALDLL)
			{
				FreeLibrary(m_hOpenALDLL);
				m_hOpenALDLL = NULL;
			}
		}

	public:
		static	Load*				ms_pfn;

	private:
		HINSTANCE					m_hOpenALDLL;

	public:
		LPALENABLE					alEnable;
		LPALDISABLE					alDisable;
		LPALISENABLED				alIsEnabled;
		LPALGETBOOLEAN				alGetBoolean;
		LPALGETINTEGER				alGetInteger;
		LPALGETFLOAT				alGetFloat;
		LPALGETDOUBLE				alGetDouble;
		LPALGETBOOLEANV				alGetBooleanv;
		LPALGETINTEGERV				alGetIntegerv;
		LPALGETFLOATV				alGetFloatv;
		LPALGETDOUBLEV				alGetDoublev;
		LPALGETSTRING				alGetString;
		LPALGETERROR				alGetError;
		LPALISEXTENSIONPRESENT		alIsExtensionPresent;
		LPALGETPROCADDRESS			alGetProcAddress;
		LPALGETENUMVALUE			alGetEnumValue;
		LPALLISTENERI				alListeneri;
		LPALLISTENERF				alListenerf;
		LPALLISTENER3F				alListener3f;
		LPALLISTENERFV				alListenerfv;
		LPALGETLISTENERI			alGetListeneri;
		LPALGETLISTENERF			alGetListenerf;
		LPALGETLISTENER3F			alGetListener3f;
		LPALGETLISTENERFV			alGetListenerfv;
		LPALGENSOURCES				alGenSources;
		LPALDELETESOURCES			alDeleteSources;
		LPALISSOURCE				alIsSource;
		LPALSOURCEI					alSourcei;
		LPALSOURCEF					alSourcef;
		LPALSOURCE3F				alSource3f;
		LPALSOURCEFV				alSourcefv;
		LPALGETSOURCEI				alGetSourcei;
		LPALGETSOURCEF				alGetSourcef;
		LPALGETSOURCEFV				alGetSourcefv;
		LPALSOURCEPLAYV				alSourcePlayv;
		LPALSOURCESTOPV				alSourceStopv;
		LPALSOURCEPLAY				alSourcePlay;
		LPALSOURCEPAUSE				alSourcePause;
		LPALSOURCESTOP				alSourceStop;
		LPALGENBUFFERS				alGenBuffers;
		LPALDELETEBUFFERS			alDeleteBuffers;
		LPALISBUFFER				alIsBuffer;
		LPALBUFFERDATA				alBufferData;
		LPALGETBUFFERI				alGetBufferi;
		LPALGETBUFFERF				alGetBufferf;
		LPALSOURCEQUEUEBUFFERS		alSourceQueueBuffers;
		LPALSOURCEUNQUEUEBUFFERS	alSourceUnqueueBuffers;
		LPALDISTANCEMODEL			alDistanceModel;
		LPALDOPPLERFACTOR			alDopplerFactor;
		LPALDOPPLERVELOCITY			alDopplerVelocity;
		LPALCGETSTRING				alcGetString;
		LPALCGETINTEGERV			alcGetIntegerv;
		LPALCOPENDEVICE				alcOpenDevice;
		LPALCCLOSEDEVICE			alcCloseDevice;
		LPALCCREATECONTEXT			alcCreateContext;
		LPALCMAKECONTEXTCURRENT		alcMakeContextCurrent;
		LPALCPROCESSCONTEXT			alcProcessContext;
		LPALCGETCURRENTCONTEXT		alcGetCurrentContext;
		LPALCGETCONTEXTSDEVICE		alcGetContextsDevice;
		LPALCSUSPENDCONTEXT			alcSuspendContext;
		LPALCDESTROYCONTEXT			alcDestroyContext;
		LPALCGETERROR				alcGetError;
		LPALCISEXTENSIONPRESENT		alcIsExtensionPresent;
		LPALCGETPROCADDRESS			alcGetProcAddress;
		LPALCGETENUMVALUE			alcGetEnumValue;


	public:
		// Effect objects
		LPALGENEFFECTS alGenEffects;
		LPALDELETEEFFECTS alDeleteEffects;
		LPALISEFFECT alIsEffect;
		LPALEFFECTI alEffecti ;
		LPALEFFECTIV alEffectiv ;
		LPALEFFECTF alEffectf ;
		LPALEFFECTFV alEffectfv ;
		LPALGETEFFECTI alGetEffecti ;
		LPALGETEFFECTIV alGetEffectiv ;
		LPALGETEFFECTF alGetEffectf ;
		LPALGETEFFECTFV alGetEffectfv ;

		//Filter objects
		LPALGENFILTERS alGenFilters ;
		LPALDELETEFILTERS alDeleteFilters ;
		LPALISFILTER alIsFilter ;
		LPALFILTERI alFilteri ;
		LPALFILTERIV alFilteriv ;
		LPALFILTERF alFilterf ;
		LPALFILTERFV alFilterfv ;
		LPALGETFILTERI alGetFilteri ;
		LPALGETFILTERIV alGetFilteriv ;
		LPALGETFILTERF alGetFilterf ;
		LPALGETFILTERFV alGetFilterfv ;

		// Auxiliary slot object
		LPALGENAUXILIARYEFFECTSLOTS alGenAuxiliaryEffectSlots ;
		LPALDELETEAUXILIARYEFFECTSLOTS alDeleteAuxiliaryEffectSlots ;
		LPALISAUXILIARYEFFECTSLOT alIsAuxiliaryEffectSlot ;
		LPALAUXILIARYEFFECTSLOTI alAuxiliaryEffectSloti ;
		LPALAUXILIARYEFFECTSLOTIV alAuxiliaryEffectSlotiv ;
		LPALAUXILIARYEFFECTSLOTF alAuxiliaryEffectSlotf ;
		LPALAUXILIARYEFFECTSLOTFV alAuxiliaryEffectSlotfv ;
		LPALGETAUXILIARYEFFECTSLOTI alGetAuxiliaryEffectSloti ;
		LPALGETAUXILIARYEFFECTSLOTIV alGetAuxiliaryEffectSlotiv ;
		LPALGETAUXILIARYEFFECTSLOTF alGetAuxiliaryEffectSlotf ;
		LPALGETAUXILIARYEFFECTSLOTFV alGetAuxiliaryEffectSlotfv ;

		LPEAXSETBUFFERMODE eaxSetBufferMode ;
		LPEAXGETBUFFERMODE eaxGetBufferMode ;

		ALenum eXRAMSize ;
		ALenum eXRAMFree ;
		ALenum eXRAMAuto ;
		ALenum eXRAMHardware ;
		ALenum eXRAMAccessible ;

	public:
		ALboolean	Initialize( )
		{
			//
			if (alIsExtensionPresent("EAX-RAM") == AL_TRUE)
			{
				// Get X-RAM Function pointers
				eaxSetBufferMode = (EAXSetBufferMode)alGetProcAddress("EAXSetBufferMode");
				eaxGetBufferMode = (EAXGetBufferMode)alGetProcAddress("EAXGetBufferMode");
			}

			//
			ALCdevice *pDevice = NULL;
			ALCcontext *pContext = NULL;
			pContext = alcGetCurrentContext();
			pDevice = alcGetContextsDevice(pContext);

			if (alcIsExtensionPresent(pDevice, (ALCchar*)ALC_EXT_EFX_NAME))
			{
				// Get function pointers
				alGenEffects = (LPALGENEFFECTS)alGetProcAddress("alGenEffects");
				alDeleteEffects = (LPALDELETEEFFECTS )alGetProcAddress("alDeleteEffects");
				alIsEffect = (LPALISEFFECT )alGetProcAddress("alIsEffect");
				alEffecti = (LPALEFFECTI)alGetProcAddress("alEffecti");
				alEffectiv = (LPALEFFECTIV)alGetProcAddress("alEffectiv");
				alEffectf = (LPALEFFECTF)alGetProcAddress("alEffectf");
				alEffectfv = (LPALEFFECTFV)alGetProcAddress("alEffectfv");
				alGetEffecti = (LPALGETEFFECTI)alGetProcAddress("alGetEffecti");
				alGetEffectiv = (LPALGETEFFECTIV)alGetProcAddress("alGetEffectiv");
				alGetEffectf = (LPALGETEFFECTF)alGetProcAddress("alGetEffectf");
				alGetEffectfv = (LPALGETEFFECTFV)alGetProcAddress("alGetEffectfv");
				alGenFilters = (LPALGENFILTERS)alGetProcAddress("alGenFilters");
				alDeleteFilters = (LPALDELETEFILTERS)alGetProcAddress("alDeleteFilters");
				alIsFilter = (LPALISFILTER)alGetProcAddress("alIsFilter");
				alFilteri = (LPALFILTERI)alGetProcAddress("alFilteri");
				alFilteriv = (LPALFILTERIV)alGetProcAddress("alFilteriv");
				alFilterf = (LPALFILTERF)alGetProcAddress("alFilterf");
				alFilterfv = (LPALFILTERFV)alGetProcAddress("alFilterfv");
				alGetFilteri = (LPALGETFILTERI )alGetProcAddress("alGetFilteri");
				alGetFilteriv= (LPALGETFILTERIV )alGetProcAddress("alGetFilteriv");
				alGetFilterf = (LPALGETFILTERF )alGetProcAddress("alGetFilterf");
				alGetFilterfv= (LPALGETFILTERFV )alGetProcAddress("alGetFilterfv");
				alGenAuxiliaryEffectSlots = (LPALGENAUXILIARYEFFECTSLOTS)alGetProcAddress("alGenAuxiliaryEffectSlots");
				alDeleteAuxiliaryEffectSlots = (LPALDELETEAUXILIARYEFFECTSLOTS)alGetProcAddress("alDeleteAuxiliaryEffectSlots");
				alIsAuxiliaryEffectSlot = (LPALISAUXILIARYEFFECTSLOT)alGetProcAddress("alIsAuxiliaryEffectSlot");
				alAuxiliaryEffectSloti = (LPALAUXILIARYEFFECTSLOTI)alGetProcAddress("alAuxiliaryEffectSloti");
				alAuxiliaryEffectSlotiv = (LPALAUXILIARYEFFECTSLOTIV)alGetProcAddress("alAuxiliaryEffectSlotiv");
				alAuxiliaryEffectSlotf = (LPALAUXILIARYEFFECTSLOTF)alGetProcAddress("alAuxiliaryEffectSlotf");
				alAuxiliaryEffectSlotfv = (LPALAUXILIARYEFFECTSLOTFV)alGetProcAddress("alAuxiliaryEffectSlotfv");
				alGetAuxiliaryEffectSloti = (LPALGETAUXILIARYEFFECTSLOTI)alGetProcAddress("alGetAuxiliaryEffectSloti");
				alGetAuxiliaryEffectSlotiv = (LPALGETAUXILIARYEFFECTSLOTIV)alGetProcAddress("alGetAuxiliaryEffectSlotiv");
				alGetAuxiliaryEffectSlotf = (LPALGETAUXILIARYEFFECTSLOTF)alGetProcAddress("alGetAuxiliaryEffectSlotf");
				alGetAuxiliaryEffectSlotfv = (LPALGETAUXILIARYEFFECTSLOTFV)alGetProcAddress("alGetAuxiliaryEffectSlotfv");
			}

			return AL_TRUE;
		}
	};
	Load*	Load::ms_pfn	= NULL;

	//
	#define	OpenALFunc	OpenAL::Load::ms_pfn 
}; //namespace	OpenAL

#endif	//__OpenALLoad_H__