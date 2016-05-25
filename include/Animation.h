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
	void setupAnimation(Ogre::Entity *entity);					//�ִϸ��̼� �����ϱ�
	void setBaseAnimation(int id, bool reset = false);			//�ִϸ��̼�
	void stopAnimation(int id);
	void StopAllAnimation();									//��� �ִϸ��̼� ����

	void playAnimation(Ogre::Real deltaTime, int id, float RevisionSpeed = 1);					//�ִϸ��̼� ���

	Ogre::AnimationState *GetAnimationState(int id) {return mAnims[id];}	//�ִϸ��̼� ����Ʈ�� �޾ƿ´�
	
private:

};