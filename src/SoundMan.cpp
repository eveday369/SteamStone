#include "stdafx.h"
#include "SoundMan.h"
#include "BaseApplication.h"

template<> SoundMan* Ogre::Singleton<SoundMan>::ms_Singleton = 0;

SoundMan::SoundMan()
{
	mSoundMgr = 0;

	initSound();
}

SoundMan::~SoundMan()
{
	if( mSoundMgr ) delete mSoundMgr;
}

bool SoundMan::initSound()
{
	mSoundMgr = SoundManager::createManager();

	if( mSoundMgr == 0 )
		return false;

	mSoundMgr->listAvailableDevices();
	mSoundMgr->init();
	mSoundMgr->setAudioPath( (char*) ".\\Sounds\\" );


	unsigned int	AudioID;

	//메인, 인게임, 어택, 승, 패//
	mSoundMgr->loadAudio( "menu.wav", &AudioID, true );
	mSoundList.insert( std::hash_map<Ogre::String, unsigned int>::value_type( "Menu", AudioID ) );

	mSoundMgr->loadAudio( "battle.wav", &AudioID, true );
	mSoundList.insert( std::hash_map<Ogre::String,unsigned int>::value_type( "Ingame", AudioID ) );

	mSoundMgr->loadAudio( "poong.wav", &AudioID, false);
	mSoundList.insert( std::hash_map<Ogre::String,unsigned int>::value_type( "attack", AudioID) );

	mSoundMgr->loadAudio( "Lose.wav", &AudioID, true);
	mSoundList.insert( std::hash_map<Ogre::String,unsigned int>::value_type( "Lose", AudioID) );
	
	mSoundMgr->loadAudio( "버튼쩔.wav", &AudioID, false);
	mSoundList.insert( std::hash_map<Ogre::String,unsigned int>::value_type( "button", AudioID) );

	mSoundMgr->loadAudio( "점프.wav", &AudioID, false);
	mSoundList.insert( std::hash_map<Ogre::String,unsigned int>::value_type( "Jump", AudioID) );

	//워커 스킬//

	mSoundMgr->loadAudio( "워커기본3타.wav", &AudioID, false);
	mSoundList.insert( std::hash_map<Ogre::String,unsigned int>::value_type( "walker", AudioID) );

	mSoundMgr->loadAudio( "워커기본.wav", &AudioID, false);
	mSoundList.insert( std::hash_map<Ogre::String,unsigned int>::value_type( "walker0", AudioID) );

	mSoundMgr->loadAudio( "워커1번스킬.wav", &AudioID, false);
	mSoundList.insert( std::hash_map<Ogre::String,unsigned int>::value_type( "walker1", AudioID) );

	mSoundMgr->loadAudio( "워커2번스킬.wav", &AudioID, false);
	mSoundList.insert( std::hash_map<Ogre::String,unsigned int>::value_type( "walker2", AudioID) );

	mSoundMgr->loadAudio( "워커3번스킬.wav", &AudioID, false);
	mSoundList.insert( std::hash_map<Ogre::String,unsigned int>::value_type( "walker3", AudioID) );


	//사이커스킬//

	mSoundMgr->loadAudio( "싸이커일반.wav", &AudioID, false);
	mSoundList.insert( std::hash_map<Ogre::String,unsigned int>::value_type( "psyker0", AudioID) );

	mSoundMgr->loadAudio( "싸이커1번스킬.wav", &AudioID, false);
	mSoundList.insert( std::hash_map<Ogre::String,unsigned int>::value_type( "psyker1", AudioID) );

	mSoundMgr->loadAudio( "싸이커2번스킬.wav", &AudioID, false);
	mSoundList.insert( std::hash_map<Ogre::String,unsigned int>::value_type( "psyker2", AudioID) );

	mSoundMgr->loadAudio( "싸이커3번스킬.wav", &AudioID, false);
	mSoundList.insert( std::hash_map<Ogre::String,unsigned int>::value_type( "psyker3", AudioID) );




	//거너스킬//

	mSoundMgr->loadAudio( "거너유탄.wav", &AudioID, false);
	mSoundList.insert( std::hash_map<Ogre::String,unsigned int>::value_type( "gunner", AudioID) );

	mSoundMgr->loadAudio( "gunner000.wav", &AudioID, false);
	mSoundList.insert( std::hash_map<Ogre::String,unsigned int>::value_type( "gunner0", AudioID) );

	mSoundMgr->loadAudio( "gunner111.wav", &AudioID, false);
	mSoundList.insert( std::hash_map<Ogre::String,unsigned int>::value_type( "gunner1", AudioID) );

	mSoundMgr->loadAudio( "gunner222.wav", &AudioID, false);
	mSoundList.insert( std::hash_map<Ogre::String,unsigned int>::value_type( "gunner2", AudioID) );
	
	mSoundMgr->loadAudio( "gunner333.wav", &AudioID, false);
	mSoundList.insert( std::hash_map<Ogre::String,unsigned int>::value_type( "gunner333", AudioID) );

	return true;

}

bool SoundMan::playSound( Ogre::String AudioName, bool forceRestart )
{
	std::hash_map<Ogre::String, unsigned int>::iterator iter = mSoundList.find( AudioName );

	if( iter == mSoundList.end() )
		return false;


	mSoundMgr->playAudio( iter->second, forceRestart );

	return true;
}

bool SoundMan::stopSound( Ogre::String AudioName, bool allStop )
{
	std::hash_map<Ogre::String, unsigned int>::iterator iter = mSoundList.find( AudioName );
	
	if( iter == mSoundList.end() )
		return false;

	if( allStop )
		mSoundMgr->stopAllAudio();
	else
		mSoundMgr->stopAudio( iter->second );

	return true;
}
