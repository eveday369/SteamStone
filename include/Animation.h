#include "BaseApplication.h"

#define NUM_ANIMS 13

#pragma once

class CAnimation
{
protected:
	//values
	/*enum AniState{ 
		ATTACK,
		ATTACK2,
		ATTACK3,
		DOWN,
		GUARD,
		IDLE,
		JUMP,
		RUN,
		NONE
	};*/

	Ogre::AnimationState	*mAnims[NUM_ANIMS];
	//State					mBaseAniID;
	int						mBaseAniID;

public:
	CAnimation();
	~CAnimation();

	//functions
	void setupAnimation(Ogre::Entity *entity);					//애니메이션 설정하기
	void setBaseAnimation(int id, bool reset = false);			//애니메이션
	void stopAnimation(int id);
	void StopAllAnimation();									//모든 애니메이션 정지

	void playAnimation(Ogre::Real deltaTime, int id, float RevisionSpeed = 1);					//애니메이션 재생

	Ogre::AnimationState *GetAnimationState(int id) {return mAnims[id];}	//애니메이션 리스트를 받아온다
	
private:

};