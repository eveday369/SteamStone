/*
- Effec Class
����Ʈ ���� ���ҽ� �� Node, Entity�� �̰����� ������
���뼺 ��� ����
Node, Entity�� �̸� �ε��� �������μ� ���� ���� ���
*/

#pragma once
//#include "stdafx.h"
#include "BaseApplication.h"

#define MaxEffect		9							// �ִ� ����Ʈ ����
#define MaxCharacter	2							// �ִ� ĳ���� ����

enum Effect{
	Hit1,
	Hit2,
	WalkerSkill1,
	WalkerSkill2,
	WalkerExplosion,
	PsykerSkill1,
	PsykerSkill2,
	PsykerExplosion,
	Shoot
};



// 1�� : ĳ���͸��� ������ ����Ʈ ��带 �߰��Ͽ� �̺�Ʈ�� �߻��� ������ ��带 ������Ʈ �Ͽ� ���
class CEffect
{
public:
	CEffect(Ogre::SceneManager* mSceneMgr, Ogre::String Name, int job);										// ����Ʈ Node, Entity ���� �۾�
	virtual ~CEffect(void);

	virtual bool Update(Ogre::SceneNode* node, const Ogre::FrameEvent& evt);								// ĳ���� ��带 �޾ƿ� ����Ʈ ��� ������Ʈ
	virtual bool Play(Ogre::SceneManager* mSceneMgr, Ogre::SceneNode* node, int mType, Ogre::Vector3 dir);	// ����Ʈ ���(����Ʈ Ÿ�� ����)

protected:
	Ogre::String			Name;							// ĳ���� �̸�
	Ogre::SceneNode*		mNode[MaxEffect];				// ����Ʈ Node
	Ogre::Entity*			mEffect[MaxEffect];				// ����Ʈ Entity
	Ogre::AnimationState	*mAnimationState[MaxEffect];	// ����Ʈ �ִϸ��̼�

	bool					AlignEffectPosition(Ogre::Node* node, int type);	// ����Ʈ ��ġ ����
	Ogre::Vector3			JobCorrection();									// ĳ���͸��� ����Ʈ ��ġ ����

private:
	bool			mReverse;								// �¿� ��ȯ ����
	int				mJob;									// ĳ����
	Ogre::Vector3	mDir;									// ĳ���Ͱ� ���� �ִ� ����
	Ogre::Vector3	mCorr;									// ����Ʈ ��ġ ����ġ
};
