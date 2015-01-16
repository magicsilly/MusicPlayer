#ifndef	__OpenALEffect_H__
#define __OpenALEffect_H__

#pragma once


#include <math.h>
#include <al/EFX-Util.h>


//
namespace	OpenAL
{
	class Effect
	{
	public:
		Effect( )	: 
		  m_bInitialize( AL_FALSE )
		{
			m_nEffect		= 0;
			m_nEffectSlot	= 0;

			// Generate an Auxiliary Effect Slot
			OpenALFunc->alGenAuxiliaryEffectSlots(1, &m_nEffectSlot);
			if (alGetError() == AL_NO_ERROR)
			{
				// Generate an Effect
				OpenALFunc->alGenEffects(1, &m_nEffect);
				if (alGetError() == AL_NO_ERROR)
				{
					// Set the Effect Type
					OpenALFunc->alEffecti(m_nEffect, AL_EFFECT_TYPE, AL_EFFECT_EAXREVERB);
					if (alGetError() == AL_NO_ERROR)
					{ m_bInitialize = TRUE; }
					else
					{ OpenALFunc->alDeleteEffects(1, &m_nEffect); }
				}
			}
		}
		virtual ~Effect( )
		{
			// Load NULL Effect into Effect Slot
			OpenALFunc->alAuxiliaryEffectSloti(m_nEffectSlot, AL_EFFECTSLOT_EFFECT, AL_EFFECT_NULL);

			// Delete Effect
			OpenALFunc->alDeleteEffects(1, &m_nEffect);

			// Delete Auxiliary Effect Slot
			OpenALFunc->alDeleteAuxiliaryEffectSlots(1, &m_nEffectSlot);

			m_bInitialize = FALSE;
		}
	private:
		ALuint		m_nEffectSlot;
		ALuint		m_nEffect;

		ALboolean	m_bInitialize;

	private:
		ALboolean	setEFXEAXReverbProperties(EFXEAXREVERBPROPERTIES *pEFXEAXReverb, ALuint uiEffect)
		{
			ALboolean bReturn = AL_FALSE;

			if (pEFXEAXReverb)
			{
				// Clear AL Error code
				alGetError();

				OpenALFunc->alEffectf(uiEffect, AL_EAXREVERB_DENSITY, pEFXEAXReverb->flDensity);
				OpenALFunc->alEffectf(uiEffect, AL_EAXREVERB_DIFFUSION, pEFXEAXReverb->flDiffusion);
				OpenALFunc->alEffectf(uiEffect, AL_EAXREVERB_GAIN, pEFXEAXReverb->flGain);
				OpenALFunc->alEffectf(uiEffect, AL_EAXREVERB_GAINHF, pEFXEAXReverb->flGainHF);
				OpenALFunc->alEffectf(uiEffect, AL_EAXREVERB_GAINLF, pEFXEAXReverb->flGainLF);
				OpenALFunc->alEffectf(uiEffect, AL_EAXREVERB_DECAY_TIME, pEFXEAXReverb->flDecayTime);
				OpenALFunc->alEffectf(uiEffect, AL_EAXREVERB_DECAY_HFRATIO, pEFXEAXReverb->flDecayHFRatio);
				OpenALFunc->alEffectf(uiEffect, AL_EAXREVERB_DECAY_LFRATIO, pEFXEAXReverb->flDecayLFRatio);
				OpenALFunc->alEffectf(uiEffect, AL_EAXREVERB_REFLECTIONS_GAIN, pEFXEAXReverb->flReflectionsGain);
				OpenALFunc->alEffectf(uiEffect, AL_EAXREVERB_REFLECTIONS_DELAY, pEFXEAXReverb->flReflectionsDelay);
				OpenALFunc->alEffectfv(uiEffect, AL_EAXREVERB_REFLECTIONS_PAN, pEFXEAXReverb->flReflectionsPan);
				OpenALFunc->alEffectf(uiEffect, AL_EAXREVERB_LATE_REVERB_GAIN, pEFXEAXReverb->flLateReverbGain);
				OpenALFunc->alEffectf(uiEffect, AL_EAXREVERB_LATE_REVERB_DELAY, pEFXEAXReverb->flLateReverbDelay);
				OpenALFunc->alEffectfv(uiEffect, AL_EAXREVERB_LATE_REVERB_PAN, pEFXEAXReverb->flLateReverbPan);
				OpenALFunc->alEffectf(uiEffect, AL_EAXREVERB_ECHO_TIME, pEFXEAXReverb->flEchoTime);
				OpenALFunc->alEffectf(uiEffect, AL_EAXREVERB_ECHO_DEPTH, pEFXEAXReverb->flEchoDepth);
				OpenALFunc->alEffectf(uiEffect, AL_EAXREVERB_MODULATION_TIME, pEFXEAXReverb->flModulationTime);
				OpenALFunc->alEffectf(uiEffect, AL_EAXREVERB_MODULATION_DEPTH, pEFXEAXReverb->flModulationDepth);
				OpenALFunc->alEffectf(uiEffect, AL_EAXREVERB_AIR_ABSORPTION_GAINHF, pEFXEAXReverb->flAirAbsorptionGainHF);
				OpenALFunc->alEffectf(uiEffect, AL_EAXREVERB_HFREFERENCE, pEFXEAXReverb->flHFReference);
				OpenALFunc->alEffectf(uiEffect, AL_EAXREVERB_LFREFERENCE, pEFXEAXReverb->flLFReference);
				OpenALFunc->alEffectf(uiEffect, AL_EAXREVERB_ROOM_ROLLOFF_FACTOR, pEFXEAXReverb->flRoomRolloffFactor);
				OpenALFunc->alEffecti(uiEffect, AL_EAXREVERB_DECAY_HFLIMIT, pEFXEAXReverb->iDecayHFLimit);

				if (alGetError() == AL_NO_ERROR)
				{ bReturn = AL_TRUE; }
			}

			return bReturn;
		}

	public:

		ALuint		getEffect( )
		{
			return m_nEffect;
		}

		ALuint		getEffectSlot( )
		{
			return m_nEffectSlot;
		}

	public:
		ALboolean	setEAXReverb( )
		{
			EAXREVERBPROPERTIES eaxReverb = REVERB_PRESET_GENERIC;
			return setEAXReverb( eaxReverb );
		}

		ALboolean	setEAXReverb( EAXREVERBPROPERTIES eaxReverb )
		{
			if( !m_bInitialize ){ return AL_FALSE; }

			//
			EFXEAXREVERBPROPERTIES	efxReverb;

			//
			ConvertReverbParameters(&eaxReverb, &efxReverb);

			// Set the Effect parameters
			if (!setEFXEAXReverbProperties(&efxReverb, m_nEffect))
			{
				return AL_FALSE;
			}
			
			// Load Effect into Auxiliary Effect Slot
			OpenALFunc->alAuxiliaryEffectSloti(m_nEffectSlot, AL_EFFECTSLOT_EFFECT, m_nEffect);


			return AL_TRUE;
		}
	};
}; //namespace	OpenAL

#endif	//__OpenALEffect_H__