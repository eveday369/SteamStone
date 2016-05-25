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
						�ִϸ��̼� �ʱ�ȭ
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
                       �ִϸ��̼� ���
================================================================*/
void CAnimation::setBaseAnimation(int id, bool reset)
{
	if(mBaseAniID == id)
		return;

	stopAnimation(mBaseAniID);

	mBaseAniID = id;

	//����ġ �������� �ִϸ��̼� ���� �ϹǷ� ����ġ�� ������ 1�� �ξ�� �Ѵ�
	mAnims[id]->setEnabled(true);
	//mAnims[id]->setWeight(1);	
	if (reset) mAnims[id]->setTimePosition(0);
}

/*================================================================
						�ִϸ��̼��� ����
================================================================*/
void CAnimation::playAnimation(Ogre::Real deltaTime, int id, float RevisionSpeed)
{
	//RevisionSpeed�� �ִϸ��̼� ���ǵ带 ���Ƿ� �����Ҷ� ���δ�
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