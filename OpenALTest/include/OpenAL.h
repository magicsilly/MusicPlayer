#ifndef	__OpenAL_H__
#define __OpenAL_H__

#pragma once

//
#include <stdio.h>
#include <vector>
#include <string>

//
#include <al\al.h>
#include <al\alc.h>
#include <al\efx.h>
#include <al\efx-creative.h>
#include <al\xram.h>

#pragma comment(lib, "openal32.lib")
#pragma comment(lib, "efx_util.lib")

//
#include "OpenALLoad.h"
#include "OpenALMedia.h"


//
namespace	OpenAL
{
	//
	typedef struct ALDEVICEINFO
	{
		std::string					strDeviceName;
		int							iMajorVersion;
		int							iMinorVersion;
		unsigned int				uiSourceCount;
		std::vector<std::string>	*pvstrExtensions;
		bool						bSelected;
	} *LPALDEVICEINFO;

	//
	class ALDeviceList
	{
	public:
		ALDeviceList()
		{
			ALDEVICEINFO	ALDeviceInfo;
			char *devices;
			int index;
			const char *defaultDeviceName;
			const char *actualDeviceName;

			// DeviceInfo vector stores, for each enumerated device, it's device name, selection status, spec version #, and extension support
			vDeviceInfo.empty();
			vDeviceInfo.reserve(10);

			defaultDeviceIndex = 0;

			// grab function pointers for 1.0-API functions, and if successful proceed to enumerate all devices
			if ( TRUE ) 
			{
				if (OpenALFunc->alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT")) 
				{
					devices = (char *)OpenALFunc->alcGetString(NULL, ALC_DEVICE_SPECIFIER);
					defaultDeviceName = (char *)OpenALFunc->alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER);
					index = 0;
					// go through device list (each device terminated with a single NULL, list terminated with double NULL)
					while (*devices != NULL) 
					{
						if (strcmp(defaultDeviceName, devices) == 0) 
						{
							defaultDeviceIndex = index;
						}
						ALCdevice *device = OpenALFunc->alcOpenDevice(devices);
						if (device) 
						{
							ALCcontext *context = OpenALFunc->alcCreateContext(device, NULL);
							if (context) 
							{
								OpenALFunc->alcMakeContextCurrent(context);
								// if new actual device name isn't already in the list, then add it...
								actualDeviceName = OpenALFunc->alcGetString(device, ALC_DEVICE_SPECIFIER);
								bool bNewName = true;
								for (int i = 0; i < GetNumDevices(); i++) 
								{
									if (strcmp(GetDeviceName(i), actualDeviceName) == 0) 
									{
										bNewName = false;
									}
								}
								if ((bNewName) && (actualDeviceName != NULL) && (strlen(actualDeviceName) > 0)) 
								{
									memset(&ALDeviceInfo, 0, sizeof(ALDEVICEINFO));
									ALDeviceInfo.bSelected = true;
									ALDeviceInfo.strDeviceName = actualDeviceName;
									OpenALFunc->alcGetIntegerv(device, ALC_MAJOR_VERSION, sizeof(int), &ALDeviceInfo.iMajorVersion);
									OpenALFunc->alcGetIntegerv(device, ALC_MINOR_VERSION, sizeof(int), &ALDeviceInfo.iMinorVersion);

									ALDeviceInfo.pvstrExtensions = new std::vector<std::string>;

									// Check for ALC Extensions
									if (OpenALFunc->alcIsExtensionPresent(device, "ALC_EXT_CAPTURE") == AL_TRUE)
										ALDeviceInfo.pvstrExtensions->push_back("ALC_EXT_CAPTURE");
									if (OpenALFunc->alcIsExtensionPresent(device, "ALC_EXT_EFX") == AL_TRUE)
										ALDeviceInfo.pvstrExtensions->push_back("ALC_EXT_EFX");

									// Check for AL Extensions
									if (OpenALFunc->alIsExtensionPresent("AL_EXT_OFFSET") == AL_TRUE)
										ALDeviceInfo.pvstrExtensions->push_back("AL_EXT_OFFSET");

									if (OpenALFunc->alIsExtensionPresent("AL_EXT_LINEAR_DISTANCE") == AL_TRUE)
										ALDeviceInfo.pvstrExtensions->push_back("AL_EXT_LINEAR_DISTANCE");
									if (OpenALFunc->alIsExtensionPresent("AL_EXT_EXPONENT_DISTANCE") == AL_TRUE)
										ALDeviceInfo.pvstrExtensions->push_back("AL_EXT_EXPONENT_DISTANCE");

									if (OpenALFunc->alIsExtensionPresent("EAX2.0") == AL_TRUE)
										ALDeviceInfo.pvstrExtensions->push_back("EAX2.0");
									if (OpenALFunc->alIsExtensionPresent("EAX3.0") == AL_TRUE)
										ALDeviceInfo.pvstrExtensions->push_back("EAX3.0");
									if (OpenALFunc->alIsExtensionPresent("EAX4.0") == AL_TRUE)
										ALDeviceInfo.pvstrExtensions->push_back("EAX4.0");
									if (OpenALFunc->alIsExtensionPresent("EAX5.0") == AL_TRUE)
										ALDeviceInfo.pvstrExtensions->push_back("EAX5.0");

									if (OpenALFunc->alIsExtensionPresent("EAX-RAM") == AL_TRUE)
										ALDeviceInfo.pvstrExtensions->push_back("EAX-RAM");

									// Get Source Count
									ALDeviceInfo.uiSourceCount = GetMaxNumSources();

									vDeviceInfo.push_back(ALDeviceInfo);
								}
								OpenALFunc->alcMakeContextCurrent(NULL);
								OpenALFunc->alcDestroyContext(context);
							}
							OpenALFunc->alcCloseDevice(device);
						}
						devices += strlen(devices) + 1;
						index += 1;
					}
				}
			}

			ResetFilters();
		}
		~ALDeviceList()
		{
			for (unsigned int i = 0; i < vDeviceInfo.size(); i++) 
			{
				if (vDeviceInfo[i].pvstrExtensions) 
				{
					vDeviceInfo[i].pvstrExtensions->empty();
					delete vDeviceInfo[i].pvstrExtensions;
				}
			}
			vDeviceInfo.empty();
		}

	private:
		std::vector<ALDEVICEINFO>	vDeviceInfo;
		int							defaultDeviceIndex;
		int							filterIndex;

	public:

		int GetNumDevices()
		{
			return (int)vDeviceInfo.size();
		}

		char *GetDeviceName(int index)
		{
			if (index < GetNumDevices())
				return (char *)vDeviceInfo[index].strDeviceName.c_str();
			else
				return NULL;
		}

		void GetDeviceVersion(int index, int *major, int *minor)
		{
			if (index < GetNumDevices()) 
			{
				if (major)
					*major = vDeviceInfo[index].iMajorVersion;
				if (minor)
					*minor = vDeviceInfo[index].iMinorVersion;
			}
			return;
		}

		unsigned int GetMaxNumSources(int index)
		{
			if (index < GetNumDevices())
				return vDeviceInfo[index].uiSourceCount;
			else
				return 0;
		}

		bool IsExtensionSupported(int index, char *szExtName)
		{
			bool bReturn = false;

			if (index < GetNumDevices()) 
			{
				for (unsigned int i = 0; i < vDeviceInfo[index].pvstrExtensions->size(); i++) 
				{
					if (!_stricmp(vDeviceInfo[index].pvstrExtensions->at(i).c_str(), szExtName)) 
					{
						bReturn = true;
						break;
					}				
				}
			}

			return bReturn;
		}

		int GetDefaultDevice()
		{
			return defaultDeviceIndex;
		}

		void FilterDevicesMinVer(int major, int minor)
		{
			int dMajor, dMinor;
			for (unsigned int i = 0; i < vDeviceInfo.size(); i++) 
			{
				GetDeviceVersion(i, &dMajor, &dMinor);
				if ((dMajor < major) || ((dMajor == major) && (dMinor < minor))) 
				{
					vDeviceInfo[i].bSelected = false;
				}
			}
		}

		void FilterDevicesMaxVer(int major, int minor)
		{
			int dMajor, dMinor;
			for (unsigned int i = 0; i < vDeviceInfo.size(); i++) 
			{
				GetDeviceVersion(i, &dMajor, &dMinor);
				if ((dMajor > major) || ((dMajor == major) && (dMinor > minor))) 
				{
					vDeviceInfo[i].bSelected = false;
				}
			}
		}

		void FilterDevicesExtension(char *szExtName)
		{
			bool bFound;

			for (unsigned int i = 0; i < vDeviceInfo.size(); i++) 
			{
				bFound = false;
				for (unsigned int j = 0; j < vDeviceInfo[i].pvstrExtensions->size(); j++) 
				{
					if (!_stricmp(vDeviceInfo[i].pvstrExtensions->at(j).c_str(), szExtName)) 
					{
						bFound = true;
						break;
					}
				}
				if (!bFound)
					vDeviceInfo[i].bSelected = false;
			}
		}

		void ResetFilters()
		{
			for (int i = 0; i < GetNumDevices(); i++) 
			{
				vDeviceInfo[i].bSelected = true;
			}
			filterIndex = 0;
		}

		int GetFirstFilteredDevice()
		{
			int i;

			for (i = 0; i < GetNumDevices(); i++) 
			{
				if (vDeviceInfo[i].bSelected == true) 
				{
					break;
				}
			}
			filterIndex = i + 1;
			return i;
		}

		int GetNextFilteredDevice()
		{
			int i;

			for (i = filterIndex; i < GetNumDevices(); i++) 
			{
				if (vDeviceInfo[i].bSelected == true) 
				{
					break;
				}
			}
			filterIndex = i + 1;
			return i;
		}

	private:
		unsigned int GetMaxNumSources()
		{
			ALuint uiSources[256];
			unsigned int iSourceCount = 0;

			// Clear AL Error Code
			OpenALFunc->alGetError();

			// Generate up to 256 Sources, checking for any errors
			for (iSourceCount = 0; iSourceCount < 256; iSourceCount++)
			{
				OpenALFunc->alGenSources(1, &uiSources[iSourceCount]);
				if (OpenALFunc->alGetError() != AL_NO_ERROR)
					break;
			}

			// Release the Sources
			OpenALFunc->alDeleteSources(iSourceCount, uiSources);
			if (OpenALFunc->alGetError() != AL_NO_ERROR)
			{
				for (unsigned int i = 0; i < 256; i++)
				{
					OpenALFunc->alDeleteSources(1, &uiSources[i]);
				}
			}

			return iSourceCount;
		}
	};

	//
	class	Main
	{
	public:
		Main( )
			:	m_pMediaManager( NULL )
		{
			//
			m_pMediaManager	= new MediaManager;
			m_pALFunction	= new Load;
		}
		virtual ~Main( )
		{
			if( m_pMediaManager )
			{ delete m_pMediaManager; m_pMediaManager = NULL; }

			if( m_pALFunction )
			{ delete m_pALFunction; m_pALFunction = NULL; }
		}
	private:
		Load*				m_pALFunction;
		MediaManager*		m_pMediaManager;

	public:

		ALboolean ALFWShutdownOpenAL()
		{
			ALCcontext *pContext;
			ALCdevice *pDevice;

			pContext = alcGetCurrentContext();
			pDevice = alcGetContextsDevice(pContext);

			alcMakeContextCurrent(NULL);
			alcDestroyContext(pContext);
			alcCloseDevice(pDevice);

			return AL_TRUE;
		}

		ALboolean ALFWInitOpenAL()
		{
			ALDeviceList *pDeviceList = NULL;
			ALCcontext *pContext = NULL;
			ALCdevice *pDevice = NULL;
			ALint i;
			ALboolean bReturn = AL_FALSE;

			pDeviceList = new ALDeviceList();
			if ((pDeviceList) && (pDeviceList->GetNumDevices()))
			{
				ALFWprintf("\nOpenAL Device:\n");
				for (i = 0; i < pDeviceList->GetNumDevices(); i++) 
				{
					ALFWprintf("%d. %s%s\n", i, pDeviceList->GetDeviceName(i), i == pDeviceList->GetDefaultDevice() ? "(DEFAULT)" : "");
				}

				pDevice = alcOpenDevice(pDeviceList->GetDeviceName(pDeviceList->GetDefaultDevice( )));
				if (pDevice)
				{
					pContext = alcCreateContext(pDevice, NULL);
					if (pContext)
					{
						ALFWprintf("\nOpened %s Device\n", alcGetString(pDevice, ALC_DEVICE_SPECIFIER));
						alcMakeContextCurrent(pContext);
						bReturn = AL_TRUE;
					}
					else
					{
						alcCloseDevice(pDevice);
					}
				}

				delete pDeviceList;
			}

			m_pALFunction->Initialize( );
			return bReturn;
		}

	public:

		ALboolean ALFWIsXRAMSupported()
		{
			ALboolean bXRAM = AL_FALSE;

			if (alIsExtensionPresent("EAX-RAM") == AL_TRUE)
			{
				if (OpenALFunc->eaxSetBufferMode && OpenALFunc->eaxGetBufferMode)
				{
					OpenALFunc->eXRAMSize = alGetEnumValue("AL_EAX_RAM_SIZE");
					OpenALFunc->eXRAMFree = alGetEnumValue("AL_EAX_RAM_FREE");
					OpenALFunc->eXRAMAuto = alGetEnumValue("AL_STORAGE_AUTOMATIC");
					OpenALFunc->eXRAMHardware = alGetEnumValue("AL_STORAGE_HARDWARE");
					OpenALFunc->eXRAMAccessible = alGetEnumValue("AL_STORAGE_ACCESSIBLE");

					if (OpenALFunc->eXRAMSize && 
						OpenALFunc->eXRAMFree && 
						OpenALFunc->eXRAMAuto && 
						OpenALFunc->eXRAMHardware && 
						OpenALFunc->eXRAMAccessible)
					{	bXRAM = AL_TRUE; }
				}
			}

			return bXRAM;
		}

		ALboolean ALFWIsEFXSupported()
		{
			ALboolean bEFXSupport = AL_FALSE;

			if (TRUE)
			{
				if (OpenALFunc->alGenEffects &&	OpenALFunc->alDeleteEffects && 
					OpenALFunc->alIsEffect && OpenALFunc->alEffecti && 
					OpenALFunc->alEffectiv && OpenALFunc->alEffectf &&
					OpenALFunc->alEffectfv && OpenALFunc->alGetEffecti && 
					OpenALFunc->alGetEffectiv && OpenALFunc->alGetEffectf && 
					OpenALFunc->alGetEffectfv && OpenALFunc->alGenFilters &&
					OpenALFunc->alDeleteFilters && OpenALFunc->alIsFilter && 
					OpenALFunc->alFilteri && OpenALFunc->alFilteriv &&	
					OpenALFunc->alFilterf && OpenALFunc->alFilterfv &&
					OpenALFunc->alGetFilteri &&	OpenALFunc->alGetFilteriv && 
					OpenALFunc->alGetFilterf && OpenALFunc->alGetFilterfv && 
					OpenALFunc->alGenAuxiliaryEffectSlots && OpenALFunc->alDeleteAuxiliaryEffectSlots &&
					OpenALFunc->alIsAuxiliaryEffectSlot && OpenALFunc->alAuxiliaryEffectSloti &&
					OpenALFunc->alAuxiliaryEffectSlotiv && OpenALFunc->alAuxiliaryEffectSlotf && 
					OpenALFunc->alAuxiliaryEffectSlotfv && OpenALFunc->alGetAuxiliaryEffectSloti && 
					OpenALFunc->alGetAuxiliaryEffectSlotiv && OpenALFunc->alGetAuxiliaryEffectSlotf &&
					OpenALFunc->alGetAuxiliaryEffectSlotfv)
				{ bEFXSupport = AL_TRUE; }
			}

			return bEFXSupport;
		}
	public:
		void ALFWprintf( const char* x, ... )
		{
			va_list args;
			va_start( args, x );
			vprintf( x, args ); 
			va_end( args );
		}

	};
}; //namespace	OpenAL

#endif	//__OpenAL_H__