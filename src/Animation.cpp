#include "stdafx.h"
#include "Animation.h"

#define ANISPEED 1

CAnimation::CAnimation()
{
	mBaseAniID = 0;
}

CAnimation::~CAnimation()
{
	for(int i=0 ; i<NUM_ANIMS ; i++)
	{
		if(mAnims[i] != NULL)
		{
			delete mAnims[i];
			mAnims[i] = NULL;
		}
	}
}

/*================================================================
						애니메이션 초기화
================================================================*/
void CAnimation::setupAnimation(Ogre::Entity *entity)
{
	entity->getSkeleton()->setBlendMode(Ogre::ANIMBLEND_CUMULATIVE);

	Ogre::String animNames[] =
		{"attack", "attack2", "attack3", "down", "guard", 
		"idle", "jump", "run", "skill1", "skill2",
	"skill3", "landing", "hit"};
	
	for (int i = 0; i < NUM_ANIMS; i++)
	{
		mAnims[i] = entity->getAnimationState(animNames[i]);
		mAnims[i]->setLoop(true);
		mAnims[i]->setWeight(1);
	}
}

/*================================================================
                       애니메이션 담당
================================================================*/
void CAnimation::setBaseAnimation(int id, bool reset)
{
	if(mBaseAniID == id)
		return;

	stopAnimation(mBaseAniID);

	mBaseAniID = id;

	//가중치 기준으로 애니메이션 블렌딩 하므로 가중치는 무조건 1로 두어야 한다
	mAnims[id]->setEnabled(true);
	//mAnims[id]->setWeight(1);	
	if (reset) mAnims[id]->setTimePosition(0);
}

/*================================================================
						애니메이션을 구동
================================================================*/
void CAnimation::playAnimation(Ogre::Real deltaTime, int id, float RevisionSpeed)
{
	//RevisionSpeed는 애니메이션 스피드를 임의로 조정할때 쓰인다
	mAnims[id]->addTime(deltaTime * ANISPEED * RevisionSpeed);
}

void CAnimation::stopAnimation(int id)
{
	mAnims[id]->setEnabled(false);
	mAnims[id]->setTimePosition(0);
}

void CAnimation::StopAllAnimation()
{
	for(int i=0 ; i<NUM_ANIMS ; i++)
	{
		mAnims[i]->setEnabled(false);
		mAnims[i]->setTimePosition(0);
	}
}