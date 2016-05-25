/*
- Effec Class
이펙트 관련 리소스 및 Node, Entity는 이곳에서 관리함
재사용성 향상에 초점
Node, Entity를 미리 로드해 놓음으로서 게임 성능 향상
*/

#pragma once
//#include "stdafx.h"
#include "BaseApplication.h"

#define MaxEffect		9							// 최대 이펙트 개수
#define MaxCharacter	2							// 최대 캐릭터 개수

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



// 1안 : 캐릭터마다 각각의 이펙트 노드를 추가하여 이벤트가 발생할 때마다 노드를 업데이트 하여 재생
class CEffect
{
public:
	CEffect(Ogre::SceneManager* mSceneMgr, Ogre::String Name, int job);										// 이펙트 Node, Entity 생성 작업
	virtual ~CEffect(void);

	virtual bool Update(Ogre::SceneNode* node, const Ogre::FrameEvent& evt);								// 캐릭터 노드를 받아와 이펙트 노드 업데이트
	virtual bool Play(Ogre::SceneManager* mSceneMgr, Ogre::SceneNode* node, int mType, Ogre::Vector3 dir);	// 이펙트 재생(이펙트 타입 지정)

protected:
	Ogre::String			Name;							// 캐릭터 이름
	Ogre::SceneNode*		mNode[MaxEffect];				// 이펙트 Node
	Ogre::Entity*			mEffect[MaxEffect];				// 이펙트 Entity
	Ogre::AnimationState	*mAnimationState[MaxEffect];	// 이펙트 애니메이션

	bool					AlignEffectPosition(Ogre::Node* node, int type);	// 이펙트 위치 정렬
	Ogre::Vector3			JobCorrection();									// 캐릭터마다 이펙트 위치 조정

private:
	bool			mReverse;								// 좌우 전환 유무
	int				mJob;									// 캐릭터
	Ogre::Vector3	mDir;									// 캐릭터가 보고 있는 방향
	Ogre::Vector3	mCorr;									// 이펙트 위치 조절치
};
