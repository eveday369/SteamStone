#pragma once

#include <hash_map>

class SoundManager;

class SoundMan : public Ogre::Singleton<SoundMan>
{
protected:

	SoundManager								*mSoundMgr;

	std::hash_map<Ogre::String, unsigned int>		mSoundList;

public:
	SoundMan();
	~SoundMan();
		
	bool initSound();

	bool playSound( Ogre::String SoundName, bool forceRestart = true );
	bool stopSound( Ogre::String SoundName, bool allStop = false );

};