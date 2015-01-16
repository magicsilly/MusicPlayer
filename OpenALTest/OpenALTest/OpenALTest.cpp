// OpenALTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "OpenAl.h"
#include "OpenALWAVDecode.h"
#include "OpenALMP3Decode.h"
#include "OpenALOGGDecode.h"
#include "OpenALFLACDecode.h"

//
int _tmain(int argc, _TCHAR* argv[])
{
	//
	OpenAL::Main		oalMain;
	if (!oalMain.ALFWInitOpenAL())
	{
		oalMain.ALFWprintf("Failed to initialize OpenAL\n");
		return 0;
	}

	OpenAL::Time			timeMax;
	OpenAL::AvgBytesPerSec	abps;
/*
	//
	OpenAL::WAVDecode		wavDecode;
	wavDecode.loadFile( "music.wav" );
	timeMax		= wavDecode.getMaxTime( );
	oalMain.ALFWprintf( "max time %d:%d:%d:%d \n", timeMax.h, timeMax.m, timeMax.s, timeMax.ms );
	wavDecode.getAvgBytesPerSec( &abps );
	oalMain.ALFWprintf( "AvgBytesPerSec %f mbps \n", abps.mbps );

	//
	OpenAL::MP3Decode		mp3Decode;
	mp3Decode.loadFile( "music.mp3" );
	timeMax		= mp3Decode.getMaxTime( );
	oalMain.ALFWprintf( "max time %d:%d:%d:%d \n", timeMax.h, timeMax.m, timeMax.s, timeMax.ms );
	mp3Decode.getAvgBytesPerSec( &abps );
	oalMain.ALFWprintf( "AvgBytesPerSec %f mbps \n", abps.mbps );
*/
	//
	OpenAL::FLACDecode		flacDecode;
	flacDecode.loadFile( "music.flac" );
	timeMax		= flacDecode.getMaxTime( );
	oalMain.ALFWprintf( "max time %d:%d:%d:%d \n", timeMax.h, timeMax.m, timeMax.s, timeMax.ms );
	flacDecode.getAvgBytesPerSec( &abps );
	oalMain.ALFWprintf( "AvgBytesPerSec %f mbps \n", abps.mbps );

	//
	OpenAL::Media*	media1 = OpenAL::MediaManager::getSingleton( ).createMedia( "music_flac" );
	media1->loadMedia( &flacDecode );
	media1->setPlaying( AL_TRUE );
	media1->setLoop( AL_TRUE );
/*
	//
	OpenAL::Media*	media2 = OpenAL::MediaManager::getSingleton( ).createMedia( "music_mp3" );
	media2->loadMedia( &mp3Decode );
	media2->setPlaying( AL_FALSE );
	media2->setLoop( AL_FALSE );
*/
	//
	OpenAL::Effect		effect;
	if( oalMain.ALFWIsEFXSupported( ) )
	{
		EAXREVERBPROPERTIES	eaxReverb	= REVERB_PRESET_CONCERTHALL;
		effect.setEAXReverb( eaxReverb );
		media1->loadEffect( &effect );
	}

	
	//
	while( TRUE )
	{
		Sleep( 20 );
		OpenAL::MediaManager::getSingleton( ).updateMedia( );
	}

	//
	oalMain.ALFWShutdownOpenAL( );
	return 0;
}

