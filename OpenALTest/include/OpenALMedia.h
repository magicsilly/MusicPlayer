#ifndef	__OpenALMedia_H__
#define __OpenALMedia_H__

#pragma once

//
#include <windows.h>
#include <map>
#include <cassert>

//
#include <al\al.h>
#include <al\alc.h>

#include "OpenALDecode.h"
#include "OpenALEffect.h"

//
namespace	OpenAL
{
	/*
	//
	class Media
	{
	public:
		Media( )
			:	m_pDecode ( NULL ),
				m_pEffect ( NULL )
		{
			alGenBuffers( 1, &m_nBuffer );
			alGenSources( 1, &m_nSource );

			//
			m_bLoop		= AL_FALSE;
		}
		virtual ~Media( )
		{
			//
			alSourcei(m_nSource, AL_BUFFER, 0);

			//
			alDeleteSources( 1, &m_nSource );
			alDeleteBuffers( 1, &m_nBuffer );
		}

	private:
		ALuint		m_nSource;
		ALuint		m_nBuffer;

		ALboolean	m_bLoop;

	private:
		Decode*		m_pDecode;
		Effect*		m_pEffect;

	public:
		ALboolean	loadMedia( Decode* decode )
		{
			m_pDecode	= decode;
			if( !m_pDecode ){ return AL_FALSE; }
			
			alBufferData( m_nBuffer, m_pDecode->getBufferFormat( ), 
				m_pDecode->getData( ), m_pDecode->getDataLength( ), m_pDecode->getFrequency( ) );
			if (alGetError() != AL_NO_ERROR)
			{ return AL_FALSE; }

			//
			alSourcei(m_nSource, AL_BUFFER, m_nBuffer);

			//
			m_bLoop		= AL_FALSE;
			return AL_TRUE;
		}

		ALboolean	loadEffect( Effect* effect )
		{
			m_pEffect	= effect;
			if( !m_pEffect )
			{ 
				alSource3i(m_nSource, AL_AUXILIARY_SEND_FILTER, AL_EFFECTSLOT_NULL, 0, AL_FILTER_NULL);
				return AL_TRUE; 
			}

			alSource3i(m_nSource, AL_AUXILIARY_SEND_FILTER, m_pEffect->getEffectSlot( ), 0, AL_FILTER_NULL);
			if( alGetError( ) != AL_NO_ERROR )
			{
				return AL_FALSE;
			}
			return AL_TRUE;
		}

	public:
		ALboolean	setPlaying( ALboolean bPlaying )
		{
			ALint	iState;
			alGetSourcei(m_nSource, AL_SOURCE_STATE, &iState);
			if( bPlaying )
			{
				if( iState != AL_PLAYING )
				{
					alSourcePlay( m_nSource );
				}
			}
			else
			{
				if( iState == AL_PLAYING )
				{ 
					alSourceStop( m_nSource ); 
				}
			}
			return AL_TRUE;
		}

		ALboolean	setLoop( ALboolean bLoop )
		{
			ALboolean	result = m_bLoop;
			m_bLoop	= bLoop;
			return result;
		}
		ALboolean	getPlaying( )
		{
			ALint	iState;
			alGetSourcei(m_nSource, AL_SOURCE_STATE, &iState);
			return iState == AL_PLAYING ? AL_TRUE : AL_FALSE;
		}
		ALboolean	getLoop( )
		{
			return m_bLoop;
		}

	public:
		void		updateMedia( )
		{
			if( !m_pDecode ){ return ; }

			ALint	iState;
			alGetSourcei(m_nSource, AL_SOURCE_STATE, &iState);
			
			if( iState == AL_INITIAL )
			{ }
			else if( iState == AL_STOPPED )
			{
				m_pDecode->setReadOffset( m_pDecode->getDataLength( ) );

				//
				if( m_bLoop )
				{
					alSourcePlay( m_nSource );
				}
			}
			else if( iState == AL_PAUSED )
			{
			}
			else if( iState == AL_PLAYING )
			{
				ALint	iOffset = 0;
				alGetSourcei( m_nSource, AL_BYTE_OFFSET, &iOffset );
				m_pDecode->setReadOffset( iOffset );
			}
			else
			{
			}
		}
	};
	*/

	//
	#define MEDIA_BUFFERS_MAXNUM              4

	//
	class Media
	{
	public:
		Media( )
			:	m_nSource ( 0 ),
				m_pDecode( NULL ),
				m_pBuffer( NULL ),
				m_pEffect( NULL )
		{
			alGenBuffers( MEDIA_BUFFERS_MAXNUM, m_nBuffers );
			alGenSources( 1, &m_nSource );

			//
			m_nPlayState= 0;
			m_bPlaying	= AL_FALSE;
			m_bLoop		= AL_FALSE;
		}
		virtual ~Media( )
		{
			//
			loadEffect( NULL );

			//
			alDeleteSources( 1, &m_nSource );
			alDeleteBuffers( MEDIA_BUFFERS_MAXNUM, m_nBuffers );

			//
			if( m_pBuffer ){ free( m_pBuffer ); m_pBuffer = NULL; }
		}
	private:
		ALuint		m_nSource;
		ALuint		m_nBuffers[MEDIA_BUFFERS_MAXNUM];

		Decode*		m_pDecode;
		void*		m_pBuffer;
		long		m_lLength;

		Effect*		m_pEffect;

	private:
		ALuint		m_nPlayState;
		ALboolean	m_bPlaying;
		ALboolean	m_bLoop;

	public:
		ALboolean	loadMedia( Decode* decode )
		{
			m_pDecode	= decode;
			if( !m_pDecode ){ return AL_FALSE; }
			
			m_lLength	= decode->getBufferLength( );
			m_pBuffer	= malloc( m_lLength );

			// Ä¬ÈÏ²»²¥·Å
			m_nPlayState	= 2;

			//
			m_bPlaying	= AL_FALSE;
			m_bLoop		= AL_FALSE;
			return AL_TRUE;
		}

		ALboolean	loadEffect( Effect* effect )
		{
			m_pEffect	= effect;
			if( !m_pEffect )
			{ 
				alSource3i(m_nSource, AL_AUXILIARY_SEND_FILTER, AL_EFFECTSLOT_NULL, 0, AL_FILTER_NULL);
				return AL_TRUE; 
			}
			
			alSource3i(m_nSource, AL_AUXILIARY_SEND_FILTER, m_pEffect->getEffectSlot( ), 0, AL_FILTER_NULL);
			if( alGetError( ) != AL_NO_ERROR )
			{
				return AL_FALSE;
			}
			return AL_TRUE;
		}

	public:
		ALboolean	setPlaying( ALboolean bPlaying )
		{
			ALboolean	result = m_bPlaying;
			m_bPlaying	= bPlaying;

			if( m_bPlaying )
			{
				m_nPlayState	= 0;
			}
			else
			{
				ALint	iState;
				alGetSourcei(m_nSource, AL_SOURCE_STATE, &iState);
				if( iState == AL_PLAYING )
				{ 
					alSourceStop( m_nSource ); 
					alSourcei(m_nSource, AL_BUFFER, 0);
				}
				m_nPlayState	= 2;
			}
			return result;
		}
		ALboolean	setLoop( ALboolean bLoop )
		{
			ALboolean	result = m_bLoop;
			m_bLoop	= bLoop;
			return result;
		}
		ALboolean	getPlaying( )
		{
			return m_bPlaying;
		}
		ALboolean	getLoop( )
		{
			return m_bLoop;
		}

	public:
		void		updateMedia( )
		{
			if( !m_pDecode ){ return ; }
			if( !m_bPlaying ){ return ; }
			
			//
			if( m_nPlayState == 0 )
			{
				m_pDecode->setReadOffset( 0 );

				// Fill all the Buffers with audio data from the wavefile
				for(ALint iLoop = 0; iLoop < 4; iLoop++)
				{
					unsigned long	ulBytesWritten	= m_pDecode->readData(m_pBuffer, m_lLength);
					alBufferData(m_nBuffers[iLoop], m_pDecode->getBufferFormat( ), m_pBuffer, ulBytesWritten, m_pDecode->getFrequency( ));
					alSourceQueueBuffers(m_nSource, 1, &m_nBuffers[iLoop]);
				}

				alSourcePlay( m_nSource );
				m_nPlayState = 1;
			}
			else if( m_nPlayState == 2 )
			{
				return ;
			}

			//
			ALint			iState;
			ALint			iBuffersProcessed = 0;
			ALint			iQueuedBuffers;

			//
			alGetSourcei(m_nSource, AL_BUFFERS_PROCESSED, &iBuffersProcessed);
			while(iBuffersProcessed)
			{
				ALuint nBuffer = 0;
				alSourceUnqueueBuffers(m_nSource, 1, &nBuffer);

				// Read more audio data (if there is any)
				unsigned long ulBytesWritten = m_pDecode->readData(m_pBuffer, m_lLength);
				if (ulBytesWritten)
				{
					// Copy audio data to Buffer
					alBufferData(nBuffer, m_pDecode->getBufferFormat( ), m_pBuffer, ulBytesWritten, m_pDecode->getFrequency( ));
					// Queue Buffer on the Source
					alSourceQueueBuffers(m_nSource, 1, &nBuffer);
				}
				iBuffersProcessed--;
			}

			alGetSourcei(m_nSource, AL_SOURCE_STATE, &iState);
			if (iState != AL_PLAYING)
			{
				// If there are Buffers in the Source Queue then the Source was starved of audio
				// data, so needs to be restarted (because there is more audio data to play)
				alGetSourcei(m_nSource, AL_BUFFERS_QUEUED, &iQueuedBuffers);
				if (iQueuedBuffers)
				{
					alSourcePlay(m_nSource);
				}
				else
				{
					// Finished playing
					m_nPlayState	= 2;
					m_bPlaying		= AL_FALSE;

					alSourceStop(m_nSource);
					alSourcei(m_nSource, AL_BUFFER, 0);

					if( m_bLoop )
					{
						m_nPlayState	= 0;
						m_bPlaying		= AL_TRUE;
					}
					//
					return;
				}
			}

			return ;
		}

	public:

	};

	//
	class MediaManager
	{
	public:
		MediaManager( )
		{
			assert( !ms_Singleton );
			ms_Singleton = static_cast< MediaManager* >( this );
		}
		virtual ~MediaManager( )
		{
			MediaTable::iterator i = m_Medias.begin( );
			for( ; i != m_Medias.end( ) ; i ++ )
			{
				if( i->second )
				{ delete i->second; i->second = NULL; }
			}
		}
	private:
		static MediaManager*	ms_Singleton;
	public:
		static MediaManager& getSingleton( void )
		{	assert( ms_Singleton );  return ( *ms_Singleton ); }
		static MediaManager* getSingletonPtr( void )
		{ return ms_Singleton; }

	private:
		typedef std::map<std::string, Media*>	MediaTable;
		MediaTable								m_Medias;

	public:
		Media*	createMedia( const char* name )
		{
			MediaTable::iterator i = m_Medias.find( name );
			if( i != m_Medias.end( ) ){ return i->second; }

			Media*	media = new Media( );
			m_Medias[name] = media;
			return media;
		}
		void	destroyMedia( const char* name )
		{
			MediaTable::iterator i = m_Medias.find( name );
			if( i == m_Medias.end( ) ){ return ; }
			delete i->second; i->second = NULL;
			m_Medias.erase( i );
		}

	public:
		void	updateMedia( )
		{
			MediaTable::iterator i = m_Medias.begin( );
			for( ; i != m_Medias.end( ) ; i ++ )
			{
				if( i->second )
				{
					i->second->updateMedia( );
				}
			}
		}
	};
	MediaManager*	MediaManager::ms_Singleton = NULL;

}; //namespace	OpenAL

#endif	//__OpenALMedia_H__