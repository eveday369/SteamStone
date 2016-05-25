/*
	스킬을 매니저 방식으로 관리합니다

	   워커					거너				   사이커
	1 올려치기			  구르기				찌릿찌릿 창
	2 몰아치기			조정간 변환				  선물이야
	3 분노의 일격		마지막 한발				  몰아쳐라
*/
#include "BaseApplication.h"

#pragma once

class CSkill{
protected:
	int		mSkillNumber;	//스킬넘버
	int		mDamage;		//대미지
	int		mCooltime;		//쿨타임
	float	mDistance;		//이동거리
	int		mAttackframe;	//공격판정 등장 프레임

private:
	//int timer_count;
	//Ogre::Timer *timer;
	bool mSkillActivate;		//현재 다른 스킬이 사용 가능한가
	//bool mSkillNum[9];			//각각 스킬에 해당하는 넘버 - 아래 함수 순서대로

public:
	CSkill();
	~CSkill();

	//function
	bool WalkerSkill_1(Ogre::SceneNode *TargetNode, Ogre::Real deltaTime, Ogre::Vector3 Dir);				//올려치기
	bool WalkerSkill_2(Ogre::SceneNode *TargetNode, Ogre::Real deltaTime, Ogre::Vector3 Dir);				//모아치기
	bool WalkerSkill_3(Ogre::SceneNode *TargetNode, Ogre::Real deltaTime, Ogre::Vector3 Dir);				//분노의 일격

	bool GunnerSkill_1(Ogre::SceneNode *TargetNode, Ogre::Real deltaTime, Ogre::Vector3 Dir);				//구르기
	bool GunnerSkill_2(Ogre::SceneNode *TargetNode, Ogre::Real deltaTime, Ogre::Vector3 Dir);				//조정간 변환
	bool GunnerSkill_3(Ogre::SceneNode *TargetNode, Ogre::Real deltaTime, Ogre::Vector3 Dir);				//마지막 한발

	bool PsykerSkill_1(Ogre::SceneNode *TargetNode, Ogre::Real deltaTime, Ogre::Vector3 Dir);				//찌릿찌릿 창
	bool PsykerSkill_2(Ogre::SceneNode *TargetNode, Ogre::Real deltaTime, Ogre::Vector3 Dir);				//선물이야
	bool PsykerSkill_3(Ogre::SceneNode *TargetNode, Ogre::Real deltaTime, Ogre::Vector3 Dir);				//몰아쳐라

	bool Run(int SkillNum);		//SkillNum - 사용할 스킬의 넘버

	//get, set
	void		SetSkillNumber(int number)	{mSkillNumber = number;}
	int			GetSkillNumber()			{return mSkillNumber;}
	void		SetDamage(int damage)		{mDamage = damage;}
	int			GetDamage()					{return mDamage;}
	void		SetCooltime(int cooltime)	{mCooltime = cooltime;}
	int			GetCooltime()				{return mCooltime;}
	void		SetDistance(float distance)	{mDistance = distance;}
	float		GetDistance()				{return mDistance;}
	void		SetAttackframe(int frame)	{mAttackframe = frame;}
	int			GetAttackframe()			{return mAttackframe;}
};